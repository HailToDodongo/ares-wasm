#include <n64/n64.hpp>

namespace ares::Nintendo64 {

RDRAM rdram;
#include "io.cpp"
#include "debugger.cpp"
#include "serialization.cpp"

auto RDRAM::load(Node::Object parent) -> void {
  node = parent->append<Node::Object>("RDRAM");

  if(!system.expansionPak) {
    //4MB internal
    ram.allocate(4_MiB);
  } else {
    //4MB internal + 4MB expansion pak
    ram.allocate(4_MiB + 4_MiB);
  }

  // parallelRDP stores hidden bits in 2-bit pairs (LSB)
  ram.hiddenBits.allocate(ram.size / 2);
  
  debugger.load(node);                                                                                            
}

auto RDRAM::unload() -> void {
  debugger = {};
  ram.reset();
  ram.hiddenBits.reset();
  node.reset();
}

auto RDRAM::power(bool reset) -> void {
  if(!reset) {
    ram.fill();
    ram.hiddenBits.fill();
    for(auto& chip : chips) chip = {};
  }
}

}
