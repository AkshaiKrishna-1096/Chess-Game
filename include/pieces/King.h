#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
private:
    bool canCastleKingSide(Board& board) const;
    bool canCastleQueenSide(Board& board) const;

public:
    King(Color c, const Position& pos);
    ~King() override;
    
    bool isValidMove(const Position& to, Board& board) override;
    std::vector<Position> getPossibleMoves(Board& board) override;
    char getSymbol() const override;
    std::string getName() const override;
    
    bool canCastle(bool kingSide, Board& board) const;
};

#endif 