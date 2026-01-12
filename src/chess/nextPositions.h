#pragma once

#include "move/Generator.h"
#include "move/GeneratorPassant.h"
#include "move/GeneratorCastling.h"



namespace chess {
  template<int Color, class Container1, class Container2>
  void nextPositions(
    std::integral_constant<int, Color>,
    const QuadBoard& board,
    Container1& nexts,
    Container2& nextNexts
  ) {
    constexpr auto color = std::integral_constant<int, Color>();
    auto info = board.info();
    // moves
    chess::move::Generator gen(board, info);
    if(gen.changeFigure(chess::Rook<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::Rook<Color>());
      }
    }
    if(gen.changeFigure(chess::Bishop<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::Bishop<Color>());
      }
    }
    if(gen.changeFigure(chess::Knight<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::Knight<Color>());
      }
    }
    if(gen.changeFigure(chess::King<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::King<Color>());
      }
    }
    if(gen.changeFigure(chess::Queen<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::Queen<Color>());
      }
    }
    if(gen.changeFigure(chess::Pawn<Color>())) {
      while (!gen.isEmpty()) {
        gen.next(nexts, chess::Pawn<Color>());
      }
    }

    // pawn passant moves
    chess::move::GeneratorPassant genPas(board, info);
    if(genPas.changeColor(color)) {
      while (!genPas.isEmptyThis()) {
        genPas.nextThis(nexts, color);
        while (!genPas.isEmptyOther()) {
          genPas.nextOther(nextNexts, color);
        }
      }
    }

    // castling moves 
    // not realizate
  }
} // namespace chess