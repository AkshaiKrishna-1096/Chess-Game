// ============================================================================
// File: include/pieces/Knight.h
// Description: Knight piece implementation
// ============================================================================

#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(Color c, const Position& pos);
    ~Knight() override;
    
    bool isValidMove(const Position& to, Board& board) override;
    std::vector<Position> getPossibleMoves(Board& board) override;
    char getSymbol() const override;
    std::string getName() const override;
    int getValue() const override;
};

#endif // KNIGHT_H
