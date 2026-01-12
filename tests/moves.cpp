#define BIG_BISHOP_TABLE

#include "../src/chess/move/notPawn.h"
#include "../src/chess/QuadBoard.h"
#include "../src/chess/FigureTypes.h"

#include <iostream>


int main() {
  chess::QuadBoard board("rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR");
  std::cout << board.toString() << '\n';
  using Rook = std::integral_constant<int, chess::char2Code('Q')>;
  auto rooks = board.getPos(Rook());
  std::cout << chess::toString(rooks) << '\n';
  auto rook = ~(rooks - 1) & rooks;
  std::cout << chess::toString(rook) << '\n';
  auto next = chess::move::byCode(board.info(), rook, Rook());
  std::cout << chess::toString(next) << '\n';
}