
// ============================================================================
// File: src/pieces/Piece.cpp
// Description: Implementation of abstract Piece class
// ============================================================================

#include "../../include/pieces/Piece.h"
#include "../../include/board/Board.h"
#include <sstream>

// Protected constructor
Piece::Piece(Color c, const Position& pos) 
    : color(c), position(pos), hasMoved(false) {}

// Virtual destructor
Piece::~Piece() {}

// Default value implementation
int Piece::getValue() const {
    return 0;
}

// Getters
Color Piece::getColor() const {
    return color;
}

Position Piece::getPosition() const {
    return position;
}

bool Piece::getHasMoved() const {
    return hasMoved;
}

// Setters
void Piece::setPosition(const Position& pos) {
    position = pos;
}

void Piece::setHasMoved(bool moved) {
    hasMoved = moved;
}

// Utility methods
bool Piece::isWhite() const {
    return color == Color::WHITE;
}

bool Piece::isBlack() const {
    return color == Color::BLACK;
}

bool Piece::isSameColor(const Piece* other) const {
    if (other == nullptr) return false;
    return color == other->color;
}

std::string Piece::toString() const {
    std::ostringstream oss;
    oss << getName() << " (" << (isWhite() ? "White" : "Black") << ") at " << position.toString();
    return oss.str();
}