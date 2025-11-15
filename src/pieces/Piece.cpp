#include "../../include/pieces/Piece.h"
#include "../../include/board/Board.h"

// Protected constructor
Piece::Piece(Color c, const Position& pos) 
    : color(c), position(pos), hasMoved(false) {}

// Virtual destructor
Piece::~Piece() {}

/**
 * @brief Returns the color of the piece(White/Black).
 */
Color Piece::getColor() const {
    return color;
}
/**
 * @brief Returns the position(row,column) of the piece.
 */
Position Piece::getPosition() const {
    return position;
}
/**
 * @brief if the piece has moved before, returns 1. else 0.
 */
bool Piece::getHasMoved() const {
    return hasMoved;
}

/**
 * @brief set the row and column of the piece 
 */
void Piece::setPosition(const Position& pos) {
    position = pos;
}
/**
 * @brief To set hasmMoved=1 if the piece has moved before,.
 */
void Piece::setHasMoved(bool moved) {
    hasMoved = moved;
}

/**
 * @brief Returns 1 if both pieces are same color.
 */
bool Piece::isSameColor(const Piece* other) const {
    if (other == nullptr) return false;
    return color == other->color;
}