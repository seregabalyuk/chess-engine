# Chess Engine Site

This folder contains a simple Full-Stack application to interact with the C++ Chess Engine.

## Structure

*   `backend/`: Python FastAPI application acting as a bridge between the Frontend and the C++ Engine.
*   `frontend/`: React application (Vite) for the User Interface.

## Prerequisites

1.  **C++ Engine**: Built and running (provides gRPC server).
    *   See root `README.md` and `Makefile`.
    *   Ensure you have installed gRPC dependencies (`sudo apt-get install protobuf-compiler-grpc libgrpc++-dev` etc.).
2.  **Node.js**: For running the frontend.
    *   Install via your package manager (e.g., `sudo apt install nodejs npm`) or using [nvm](https://github.com/nvm-sh/nvm).
3.  **Python 3.9+**: For the backend.
4.  **uv**: For Python dependency management.
    *   Install with: `curl -LsSf https://astral.sh/uv/install.sh | sh`

## Installation & Setup

### 1. Build and Run C++ Engine

From the root of the repo:

```bash
# Build and start the gRPC server
make dev
```
*Keep this terminal open. The server runs on port 50051.*

### 2. Setup Backend

Open a new terminal.

```bash
cd site/backend

# Install dependencies using uv
uv venv
source .venv/bin/activate
uv pip install -e .

# Generate gRPC Python Code
python -m grpc_tools.protoc -I../../protos --python_out=. --grpc_python_out=. ../../protos/chess.proto

# Run the Backend
uvicorn main:app --reload
```
*The backend runs on http://127.0.0.1:8000.*

### 3. Setup Frontend

Open a third terminal.

```bash
cd site/frontend

# Install dependencies
npm install

# Run the Frontend
npm run dev
```
*The frontend typically runs on http://localhost:5173.*

Note: The frontend now uses `chessboard.js` (via CDN) and `jquery`. These are loaded dynamically.

## Usage

1.  Open your browser to the Frontend URL (e.g., http://localhost:5173).
2.  Click "Play White" or "Play Black".
3.  Make a move by dragging and dropping pieces.
4.  The Frontend sends the move to the Python Backend.
5.  The Python Backend asks the C++ Engine for a response move via gRPC.
6.  The C++ Engine calculates and returns the new board state (position string).
7.  The board updates with the engine's move.

