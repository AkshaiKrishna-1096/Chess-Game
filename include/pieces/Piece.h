#ifndef PIECE_H
#define PIECE_H
#include <bits/stdc++.h>
//#include "Board.h" "Position" etc is it the square file?
class Piece
{
    protected:
        bool colour; //true=white.false=black. 
        Position pos;//current position
        bool moved;//has the piece moved atleast once? useful for castling,pawn first move etc
    public:
        //constructor
        Piece(bool c,const Position &p); //c=colour. and we need current position

        //destructor
        virtual ~Piece() =default;

        //Encapsulation:accessors...
        bool isWhite() const;
        Position getPosition() const;
        void setPosition(const Position &p);
        bool hasMoved() const;

        //Abstraction:pure virtual function
        virtual char symbol() const =0; //white or black,im assuming capital or small letter
        virtual std::string name() const=0; //which piece
        virtual bool isValidMove(const Position &to/*,chessboard?*/) const=0; //current position and is it in board etc...and individual piece restriction
        virtual std::vector<Position> possibleMoves(/*board*/) const=0;//list all the possible squares for each piece
        //virtual Piece* clone() const=0; do we need this?

        //common function.non virtual.
        void Move(const Position &to);//move the piece to new position.

};
#endif