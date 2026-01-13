#pragma once

#include "QuadBoard.h"
#include "move/Generator.h"
//#include "move/GeneratorPassant.h"
#include "eval.h"

#include <iostream>


namespace chess {
  
  inline float minmax(float a, float b, std::integral_constant<int, 1>) {
    return a > b ? a : b; 
  }

  inline float minmax(float a, float b, std::integral_constant<int, 0>) {
    return a < b ? a : b; 
  }
  
  inline bool returnminmax(float alpha, float beta, float val, std::integral_constant<int, 1>) {
    return val >= beta; 
  }

  inline bool returnminmax(float alpha, float beta, float val, std::integral_constant<int, 0>) {
    return val <= alpha; 
  }

  inline void setalphabeta(float& alpha, float& beta, float val, std::integral_constant<int, 1>) {
    alpha = alpha < val ? val : alpha;
  }

  inline void setalphabeta(float& alpha, float& beta, float val, std::integral_constant<int, 0>) {
    beta = beta > val ? val : beta;
  }


  template<int Color>
  float alphabeta(const QuadBoard& board, int deep, float alpha, float beta, std::integral_constant<int, Color>) {
    //std::cout << deep << '\n';
    if (deep == 0) {
      return eval(board);
    }
    constexpr auto color = std::integral_constant<int, Color>();
    constexpr auto colorNext = std::integral_constant<int, Color == 0>();
    
    
    float value = Color ? -10000000: 100000000;
    auto info = board.info();
    {
      chess::move::Generator gen(board, info);
      if(gen.changeFigure(chess::Rook<Color>())) {
         while (!gen.isEmpty()) {
          auto next = gen.next(chess::Rook<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
      if(gen.changeFigure(chess::Bishop<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Bishop<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
      if(gen.changeFigure(chess::Knight<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Knight<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
      if(gen.changeFigure(chess::King<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::King<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
      if(gen.changeFigure(chess::Queen<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Queen<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
      if(gen.changeFigure(chess::Pawn<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Pawn<Color>());
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      } 
    }
    {
      chess::move::GeneratorPassant genPas(board, info);
      if(genPas.changeColor(color)) {
        while (!genPas.isEmptyThis()) {
          auto next = genPas.nextThis(color);
          value = minmax(value, alphabeta(next, deep - 1, alpha, beta, colorNext), color);
          if (returnminmax(alpha, beta, value, color)) return value;
          setalphabeta(alpha, beta, value, color);
        }
      }
    }
    return value;
  }

} // namespace chess