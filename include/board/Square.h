// ============================================================================
// File: include/board/Square.h
// Description: Class representing a single square on the chess board
// Demonstrates: Encapsulation, Composition, Aggregation
// ============================================================================

#ifndef SQUARE_H
#define SQUARE_H

#include "../utils/Position.h"
#include "../utils/Color.h"

// Forward declaration to avoid circular dependency
class Piece;

class Square {
private:
    Position position;        // Composition - Square owns its position
    Piece* piece;            // Aggregation - Square references a piece but doesn't own it
    Color squareColor;       // Color of the square itself (light/dark)

public:
    // Constructors
    Square();
    Square(const Position& pos, Color color);
    Square(int row, int col, Color color);
    
    // Destructor (doesn't delete piece - aggregation relationship)
    ~Square();
    
    // Getters (Encapsulation)
    Position getPosition() const;
    Piece* getPiece() const;
    Color getSquareColor() const;
    
    // Square state queries
    bool isEmpty() const;
    bool isOccupied() const;
    bool hasWhitePiece() const;
    bool hasBlackPiece() const;
    bool hasPieceOfColor(Color color) const;
    
    // Piece management (Aggregation - doesn't take ownership)
    void setPiece(Piece* p);
    void removePiece();
    Piece* removePieceAndReturn();  // Remove and return the piece
    
    // Display
    char getDisplayChar() const;
    std::string toString() const;
};

#endif // SQUARE_H
