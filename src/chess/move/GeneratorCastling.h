#pragma once


#include "all.h"
#include "../QuadBoard.h"
#include "../../bit/next.h"


namespace chess::move {
  template<class Info = InfoBoard>
  class GeneratorCastlong {
    const QuadBoard* board;
    const Info* info;

   public:
    GeneratorCastlong(
      const QuadBoard& board, 
      const Info& info
    ): board(&board), info(&info) {}

    void next() {

    }
  };
}