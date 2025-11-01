// ============================================================================
// File: include/utils/Position.h
// Description: Class representing a position on the chess board
// Demonstrates: Encapsulation, Data Validation, Value Object Pattern
// ============================================================================

#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position {
private:
    int row;  // 0-7, where 0 is top (rank 8) and 7 is bottom (rank 1)
    int col;  // 0-7, where 0 is 'a' and 7 is 'h'

public:
    // Constructors
    Position();
    Position(int r, int c);
    
    // Copy constructor (demonstrates Rule of Three)
    Position(const Position& other);
    
    // Assignment operator
    Position& operator=(const Position& other);
    
    // Destructor
    ~Position();
    
    // Getters (Encapsulation)
    int getRow() const;
    int getCol() const;
    
    // Setters (Encapsulation with Validation)
    void setRow(int r);
    void setCol(int c);
    void setPosition(int r, int c);
    
    // Validation
    bool isValid() const;
    static bool isValid(int row, int col);
    
    // Comparison operators (Operator Overloading)
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    
    // Utility methods
    bool equals(const Position& other) const;
    std::string toString() const;  // Returns algebraic notation (e.g., "e4")
    
    // Static factory method for creating position from algebraic notation
    static Position fromString(const std::string& notation);
    
    // Calculate distance between positions
    int distanceTo(const Position& other) const;
    int manhattanDistance(const Position& other) const;
    
    // Check if positions are diagonal
    bool isDiagonal(const Position& other) const;
    
    // Check if positions are in same row or column
    bool isSameRow(const Position& other) const;
    bool isSameColumn(const Position& other) const;
};

#endif // POSITION_H