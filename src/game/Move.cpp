// ============================================================================
// File: src/game/Move.cpp
// Description: Implementation of Move class
// ============================================================================

#include "../../include/game/Move.h"
#include "../../include/pieces/Piece.h"
#include <sstream>

// Default constructor
Move::Move() 
    : from(Position()), to(Position()), movedPiece(nullptr), 
      capturedPiece(nullptr), isCastling(false), isEnPassant(false), 
      isPromotion(false), promotionPiece('Q') {}

// Parameterized constructor
Move::Move(const Position& fromPos, const Position& toPos, Piece* moved)
    : from(fromPos), to(toPos), movedPiece(moved), capturedPiece(nullptr),
      isCastling(false), isEnPassant(false), isPromotion(false), promotionPiece('Q') {}

Move::Move(const Position& fromPos, const Position& toPos, Piece* moved, Piece* captured)
    : from(fromPos), to(toPos), movedPiece(moved), capturedPiece(captured),
      isCastling(false), isEnPassant(false), isPromotion(false), promotionPiece('Q') {}

// Copy constructor
Move::Move(const Move& other)
    : from(other.from), to(other.to), movedPiece(other.movedPiece),
      capturedPiece(other.capturedPiece), isCastling(other.isCastling),
      isEnPassant(other.isEnPassant), isPromotion(other.isPromotion),
      promotionPiece(other.promotionPiece) {}

// Assignment operator
Move& Move::operator=(const Move& other) {
    if (this != &other) {
        from = other.from;
        to = other.to;
        movedPiece = other.movedPiece;
        capturedPiece = other.capturedPiece;
        isCastling = other.isCastling;
        isEnPassant = other.isEnPassant;
        isPromotion = other.isPromotion;
        promotionPiece = other.promotionPiece;
    }
    return *this;
}

// Destructor
Move::~Move() {
    // Don't delete pieces - they're managed by Game class
}

// Getters
Position Move::getFrom() const { return from; }
Position Move::getTo() const { return to; }
Piece* Move::getMovedPiece() const { return movedPiece; }
Piece* Move::getCapturedPiece() const { return capturedPiece; }
bool Move::getIsCastling() const { return isCastling; }
bool Move::getIsEnPassant() const { return isEnPassant; }
bool Move::getIsPromotion() const { return isPromotion; }
char Move::getPromotionPiece() const { return promotionPiece; }

// Setters
void Move::setCapturedPiece(Piece* piece) { capturedPiece = piece; }
void Move::setIsCastling(bool castling) { isCastling = castling; }
void Move::setIsEnPassant(bool enPassant) { isEnPassant = enPassant; }
void Move::setIsPromotion(bool promotion, char piece) {
    isPromotion = promotion;
    promotionPiece = piece;
}

// String representation
std::string Move::toString() const {
    std::ostringstream oss;
    oss << from.toString() << " -> " << to.toString();
    if (capturedPiece != nullptr) {
        oss << " (captures " << capturedPiece->getName() << ")";
    }
    if (isCastling) oss << " (castling)";
    if (isEnPassant) oss << " (en passant)";
    if (isPromotion) oss << " (promotion to " << promotionPiece << ")";
    return oss.str();
}

// Algebraic notation
std::string Move::toAlgebraicNotation() const {
    std::string notation;
    if (movedPiece != nullptr) {
        char symbol = movedPiece->getSymbol();
        if (symbol != 'P' && symbol != 'p') {
            notation += toupper(symbol);
        }
    }
    notation += from.toString();
    notation += (capturedPiece != nullptr) ? "x" : "-";
    notation += to.toString();
    return notation;
}

// Check if move is a capture
bool Move::isCapture() const {
    return capturedPiece != nullptr || isEnPassant;
}

// Check if move is valid
bool Move::isValid() const {
    return from.isValid() && to.isValid() && movedPiece != nullptr;
}