// ============================================================================
// File: src/pieces/Rook.cpp
// Description: Implementation of Rook class
// ============================================================================

#include "../../include/pieces/Rook.h"
#include "../../include/board/Board.h"

Rook::Rook(Color c, const Position& pos) : Piece(c, pos) {}

Rook::~Rook() {}

bool Rook::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    // Rook moves horizontally or vertically
    if (position.getRow() != to.getRow() && position.getCol() != to.getCol()) {
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

std::vector<Position> Rook::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    
    // Four directions: up, down, left, right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
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

char Rook::getSymbol() const {
    return (color == Color::WHITE) ? 'R' : 'r';
}

std::string Rook::getName() const {
    return "Rook";
}

int Rook::getValue() const {
    return 5;
}