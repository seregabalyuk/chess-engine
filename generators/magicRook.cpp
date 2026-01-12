#include "../src/bit/findMagic.h"
#include "../src/chess/BitBoard.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>

template<class stream, class T>
void printArray(
  stream& fout,
  const std::string& name, 
  const T& array, 
  const std::string& sep = "\n    "
) {
  size_t size = 0;
  for (const auto& val: array) {
    val;
    ++ size;
    
  }
  fout << "  constexpr BitBoard " << name << std::dec 
  << "[" << size << "] = {";
  for (const auto& val: array) {
    fout << sep << "0x" 
    << std::hex << std::setw(16) << std::setfill('0') 
    << val
    << ",";
  }
  fout << sep << "};\n";
}

template<class stream>
int generateBigTable(stream& fout) {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  uint64_t attacks[64];
  uint64_t magic[64];
  uint64_t shift[64];
  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    attacks[id] = 0;
    for (int k = i + 1; k < 7; ++ k) {
      attacks[id] |= 1ULL << 8 * k + j;
    }
    for (int k = j + 1; k < 7; ++ k) {
      attacks[id] |= 1ULL << 8 * i + k;
    }
    for (int k = i - 1; k > 0; -- k) {
      attacks[id] |= 1ULL << 8 * k + j;
    }
    for (int k = j - 1; k > 0; -- k) {
      attacks[id] |= 1ULL << 8 * i + k;
    }
    shift[id] = 64 - bit::count(attacks[id]);
  }
  
  for (size_t i = 0; i < 64; ++ i) {
    magic[i] = bit::findMagic(attacks[i], gen);
  }

  std::vector<uint64_t> table;
  size_t bounds[64];

  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    bounds[id] = table.size();
    table.resize(bounds[id] +  (1ULL << bit::count(attacks[id])));
    for (uint64_t mask = attacks[id]; mask > 0; mask = (mask - 1) & attacks[id]) {
      uint64_t index = bounds[id] + (mask * magic[id] >> shift[id]);
      table[index] = 0;
      for (int k = i + 1; k < 8; ++ k) {
        table[index] |= 1ULL << 8 * k + j;
        if (mask & (1ULL << 8 * k + j)) break;
      }
      for (int k = j + 1; k < 8; ++ k) {
        table[index] |= 1ULL << 8 * i + k;
        if (mask & (1ULL << 8 * i + k)) break;
      }
      for (int k = i - 1; k >= 0; -- k) {
        table[index] |= 1ULL << 8 * k + j;
        if (mask & (1ULL << 8 * k + j)) break;
      }
      for (int k = j - 1; k >= 0; -- k) {
        table[index] |= 1ULL << 8 * i + k;
        if (mask & (1ULL << 8 * i + k)) break;
      }
      if (table[index] == 0) {
        std::cout 
        << chess::toString(attacks[id]) << '\n'
        << chess::toString(mask) << '\n'
        << chess::toString(table[index]) << '\n';
        return 1;
      }
    }
    for (size_t i = 1; i < (1ULL << bit::count(attacks[id])); ++ i) {
      uint64_t index = bounds[id] + i;
      if (table[index] == 0) {
        std::cout << i << ' ' << id  << '\n';
        return 0;
      }
    }
    table[bounds[id]] = 0;
    for (int k = i + 1; k < 8; ++ k) {
      table[bounds[id]] |= 1ULL << 8 * k + j;
    }
    for (int k = j + 1; k < 8; ++ k) {
      table[bounds[id]] |= 1ULL << 8 * i + k;
    }
    for (int k = i - 1; k >= 0; -- k) {
      table[bounds[id]] |= 1ULL << 8 * k + j;
    }
    for (int k = j - 1; k >= 0; -- k) {
      table[bounds[id]] |= 1ULL << 8 * i + k;
    }
    if (table[bounds[id]] == 0) {
      std::cout 
      << chess::toString(attacks[bounds[id]]) << '\n'
      << chess::toString(0) << '\n'
      << chess::toString(table[bounds[id]]) << '\n';
      return 1;
    }
  }
  fout << "namespace chess::attack {\n";
  printArray(fout, "attacksRook", attacks);
  printArray(fout, "shiftRook", shift);
  printArray(fout, "magicRook", magic);
  printArray(fout, "boundsRook", bounds);
  printArray(fout, "tableRook", table); //, " ");
  fout << "\n"
  << "  constexpr BitBoard rook(BitBoard all, BitBoard pos) {\n"
  << "    size_t id = bit::specLog2(pos);\n"
  << "    return tableRook[boundsRook[id] + \n" 
  << "      ((attacksRook[id] & all) * magicRook[id] >> shiftRook[id])];\n"
  << "  }\n"
  << "} // namespace::chess::attack\n";
  return 0;
}


template<class stream>
int generateRowTable(stream& fout) {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  uint64_t attacks[64];
  uint64_t magic[64];
  uint64_t shift[64];
  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    attacks[id] = 0;
    for (int k = j + 1; k < 7; ++ k) {
      attacks[id] |= 1ULL << 8 * i + k;
    }
    for (int k = j - 1; k > 0; -- k) {
      attacks[id] |= 1ULL << 8 * i + k;
    }
    shift[id] = 64 - bit::count(attacks[id]);
  }
  
  for (size_t i = 0; i < 64; ++ i) {
    magic[i] = bit::findMagic(attacks[i], gen);
  }

  std::vector<uint64_t> table;
  size_t bounds[64];

  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    bounds[id] = table.size();
    table.resize(bounds[id] +  (1ULL << bit::count(attacks[id])));
    for (uint64_t mask = attacks[id]; mask > 0; mask = (mask - 1) & attacks[id]) {
      uint64_t index = bounds[id] + (mask * magic[id] >> shift[id]);
      table[index] = 0;
      for (int k = j + 1; k < 8; ++ k) {
        table[index] |= 1ULL << 8 * i + k;
        if (mask & (1ULL << 8 * i + k)) break;
      }
      for (int k = j - 1; k >= 0; -- k) {
        table[index] |= 1ULL << 8 * i + k;
        if (mask & (1ULL << 8 * i + k)) break;
      }
      if (table[index] == 0) {
        std::cout 
        << chess::toString(attacks[id]) << '\n'
        << chess::toString(mask) << '\n'
        << chess::toString(table[index]) << '\n';
        return 1;
      }
    }
    for (size_t i = 1; i < (1ULL << bit::count(attacks[id])); ++ i) {
      uint64_t index = bounds[id] + i;
      if (table[index] == 0) {
        std::cout << i << ' ' << id  << '\n';
        return 0;
      }
    }
    table[bounds[id]] = 0;
    for (int k = j + 1; k < 8; ++ k) {
      table[bounds[id]] |= 1ULL << 8 * i + k;
    }
    for (int k = j - 1; k >= 0; -- k) {
      table[bounds[id]] |= 1ULL << 8 * i + k;
    }
    if (table[bounds[id]] == 0) {
      std::cout 
      << chess::toString(attacks[bounds[id]]) << '\n'
      << chess::toString(0) << '\n'
      << chess::toString(table[bounds[id]]) << '\n';
      return 1;
    }
  }
  printArray(fout, "rowMovesRook", attacks);
  printArray(fout, "rowShiftRook", shift);
  printArray(fout, "rowMagicRook", magic);
  printArray(fout, "rowBoundsRook", bounds);
  printArray(fout, "rowTableRook", table); //, " ");
  return 0;
}

template<class stream>
int generateColomnTable(stream& fout) {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  uint64_t attacks[64];
  uint64_t magic[64];
  uint64_t shift[64];
  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    attacks[id] = 0;
    for (int k = i + 1; k < 7; ++ k) {
      attacks[id] |= 1ULL << 8 * k + j;
    }
    for (int k = i - 1; k > 0; -- k) {
      attacks[id] |= 1ULL << 8 * k + j;
    }
    shift[id] = 64 - bit::count(attacks[id]);
  }
  
  for (size_t i = 0; i < 64; ++ i) {
    magic[i] = bit::findMagic(attacks[i], gen);
  }

  std::vector<uint64_t> table;
  size_t bounds[64];

  for (int i = 0; i < 8; ++ i) 
  for (int j = 0; j < 8; ++ j) {
    size_t id = 8 * i + j;
    bounds[id] = table.size();
    table.resize(bounds[id] +  (1ULL << bit::count(attacks[id])));
    for (uint64_t mask = attacks[id]; mask > 0; mask = (mask - 1) & attacks[id]) {
      uint64_t index = bounds[id] + (mask * magic[id] >> shift[id]);
      table[index] = 0;
      for (int k = i + 1; k < 8; ++ k) {
        table[index] |= 1ULL << 8 * k + j;
        if (mask & (1ULL << 8 * k + j)) break;
      }
      for (int k = i - 1; k >= 0; -- k) {
        table[index] |= 1ULL << 8 * k + j;
        if (mask & (1ULL << 8 * k + j)) break;
      }
      if (table[index] == 0) {
        std::cout 
        << chess::toString(attacks[id]) << '\n'
        << chess::toString(mask) << '\n'
        << chess::toString(table[index]) << '\n';
        return 1;
      }
    }
    for (size_t i = 1; i < (1ULL << bit::count(attacks[id])); ++ i) {
      uint64_t index = bounds[id] + i;
      if (table[index] == 0) {
        std::cout << i << ' ' << id  << '\n';
        return 0;
      }
    }
    table[bounds[id]] = 0;
    for (int k = i + 1; k < 8; ++ k) {
      table[bounds[id]] |= 1ULL << 8 * k + j;
    }
    for (int k = i - 1; k >= 0; -- k) {
      table[bounds[id]] |= 1ULL << 8 * k + j;
    }
    if (table[bounds[id]] == 0) {
      std::cout 
      << chess::toString(attacks[bounds[id]]) << '\n'
      << chess::toString(0) << '\n'
      << chess::toString(table[bounds[id]]) << '\n';
      return 1;
    }
  }
  printArray(fout, "colomnMovesRook", attacks);
  printArray(fout, "colomnShiftRook", shift);
  printArray(fout, "colomnMagicRook", magic);
  printArray(fout, "colomnBoundsRook", bounds);
  printArray(fout, "colomnTableRook", table);//, " ");
  return 0;
}

template<class stream>
int generateLightTable(stream& fout) {
  
  fout << "namespace chess::attack {\n";
  int row = generateRowTable(fout);
  int colomn = generateColomnTable(fout);
  if (row | colomn) {
    return row | colomn;
  }
  fout << "\n"
  << "  constexpr BitBoard rook(BitBoard all, BitBoard pos) {\n"
  << "    size_t id = bit::specLog2(pos);\n"
  << "    BitBoard row = rowTableRook[rowBoundsRook[id] +\n" 
  << "      ((rowMovesRook[id] & all) * rowMagicRook[id] >> rowShiftRook[id])];\n"
  << "    BitBoard colomn = colomnTableRook[colomnBoundsRook[id] +\n"
  << "      ((colomnMovesRook[id] & all) * colomnMagicRook[id] >> colomnShiftRook[id])];\n"
  << "    return row | colomn;\n"
  << "  }\n"
  << "} // namespace::chess::attack::rook\n";
  return 0;
}

std::string toHex(
  const std::string& str
) {
  std::string out;
  for (size_t i = 0; i < 16; ++ i) {
    int k = 0;
    for (size_t j = 0; j < 4; ++ j) {
      k += (int)(str[i * 4 + j] != '.') << j;
    }
    if (k < 10) {
      out.push_back('0' + k);
    } else {
      out.push_back('a' + k - 10);
    }
  }
  std::reverse(out.begin(), out.end());
  return "0x" + out;
}

template<class stream>
void gen_asserts(
  stream& fout,
  const std::string& all,
  const std::string& rook,
  const std::string& ans
) {
  fout << "static_assert(chess::attack::rook("
  << toHex(all) << ", "
  << toHex(rook) << ") == "
  << toHex(ans) << ", \"rook attacks work not correct\");\n";
}


int main() {
  
  std::ofstream fout("src/chess/attack/rook.h");
  fout 
  << "#pragma once\n\n"
  << "#include \"../../bit/log2.h\"\n"
  << "#include \"../BitBoard.h\"\n\n"
  << "#include \"../Codes.h\"\n\n"
  << "\n"
  << "#ifndef BIG_ROOK_TABLE\n";
  
  if (generateLightTable(fout)) {
    return 2;
  }
  fout << "#else\n";

  if (generateBigTable(fout)) {
    return 1;
  }
  fout << "#endif\n";
  fout 
    << "namespace chess::attack {\n"
    << "  constexpr BitBoard byCode(\n" 
    << "    const BitBoard& all,\n"
    << "    const BitBoard& pos,\n" 
    << "    std::integral_constant<int, char2Code('R')>\n"
    << "  ) {\n"
    << "    return rook(all, pos); \n"
    << "  }\n"
    << "\n"
    << "  constexpr BitBoard byCode(\n" 
    << "    const BitBoard& all,\n"
    << "    const BitBoard& pos,\n" 
    << "    std::integral_constant<int, char2Code('r')>\n"
    << "  ) {\n"
    << "    return rook(all, pos); \n"
    << "  }\n"
    << "} // namespace chess::attack\n";

  auto rook1 = std::string("") + 
  "........" + 
  "........" + 
  "........" + 
  "....X..." + 
  "........" + 
  "........" + 
  "........" + 
  "........";

  auto all1 = std::string("") + 
  "........" + 
  "..X...X." + 
  "........" + 
  "....X..." + 
  "........" + 
  "........" + 
  ".X....X." + 
  "........";
  
  auto res1 = std::string("") + 
  "....X..." + 
  "....X..." + 
  "....X..." + 
  "XXXX.XXX" + 
  "....X..." + 
  "....X..." + 
  "....X..." + 
  "....X...";


  auto rook2 = std::string("") + 
  "........" + 
  "........" + 
  "......X." + 
  "........" + 
  "........" + 
  "........" + 
  "........" + 
  "........";

  auto all2 = std::string("") + 
  "......X." + 
  "........" + 
  "X.X...X." + 
  "........" + 
  ".X....X." + 
  "....X..." + 
  "......X." + 
  "......X.";
  
  auto res2 = std::string("") + 
  "......X." + 
  "......X." + 
  "..XXXX.X" + 
  "......X." + 
  "......X." + 
  "........" + 
  "........" + 
  "........";

  fout << "\n\n";
  gen_asserts(fout, all1, rook1, res1);
  gen_asserts(fout, all2, rook2, res2);
}