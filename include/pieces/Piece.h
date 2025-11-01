// ============================================================================
// File: include/pieces/Piece.h
// Description: Abstract base class for all chess pieces
// Demonstrates: Abstraction, Polymorphism, Inheritance
// ============================================================================

#ifndef PIECE_H
#define PIECE_H

#include "../utils/Position.h"
#include "../utils/Color.h"
#include <vector>
#include <string>

// Forward declaration
class Board;

class Piece {
protected:
    Color color;
    Position position;
    bool hasMoved;
    
    // Protected constructor (cannot instantiate abstract class)
    Piece(Color c, const Position& pos);

public:
    // Virtual destructor (important for polymorphism)
    virtual ~Piece();
    
    // Pure virtual functions (Abstract interface)
    virtual bool isValidMove(const Position& to, Board& board) = 0;
    virtual std::vector<Position> getPossibleMoves(Board& board) = 0;
    virtual char getSymbol() const = 0;
    virtual std::string getName() const = 0;
    
    // Virtual function with default implementation (can be overridden)
    virtual int getValue() const;
    
    // Common getters (Encapsulation)
    Color getColor() const;
    Position getPosition() const;
    bool getHasMoved() const;
    
    // Common setters
    void setPosition(const Position& pos);
    void setHasMoved(bool moved);
    
    // Utility methods
    bool isWhite() const;
    bool isBlack() const;
    bool isSameColor(const Piece* other) const;
    std::string toString() const;
};

#endif // PIECE_H