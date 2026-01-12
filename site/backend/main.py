import fastapi
import chess
import grpc
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import sys
import os

# Add current directory to path so we can import generated modules
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

# Import generated grpc code (will be generated in the next step)
# Assuming they will be named chess_pb2 and chess_pb2_grpc
try:
    import chess_pb2
    import chess_pb2_grpc
except ImportError:
    # This might fail before generation, handled in setup instructions
    pass

app = FastAPI()

class MoveRequest(BaseModel):
    fen: str

# gRPC channel setup
# Connect to the C++ server running on localhost:50051
channel = grpc.insecure_channel('localhost:50051')
stub = chess_pb2_grpc.ChessStub(channel) if 'chess_pb2_grpc' in locals() else None

@app.post("/move")
async def make_move(request: MoveRequest):
    print(f"Received FEN: {request.fen}")
    if stub is None:
        raise HTTPException(status_code=503, detail="gRPC stub not initialized. Is the generated code present?")

    board = chess.Board(request.fen)
    
    if board.is_game_over():
        return {"fen": board.fen(), "game_over": True}

    # Determine whose turn it is
    # The C++ engine needs to know the color it is playing
    # If the user just moved (which is what we assume happened before this call),
    # then the current turn in FEN is the engine's turn.
    is_white_turn = board.turn == chess.WHITE
    
    # Prepare gRPC request
    # The C++ engine expects board_state and is_white (true for White, false for Black)
    # logic in C++ main.cpp:
    # if (is_white) nextPositions(..., 0, ...) -> 0 is White in that codebase?
    # Let's double check C++ main.cpp logic.
    # main.cpp:
    # if (is_white) { chess::nextPositions(..., 0, ...) } else { chess::nextPositions(..., 1, ...) }
    # Engine.h: Info(..., Color) -> (Color == 1) usually means Black or White depending on convention.
    # FigureTypes.h: Rook<0> is 'R' (White), Rook<1> is 'r' (Black).
    # So 0 is White, 1 is Black.
    # So if is_white is true, it calls nextPositions with 0 (White).
    # So passing is_white=True means generate moves for White.
    
    grpc_request = chess_pb2.MoveRequest(
        board_state=request.fen,
        is_white=is_white_turn
    )

    try:
        response = stub.MakeMove(grpc_request)
        new_fen = response.board_state
        
        # Verify if the returned FEN is valid
        # The C++ engine returns a FEN-like string.
        # If it returns the same board state, it might mean no moves found.
        
        # Let's check if new_fen is different
        if new_fen == request.fen:
             # Checkmate or stalemate or error
             # If game was not over, but engine returned same state, it implies no move could be made.
             return {"fen": new_fen, "game_over": True, "error": "No move returned by engine (Stalemate/Checkmate/Error)"}

        return {"fen": new_fen, "game_over": False}

    except grpc.RpcError as e:
        raise HTTPException(status_code=500, detail=f"gRPC error: {e.details()}")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/health")
def health():
    return {"status": "ok"}

