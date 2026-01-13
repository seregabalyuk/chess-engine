#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "chess.grpc.pb.h"

#include "chess/QuadBoard.h"
#include "chess/nextPositions.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chess_grpc::Chess;
using chess_grpc::MoveRequest;
using chess_grpc::MoveResponse;

// Function to make a move based on position and color
// Returns the new board state as a string (visual representation or FEN depending on QuadBoard::toString)

template<int Color>
bool better(float& value, float num, std::integral_constant<int, 1>) {
    if (value < num) {
        value = num;
        return true;
    }
    return false;
}

template<int Color>
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
    chess::QuadBoard ret;
    constexpr auto color = std::integral_constant<int, Color>();
    constexpr auto colorNext = std::integral_constant<int, Color == 0>();
    
    
    float value = Color ? -10000000: 100000000;
    {
      auto info = pos.info();
      move::Generator gen(pos, info);
      chess::move::Generator gen(board, info);
      if(gen.changeFigure(chess::Rook<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Rook<Color>());
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Bishop<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Bishop<Color>());
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Knight<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Knight<Color>());
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::King<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::King<Color>());
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Queen<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Queen<Color>());
         float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
      if(gen.changeFigure(chess::Pawn<Color>())) {
        while (!gen.isEmpty()) {
          auto next = gen.next(chess::Pawn<Color>());
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
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
          float num = alphabeta(next, deep - 1, -1000000, 1000000, colorNext);
          if (better(value, num, color)) {
            ret = next;
          }
        }
      }
    }
    // Simple AI: just pick the first available move
    // logic can be improved here
    return ret.toString();
}

class ChessServiceImpl final : public Chess::Service {
    Status MakeMove(ServerContext* context, const MoveRequest* request,
                  MoveResponse* reply) override {
        std::string new_board_state = make_move(request->board_state(), request->is_white());
        reply->set_board_state(new_board_state);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ChessServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}

