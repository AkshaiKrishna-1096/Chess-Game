// ============================================================================
// File: src/utils/Position.cpp
// Description: Implementation of Position class
// ============================================================================

#include "../../include/utils/Position.h"
#include <cmath>
#include <cctype>
#include <sstream>

// Default constructor
Position::Position() : row(0), col(0) {}

// Parameterized constructor
Position::Position(int r, int c) : row(r), col(c) {}

// Copy constructor
Position::Position(const Position& other) : row(other.row), col(other.col) {}

// Assignment operator
Position& Position::operator=(const Position& other) {
    if (this != &other) {  // Check for self-assignment
        row = other.row;
        col = other.col;
    }
    return *this;
}

// Destructor
Position::~Position() {
    // Nothing to clean up for primitive types
}

// Getters
int Position::getRow() const {
    return row;
}

int Position::getCol() const {
    return col;
}

// Setters
void Position::setRow(int r) {
    row = r;
}

void Position::setCol(int c) {
    col = c;
}

void Position::setPosition(int r, int c) {
    row = r;
    col = c;
}

// Validation - instance method
bool Position::isValid() const {
    return isValid(row, col);
}

// Validation - static method
bool Position::isValid(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Equality operator
bool Position::operator==(const Position& other) const {
    return row == other.row && col == other.col;
}

// Inequality operator
bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

// Equals method (alternative to operator)
bool Position::equals(const Position& other) const {
    return *this == other;
}

// Convert to algebraic notation string
std::string Position::toString() const {
    if (!isValid()) {
        return "Invalid";
    }
    
    char colChar = 'a' + col;
    char rowChar = '1' + (7 - row);  // Convert: row 0 = rank 8, row 7 = rank 1
    
    std::string result;
    result += colChar;
    result += rowChar;
    return result;
}

// Create position from algebraic notation (e.g., "e4")
Position Position::fromString(const std::string& notation) {
    if (notation.length() != 2) {
        return Position(-1, -1);  // Invalid position
    }
    
    char colChar = std::tolower(notation[0]);
    char rowChar = notation[1];
    
    if (colChar < 'a' || colChar > 'h' || rowChar < '1' || rowChar > '8') {
        return Position(-1, -1);  // Invalid position
    }
    
    int col = colChar - 'a';
    int row = 8 - (rowChar - '0');  // Convert: rank 1 = row 7, rank 8 = row 0
    
    return Position(row, col);
}

// Calculate Euclidean distance
int Position::distanceTo(const Position& other) const {
    int dr = row - other.row;
    int dc = col - other.col;
    return static_cast<int>(std::sqrt(dr * dr + dc * dc));
}

// Calculate Manhattan distance
int Position::manhattanDistance(const Position& other) const {
    return std::abs(row - other.row) + std::abs(col - other.col);
}

// Check if diagonal
bool Position::isDiagonal(const Position& other) const {
    int dr = std::abs(row - other.row);
    int dc = std::abs(col - other.col);
    return dr == dc && dr != 0;
}

// Check if same row
bool Position::isSameRow(const Position& other) const {
    return row == other.row && col != other.col;
}

// Check if same column
bool Position::isSameColumn(const Position& other) const {
    return col == other.col && row != other.row;
}