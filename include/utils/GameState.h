#ifndef GAMESTATE_H
#define GAMESTATE_H

/**
 * @brief Define the state of the game right now
 * 
 */
enum class GameState {
    ACTIVE,      // Game is ongoing
    CHECK,       // A king is in check
    CHECKMATE,   // Game over - checkmate
    STALEMATE,   // Game over - stalemate (no legal moves but not in check)
    DRAW         // Game over - draw (other reasons like insufficient material)
};

#endif // GAMESTATE_H