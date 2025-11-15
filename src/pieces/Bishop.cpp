#include "../../include/pieces/Bishop.h"
#include "../../include/board/Board.h"
#include <cmath>

/**
 * @brief Creates a Bishop with given color and position.
 * @param c   Piece color.
 * @param pos Initial position.
 */
Bishop::Bishop(Color c, const Position& pos) : Piece(c, pos) {}

Bishop::~Bishop() {}

/**
 * @brief Checks if the move to the given position is valid for the Bishop.
 * 
 * Bishop moves diagonally and cannot jump over pieces.
 *
 * @param to Target position.
 * @param board Reference to the board.
 * @return true if the move is valid, false otherwise.
 */
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


/**
 * @brief Returns all possible legal moves for the Bishop.
 * @param board Reference to the board.
 * @return Vector of valid target positions.
 */
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


/**
 * @brief Returns the symbol used to represent the Bishop.
 * @return 'B' for white, 'b' for black.
 */
char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}


/**
 * @brief Returns the name of the piece.
 */
std::string Bishop::getName() const {
    return "Bishop";
}

