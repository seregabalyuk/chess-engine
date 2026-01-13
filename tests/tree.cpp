#define BIG_BISHOP_TABLE

#include "../src/chess.h"

#include <iostream>


int main() {
  chess::QuadBoard a("8/8/8/8/p1p5/8/PPPPPPPP/8");
  chess::QuadBoard b("8/8/8/8/pPp5/8/P1PPPPPP/8");
  
  chess::Tree tree(b, a, 0);

  for (auto& a: tree.nodes) {
    std::cout << a.board.toString() << '\n';
  }
}