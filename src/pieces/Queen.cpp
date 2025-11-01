// ============================================================================
// File: src/pieces/Queen.cpp
// Description: Implementation of Queen class
// ============================================================================

#include "../../include/pieces/Queen.h"
#include "../../include/board/Board.h"

Queen::Queen(Color c, const Position& pos) : Piece(c, pos) {}

Queen::~Queen() {}

bool Queen::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    // Queen moves like rook or bishop (horizontally, vertically, or diagonally)
    bool isRookMove = (position.getRow() == to.getRow() || position.getCol() == to.getCol());
    bool isBishopMove = position.isDiagonal(to);
    
    if (!isRookMove && !isBishopMove) {
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

std::vector<Position> Queen::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    
    // Eight directions: four rook directions + four bishop directions
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},      // Rook moves
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     // Bishop moves
    };
    
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

char Queen::getSymbol() const {
    return (color == Color::WHITE) ? 'Q' : 'q';
}

std::string Queen::getName() const {
    return "Queen";
}

int Queen::getValue() const {
    return 9;
}