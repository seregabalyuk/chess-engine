#pragma once

#include <string>

namespace chess {
  constexpr char code2Char(int code) {
    switch (code) {
    case 0:  return '.';
    case 1:  return 'K';
    case 2:  return 'Q';
    case 3:  return 'B';
    case 4:  return 'N';
    case 5:  return 'R';
    case 6:  return 'P';
    case 7:  return 'S';
    case 8:  return 's';
    case 9:  return 'p';
    case 10: return 'r';
    case 11: return 'n';
    case 12: return 'b';
    case 13: return 'q';
    case 14: return 'k';
    default: return '?';
    }
  }

  constexpr std::string code2CoolChar(int code) {
    switch (code) {
    case 0:  return ".";
    case 1:  return "\033[91mK\033[0m";
    case 2:  return "\033[91mQ\033[0m";
    case 3:  return "\033[91mB\033[0m";
    case 4:  return "\033[91mN\033[0m";
    case 5:  return "\033[91mR\033[0m";
    case 6:  return "\033[91mP\033[0m";
    case 7:  return "\033[91mS\033[0m";
    case 8:  return "\033[94ms\033[0m";
    case 9:  return "\033[94mp\033[0m";
    case 10: return "\033[94mr\033[0m";
    case 11: return "\033[94mn\033[0m";
    case 12: return "\033[94mb\033[0m";
    case 13: return "\033[94mq\033[0m";
    case 14: return "\033[94mk\033[0m";
    default: return "?";
    }
  }

  constexpr int char2Code(char c) {
    switch (c) {
    case '.': return 0;
    case 'K': return 1;
    case 'Q': return 2;
    case 'B': return 3;
    case 'N': return 4;
    case 'R': return 5;
    case 'P': return 6;
    case 'S': return 7;
    case 's': return 8;
    case 'p': return 9;
    case 'r': return 10;
    case 'n': return 11;
    case 'b': return 12;
    case 'q': return 13;
    case 'k': return 14;
    default:  return -1;
    }
  }

  constexpr int colorByCode(int code) {
    return (code & (1 << 3)) ? 1 : 0;
  }
} // namespace chess