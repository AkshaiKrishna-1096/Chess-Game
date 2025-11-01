#ifndef PAWN_H
#define PAWN_H
#include "piece.h"
//#include "Board.h",and others
class Pawn:public Piece
{
    public:
        Pawn(bool colour,const Position &p);
        ~Pawn() override =default;

        char symbol() const override; //P or p
        std::string name() const override;//Pawn
        bool isValidMove(const Position &to/*,board*/) const override;//if !hasmoved then we can move two step.capture movement etc.
        std::vector<Position> possibleMoves(/*board*/) const override;//check if valid.then current pos+1 or 2 or diagonal etc
        //Piece* clone() const override;
};
#endif