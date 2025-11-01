// ============================================================================
// File: src/pieces/Bishop.cpp
// Description: Implementation of Bishop class
// ============================================================================

#include "../../include/pieces/Bishop.h"
#include "../../include/board/Board.h"
#include <cmath>

Bishop::Bishop(Color c, const Position& pos) : Piece(c, pos) {}

Bishop::~Bishop() {}

bool Bishop::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    // Bishop moves diagonally
    if (!position.isDiagonal(to)) {
        return false;
    }
    
    // Check if path is clear
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Check destination square
    if (board.isSquareEmpty(to)) {
        return true;
    }
    
    Piece* targetPiece = board.getPieceAt(to);
    return targetPiece != nullptr && !isSameColor(targetPiece);
}

std::vector<Position> Bishop::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    
    // Four diagonal directions
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    
    for (auto& dir : directions) {
        int row = position.getRow();
        int col = position.getCol();
        
        while (true) {
            row += dir[0];
            col += dir[1];
            Position newPos(row, col);
            
            if (!newPos.isValid()) break;
            
            if (board.isSquareEmpty(newPos)) {
                moves.push_back(newPos);
            } else {
                Piece* target = board.getPieceAt(newPos);
                if (target != nullptr && !isSameColor(target)) {
                    moves.push_back(newPos);
                }
                break; // Stop at first piece
            }
        }
    }
    
    return moves;
}

char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}

std::string Bishop::getName() const {
    return "Bishop";
}

int Bishop::getValue() const {
    return 3;
}
