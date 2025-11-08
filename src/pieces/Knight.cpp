

#include "../../include/pieces/Knight.h"
#include "../../include/board/Board.h"
#include <cmath>

/**
 * @brief Constructs a Knight piece with a given color and initial position.
 * @param c The color of the Knight (WHITE or BLACK).
 * @param pos The initial position of the Knight on the board.
 */
Knight::Knight(Color c, const Position& pos) : Piece(c, pos) {}

/**
 * @brief Destructor for the Knight class.
 */
Knight::~Knight() {}

/**
 * @brief Checks whether a move to a given position is valid for the Knight.
 * 
 * The Knight moves in an 'L' shape: two squares in one direction and 
 * one square perpendicular to it. It can jump over other pieces.
 * 
 * @param to The target position to move to.
 * @param board Reference to the current game board.
 * @return true If the move is valid according to chess rules.
 * @return false If the move is invalid.
 */
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

/**
 * @brief Retrieves all possible legal moves for the Knight from its current position.
 * 
 * The Knight has up to eight possible moves in an 'L' shape pattern.
 * It can jump over other pieces and capture enemy pieces that occupy its landing square.
 * 
 * @param board Reference to the current game board.
 * @return std::vector<Position> A list of all valid target positions for the Knight.
 */
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

/**
 * @brief Returns the character symbol representing the Knight.
 * 
 * Uses uppercase ('N') for white and lowercase ('n') for black.
 * 
 * @return char The Knight’s symbol.
 */
char Knight::getSymbol() const {
    return (color == Color::WHITE) ? 'N' : 'n';
}

/**
 * @brief Returns the name of the piece.
 * @return std::string Always returns "Knight".
 */
std::string Knight::getName() const {
    return "Knight";
}

/**
 * @brief Returns the relative material value of the Knight piece.
 * 
 * In standard chess evaluation, the Knight is typically worth 3 points.
 * 
 * @return int The value of the Knight (3).
 */
int Knight::getValue() const {
    return 3;
}
