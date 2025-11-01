// ============================================================================
// File: include/pieces/Pawn.h
// Description: Pawn piece implementation
// Demonstrates: Inheritance, Polymorphism
// ============================================================================

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
private:
    bool canEnPassant(const Position& to, Board& board) const;
    int getDirection() const; // 1 for white (moving up), -1 for black (moving down)

public:
    // Constructor
    Pawn(Color c, const Position& pos);
    
    // Destructor
    ~Pawn() override;
    
    // Override pure virtual functions
    bool isValidMove(const Position& to, Board& board) override;
    std::vector<Position> getPossibleMoves(Board& board) override;
    char getSymbol() const override;
    std::string getName() const override;
    
    // Override value
    int getValue() const override;
    
    // Pawn-specific methods
    bool canPromote() const;
    bool isAtStartingPosition() const;
};

#endif // PAWN_H
