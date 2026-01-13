#define BIG_BISHOP_TABLE

#include "../src/chess.h"

#include <iostream>


bool better(float& value, float num, std::integral_constant<int, 1>) {
    if (value < num) {
        value = num;
        return true;
    }
    return false;
}

bool better(float& value, float num, std::integral_constant<int, 0>) {
    if (value > num) {
        value = num;
        return true;
    }
    return false;
}

template<int Color>
std::string make_move(const std::string& board_state, int deep, std::integral_constant<int, Color>) {
    chess::QuadBoard board(board_state);
    
    // Vectors to store next positions
    chess::QuadBoard ret(board);
    constexpr auto color = std::integral_constant<int, Color>();
    constexpr auto colorNext = std::integral_constant<int, Color == 0>();
    
    
    float value = Color ? -10000000: 100000000;
    auto info = board.info();
    {
      chess::move::Generator gen(board, info);
      if(gen.changeFigure(chess::Rook<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Rook<Color>());
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Bishop<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Bishop<Color>());
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Knight<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Knight<Color>());
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::King<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::King<Color>());
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Queen<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Queen<Color>());
         float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Pawn<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Pawn<Color>());
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      } 
    }
    {
      chess::move::GeneratorPassant genPas(board, info);
      if(genPas.changeColor(color)) {
        while (!genPas.isEmptyThis()) {
          auto next = genPas.nextThis(color);
          float num = chess::alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
    }
    // Simple AI: just pick the first available move
    // logic can be improved here

    //std::cout << ret.toString() << '\n';
    //std::cout << ret.toFEN() << '\n';
    return ret.toFEN();
}


int main() {
  chess::QuadBoard a(chess::startingPosition);

  /*auto info = a.info();
  chess::move::Generator gen(a, info);
  std::cout << "ok\n";
  if(gen.changeFigure(chess::Rook<1>())) {
      std::cout << "ok\n";
    while (!gen.isEmpty()) {
      std::cout << "ok\n";
      auto next = gen.next(chess::Rook<1>());
      std::cout << next.toString() << '\n';
      //float ret = alphabeta(next, deep - 1, alpha, beta, colorNext);
      //value = minmax(value, , color);
      //if (returnminmax(alpha, beta, value, color)) return value;
      //setalphabeta(alpha, beta, value, color);
    }
  }*/
      
  
  float t = chess::alphabeta(a, 6, -100000, 100000, std::integral_constant<int, 1>());

  std::cout << t << '\n';

  std::cout << make_move("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR", 4, std::integral_constant<int, 1>()) << '\n';
}