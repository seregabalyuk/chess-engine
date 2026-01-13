#pragma once

#include "../bit/reverse.h"
#include "../bit/next.h"
#include "BitBoard.h"
#include "UnwrapBoard.h"
#include "Codes.h"
#include "FigureTypes.h"
#include "attack/all.h"


namespace chess {
  struct QuadBoard {
    BitBoard boards[4];

    QuadBoard() = default;

    QuadBoard(const QuadBoard&) = default;

    QuadBoard(const std::string& fen) {
      int i = 0;
      int j = 0;
      for (char c: fen) {
        if (c == ' ') break;
        if (c == '/') {
          i += 8;
          j = 0;
        } else if (c >= '0' && c <= '9') {
          for (int k = 0; k < c - '0'; ++ k, ++ j) {
            set(i + j, char2Code('.'));
          }
        } else if (char2Code(c) != -1) {
          set(i + j, char2Code(c));
          ++ j;
        }
      }
    }

    QuadBoard& operator=(const QuadBoard&) & = default;

    bool operator < (const QuadBoard& other) const {
      if (boards[0] != other.boards[0]) 
        return boards[0] < other.boards[0];
      else if (boards[1] != other.boards[1]) 
        return boards[1] < other.boards[1];
      else if (boards[2] != other.boards[2])
        return boards[2] < other.boards[2];
      else
        return boards[3] < other.boards[3];
    }

    bool operator == (const QuadBoard& other) const {
      return 
        boards[0] == other.boards[0] &&
        boards[1] == other.boards[1] &&
        boards[2] == other.boards[2] &&
        boards[3] == other.boards[3];
    }

    int get(int i) const {
      return 
        getBB(boards[0], i)
      + getBB(boards[1], i) * (1 << 1)
      + getBB(boards[2], i) * (1 << 2)
      + getBB(boards[3], i) * (1 << 3);
    }

    QuadBoard& set(int i, int v) & {
      setBB(boards[0], i, v & (1 << 0));
      setBB(boards[1], i, v & (1 << 1));
      setBB(boards[2], i, v & (1 << 2));
      setBB(boards[3], i, v & (1 << 3));
      return *this;
    }

    QuadBoard& free(const BitBoard& i) & {
      fastSet(i, std::integral_constant<int,0>{});
      return *this;
    }

    template<int Code>
    QuadBoard& fastSet(const BitBoard& i, std::integral_constant<int, Code>) & {
      fastSetBB(boards[0], i, std::bool_constant<Code & 1>());
      fastSetBB(boards[1], i, std::bool_constant<Code & 2>());
      fastSetBB(boards[2], i, std::bool_constant<Code & 4>());
      fastSetBB(boards[3], i, std::bool_constant<Code & 8>());
      return *this;
    }

    template<int Code>
    BitBoard getPos(std::integral_constant<int, Code>) const {
      return 
          getPosBB(boards[0], std::bool_constant<Code & 1>())
        & getPosBB(boards[1], std::bool_constant<Code & 2>())
        & getPosBB(boards[2], std::bool_constant<Code & 4>())
        & getPosBB(boards[3], std::bool_constant<Code & 8>());
    }

    std::string toString() const {
      std::string ret;
      for (int i = 0; i < 64; ++ i) {
        ret.push_back(code2Char(get(i)));
        if (i % 8 == 7) {
          ret.push_back('\n');
        }
      }
      return ret;
    }

    std::string toFEN() const {
      std::string ret;
      for (int i = 0; i < 64; ++ i) {
        if (code2Char(get(i)) == '.') {
          if (ret.size() && '0' <= ret.back() && ret.back() <= '9') {
              ++ ret.back();
          } else {
              ret.push_back('1');
          }
        } else {
          ret.push_back(code2Char(get(i)));
        }
        if (i % 8 == 7) {
          ret.push_back('/');
        }
      }
      ret.pop_back();
      return ret;
    }

    std::string toCoolString() const {
      std::string ret;
      for (int i = 0; i < 64; ++ i) {
        ret += code2CoolChar(get(i));
        if (i % 8 == 7) {
          ret.push_back('\n');
        }
      }
      return ret;
    }

    BitBoard all() const {
      return 
        boards[0] 
      | boards[1]
      | boards[2]
      | boards[3];
    }

    BitBoard black() const {
      return boards[3];
    }

    BitBoard white() const {
      return all() ^ black();
    }

    UnwrapBoard info() const {
      return UnwrapBoard(UnwrapBoard::AllAndBlack{}, all(), black());
    }

    QuadBoard& invert() & {
      boards[0] = ~boards[0];
      boards[1] = ~boards[1];
      boards[2] = ~boards[2];
      boards[3] = ~boards[3];
      auto space = 
        boards[0] 
      & boards[1]
      & boards[2]
      & boards[3];
      boards[0] ^= space;
      boards[1] ^= space;
      boards[2] ^= space;
      boards[3] ^= space;
      return *this;
    }

    template<int Color>
    BitBoard attacks(std::integral_constant<int, Color>) const {
      using ColorType = std::integral_constant<int, Color>;
      BitBoard _all = all();
      BitBoard out = 0;
      {   // Rook
        auto poses = getPos(Rook<Color>());
        while (poses) {
          auto pos = bit::next(poses);
          out |= attack::rook(_all, pos);
        }
      } { // Bishop
        auto poses = getPos(Bishop<Color>());
        while (poses) {
          auto pos = bit::next(poses);
          out |= attack::bishop(_all, pos);
        }
      } { // Queen
        auto poses = getPos(Queen<Color>());
        while (poses) {
          auto pos = bit::next(poses);
          out |= attack::queen(_all, pos);
        }
      } { // Knight
        auto poses = getPos(Knight<Color>());
        out |= attack::knight(_all, poses);
      } { // King
        auto poses = getPos(King<Color>());
        out |= attack::king(_all, poses);
      } { // Pawn
        auto poses = getPos(Pawn<Color>());
        out |= attack::pawn(_all, poses);
      }
      return out;
    }

    QuadBoard& reverse() & {
      boards[0] = bit::reverse(boards[0]);
      boards[1] = bit::reverse(boards[1]);
      boards[2] = bit::reverse(boards[2]);
      boards[3] = bit::reverse(boards[3]);
      return *this;
    }

    QuadBoard& dualRotation() & {
      reverse();
      invert();
      return *this;
    }
  };

  QuadBoard startingPosition = 
    QuadBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
} // namespace chess