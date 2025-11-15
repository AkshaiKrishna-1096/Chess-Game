#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(Color c, const Position& pos);
    ~Rook() override;
    
    bool isValidMove(const Position& to, Board& board) override;
    std::vector<Position> getPossibleMoves(Board& board) override;
    char getSymbol() const override;
    std::string getName() const override;
};

#endif // ROOK_H
