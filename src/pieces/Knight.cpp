// ============================================================================
// File: src/pieces/Knight.cpp
// Description: Implementation of Knight class
// ============================================================================

#include "../../include/pieces/Knight.h"
#include "../../include/board/Board.h"
#include <cmath>

Knight::Knight(Color c, const Position& pos) : Piece(c, pos) {}

Knight::~Knight() {}

bool Knight::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Knight moves in L-shape: 2 squares in one direction, 1 in perpendicular
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false;
    }
    
    // Check destination square
    if (board.isSquareEmpty(to)) {
        return true;
    }
    
    Piece* targetPiece = board.getPieceAt(to);
    return targetPiece != nullptr && !isSameColor(targetPiece);
}

std::vector<Position> Knight::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    
    // All 8 possible L-shaped moves
    int moveOffsets[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (auto& offset : moveOffsets) {
        Position newPos(position.getRow() + offset[0], position.getCol() + offset[1]);
        
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
    
    return moves;
}

char Knight::getSymbol() const {
    return (color == Color::WHITE) ? 'N' : 'n';
}

std::string Knight::getName() const {
    return "Knight";
}

int Knight::getValue() const {
    return 3;
}
