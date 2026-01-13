

#include <iostream>
#include <vector>
#include <algorithm>

#include "../src/chess.h"

std::string draw(const std::vector<chess::QuadBoard>& boards) {
  std::string out;
  for (int row = 0; row < 64; row += 8) {
    for (size_t i = 0; i < boards.size(); ++ i) {
      for (int colomn = 0; colomn < 8; ++ colomn) {
        out.push_back(chess::code2Char(boards[i].get(row + colomn)));
      }
      out += "  ";
    }
    out += "\n";
  }
  return out;
}

template<int Color>
void next(
  const std::vector<chess::QuadBoard>& prev,
  std::integral_constant<int, Color>,
  std::vector<chess::QuadBoard>& out,
  std::vector<chess::QuadBoard>& outNext
) {
  auto color = std::integral_constant<int, Color>();
  for (auto& board: prev) {
    nextPositions(color, board, out, outNext);
  }
  //std::sort(out.begin(), out.end());
  //out.resize(std::unique(out.begin(), out.end()) - out.begin());
}


int main() {
  auto moves = std::vector(1, std::vector(1, 
    chess::startingPosition
  ));
  moves[0][0].dualRotation();
  std::cout << moves[0][0].toString() << '\n';
  moves.emplace_back();
  moves.emplace_back();
  next(moves[0], std::integral_constant<int, 1>(), moves[1], moves[2]);
  /*moves.emplace_back(next(moves.back(), std::integral_constant<int, 1>()));
  moves.emplace_back(next(moves.back(), std::integral_constant<int, 0>()));
  moves.emplace_back(next(moves.back(), std::integral_constant<int, 1>()));
  moves.emplace_back(next(moves.back(), std::integral_constant<int, 0>()));*/
  /*std::vector<chess::QuadBoard> out;
  for (auto& board: moves.back()) {
    if (board.getPos(chess::King<1>()) == 0) {
      out.emplace_back(board);
    }
  }
  std::cout << out.size() << '\n';
  std::cout << draw(out) << '\n';
  for (auto& move: moves)
    std::cout << move.size() << '\n';
  
  */
  /*for (auto& board: moves[1]) {
    if (board.getPos(chess::King<1>()) == 0) {
      out.emplace_back(board);
    }
  }*/
  std::cout << draw(moves[1]) << '\n';
  std::cout << draw(moves[2]) << '\n';
  std::cout << '\n';
}