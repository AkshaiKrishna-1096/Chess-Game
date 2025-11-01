// ============================================================================
// File: include/ChessGame.h
// Description: Main header file that includes all necessary chess game headers
// ============================================================================

#ifndef CHESSGAME_H
#define CHESSGAME_H

// Utility headers
#include "utils/Color.h"
#include "utils/GameState.h"
#include "utils/Position.h"

// Piece headers
#include "pieces/Piece.h"
#include "pieces/Pawn.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"

// Board headers
#include "board/Square.h"
#include "board/Board.h"

// Game headers
#include "game/Move.h"
#include "game/Player.h"
#include "game/Game.h"

// Standard library includes commonly used
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#endif // CHESSGAME_H