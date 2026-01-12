#pragma once

#include "all.h"
#include "../QuadBoard.h"
#include "../../bit/next.h"


namespace chess::move {
  template<class Info = InfoBoard>
  class GeneratorPassant {
    const QuadBoard* board;
    const Info* info;

    BitBoard pawnThis;
    BitBoard pawnOther;
    BitBoard pawnNow;
    BitBoard pawnPrev;
    BitBoard pawnEats;
    BitBoard pawnEatFrom;
    BitBoard pawnEatTo;
    
   public:
    GeneratorPassant(
      const QuadBoard& board, 
      const Info& info
    ): board(&board), info(&info) {}

    template<int Color>
    bool changeColor(std::integral_constant<int, Color>) & {
      using PawnThis = Pawn<Color>;
      constexpr BitBoard rowThis = Color ? 0x000000000000FF00 : 0x00FF000000000000;
      pawnThis = board->getPos(PawnThis()) & rowThis;
      if (!pawnThis) return 0;
      if constexpr (Color) pawnThis <<= 8;
      else       pawnThis >>= 8;
      pawnThis &= info->getNotAll();
      if constexpr (Color) pawnThis <<= 8;
      else       pawnThis >>= 8;
      pawnThis &= info->getNotAll();
      if (!pawnThis) return 0;
      pawnNow = bit::next(pawnThis);

      using PawnOther = Pawn<Color == 1 ? 0 : 1>;
      constexpr BitBoard rowOther = Color ? 0x00000000FF000000 : 0x000000FF00000000;
      pawnOther = board->getPos(PawnOther()) & rowOther;
      
      return 1;
    }

    template<int Color>
    QuadBoard nextThis(std::integral_constant<int, Color>) & {
      QuadBoard out = *board;
      pawnPrev = pawnNow;
      if constexpr (Color) pawnPrev >>= 16;
      else       pawnPrev <<= 16;
      out.free(pawnPrev);
      out.fastSet(pawnNow, Pawn<Color>());
      pawnEats = ((pawnNow * 5) >> 1) & pawnOther;
      pawnEatFrom = bit::next(pawnEats);
      pawnEatTo = Color == 1 ? (pawnNow >> 8) : (pawnNow << 8);
      pawnNow = bit::next(pawnThis);
      return out;
    }

    template<class Container, int Color>
    void nextThis(Container& container, std::integral_constant<int, Color>) & {
      container.emplace_back(*board);
      pawnPrev = pawnNow;
      if constexpr (Color) pawnPrev >>= 16;
      else       pawnPrev <<= 16;
      container.back().free(pawnPrev);
      container.back().fastSet(pawnNow, Pawn<Color>());
      pawnEats = ((pawnNow * 5) >> 1) & pawnOther;
      pawnEatFrom = bit::next(pawnEats);
      pawnEatTo = Color == 1 ? (pawnNow >> 8) : (pawnNow << 8);
      pawnNow = bit::next(pawnThis);
    }

    template<int Color>
    QuadBoard nextOther(std::integral_constant<int, Color>) & {
      QuadBoard out = *board;
      out.free(pawnPrev);
      out.free(pawnEatFrom);
      out.fastSet(pawnEatTo, Pawn<Color == 1 ? 0 : 1>());
      pawnEatFrom = bit::next(pawnEats);
      return out;
    }

    template<class Container, int Color>
    void nextOther(Container& container, std::integral_constant<int, Color>) & {
      container.emplace_back(*board);
      container.back().free(pawnPrev);
      container.back().free(pawnEatFrom);
      container.back().fastSet(pawnEatTo, Pawn<Color == 1 ? 0 : 1>());
      pawnEatFrom = bit::next(pawnEats);
    }

    bool isEmptyThis() const { return !pawnNow; }

    bool isEmptyOther() const { return !pawnEatFrom; }
  };
}