#pragma once 

#include "QuadBoard.h"
#include "Codes.h"


namespace chess {
  float eval(const QuadBoard& board) {
    BitBoard pawns = board.getPos(Pawn<1>());
    BitBoard pawns_black = board.getPos(Pawn<0>());
    BitBoard knights = board.getPos(Knight<1>());
    BitBoard knights_black = board.getPos(Knight<0>());
    BitBoard bishops = board.getPos(Bishop<1>());
    BitBoard bishops_black = board.getPos(Bishop<0>());
    BitBoard rooks = board.getPos(Rook<1>());
    BitBoard rooks_black = board.getPos(Rook<0>());
    BitBoard queens = board.getPos(Queen<1>());
    BitBoard queens_black = board.getPos(Queen<0>());
    BitBoard kings = board.getPos(King<1>());
    BitBoard kings_black = board.getPos(King<0>());
    return 
      countBB(pawns) * 100 +
      countBB(knights) * 300 +
      countBB(bishops) * 300 +
      countBB(rooks) * 500 +
      countBB(queens) * 900 +
      countBB(pawns_black) * 100 -
      countBB(knights_black) * 300 -
      countBB(bishops_black) * 300 -
      countBB(rooks_black) * 500 -
      countBB(queens_black) * 900;
  }
} // namespace chess