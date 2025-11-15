#include "../../include/game/Move.h"
#include "../../include/pieces/Piece.h"

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

// Getters (only what's actually used)
Position Move::getFrom() const { return from; }
Position Move::getTo() const { return to; }
Piece* Move::getMovedPiece() const { return movedPiece; }

// Setters
void Move::setCapturedPiece(Piece* piece) { capturedPiece = piece; }
void Move::setIsCastling(bool castling) { isCastling = castling; }
void Move::setIsEnPassant(bool enPassant) { isEnPassant = enPassant; }
void Move::setIsPromotion(bool promotion, char piece) {
    isPromotion = promotion;
    promotionPiece = piece;
}

