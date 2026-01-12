//#define BIG_BISHOP_TABLE

#include "../src/chess/attack/bishop.h"
#include "../src/chess/attack/rook.h"
#include "../src/chess/attack/king.h"
#include "../src/chess/attack/knight.h"
#include "../src/chess/attack/pawn.h"
#include "../src/chess/attack/queen.h"

#include <iostream>


int main() {
  chess::BitBoard pos = 0;
  chess::setBB(pos, 31, 1);
  //chess::setBB(pos, 24, 1);
  std::cout << chess::toString(pos) << '\n';
  std::cout << chess::toString(chess::attack::queen(0, pos)) << '\n';
}