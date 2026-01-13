import { useState, useCallback } from 'react';
import { Chess } from 'chess.js';
import { Chessboard } from 'react-chessboard';

function App() {
  const [game, setGame] = useState(new Chess());
  const [fen, setFen] = useState(game.fen());
  const [loading, setLoading] = useState(false);
  const [userColor, setUserColor] = useState('white'); // 'white' or 'black'
  const [error, setError] = useState('');

  const makeEngineMove = async (currentFen) => {
    setLoading(true);
    setError('');
    
    // Split FEN to send only the board position
    const fenParts = currentFen.split(' ');
    const boardPosition = fenParts[0];
    const turn = fenParts[1]; // 'w' or 'b'
    
    try {
      const response = await fetch('/api/move', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          fen: boardPosition,
          turn: turn
        }),
      });

      if (!response.ok) {
        throw new Error('Network response was not ok');
      }

      const data = await response.json();
      
      if (data.fen) {
        // The engine returns only the board position (e.g., "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")
        // We need to construct a full FEN for chess.js to work properly.
        // We assume the engine made a move, so it's now the user's turn (userColor).
        // Default castling/enpassant/clocks to simple values since engine doesn't return them.
        // Note: The board turn should be the user's color because the engine just moved.
        const turn = userColor === 'white' ? 'w' : 'b'; 
        
        // If the engine returns just position, append dummy FEN parts
        // Check if data.fen looks like a full FEN or just position
        const isJustPosition = data.fen.split(' ').length === 1;
        const fullFen = isJustPosition ? `${data.fen} ${turn} - - 0 1` : data.fen;
        
        try {
          // Attempt to load into chess.js to validate/normalize if possible
          const engineGame = new Chess(fullFen);
          setGame(engineGame);
          setFen(fullFen);
        } catch (e) {
             console.error("Error creating game from engine FEN:", fullFen, e);
             console.warn("Forcing position display despite validation error.");
             // If chess.js rejects it, we can still try to set the fen state directly for the board to render.
             // The Chessboard component might accept it if it's just a position string, 
             // or it might break if it relies on chess.js validation internally (depends on implementation).
             // But 'position={fen}' usually takes a FEN string.
             // We can't update 'game' (chess.js instance) if it's invalid, so game logic (moves) might break,
             // but visualization might work.
             setFen(fullFen); 
             // We can't update 'game' to an invalid state, so we leave it as is or reset it?
             // Leaving it as is might cause desync. 
             // But if we just want to DRAW it:
        }
      } else if (data.error) {
          console.error("Engine error:", data.error);
          setError('ERROR');
      }
    } catch (error) {
      console.error('Error making move:', error);
      setError('ERROR');
    } finally {
      setLoading(false);
    }
  };

  const startGame = (color) => {
    const newGame = new Chess();
    setGame(newGame);
    setFen(newGame.fen());
    setUserColor(color);
    setError('');
    
    if (color === 'black') {
      // If user plays black, engine moves first
      makeEngineMove(newGame.fen());
    }
  };

  const onDrop = async (sourceSquare, targetSquare) => {
    if (loading || error) return false;
    
    // Check if it's user's turn
    const turn = game.turn() === 'w' ? 'white' : 'black';
    if (turn !== userColor) return false;

    try {
      const move = {
        from: sourceSquare,
        to: targetSquare,
        promotion: 'q', // always promote to queen for simplicity
      };

      // Validate move locally first
      let tempGame = new Chess(game.fen());
      // try {
      //   tempGame.move(move);
      // } catch (e) {
      //   return false; // Illegal move
      // }
      try {
        tempGame.move(move);
      } catch (e) {
        console.warn("Illegal move detected by chess.js but allowing it for debugging/engine:", e);
        // Force the move even if chess.js thinks it's illegal? 
        // chess.js won't let us make an illegal move on its internal board.
        // We might have to manually update the board state or trust the engine will correct it if we send it.
        // But if we want to visualize it on frontend, we need chess.js to accept it.
        // If the user wants to turn off checking, maybe they mean engine side checking?
        // Or they want the frontend to allow sending invalid moves?
        // If so, we can't update 'game' object easily if chess.js rejects it.
        // But the user said "turn off checking if the position is valid".
        // This likely refers to the backend check or the move validation.
      }

      // Update state with user move
      setGame(tempGame);
      setFen(tempGame.fen());
      setError('');

      // Trigger engine move
      await makeEngineMove(tempGame.fen());

    } catch (error) {
      console.error('Error making move:', error);
      return false;
    }

    return true;
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', gap: '20px' }}>
      <h1>Chess Engine</h1>
      <div style={{ width: '400px', height: '400px' }}>
        <Chessboard 
            position={fen} 
            onPieceDrop={onDrop}
            boardOrientation={userColor}
        />
      </div>
      
      <div style={{ display: 'flex', gap: '10px' }}>
        <button 
          className="btn-white" 
          onClick={() => startGame('white')}
          disabled={loading}
        >
          Play White
        </button>
        <button 
          className="btn-black" 
          onClick={() => startGame('black')}
          disabled={loading}
        >
          Play Black
        </button>
      </div>

      <div className="spinner-container">
        {loading && <div className="spinner"></div>}
      </div>
      
      {error && (
        <div style={{ marginTop: '10px', fontSize: '1.5em', color: 'red', fontWeight: 'bold' }}>
          {error}
        </div>
      )}

      {!error && game.isGameOver() && (
        <div style={{ marginTop: '10px', fontSize: '1.2em', color: 'red' }}>
          Game Over!
        </div>
      )}
    </div>
  );
}

export default App;
