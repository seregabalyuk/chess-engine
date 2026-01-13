#pragma once

#include "nextPosition.h"
#include <vector>
#include <list>


namespace chess {
  struct Engine {
    class Info {
      size_t info;
     public:
      size_t prev() const {
        return info >> 1;
      }
      int color() const {
        return info & 1;
      }
      Info(size_t prev, int Color): info((prev << 1) | (Color == 1)) {}
      Info(const Info&) = default;
    };

    struct Link {
      Info info;
      std::vector<QuadBoard>* boards;
      std::vector<Info>* infos;
      void emplace_back(const QuadBoard& board) {
        boards->emplace_back(board);
        infos->emplace_back(info);
      }
      QuadBoard& back() { return boards->back(); }
    };
    
    std::vector<QuadBoard> boards;
    std::vector<Info> infos;


    Engine(size_t reserve = 200000)  {
      boards.reserve(reserve);
      infos.reserve(reserve);
    }
    Engine(const QuadBoard& board, int Color, size_t reserve = 200000): Engine(reserve) {
      boards.emplace_back(board);
      infos.emplace_back(0, Color);
    }


    template<int Color>
    void _next(size_t id, std::integral_constant<int, Color>) {
      constexpr auto color = std::integral_constant<int, Color>();
      Link link(Info(id, Color == 0), &boards, &prevs);
      const auto& board = boards[id];
      auto info = board.info();
     // moves
      chess::move::Generator gen(board, info);
      if(gen.changeFigure(chess::Rook<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::Rook<Color>());
        }
      }
      if(gen.changeFigure(chess::Bishop<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::Bishop<Color>());
        }
      }
      if(gen.changeFigure(chess::Knight<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::Knight<Color>());
        }
      }
      if(gen.changeFigure(chess::King<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::King<Color>());
        }
      }
      if(gen.changeFigure(chess::Queen<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::Queen<Color>());
        }
      }
      if(gen.changeFigure(chess::Pawn<Color>())) {
        while (!gen.isEmpty()) {
          gen.next(link, chess::Pawn<Color>());
        }
      }

     // pawn passant moves
      chess::move::GeneratorPassant genPas(board, info);
      if(genPas.changeColor(color)) {
        while (!genPas.isEmptyThis()) {
          link.prev = Info(id, Color == 0);
          genPas.nextThis(link, color);
          link.prev = Info(boards.size() - 1, Color);
          while (!genPas.isEmptyOther()) {
            genPas.nextOther(nextNexts, color);
          }
        }
      }
    }

    void next(size_t id) {
      if (infos[id].color() == 1) {
        _next(id, std::integral_constant<int, 1>());
      } else {
        _next(id, std::integral_constant<int, 0>());
      }
    }
  }
} // namespace chess