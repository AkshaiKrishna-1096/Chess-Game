#ifndef BOARD_H
#define BOARD_H

#include "Square.h"
#include "../utils/Position.h"
#include "../utils/Color.h"
#include <vector>
#include <string>

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
    
    void createSquares();
    
    bool isPathClearVertical(const Position& from, const Position& to) const;
    bool isPathClearHorizontal(const Position& from, const Position& to) const;
    bool isPathClearDiagonal(const Position& from, const Position& to) const;

public:
    Board();
    ~Board();

    void clear();
    
    Square* getSquare(const Position& pos) const;
    
    Piece* getPieceAt(const Position& pos) const;
    std::vector<Piece*> getPieces(Color color) const;
    std::vector<Piece*> getAllPieces() const;
    
    void addPiece(Piece* piece, const Position& pos);
    Piece* removePiece(const Position& pos);
    bool movePiece(const Position& from, const Position& to);
    
    bool isPathClear(const Position& from, const Position& to) const;
    bool isSquareEmpty(const Position& pos) const;
    bool isSquareOccupied(const Position& pos) const;
    bool isSquareOccupiedByColor(const Position& pos, Color color) const;
    
    bool isSquareUnderAttack(const Position& pos, Color byColor) const;
    bool isKingInCheck(Color kingColor) const;
    std::vector<Position> getAttackersOfSquare(const Position& pos, Color byColor) const;
    
    Position findKing(Color color) const;
    
    void setLastMove(Move* move);
    Move* getLastMove() const;
    
    void display() const;

    bool isValidPosition(const Position& pos) const;
};

#endif // BOARD_H