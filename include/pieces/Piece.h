#ifndef PIECE_H
#define PIECE_H

#include "../utils/Position.h"
#include "../utils/Color.h"
#include <vector>
#include <string>

class Board;

class Piece {
protected:
    Color color;
    Position position;
    bool hasMoved;
    Piece(Color c, const Position& pos);
public:
    virtual ~Piece();
    
    virtual bool isValidMove(const Position& to, Board& board) = 0;
    virtual std::vector<Position> getPossibleMoves(Board& board) = 0;
    virtual char getSymbol() const = 0;
    virtual std::string getName() const = 0;   

    virtual int getValue() const;

    Color getColor() const;
    Position getPosition() const;
    bool getHasMoved() const;
    
    void setPosition(const Position& pos);
    void setHasMoved(bool moved);
    
    bool isWhite() const;
    bool isBlack() const;
    bool isSameColor(const Piece* other) const;
    std::string toString() const;
};

#endif 