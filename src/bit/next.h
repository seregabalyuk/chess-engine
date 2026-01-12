#pragma once

#include <cstdint>


namespace bit {
  uint64_t next(uint64_t& bits) {
    auto help = bits - 1;
    auto out = ~help & bits;
    bits &= help;
    return out;
  }
} // namespace bit