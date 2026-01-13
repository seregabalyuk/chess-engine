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
    turn: str = "w"

# gRPC channel setup
# Connect to the C++ server running on localhost:50051
channel = grpc.insecure_channel('localhost:50051')
stub = chess_pb2_grpc.ChessStub(channel) if 'chess_pb2_grpc' in locals() else None

@app.post("/move")
async def make_move(request: MoveRequest):
    print(f"--- Move Request Received ---")
    print(f"Frontend FEN: {request.fen}")
    print(f"Frontend Turn: {request.turn}")

    if stub is None:
        print("ERROR: gRPC stub is None")
        raise HTTPException(status_code=503, detail="gRPC stub not initialized. Is the generated code present?")

    # Initialize board with the position string
    # If the FEN is incomplete (only position), chess.Board might default to White turn.
    # We must set the turn explicitly to ensure correct logic if we depend on board.turn
    # board = chess.Board(request.fen)
    # board.turn = chess.WHITE if request.turn == 'w' else chess.BLACK
    
    # if board.is_game_over():
    #     return {"fen": board.fen(), "game_over": True}

    # Determine whose turn it is
    # is_white_turn = board.turn == chess.WHITE
    is_white_turn = request.turn == 'w'
    print(f"Calculated is_white_turn: {is_white_turn}")
    
    grpc_request = chess_pb2.MoveRequest(
        board_state=request.fen,
        is_white=is_white_turn
    )

    try:
        print("Sending request to C++ engine...")
        response = stub.MakeMove(grpc_request)
        new_fen = response.board_state
        print(f"Engine response FEN: {new_fen}")
        
        # Check if engine returned an error (same state)
        if new_fen == request.fen:
             print("ERROR: Engine returned same FEN (Stalemate/Checkmate/Error)")
             return {"fen": new_fen, "game_over": True, "error": "No move returned by engine (Stalemate/Checkmate/Error)"}

        # The C++ engine returns a FEN-like string, but potentially only the board part or visual string.
        # If the frontend expects a full FEN or can handle partial, we just return what we got.
        # However, to be safe for the frontend's next turn, we might want to append turn info if missing?
        # But let's trust the frontend/engine contract for now or return as is.
        # If new_fen is just position, the frontend will need to know it's the other player's turn now.
        # But wait, if C++ returns visual string, we might need to parse it back to FEN?
        # Let's return raw new_fen for now as requested.
        
        print("Returning success response to frontend")
        return {"fen": new_fen, "game_over": False}

    except grpc.RpcError as e:
        print(f"gRPC Error: {e.details()}")
        raise HTTPException(status_code=500, detail=f"gRPC error: {e.details()}")
    except Exception as e:
        print(f"General Error: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/health")
def health():
    return {"status": "ok"}
