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
std::string make_move(const std::string& board_state, bool is_white) {
    chess::QuadBoard board(board_state);
    
    // Vectors to store next positions
    std::vector<chess::QuadBoard> nexts;
    std::vector<chess::QuadBoard> nextNexts; // For en passant, though nextPositions uses it for something else

    if (is_white) {
        chess::nextPositions(std::integral_constant<int, 0>{}, board, nexts, nextNexts);
    } else {
        chess::nextPositions(std::integral_constant<int, 1>{}, board, nexts, nextNexts);
    }

    if (nexts.empty()) {
        // No moves available (checkmate or stalemate)
        // For now, return the current board state or handle as game over
        return board_state; 
    }

    // Simple AI: just pick the first available move
    // logic can be improved here
    return nexts[0].toString();
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

