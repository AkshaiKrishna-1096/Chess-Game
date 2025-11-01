// ============================================================================
// File: include/utils/GameState.h
// Description: Enumeration for game states
// Demonstrates: Enumeration, State Management
// ============================================================================

#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState {
    ACTIVE,      // Game is ongoing
    CHECK,       // A king is in check
    CHECKMATE,   // Game over - checkmate
    STALEMATE,   // Game over - stalemate (no legal moves but not in check)
    DRAW         // Game over - draw (other reasons like insufficient material)
};

#endif // GAMESTATE_H