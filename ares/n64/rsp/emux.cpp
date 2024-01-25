auto RSP::TNE(cr32& rs, cr32& rt, u32 code) -> void {
  if(&rs != &rt) return INVALID();
  EMUX(rt, code);
}

namespace {
  enum class FormatStage {
    SEARCH_MARKER, 
    IN_FORMAT,
    IN_PLACEHOLDER,
    IN_LANE,
  };
}

auto RSP::EMUX(cr32& rt, u32 code) -> void {

      static const char *mips_reg_names[32] = { "zr", "at", "v0", "v1", "a0",
                                            "a1", "a2", "a3", "t0", "t1",
                                            "t2", "t3", "t4", "t5", "t6",
                                            "t7", "s0", "s1", "s2", "s3",
                                            "s4", "s5", "s6", "s7", "t8",
                                            "t9", "k0", "k1", "gp", "sp",
                                            "fp", "ra" };

    switch (code) {
    case 0x20: // trace(start)
        printf("[emux] trace start\n");
        debugger.tracer.instruction->setEnabled(true);
        debugger.tracer.instruction->setTerminal(true);
        debugger.tracer.instructionCountdown = 0;
        rsp.pipeline.clocksTotal = 0;
        break;
    case 0x21: // trace(count)
        printf("[emux] trace(count): %08x\n", rt.u32);
        debugger.tracer.instruction->setEnabled(true);
        debugger.tracer.instructionCountdown = rt.u32;
        break;
    case 0x22: // trace(stop)
        debugger.tracer.instruction->setTerminal(false);
        debugger.tracer.instruction->setEnabled(false);
        printf("[emux] trace stop\n");
        break;
    case 0x30: // log(byte)
        fputc(rt.u32 & 0xFF, stdout);
        break;
    case 0x31: { // log(string)
        u32 i = 0;
        do {
            char c = dmem.read<Byte>(rt.u32 + i);
            if (c == '\0') break;
            fputc(c, stdout);
        } while (++i < 4096);
    }   break;
    case 0x32: // log(buflen)
        emux.buflen = rt.u32;
        break;
    case 0x33: // log(buf)
        for (auto i : range(emux.buflen)) {
            fputc(dmem.read<Byte>(rt.u32 + i), stdout);
        }
        break;
    case 0x34: { // log(formatted string)
        std::string fmtReg;
        std::string fmtLane;
        char fmtType = 'x';
        FormatStage stage = FormatStage::SEARCH_MARKER;

        auto print_lane = [&](r128& vr, r128& vrNext, int lane, char fmt) {
          switch(fmt) {
            default:
            case 'x': fprintf(stdout, "%04X", vr.u16(lane)); return;
            case 'u': fprintf(stdout, "%u", vr.u16(lane)); return;
            case 'd': fprintf(stdout, "%d", vr.s16(lane)); return;
            case 'f':  {
              s32 partInt = (s32)((u32)vr.u16(lane) << 16);
              partInt |= vrNext.u16(lane);
              f64 val = partInt;
              val /= 65536.0f;

              fprintf(stdout, "%.6f", val); 
              return;
            }
          }
        };

        u32 i = 0;
        do 
        {
            char c = dmem.read<Byte>(rt.u32 + i);

            if(stage == FormatStage::SEARCH_MARKER && c == '%') {
              stage = FormatStage::IN_FORMAT;
              continue;
            }

            if(stage == FormatStage::IN_FORMAT) {
              fmtType = c;
              stage = FormatStage::IN_PLACEHOLDER;
              continue;
            }

            if(stage == FormatStage::IN_PLACEHOLDER || stage == FormatStage::IN_LANE) 
            {
              bool isPlaceholderChar = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.';

              if(!isPlaceholderChar) 
              {
                const char* fmt = fmtType == 'x' ? "%04X" : "%d";
                bool isUnsigned = fmtType != 'd';

                if(fmtReg[0] == 'v' && fmtReg.length() == 3) {
                  auto regIdx = std::stoi(&fmtReg[1]);
                  int lane = -1;
                  if(fmtLane.length() >= 2) {
                    lane = std::stoi(&fmtLane[1]);
                  }

                  auto &vr = vpu.r[regIdx];
                  auto &vrNext = vpu.r[(regIdx+1)];

                  if(lane < 0) {
                    for(int i=0; i<7; ++i) {
                      print_lane(vr, vrNext, i, fmtType);
                      fputc(' ', stdout);
                    }
                    print_lane(vr, vrNext, 7, fmtType);
                  } else {
                    print_lane(vr, vrNext, lane, fmtType);
                  }

                } else {
                  u32 r=0;
                  for(const char* regName : mips_reg_names) {
                    if(fmtReg == std::string_view{regName}) {
                      if(fmtType == 'x') {
                        fprintf(stdout, "%08X", isUnsigned ? ipu.r[r].u32 : ipu.r[r].s32);
                      } else {
                        fprintf(stdout, "%d", isUnsigned ? ipu.r[r].u32 : ipu.r[r].s32);
                      }
                    }
                    ++r;
                  }
                }

                fmtReg = "";
                fmtLane = "";
                fmtType = 'x';
                stage = FormatStage::SEARCH_MARKER;
                i--;
                continue;
              } else {
                if(c == '.') {
                  stage = FormatStage::IN_LANE; 
                  continue; 
                }
                if(stage == FormatStage::IN_LANE) {
                  fmtLane += c;
                } else {
                  fmtReg += c;
                }
                continue;
              }
            }

            if (c == '\0')break;
            fputc(c, stdout);

        } while (++i < 4096);
    }   break;
    case 0x40: case 0x48: { // dump_regs(gpr)
        n32 mask = rt.u32;
        const char *fmt = code & 0x8 ? "%s: %-12d" : "%s: %04x %04x";
        bool partial = false;
        for (u32 i : range(32)) {
            if (mask && !mask.bit(i)) continue;
            u32 val = ipu.r[i].u32;
            fprintf(stdout, fmt, mips_reg_names[i], val >> 16, val & 0xFFFF);
            if (i % 4 == 3) fputc('\n', stdout),  partial = false;
            else            fputs("   ", stdout), partial = true;
        }
        if (partial) fputc('\n', stdout);
    }   break;
    case 0x41: case 0x44: { // dump_regs(cop0)
        static const char *cop0_reg_names[16] = { 
            "dma_spaddr", "dma_ramaddr", "dma_read",     "dma_write",
            "sp_status",  "dma_full",    "dma_busy",     "semaphore",
            "dp_start",   "dp_end",      "dp_current",   "dp_status",
            "dp_clock",   "dp_busy",     "dp_pipe_busy", "dp_tmem_busy"
        };
        n32 mask = rt.u32;
        const char *fmt = code & 0x8 ? "%s: %-12d" : "%s: %04x %04x";
        bool partial = false;
        Thread dummyThread{};
        for (u32 i : range(16)) {
            if (mask && !mask.bit(i)) continue;
            u32 val;
            if (i == 7) val = status.semaphore; // avoid side effects
            else        val = (i & 8) ? Nintendo64::rdp.readWord(i & 7, *this) : Nintendo64::rsp.ioRead(i & 7, dummyThread);
            fprintf(stdout, fmt, cop0_reg_names[i], val >> 16, val & 0xFFFF);
            if (i % 4 == 3) fputc('\n', stdout),  partial = false;
            else            fputs("   ", stdout), partial = true;
        }
        if (partial) fputc('\n', stdout);
    }   break;
    case 0x42: case 0x46: case 0x4A: case 0x4C: { // dump_regs(cop2)
        auto dump_vr = [&](r128& vr) {
            for (auto i : range(8)) {
                if (code & 0x8)  fprintf(stdout, "%-6d", vr.s16(i));
                else             fprintf(stdout, "%04x", vr.u16(i));
                if (i < 7)  fputc(' ', stdout);
                if (i == 3) fputc(' ', stdout);
            }
        };

        n32 mask = rt.u32;
        bool partial = false;
        for (u32 i : range(32)) {
            if (mask && !mask.bit(i)) continue;
            fprintf(stdout, "v%02d: ", i);
            dump_vr(vpu.r[i]);
            if (i % 2 == 1) fputc('\n', stdout),  partial = false;
            else            fputs("   ", stdout), partial = true;
        }
        if (partial) fputc('\n', stdout);
        if (code & 0x4) {
            r128* accs[3] = { &vpu.acch, &vpu.accm, &vpu.accl };
            const char *accnames[] = { "acch", "accm", "accl" };

            auto dump_vc8 = [&](r128& vc) {
                for (auto i : range(8)) {
                    fprintf(stdout, "%c", vc.get(i) ? '1' : '-');
                    if (i == 3) fputc(' ', stdout);
                }
            };
            auto dump_vc16 = [&](r128& vch, r128& vcl) {
                dump_vc8(vch);
                fputc(' ', stdout);
                dump_vc8(vcl);
            };

            for (auto i : range(3)) {
                fprintf(stdout, "%s: ", accnames[i]);
                dump_vr(*accs[i]);
                fputs("   ", stdout);
                switch (i) {
                case 0: fprintf(stdout, "vco: "); dump_vc16(vpu.vcoh, vpu.vcol); fprintf(stdout, "\n"); break;
                case 1: fprintf(stdout, "vcc: "); dump_vc16(vpu.vcch, vpu.vccl); fprintf(stdout, "\n"); break;
                case 2: fprintf(stdout, "vce: "); dump_vc8(vpu.vce); fprintf(stdout, "\n"); break;
                }
            }
        }
    }   break;
    default:
        printf("[emux] unknown emux code: %08x\n", code);
        break;
    }
}