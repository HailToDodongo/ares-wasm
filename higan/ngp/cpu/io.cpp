auto CPU::readIO(uint8 address) -> uint8 {
  uint8 data = 0xff;

  switch(address) {

  //TRUN
  case 0x20:
    data.bit(0) = timers.timer01.lo.enable;
    data.bit(1) = timers.timer01.hi.enable;
    data.bit(2) = timers.timer23.lo.enable;
    data.bit(3) = timers.timer23.hi.enable;
    data.bit(4) = timers.timer4.enable;
    data.bit(5) = timers.timer5.enable;
    data.bit(7) = timers.enable;
    return data;

  //TREG0 (write-only)
  case 0x22: return data;

  //TREG1 (write-only)
  case 0x23: return data;

  //T01MOD
  case 0x24:
    data.bits(0,1) = timers.timer01.lo.mode;
    data.bits(2,3) = timers.timer01.hi.mode;
    data.bits(4,5) = timers.timer01.pwm;
    data.bits(6,7) = timers.timer01.mode;
    return data;

  //TFFCR
  case 0x25:
    data.bit(0) = timers.timer01.ff.select;
    data.bit(1) = timers.timer01.ff.enable;
    data.bits(2,3) = 0b11;  //write-only
    data.bit(4) = timers.timer23.ff.select;
    data.bit(5) = timers.timer23.ff.enable;
    data.bits(6,7) = 0b11;  //write-only
    return data;

  //TREG2 (write-only)
  case 0x26: return data;

  //TREG3 (write-only)
  case 0x27: return data;

  //T23MOD
  case 0x28:
    data.bits(0,1) = timers.timer23.lo.mode;
    data.bits(2,3) = timers.timer23.hi.mode;
    data.bits(4,5) = timers.timer23.pwm;
    data.bits(6,7) = timers.timer23.mode;
    return data;

  //TRDC
  case 0x29:
    data.bit(0) = timers.timer01.buffer.enable;
    data.bit(1) = timers.timer23.buffer.enable;
    return data;

  //TREG4 (write-only?)
  case 0x30: return data;
  case 0x31: return data;

  //TREG5 (write-only?)
  case 0x32: return data;
  case 0x33: return data;

  //CAP1
  case 0x34: return timers.timer4.captureA.byte(0);
  case 0x35: return timers.timer4.captureA.byte(1);

  //CAP2
  case 0x36: return timers.timer4.captureB.byte(0);
  case 0x37: return timers.timer4.captureB.byte(1);

  //T4MOD
  case 0x38:
    data.bits(0,1) = timers.timer4.mode;
    data.bit(2) = timers.timer4.clearOnMatch;
    data.bits(3,4) = timers.timer4.captureMode;
    data.bit(5) = 1;
    data.bit(6) = timers.timer4.ffB.flipOnMatchB;
    data.bit(7) = timers.timer4.ffB.flipOnLoadB;
    return data;

  //T4FFCR
  case 0x39:
    data.bits(0,1) = 0b11;
    data.bit(2) = timers.timer4.ffA.flipOnMatchA;
    data.bit(3) = timers.timer4.ffA.flipOnMatchB;
    data.bit(4) = timers.timer4.ffA.flipOnLoadA;
    data.bit(5) = timers.timer4.ffA.flipOnLoadB;
    data.bits(6,7) = 0b11;
    return data;

  //TREG6 (write-only?)
  case 0x40: return data;
  case 0x41: return data;

  //TREG7 (write-only?)
  case 0x42: return data;
  case 0x43: return data;

  //CAP3
  case 0x44: return timers.timer5.captureA.byte(0);
  case 0x45: return timers.timer5.captureA.byte(1);

  //CAP4
  case 0x46: return timers.timer5.captureB.byte(0);
  case 0x47: return timers.timer5.captureB.byte(1);

  //T5MOD
  case 0x48:
    data.bits(0,1) = timers.timer5.mode;
    data.bit(2) = timers.timer5.clearOnMatch;
    data.bits(3,4) = timers.timer5.captureMode;
    data.bit(5) = 1;
    //note: this is unverified
    //either timer 5 has the flip-flop but doesn't output the pin (assumed here),
    //or the hardware itself doesn't exist, and this (probably) would return 0b11
    data.bit(6) = timers.timer5.ffB.flipOnMatchB;
    data.bit(7) = timers.timer5.ffB.flipOnLoadB;
    return data;

  //T5FFCR
  case 0x49:
    data.bits(0,1) = 0b11;
    data.bit(2) = timers.timer5.ffA.flipOnMatchA;
    data.bit(3) = timers.timer5.ffA.flipOnMatchB;
    data.bit(4) = timers.timer5.ffA.flipOnLoadA;
    data.bit(5) = timers.timer5.ffA.flipOnLoadB;
    data.bits(6,7) = 0b11;
    return data;

  //ADREG04L
  case 0x60:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[0].bits(0,1);
    adc.end = 0;
    return data;

  //ADREG04H
  case 0x61:
    data.bits(0,7) = adc.result[0].bits(2,9);
    adc.end = 0;
    intad.clear();
    return data;

  //ADREG15L
  case 0x62:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[1].bits(0,1);
    adc.end = 0;
    return data;

  //ADREG15H
  case 0x63:
    data.bits(0,7) = adc.result[1].bits(2,9);
    adc.end = 0;
    intad.clear();
    return data;

  //ADREG2L
  case 0x64:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[2].bits(0,1);
    adc.end = 0;
    return data;

  //ADREG2H
  case 0x65:
    data.bits(0,7) = adc.result[2].bits(2,9);
    adc.end = 0;
    intad.clear();
    return data;

  //ADREG3L
  case 0x66:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[3].bits(0,1);
    adc.end = 0;
    return data;

  //ADREG3H
  case 0x67:
    data.bits(0,7) = adc.result[3].bits(2,9);
    adc.end = 0;
    intad.clear();
    return data;

  //ADMOD
  case 0x6d:
    data.bits(0,1) = adc.channel;
    data.bit(2) = 0;  //always reads as zero
    data.bit(3) = adc.speed;
    data.bit(4) = adc.scan;
    data.bit(5) = adc.repeat;
    data.bit(6) = adc.busy;
    data.bit(7) = adc.end;
    return data;

  //WDMOD
  case 0x6e:
    data.bit(0) = watchdog.drive;
    data.bit(1) = watchdog.reset;
    data.bits(2,3) = watchdog.standby;
    data.bit(4) = watchdog.warmup;
    data.bits(5,6) = watchdog.frequency;
    data.bit(7) = watchdog.enable;
    return data;

  //WDCR (write-only)
  case 0x6f: return data;

  //INTE0AD
  case 0x70:
    data.bits(0,2) = 0b000;
    data.bit(3) = int0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intad.request;
    return data;

  //INTE45
  case 0x71:
    data.bits(0,2) = 0b000;
    data.bit(3) = int4.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = int5.request;
    return data;

  //INTE67
  case 0x72:
    data.bits(0,2) = 0b000;
    data.bit(3) = int6.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = int7.request;
    return data;

  //INTET01
  case 0x73:
    data.bits(0,2) = 0b000;
    data.bit(3) = intt0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intt1.request;
    return data;

  //INTET23
  case 0x74:
    data.bits(0,2) = 0b000;
    data.bit(3) = intt2.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intt3.request;
    return data;

  //INTET45
  case 0x75:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttr4.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttr5.request;
    return data;

  //INTET67
  case 0x76:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttr6.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttr7.request;
    return data;

  //INTE50
  case 0x77:
    data.bits(0,2) = 0b000;
    data.bit(3) = intrx0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttx0.request;
    return data;

  //INTE51
  case 0x78:
    data.bits(0,2) = 0b000;
    data.bit(3) = intrx1.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttx1.request;
    return data;

  //INTETC01
  case 0x79:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttc0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttc1.request;
    return data;

  //INTETC23
  case 0x7a:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttc2.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttc3.request;
    return data;

  //IIMC
  case 0x7b:
    data.bit(0) = nmi.edge.rising;
    data.bit(1) = int0.level.high;
    data.bit(2) = int0.enable;
    return data;

  case 0xb0:
    controls.poll();
    data.bit(0) = controls.upLatch;
    data.bit(1) = controls.downLatch;
    data.bit(2) = controls.leftLatch;
    data.bit(3) = controls.rightLatch;
    data.bit(4) = controls.a->value;
    data.bit(5) = controls.b->value;
    data.bit(6) = controls.option->value;
    data.bit(7) = 0;  //unused?
    return data;

  case 0xb1:
    data.bit(0) = !controls.power->value;
    data.bit(1) = 1;  //sub battery (CR2032)
  //note: d6 is probably? the debug button for development hardware ...
    return data;

  case 0xb3:
    data.bit(2) = nmi.enable;
    return data;

  case 0xbc:
    data = io.apuPort;
    return data;

  }

  return data;
}

auto CPU::writeIO(uint8 address, uint8 data) -> void {
  switch(address) {

  //TRUN
  case 0x20:
    timers.timer01.lo.enableIf(data.bit(0));
    timers.timer01.hi.enableIf(data.bit(1));
    timers.timer23.lo.enableIf(data.bit(2));
    timers.timer23.hi.enableIf(data.bit(3));
    timers.timer4.enableIf(data.bit(4));
    timers.timer5.enableIf(data.bit(5));
    timers.enable = data.bit(7);
    return;

  //TREG0
  case 0x22:
    if(!timers.timer01.buffer.enable) timers.timer01.lo.compare = data;
    timers.timer01.buffer.compare = data;
    if(timers.timer01.mode == 1) timers.timer01.lo.enable = 0;
    return;

  //TREG1
  case 0x23:
    timers.timer01.hi.compare = data;
    if(timers.timer01.mode == 1) timers.timer01.lo.enable = 1;
    return;

  //T01MOD
  case 0x24:
    timers.timer01.lo.mode = data.bits(0,1);
    timers.timer01.hi.mode = data.bits(2,3);
    timers.timer01.pwm = data.bits(4,5);
    timers.timer01.mode = data.bits(6,7);
    return;

  //TFFCR
  case 0x25:
    timers.timer01.ff.select = data.bit(0);
    timers.timer01.ff.enable = data.bit(1);
    if(data.bits(2,3) == 0) timers.timer01.ff.invert();
    if(data.bits(2,3) == 1) timers.timer01.ff.raise();
    if(data.bits(2,3) == 2) timers.timer01.ff.lower();
    timers.timer23.ff.select = data.bit(4);
    timers.timer23.ff.enable = data.bit(5);
    if(data.bits(6,7) == 0) timers.timer23.ff.invert();
    if(data.bits(6,7) == 1) timers.timer23.ff.raise();
    if(data.bits(6,7) == 2) timers.timer23.ff.lower();
    return;

  //TREG2
  case 0x26:
    if(!timers.timer23.buffer.enable) timers.timer23.lo.compare = data;
    timers.timer23.buffer.compare = data;
    if(timers.timer23.mode == 1) timers.timer23.lo.enable = 0;
    return;

  //TREG3
  case 0x27:
    timers.timer23.hi.compare = data;
    if(timers.timer23.mode == 1) timers.timer23.lo.enable = 1;
    return;

  //T23MOD
  case 0x28:
    timers.timer23.lo.mode = data.bits(0,1);
    timers.timer23.hi.mode = data.bits(2,3);
    timers.timer23.pwm = data.bits(4,5);
    timers.timer23.mode = data.bits(6,7);
    return;

  //TRDC
  case 0x29:
    timers.timer01.buffer.enable = data.bit(0);
    timers.timer23.buffer.enable = data.bit(1);
    return;

  //TREG4
  case 0x30:
    if(!timers.timer4.buffer.enable) timers.timer4.compareA.byte(0) = data;
    timers.timer4.buffer.compare.byte(0) = data;
    return;
  case 0x31:
    if(!timers.timer4.buffer.enable) timers.timer4.compareA.byte(1) = data;
    timers.timer4.buffer.compare.byte(1) = data;
    return;

  //TREG5
  case 0x32: timers.timer4.compareB.byte(0) = data; return;
  case 0x33: timers.timer4.compareB.byte(1) = data; return;

  //CAP1
  case 0x34: timers.timer4.captureA.byte(0) = data; return;
  case 0x35: timers.timer4.captureA.byte(1) = data; return;

  //CAP2
  case 0x36: timers.timer4.captureB.byte(0) = data; return;
  case 0x37: timers.timer4.captureB.byte(1) = data; return;

  //T4MOD
  case 0x38:
    timers.timer4.mode = data.bits(0,1);
    timers.timer4.clearOnMatch = data.bit(2);
    timers.timer4.captureMode = data.bits(3,4);
    if(!data.bit(5)) timers.timer4.captureA = timers.timer4.counter;
    timers.timer4.ffB.flipOnMatchB = data.bit(6);
    timers.timer4.ffB.flipOnLoadB = data.bit(7);
    int4.edge.rising  = data.bits(3,4) != 2;
    int4.edge.falling = data.bits(3,4) == 2;
    return;

  //T4FFCR
  case 0x39:
    if(data.bits(0,1) == 0) timers.timer4.outputA(timers.timer4.ffA.output ^= 1);
    if(data.bits(0,1) == 1) timers.timer4.outputA(timers.timer4.ffA.output  = 1);
    if(data.bits(0,1) == 2) timers.timer4.outputA(timers.timer4.ffA.output  = 0);
    timers.timer4.ffA.flipOnMatchA = data.bit(2);
    timers.timer4.ffA.flipOnMatchB = data.bit(3);
    timers.timer4.ffA.flipOnLoadA = data.bit(4);
    timers.timer4.ffA.flipOnLoadB = data.bit(5);
    if(data.bits(6,7) == 0) timers.timer4.outputB(timers.timer4.ffB.output ^= 1);
    if(data.bits(6,7) == 1) timers.timer4.outputB(timers.timer4.ffB.output  = 1);
    if(data.bits(6,7) == 2) timers.timer4.outputB(timers.timer4.ffB.output  = 0);
    return;

  //T45CR
  case 0x3a:
    timers.timer4.buffer.enable = data.bit(0);
    timers.timer5.buffer.enable = data.bit(1);
    return;

  //TREG6
  case 0x40:
    if(!timers.timer5.buffer.enable) timers.timer5.compareA.byte(0) = data;
    timers.timer5.buffer.compare.byte(0) = data;
    return;
  case 0x41:
    if(!timers.timer5.buffer.enable) timers.timer5.compareA.byte(1) = data;
    timers.timer5.buffer.compare.byte(1) = data;
    return;

  //TREG7
  case 0x42: timers.timer5.compareB.byte(0) = data; return;
  case 0x43: timers.timer5.compareB.byte(1) = data; return;

  //CAP3
  case 0x44: timers.timer5.captureA.byte(0) = data; return;
  case 0x45: timers.timer5.captureA.byte(1) = data; return;

  //CAP4
  case 0x46: timers.timer5.captureB.byte(0) = data; return;
  case 0x47: timers.timer5.captureB.byte(1) = data; return;

  //T5MOD
  case 0x48:
    timers.timer5.mode = data.bits(0,1);
    timers.timer5.clearOnMatch = data.bit(2);
    timers.timer5.captureMode = data.bits(3,4);
    if(!data.bit(5)) timers.timer5.captureA = timers.timer5.counter;
    //note: this may not actually exist ...
    timers.timer5.ffB.flipOnMatchB = data.bit(6);
    timers.timer5.ffB.flipOnLoadB = data.bit(7);
    int6.edge.rising  = data.bits(3,4) != 2;
    int6.edge.falling = data.bits(3,4) == 2;
    return;

  //T5FFCR
  case 0x49:
    if(data.bits(0,1) == 0) timers.timer5.outputA(timers.timer5.ffA.output ^= 1);
    if(data.bits(0,1) == 1) timers.timer5.outputA(timers.timer5.ffA.output  = 1);
    if(data.bits(0,1) == 2) timers.timer5.outputA(timers.timer5.ffA.output  = 0);
    timers.timer5.ffA.flipOnMatchA = data.bit(2);
    timers.timer5.ffA.flipOnMatchB = data.bit(3);
    timers.timer5.ffA.flipOnLoadA = data.bit(4);
    timers.timer5.ffA.flipOnLoadB = data.bit(5);
    if(data.bits(6,7) == 0) timers.timer5.outputB(timers.timer5.ffB.output ^= 1);
    if(data.bits(6,7) == 1) timers.timer5.outputB(timers.timer5.ffB.output  = 1);
    if(data.bits(6,7) == 2) timers.timer5.outputB(timers.timer5.ffB.output  = 0);
    return;

  //ADMOD
  case 0x6d: {
    uint1 busy = adc.busy;
    adc.channel = data.bits(0,1);
    uint1 start = data.bit(2);
    adc.speed = data.bit(3);
    adc.scan = data.bit(4);
    adc.repeat = data.bit(5);

    if(!busy && start) {
      adc.busy = 1;
      adc.counter = !adc.speed ? 160 : 320;
    }
  } return;

  //WDMOD
  case 0x6e:
    watchdog.drive = data.bit(0);
    watchdog.reset = data.bit(1);
    watchdog.standby = data.bits(2,3);
    watchdog.warmup = data.bit(4);
    watchdog.frequency = data.bits(5,6);
    watchdog.enable = data.bit(7);
    return;

  //WDCR
  case 0x6f:
    if(data == 0x4e);  //clear
    if(data == 0xb1);  //disable
    return;

  //INTE0AD
  case 0x70:
    int0.priority = data.bits(0,2);
    if(!data.bit(3)) int0.clear();
    intad.priority = data.bits(4,6);
    if(!data.bit(7)) intad.clear();
    return;

  //INTE45
  case 0x71:
    int4.priority = data.bits(0,2);
    if(!data.bit(3)) int4.clear();
    int5.priority = data.bits(4,6);
    if(!data.bit(7)) int5.clear();
    return;

  //INTE67
  case 0x72:
    int6.priority = data.bits(0,2);
    if(!data.bit(3)) int6.clear();
    int7.priority = data.bits(4,6);
    if(!data.bit(7)) int7.clear();
    return;

  //INTET01
  case 0x73:
    intt0.priority = data.bits(0,2);
    if(!data.bit(3)) intt0.clear();
    intt1.priority = data.bits(4,6);
    if(!data.bit(7)) intt1.clear();
    return;

  //INTET23
  case 0x74:
    intt2.priority = data.bits(0,2);
    if(!data.bit(3)) intt2.clear();
    intt3.priority = data.bits(4,6);
    if(!data.bit(7)) intt3.clear();
    return;

  //INTET45
  case 0x75:
    inttr4.priority = data.bits(0,2);
    if(!data.bit(3)) inttr4.clear();
    inttr5.priority = data.bits(4,6);
    if(!data.bit(7)) inttr5.clear();
    return;

  //INTET67
  case 0x76:
    inttr6.priority = data.bits(0,2);
    if(!data.bit(3)) inttr6.clear();
    inttr7.priority = data.bits(4,6);
    if(!data.bit(7)) inttr7.clear();
    return;

  //INTE50
  case 0x77:
    intrx0.priority = data.bits(0,2);
    if(!data.bit(3)) intrx0.clear();
    inttx0.priority = data.bits(4,6);
    if(!data.bit(7)) inttx0.clear();
    return;

  //INTE51
  case 0x78:
    intrx1.priority = data.bits(0,2);
    if(!data.bit(3)) intrx1.clear();
    inttx1.priority = data.bits(4,6);
    if(!data.bit(7)) inttx1.clear();
    return;

  //INTETC01
  case 0x79:
    inttc0.priority = data.bits(0,2);
    if(!data.bit(3)) inttc0.clear();
    inttc1.priority = data.bits(4,6);
    if(!data.bit(7)) inttc1.clear();
    return;

  //INTETC23
  case 0x7a:
    inttc2.priority = data.bits(0,2);
    if(!data.bit(3)) inttc2.clear();
    inttc3.priority = data.bits(4,6);
    if(!data.bit(7)) inttc3.clear();
    return;

  //IIMC
  case 0x7b:
    nmi.edge.rising = data.bit(0);
    int0.edge.rising = !data.bit(1);
    int0.level.high = data.bit(1);
    int0.enable = data.bit(2);
    return;

  case 0xa0:
    psg.psgRight.write(data);
    return;

  case 0xa1:
    psg.psgLeft.write(data);
    return;

  case 0xa2:
    psg.dacRight = data;
    return;

  case 0xa3:
    psg.dacLeft = data;
    return;

  //???
  case 0xb2:
    return;

  case 0xb3:
    nmi.enable = data.bit(2);
    return;

  case 0xb8:
    if(data == 0x55) psg.psgEnable = 1;
    if(data == 0xaa) psg.psgEnable = 0;
    return;

  case 0xb9:
    if(data == 0x55) apu.enable();
    if(data == 0xaa) apu.disable();
    return;

  case 0xba:
    apu.nmi.line = 1;
    return;

  case 0xbc:
    io.apuPort = data;
    return;
  }

//use to detect unimplemented internal registers
//print("CPU::writeIO(", hex(address, 2L), " = ", hex(data, 2L), ")\n");
}