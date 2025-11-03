#ifndef SQUARE_H
#define SQUARE_H

#include "../utils/Position.h"
#include "../utils/Color.h"

class Piece;

class Square {
private:
    Position position;        // Composition - Square owns its position
    Piece* piece;            // Aggregation - Square references a piece but doesn't own it
    Color squareColor;       // Color of the square itself (light/dark)

public:
    Square();
    Square(const Position& pos, Color color);
    Square(int row, int col, Color color);
    
    ~Square();
    
    Position getPosition() const;
    Piece* getPiece() const;
    Color getSquareColor() const;
    
    bool isEmpty() const;
    bool isOccupied() const;
    bool hasPieceOfColor(Color color) const;
    
    void setPiece(Piece* p);
    void removePiece();
    Piece* removePieceAndReturn();  // Remove and return the piece
    
    char getDisplayChar() const;
};

#endif // SQUARE_H
