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
Position::~Position() {}

// Getters
int Position::getRow() const {
    return row;
}

int Position::getCol() const {
    return col;
}

// Setters
void Position::setPosition(int r, int c) {
    row = r;
    col = c;
}

// Validation
bool Position::isValid() const {
    return isValid(row, col);
}

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

/**
 * @brief Output the Col and Row into Rank and File format
 * 
 * @return std::string 
 */
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

/**
 * @brief Check if the param Position is diagonal to this Position or not
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool Position::isDiagonal(const Position& other) const {
    int dr = std::abs(row - other.row);
    int dc = std::abs(col - other.col);
    return dr == dc && dr != 0;
}