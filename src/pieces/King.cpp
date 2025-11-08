#include "../../include/pieces/King.h"
#include "../../include/board/Board.h"
#include <cmath>

/**
 * @brief Constructs a King piece with a given color and initial position.
 * @param c The color of the King (WHITE or BLACK).
 * @param pos The initial position of the King on the board.
 */
King::King(Color c, const Position& pos) : Piece(c, pos) {}

/**
 * @brief Destructor for the King class.
 */
King::~King() {}

/**
 * @brief Checks whether a move to a given position is valid for the King.
 * 
 * The King can move one square in any direction or perform a castling move 
 * (two squares horizontally under specific conditions).
 * 
 * @param to The target position to move to.
 * @param board Reference to the current game board.
 * @return true If the move is valid according to chess rules.
 * @return false If the move is invalid.
 */
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

/**
 * @brief Retrieves all possible legal moves for the King from its current position.
 * 
 * Includes normal one-square moves in all directions and potential castling moves.
 * 
 * @param board Reference to the current game board.
 * @return std::vector<Position> A list of all valid target positions.
 */
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

/**
 * @brief Determines if the King can castle on a given side.
 * 
 * This checks if the King and corresponding Rook have not moved, and 
 * if all intermediate squares are empty and not under attack.
 * 
 * @param kingSide True for king-side castling, false for queen-side castling.
 * @param board Reference to the current game board.
 * @return true If castling is legally possible.
 * @return false Otherwise.
 */
bool King::canCastle(bool kingSide, Board& board) const {
    if (hasMoved) return false;
    
    if (kingSide) {
        return canCastleKingSide(board);
    } else {
        return canCastleQueenSide(board);
    }
}

/**
 * @brief Checks whether the King can legally castle on the king-side (short castling).
 * 
 * Conditions:
 * - The King and the rook on the h-file have not moved.
 * - Squares between them are empty.
 * - The King is not in check and does not pass through attacked squares.
 * 
 * @param board Reference to the current game board.
 * @return true If king-side castling is possible.
 * @return false Otherwise.
 */
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

/**
 * @brief Checks whether the King can legally castle on the queen-side (long castling).
 * 
 * Conditions:
 * - The King and the rook on the a-file have not moved.
 * - Squares between them are empty.
 * - The King is not in check and does not pass through attacked squares.
 * 
 * @param board Reference to the current game board.
 * @return true If queen-side castling is possible.
 * @return false Otherwise.
 */
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

/**
 * @brief Returns the character symbol representing the King.
 * 
 * Uppercase for white ('K'), lowercase for black ('k').
 * 
 * @return char The King’s symbol.
 */
char King::getSymbol() const {
    return (color == Color::WHITE) ? 'K' : 'k';
}

/**
 * @brief Returns the name of the piece.
 * @return std::string Always returns "King".
 */
std::string King::getName() const {
    return "King";
}

/**
 * @brief Returns the value of the King piece.
 * 
 * Since the King is irreplaceable (the game ends if captured),
 * its value is represented as 0.
 * 
 * @return int Always returns 0.
 */
int King::getValue() const {
    return 0; // King is priceless (game ends if captured)
}
