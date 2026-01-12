#pragma once

#include "all.h"
#include "../QuadBoard.h"
#include "../../bit/next.h"
#include "../Codes.h"


namespace chess::move {
  template<class Info = UnwrapBoard>
  class Generator {
    const Info* info;
    const QuadBoard* board;
    
    QuadBoard withoutFigure;
    BitBoard moves;
    BitBoard nextPoses;
    BitBoard pos;

    template<int Code>
    void _updatePos(std::integral_constant<int, Code>) {
      using Figure = std::integral_constant<int, Code>;
      if (!moves) {
        while (!moves && pos) {
          pos = bit::next(nextPoses);
          moves = byCode(*info, pos, Figure());
        }
        if (pos) {
          withoutFigure = *board;
          withoutFigure.free(pos);
        }
      }
    }
   public:

    Generator(const QuadBoard& board, const Info& info):
      board(&board), info(&info)
    {}

    template<int Code>
    bool changeFigure(std::integral_constant<int, Code>) & {
      using Figure = std::integral_constant<int, Code>;
      nextPoses = board->getPos(Figure());
      if (!nextPoses) return false;
      pos = bit::next(nextPoses);
      moves = byCode(*info, pos, Figure());
      if (moves) {
        withoutFigure = *board;
        withoutFigure.free(pos);
      }
      _updatePos(Figure());
      return true;
    }

    template<int Code>
    QuadBoard next(std::integral_constant<int, Code>) {
      using Figure = std::integral_constant<int, Code>;
      auto help = moves - 1;
      auto oneMove = bit::next(moves);
      QuadBoard out = withoutFigure;
      _updatePos(Figure());
      return out.fastSet(oneMove, Figure());
    }

    template<class Container, int Code>
    void next(Container& container, std::integral_constant<int, Code>) {
      using Figure = std::integral_constant<int, Code>;
      auto help = moves - 1;
      auto oneMove = bit::next(moves);
      container.emplace_back(withoutFigure);
      // pawn transform check
      if constexpr (Code == char2Code('p') || Code == char2Code('P')) {
        constexpr int Color = colorByCode(Code);
        constexpr BitBoard check = 0xFF000000000000FF;
        if (oneMove & check) {
          container.back().fastSet(oneMove, Queen<Color>());
          container.emplace_back(withoutFigure);
          container.back().fastSet(oneMove, Knight<Color>());
          _updatePos(Figure());
          return;
        }
      } 
      container.back().fastSet(oneMove, Figure());
      _updatePos(Figure());
      
    }

    bool isEmpty() const { return !pos; }
  };
}