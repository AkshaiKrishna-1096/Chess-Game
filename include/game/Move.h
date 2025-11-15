// ============================================================================
// File: include/game/Move.h
// Description: Class representing a chess move
// Demonstrates: Encapsulation, Value Object Pattern
// ============================================================================

#ifndef MOVE_H
#define MOVE_H

#include "../utils/Position.h"
#include <string>

// Forward declaration
class Piece;

class Move {
private:
    Position from;
    Position to;
    Piece* movedPiece;
    Piece* capturedPiece;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    char promotionPiece;  // 'Q', 'R', 'B', 'N'

public:
    // Constructors
    Move();
    Move(const Position& fromPos, const Position& toPos, Piece* moved);
    Move(const Position& fromPos, const Position& toPos, Piece* moved, Piece* captured);
    
    // Copy constructor
    Move(const Move& other);
    
    // Assignment operator
    Move& operator=(const Move& other);
    
    // Destructor
    ~Move();
    
    // Getters (only what's actually used)
    Position getFrom() const;
    Position getTo() const;
    Piece* getMovedPiece() const;
    
    // Setters
    void setCapturedPiece(Piece* piece);
    void setIsCastling(bool castling);
    void setIsEnPassant(bool enPassant);
    void setIsPromotion(bool promotion, char piece = 'Q');
};

#endif // MOVE_H