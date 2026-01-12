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
    try {
      const response = await fetch('/api/move', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          fen: currentFen,
        }),
      });

      if (!response.ok) {
        throw new Error('Network response was not ok');
      }

      const data = await response.json();
      
      if (data.fen) {
        const engineGame = new Chess(data.fen);
        setGame(engineGame);
        setFen(data.fen);
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
      try {
        tempGame.move(move);
      } catch (e) {
        return false; // Illegal move
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

      {loading && <div className="spinner"></div>}
      
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
