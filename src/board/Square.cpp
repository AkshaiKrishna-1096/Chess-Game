#include "../../include/board/Square.h"
#include "../../include/pieces/Piece.h"
#include <sstream>

// Default constructor
Square::Square() : position(Position()), piece(nullptr), squareColor(Color::WHITE) {}

// Parameterized constructor with Position
Square::Square(const Position& pos, Color color) 
    : position(pos), piece(nullptr), squareColor(color) {}

// Parameterized constructor with row and col
Square::Square(int row, int col, Color color) 
    : position(Position(row, col)), piece(nullptr), squareColor(color) {}

// Destructor (doesn't delete piece - aggregation)
Square::~Square() {
    // Don't delete piece - it's managed elsewhere
    piece = nullptr;
}

// Getters
Position Square::getPosition() const {
    return position;
}

Piece* Square::getPiece() const {
    return piece;
}

Color Square::getSquareColor() const {
    return squareColor;
}

// State queries
bool Square::isEmpty() const {
    return piece == nullptr;
}

bool Square::isOccupied() const {
    return piece != nullptr;
}

bool Square::hasPieceOfColor(Color color) const {
    return piece != nullptr && piece->getColor() == color;
}

void Square::setPiece(Piece* p) {
    piece = p;
}

void Square::removePiece() {
    piece = nullptr;
}

Piece* Square::removePieceAndReturn() {
    Piece* temp = piece;
    piece = nullptr;
    return temp;
}

/**
 * @brief Display the Character in each Square
 * 
 * @return char 
 */
char Square::getDisplayChar() const {
    if (isEmpty()) {
        return '.';
    }
    return piece->getSymbol();
}