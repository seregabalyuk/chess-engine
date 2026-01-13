import { useState, useEffect, useRef } from 'react';
import { Chess } from 'chess.js'; // still keep chess.js for move validation if needed, or backend proxy
import '@chrisoakman/chessboardjs/dist/chessboard-1.0.0.min.css';

// We need to import jQuery and chessboard.js. 
// Since chessboard.js is an older library that depends on jQuery, 
// using it in React requires some workarounds or ensuring it's loaded globally.
// A better React wrapper is react-chessboard, but user explicitly asked for chessboard.js.
// However, standard import might not work out of the box with Vite without setup.
// Let's try dynamic import or assume global script for now, 
// BUT typically npm install works if we alias it.
// Actually, standard chessboard.js is hard to use with ES6 modules.
// Let's try to use the raw file or a wrapper if possible.
// Wait, 'react-chessboard' IS the modern wrapper for this.
// But the user specifically pointed to chessboardjs.com docs and said "Rewrite everything on the ...".
// They probably want the VISUALS/BEHAVIOR of chessboard.js, or literally use that library.
// If I use the raw library, I need to manipulate the DOM directly.

function App() {
  const [fen, setFen] = useState('start');
  const [loading, setLoading] = useState(false);
  const [userColor, setUserColor] = useState('white');
  const [error, setError] = useState('');
  const boardRef = useRef(null);
  const boardInstance = useRef(null);
  const game = useRef(new Chess());

  // Load jQuery and Chessboard.js from CDN or local is tricky in React/Vite without legacy plugin.
  // We will assume they are available or try to load them via useEffect script injection 
  // if not using a react-compatible version.
  // Actually, let's use the provided npm package if it works, otherwise script tags.
  // The npm package '@chrisoakman/chessboardjs' exists but requires jquery window object.

  useEffect(() => {
    // Dynamically load jQuery and chessboard.js from CDN for simplicity 
    // as it's the most reliable way to get the original library working in a modern stack quickly.
    
    const loadScripts = async () => {
      if (!window.jQuery) {
        const jq = document.createElement('script');
        jq.src = 'https://code.jquery.com/jquery-3.5.1.min.js';
        document.head.appendChild(jq);
        await new Promise(r => jq.onload = r);
      }

      if (!window.Chessboard) {
        const cb = document.createElement('script');
        cb.src = 'https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js';
        document.head.appendChild(cb);
        await new Promise(r => cb.onload = r);
      }

      initBoard();
    };

    loadScripts();
  }, []);

  const initBoard = () => {
    if (window.Chessboard && boardRef.current) {
        const config = {
            draggable: true,
            position: 'start',
            onDragStart: onDragStart,
            onDrop: onDrop,
            onSnapEnd: onSnapEnd,
            pieceTheme: 'https://chessboardjs.com/img/chesspieces/wikipedia/{piece}.png'
        };
        boardInstance.current = window.Chessboard(boardRef.current, config);
    }
  };

  const onDragStart = (source, piece, position, orientation) => {
    if (game.current.isGameOver()) return false;
    if (loading) return false;

    // Only allow user to move their own pieces
    if ((userColor === 'white' && piece.search(/^b/) !== -1) ||
        (userColor === 'black' && piece.search(/^w/) !== -1)) {
        return false;
    }
  };

  const onDrop = (source, target) => {
    // If user drops on same square, do nothing (return snapback so it doesn't vanish if logic was weird, but usually null is fine)
    if (source === target) return;

    // see if the move is legal
    let move = null;
    try {
        move = game.current.move({
            from: source,
            to: target,
            promotion: 'q' // NOTE: always promote to a queen for example simplicity
        });
    } catch (e) {
        return 'snapback';
    }

    // illegal move
    if (move === null) return 'snapback';

    // Legal move made locally
    // setFen(game.current.fen()); // Do NOT update state instantly here, let board animation finish
    setError('');
    
    // Trigger engine move later (in onSnapEnd to ensure animation finishes first)
    // Actually, onSnapEnd fires for any snap. 
    // It's safer to just set a flag or trigger it here but be careful about visuals.
    // If we trigger it here, the engine might respond fast. 
    // But chessboard.js queues animations usually.
  };

  const onSnapEnd = () => {
      // update the board position after the piece snap
      // for castling, en passant, pawn promotion
      if (boardInstance.current) {
          boardInstance.current.position(game.current.fen());
      }
      
      // Check if it's engine's turn to move (meaning user just moved)
      // If user plays white, and turn is black, engine moves.
      // If user plays black, and turn is white, engine moves.
      const turn = game.current.turn(); // 'w' or 'b'
      const userColorChar = userColor === 'white' ? 'w' : 'b';
      
      if (turn !== userColorChar && !loading && !game.current.isGameOver()) {
          makeEngineMove(game.current.fen());
      }
  };

  const makeEngineMove = async (currentFen) => {
    setLoading(true);
    setError('');
    
    const fenParts = currentFen.split(' ');
    const boardPosition = fenParts[0];
    const turn = fenParts[1];

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

      if (!response.ok) throw new Error('Network response was not ok');
      const data = await response.json();

      if (data.fen) {
        // Construct full FEN
        // User just moved (if we called this), so turn passed to engine, engine moved, now it's user turn.
        // Wait, if engine moved, the turn in FEN should be the user's color.
        const nextTurn = userColor === 'white' ? 'w' : 'b'; 
        const isJustPosition = data.fen.split(' ').length === 1;
        const fullFen = isJustPosition ? `${data.fen} ${nextTurn} - - 0 1` : data.fen;

        // Load into chess.js logic
        // Note: we can force load even if invalid if we really want, but for game logic we need valid state.
        // If we just want to display what engine sent:
        try {
            game.current.load(fullFen);
        } catch(e) {
            console.warn("Invalid FEN from engine, forcing board update anyway:", fullFen);
            // If chess.js fails, we can't track game rules anymore, but we can update the board
        }
        
        if (boardInstance.current) {
            boardInstance.current.position(data.fen, true); // Use animation
        }
        setFen(fullFen);

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
    game.current = newGame;
    setUserColor(color);
    setFen(newGame.fen());
    setLoading(false);
    setError('');

    if (boardInstance.current) {
        boardInstance.current.start();
        boardInstance.current.orientation(color);
    }

    if (color === 'black') {
        makeEngineMove(newGame.fen());
    }
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', gap: '20px' }}>
      <h1>Chess Engine</h1>
      
      {/* Container for chessboard.js */}
      <div id="myBoard" ref={boardRef} style={{ width: '400px' }}></div>
      
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

      {!error && game.current.isGameOver() && (
        <div style={{ marginTop: '10px', fontSize: '1.2em', color: 'red' }}>
          Game Over!
        </div>
      )}
    </div>
  );
}

export default App;
