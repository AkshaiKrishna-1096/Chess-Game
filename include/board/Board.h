// ============================================================================
// File: include/board/Board.h
// Description: Class representing the chess board
// Demonstrates: Composition, Aggregation, Complex Object Management
// ============================================================================

#ifndef BOARD_H
#define BOARD_H

#include "Square.h"
#include "../utils/Position.h"
#include "../utils/Color.h"
#include <vector>
#include <string>

// Forward declarations
class Piece;
class Pawn;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;
class Move;

class Board {
private:
    Square* squares[8][8];           // Composition - Board owns all squares
    std::vector<Piece*> whitePieces; // Aggregation - Board tracks pieces but Game owns them
    std::vector<Piece*> blackPieces;
    Move* lastMove;                  // Track last move for en passant
    
    // Helper methods for initialization
    void createSquares();
    void setupInitialPieces();
    void placePiece(Piece* piece, const Position& pos);
    
    // Helper methods for move validation
    bool isPathClearVertical(const Position& from, const Position& to) const;
    bool isPathClearHorizontal(const Position& from, const Position& to) const;
    bool isPathClearDiagonal(const Position& from, const Position& to) const;

public:
    // Constructors and Destructor
    Board();
    ~Board();
    
    // Board initialization
    void initialize();
    void clear();
    void reset();
    
    // Square access (Encapsulation)
    Square* getSquare(const Position& pos) const;
    Square* getSquare(int row, int col) const;
    
    // Piece access
    Piece* getPieceAt(const Position& pos) const;
    Piece* getPieceAt(int row, int col) const;
    std::vector<Piece*> getPieces(Color color) const;
    std::vector<Piece*> getAllPieces() const;
    
    // Piece management
    void addPiece(Piece* piece, const Position& pos);
    Piece* removePiece(const Position& pos);
    bool movePiece(const Position& from, const Position& to);
    
    // Move validation helpers
    bool isPathClear(const Position& from, const Position& to) const;
    bool isSquareEmpty(const Position& pos) const;
    bool isSquareOccupied(const Position& pos) const;
    bool isSquareOccupiedByColor(const Position& pos, Color color) const;
    
    // Attack and threat detection
    bool isSquareUnderAttack(const Position& pos, Color byColor) const;
    bool isKingInCheck(Color kingColor) const;
    std::vector<Position> getAttackersOfSquare(const Position& pos, Color byColor) const;
    
    // King finding
    Position findKing(Color color) const;
    
    // Move history
    void setLastMove(Move* move);
    Move* getLastMove() const;
    
    // Display
    void display() const;
    void displayWithCoordinates() const;
    std::string toString() const;
    
    // Utility
    bool isValidPosition(const Position& pos) const;
    bool isValidPosition(int row, int col) const;
};

#endif // BOARD_H