// ============================================================================
// File: src/pieces/King.cpp
// Description: Implementation of King class
// ============================================================================

#include "../../include/pieces/King.h"
#include "../../include/board/Board.h"
#include <cmath>

King::King(Color c, const Position& pos) : Piece(c, pos) {}

King::~King() {}

bool King::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // King moves one square in any direction
    if (rowDiff <= 1 && colDiff <= 1) {
        // Check destination square
        if (board.isSquareEmpty(to)) {
            return true;
        }
        
        Piece* targetPiece = board.getPieceAt(to);
        return targetPiece != nullptr && !isSameColor(targetPiece);
    }
    
    // Check for castling (king moves 2 squares horizontally)
    if (rowDiff == 0 && colDiff == 2 && !hasMoved) {
        bool kingSide = (to.getCol() > position.getCol());
        return canCastle(kingSide, board);
    }
    
    return false;
}

std::vector<Position> King::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    
    // Eight directions: all adjacent squares
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (auto& dir : directions) {
        Position newPos(position.getRow() + dir[0], position.getCol() + dir[1]);
        
        if (newPos.isValid()) {
            if (board.isSquareEmpty(newPos)) {
                moves.push_back(newPos);
            } else {
                Piece* target = board.getPieceAt(newPos);
                if (target != nullptr && !isSameColor(target)) {
                    moves.push_back(newPos);
                }
            }
        }
    }
    
    // Add castling moves
    if (!hasMoved) {
        if (canCastleKingSide(board)) {
            moves.push_back(Position(position.getRow(), position.getCol() + 2));
        }
        if (canCastleQueenSide(board)) {
            moves.push_back(Position(position.getRow(), position.getCol() - 2));
        }
    }
    
    return moves;
}

bool King::canCastle(bool kingSide, Board& board) const {
    if (hasMoved) return false;
    
    if (kingSide) {
        return canCastleKingSide(board);
    } else {
        return canCastleQueenSide(board);
    }
}

bool King::canCastleKingSide(Board& board) const {
    int row = position.getRow();
    int col = position.getCol();
    
    // Check if rook is in position and hasn't moved
    Position rookPos(row, 7);
    Piece* rook = board.getPieceAt(rookPos);
    if (rook == nullptr || rook->getName() != "Rook" || rook->getHasMoved()) {
        return false;
    }
    
    // Check if squares between king and rook are empty
    for (int c = col + 1; c < 7; c++) {
        if (!board.isSquareEmpty(Position(row, c))) {
            return false;
        }
    }
    
    // Check if king is in check or passes through check
    Color oppositeColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int c = col; c <= col + 2; c++) {
        if (board.isSquareUnderAttack(Position(row, c), oppositeColor)) {
            return false;
        }
    }
    
    return true;
}

bool King::canCastleQueenSide(Board& board) const {
    int row = position.getRow();
    int col = position.getCol();
    
    // Check if rook is in position and hasn't moved
    Position rookPos(row, 0);
    Piece* rook = board.getPieceAt(rookPos);
    if (rook == nullptr || rook->getName() != "Rook" || rook->getHasMoved()) {
        return false;
    }
    
    // Check if squares between king and rook are empty
    for (int c = 1; c < col; c++) {
        if (!board.isSquareEmpty(Position(row, c))) {
            return false;
        }
    }
    
    // Check if king is in check or passes through check
    Color oppositeColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int c = col - 2; c <= col; c++) {
        if (board.isSquareUnderAttack(Position(row, c), oppositeColor)) {
            return false;
        }
    }
    
    return true;
}

char King::getSymbol() const {
    return (color == Color::WHITE) ? 'K' : 'k';
}

std::string King::getName() const {
    return "King";
}

int King::getValue() const {
    return 0; // King is priceless (game ends if captured)
}