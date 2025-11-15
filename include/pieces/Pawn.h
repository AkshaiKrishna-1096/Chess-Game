#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
private:
    bool canEnPassant(const Position& to, Board& board) const;
    int getDirection() const; 

public:
    
    Pawn(Color c, const Position& pos);
    
    
    ~Pawn() override;
    
    
    bool isValidMove(const Position& to, Board& board) override;
    std::vector<Position> getPossibleMoves(Board& board) override;
    char getSymbol() const override;
    std::string getName() const override;
    
    bool canPromote() const;
};

#endif 
