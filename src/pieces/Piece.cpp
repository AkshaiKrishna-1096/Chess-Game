#include "../../include/pieces/Piece.h"
#include "../../include/board/Board.h"
#include <sstream>

// Protected constructor
Piece::Piece(Color c, const Position& pos) 
    : color(c), position(pos), hasMoved(false) {}

// Virtual destructor
Piece::~Piece() {}

/**
 * @brief Virtual function.Returns the value of each piece
 * 
 * eg.King=0(priceless)
 *    ,Pawn=1
 *    ,Queen=9
 */
int Piece::getValue() const {
    return 0;
}

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
 * @brief Returns 1 if piece=White.
 */
bool Piece::isWhite() const {
    return color == Color::WHITE;
}
/**
 * @brief Returns 1 if piece=Black.
 */
bool Piece::isBlack() const {
    return color == Color::BLACK;
}
/**
 * @brief Returns 1 if both pieces are same color.
 */
bool Piece::isSameColor(const Piece* other) const {
    if (other == nullptr) return false;
    return color == other->color;
}
/**
 * @brief Returns the piece's name, color, and position in string form.
 */
std::string Piece::toString() const {
    std::ostringstream oss;
    oss << getName() << " (" << (isWhite() ? "White" : "Black") << ") at " << position.toString();
    return oss.str();
}