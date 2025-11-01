// ============================================================================
// File: src/board/Board.cpp
// Description: Implementation of Board class
// ============================================================================

#include "../../include/board/Board.h"
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/Pawn.h"
#include "../../include/pieces/Rook.h"
#include "../../include/pieces/Knight.h"
#include "../../include/pieces/Bishop.h"
#include "../../include/pieces/Queen.h"
#include "../../include/pieces/King.h"
#include "../../include/game/Move.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

// Constructor
Board::Board() : lastMove(nullptr) {
    createSquares();
}

// Destructor
Board::~Board() {
    // Delete all squares (composition relationship)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete squares[i][j];
            squares[i][j] = nullptr;
        }
    }
    
    // Don't delete pieces - they're managed by Game class
    whitePieces.clear();
    blackPieces.clear();
    
    lastMove = nullptr; // Don't delete - managed by Game
}

// Create all 64 squares
void Board::createSquares() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Checkerboard pattern: (row + col) % 2 determines color
            Color squareColor = ((row + col) % 2 == 0) ? Color::BLACK : Color::WHITE;
            squares[row][col] = new Square(row, col, squareColor);
        }
    }
}

// Initialize board with pieces in starting positions
void Board::initialize() {
    setupInitialPieces();
}

// Setup initial piece positions
void Board::setupInitialPieces() {
    // This method assumes pieces are created and added via addPiece()
    // The actual piece creation is done by the Game class
    // This method just prepares the board structure
}

// Clear all pieces from board
void Board::clear() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares[i][j]->removePiece();
        }
    }
    whitePieces.clear();
    blackPieces.clear();
    lastMove = nullptr;
}

// Reset board to initial state
void Board::reset() {
    clear();
    initialize();
}

// Get square at position
Square* Board::getSquare(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return nullptr;
    }
    return squares[pos.getRow()][pos.getCol()];
}

Square* Board::getSquare(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return nullptr;
    }
    return squares[row][col];
}

// Get piece at position
Piece* Board::getPieceAt(const Position& pos) const {
    Square* square = getSquare(pos);
    if (square == nullptr) {
        return nullptr;
    }
    return square->getPiece();
}

Piece* Board::getPieceAt(int row, int col) const {
    return getPieceAt(Position(row, col));
}

// Get all pieces of a color
std::vector<Piece*> Board::getPieces(Color color) const {
    if (color == Color::WHITE) {
        return whitePieces;
    } else if (color == Color::BLACK) {
        return blackPieces;
    }
    return std::vector<Piece*>();
}

// Get all pieces
std::vector<Piece*> Board::getAllPieces() const {
    std::vector<Piece*> allPieces;
    allPieces.insert(allPieces.end(), whitePieces.begin(), whitePieces.end());
    allPieces.insert(allPieces.end(), blackPieces.begin(), blackPieces.end());
    return allPieces;
}

// Add piece to board
void Board::addPiece(Piece* piece, const Position& pos) {
    if (piece == nullptr || !isValidPosition(pos)) {
        return;
    }
    
    Square* square = getSquare(pos);
    if (square != nullptr) {
        square->setPiece(piece);
        piece->setPosition(pos);
        
        // Add to appropriate piece list
        if (piece->getColor() == Color::WHITE) {
            whitePieces.push_back(piece);
        } else if (piece->getColor() == Color::BLACK) {
            blackPieces.push_back(piece);
        }
    }
}

// Remove piece from board
Piece* Board::removePiece(const Position& pos) {
    Square* square = getSquare(pos);
    if (square == nullptr || square->isEmpty()) {
        return nullptr;
    }
    
    Piece* piece = square->removePieceAndReturn();
    
    // Remove from piece list
    if (piece->getColor() == Color::WHITE) {
        auto it = std::find(whitePieces.begin(), whitePieces.end(), piece);
        if (it != whitePieces.end()) {
            whitePieces.erase(it);
        }
    } else if (piece->getColor() == Color::BLACK) {
        auto it = std::find(blackPieces.begin(), blackPieces.end(), piece);
        if (it != blackPieces.end()) {
            blackPieces.erase(it);
        }
    }
    
    return piece;
}

// Move piece from one position to another
bool Board::movePiece(const Position& from, const Position& to) {
    if (!isValidPosition(from) || !isValidPosition(to)) {
        return false;
    }
    
    Piece* piece = getPieceAt(from);
    if (piece == nullptr) {
        return false;
    }
    
    // Remove captured piece if any
    Piece* capturedPiece = removePiece(to);
    
    // Move the piece
    getSquare(from)->removePiece();
    getSquare(to)->setPiece(piece);
    piece->setPosition(to);
    piece->setHasMoved(true);
    
    return true;
}

// Check if path is clear (for sliding pieces)
bool Board::isPathClear(const Position& from, const Position& to) const {
    if (!isValidPosition(from) || !isValidPosition(to)) {
        return false;
    }
    
    // Check if move is vertical, horizontal, or diagonal
    int rowDiff = to.getRow() - from.getRow();
    int colDiff = to.getCol() - from.getCol();
    
    if (rowDiff == 0) {
        return isPathClearHorizontal(from, to);
    } else if (colDiff == 0) {
        return isPathClearVertical(from, to);
    } else if (std::abs(rowDiff) == std::abs(colDiff)) {
        return isPathClearDiagonal(from, to);
    }
    
    return false; // Not a valid sliding direction
}

// Check vertical path
bool Board::isPathClearVertical(const Position& from, const Position& to) const {
    int col = from.getCol();
    int startRow = std::min(from.getRow(), to.getRow()) + 1;
    int endRow = std::max(from.getRow(), to.getRow());
    
    for (int row = startRow; row < endRow; row++) {
        if (!isSquareEmpty(Position(row, col))) {
            return false;
        }
    }
    return true;
}

// Check horizontal path
bool Board::isPathClearHorizontal(const Position& from, const Position& to) const {
    int row = from.getRow();
    int startCol = std::min(from.getCol(), to.getCol()) + 1;
    int endCol = std::max(from.getCol(), to.getCol());
    
    for (int col = startCol; col < endCol; col++) {
        if (!isSquareEmpty(Position(row, col))) {
            return false;
        }
    }
    return true;
}

// Check diagonal path
bool Board::isPathClearDiagonal(const Position& from, const Position& to) const {
    int rowDir = (to.getRow() > from.getRow()) ? 1 : -1;
    int colDir = (to.getCol() > from.getCol()) ? 1 : -1;
    
    int row = from.getRow() + rowDir;
    int col = from.getCol() + colDir;
    
    while (row != to.getRow() && col != to.getCol()) {
        if (!isSquareEmpty(Position(row, col))) {
            return false;
        }
        row += rowDir;
        col += colDir;
    }
    return true;
}

// Square state checks
bool Board::isSquareEmpty(const Position& pos) const {
    Square* square = getSquare(pos);
    return square != nullptr && square->isEmpty();
}

bool Board::isSquareOccupied(const Position& pos) const {
    return !isSquareEmpty(pos);
}

bool Board::isSquareOccupiedByColor(const Position& pos, Color color) const {
    Square* square = getSquare(pos);
    return square != nullptr && square->hasPieceOfColor(color);
}

// Check if square is under attack
bool Board::isSquareUnderAttack(const Position& pos, Color byColor) const {
    std::vector<Piece*> attackingPieces = getPieces(byColor);
    
    for (Piece* piece : attackingPieces) {
        if (piece->isValidMove(pos, const_cast<Board&>(*this))) {
            return true;
        }
    }
    return false;
}

// Check if king is in check
bool Board::isKingInCheck(Color kingColor) const {
    Position kingPos = findKing(kingColor);
    if (!kingPos.isValid()) {
        return false;
    }
    
    Color oppositeColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareUnderAttack(kingPos, oppositeColor);
}

// Get all pieces attacking a square
std::vector<Position> Board::getAttackersOfSquare(const Position& pos, Color byColor) const {
    std::vector<Position> attackers;
    std::vector<Piece*> pieces = getPieces(byColor);
    
    for (Piece* piece : pieces) {
        if (piece->isValidMove(pos, const_cast<Board&>(*this))) {
            attackers.push_back(piece->getPosition());
        }
    }
    return attackers;
}

// Find king position
Position Board::findKing(Color color) const {
    std::vector<Piece*> pieces = getPieces(color);
    
    for (Piece* piece : pieces) {
        if (piece->getName() == "King") {
            return piece->getPosition();
        }
    }
    return Position(-1, -1); // Invalid position if king not found
}

// Move history
void Board::setLastMove(Move* move) {
    lastMove = move;
}

Move* Board::getLastMove() const {
    return lastMove;
}

// Display board
void Board::display() const {
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    
    for (int row = 0; row < 8; row++) {
        std::cout << (8 - row) << " |";
        
        for (int col = 0; col < 8; col++) {
            Square* square = squares[row][col];
            std::cout << " " << square->getDisplayChar() << " |";
        }
        
        std::cout << " " << (8 - row);
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
}

// Display with coordinates
void Board::displayWithCoordinates() const {
    display();
}

// String representation
std::string Board::toString() const {
    std::ostringstream oss;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            oss << squares[row][col]->getDisplayChar();
        }
        oss << "\n";
    }
    return oss.str();
}

// Position validation
bool Board::isValidPosition(const Position& pos) const {
    return pos.isValid();
}

bool Board::isValidPosition(int row, int col) const {
    return Position::isValid(row, col);
}