#include "../../include/pieces/Pawn.h"
#include "../../include/board/Board.h"
#include "../../include/game/Move.h"
#include <cmath>

/**
 * @brief Constructs a Pawn piece with a given color and initial position.
 * @param c The color of the Pawn (WHITE or BLACK).
 * @param pos The initial position of the Pawn on the board.
 */
Pawn::Pawn(Color c, const Position& pos) : Piece(c, pos) {}

/**
 * @brief Destructor for the Pawn class.
 */
Pawn::~Pawn() {}

/**
 * @brief Determines the forward movement direction of the Pawn.
 * 
 * White pawns move upward (decreasing row index), and black pawns move downward (increasing row index).
 * 
 * @return int -1 for white pawns, +1 for black pawns.
 */
int Pawn::getDirection() const {
    return (color == Color::WHITE) ? -1 : 1; // White moves up (decreasing row), Black moves down
}

/**
 * @brief Checks whether a move to a given position is valid for the Pawn.
 * 
 * The Pawn can:
 * - Move forward one square if empty.
 * - Move forward two squares from its starting position if both squares are empty.
 * - Capture diagonally forward one square if occupied by an enemy piece.
 * - Perform an en passant capture if conditions are met.
 * 
 * @param to The target position to move to.
 * @param board Reference to the current game board.
 * @return true If the move is valid according to chess rules.
 * @return false If the move is invalid.
 */
bool Pawn::isValidMove(const Position& to, Board& board) {
    if (!to.isValid() || to == position) {
        return false;
    }
    
    int rowDiff = to.getRow() - position.getRow();
    int colDiff = std::abs(to.getCol() - position.getCol());
    int direction = getDirection();
    
    // Forward move (1 square)
    if (rowDiff == direction && colDiff == 0) {
        return board.isSquareEmpty(to);
    }
    
    // Forward move (2 squares from starting position)
    if (rowDiff == 2 * direction && colDiff == 0 && !hasMoved) {
        Position intermediate(position.getRow() + direction, position.getCol());
        return board.isSquareEmpty(intermediate) && board.isSquareEmpty(to);
    }
    
    // Diagonal capture
    if (rowDiff == direction && colDiff == 1) {
        // Normal capture
        if (board.isSquareOccupied(to)) {
            Piece* targetPiece = board.getPieceAt(to);
            return targetPiece != nullptr && !isSameColor(targetPiece);
        }
        // En passant
        return canEnPassant(to, board);
    }
    
    return false;
}

/**
 * @brief Checks whether the Pawn can perform an en passant capture.
 * 
 * En passant is possible if:
 * - The opponent’s last move was a two-square pawn advance.
 * - The opponent’s pawn is adjacent horizontally to this pawn.
 * - The target square is directly behind the opponent’s pawn.
 * 
 * @param to The target position for the en passant capture.
 * @param board Reference to the current game board.
 * @return true If en passant is legally possible.
 * @return false Otherwise.
 */
bool Pawn::canEnPassant(const Position& to, Board& board) const {
    Move* lastMove = board.getLastMove();
    if (lastMove == nullptr) return false;
    
    Piece* lastMovedPiece = lastMove->getMovedPiece();
    if (lastMovedPiece == nullptr || lastMovedPiece->getName() != "Pawn") {
        return false;
    }
    
    // Check if last move was a two-square pawn advance
    int lastMoveRowDiff = std::abs(lastMove->getTo().getRow() - lastMove->getFrom().getRow());
    if (lastMoveRowDiff != 2) return false;
    
    // Check if pawns are adjacent
    Position lastMovePos = lastMove->getTo();
    if (lastMovePos.getRow() != position.getRow()) return false;
    if (std::abs(lastMovePos.getCol() - position.getCol()) != 1) return false;
    
    // Check if target square is behind the opponent's pawn
    int direction = getDirection();
    return to.getRow() == lastMovePos.getRow() + direction && to.getCol() == lastMovePos.getCol();
}

/**
 * @brief Retrieves all possible legal moves for the Pawn from its current position.
 * 
 * Includes forward moves (1 or 2 squares), diagonal captures, and possible en passant captures.
 * 
 * @param board Reference to the current game board.
 * @return std::vector<Position> A list of all valid target positions.
 */
std::vector<Position> Pawn::getPossibleMoves(Board& board) {
    std::vector<Position> moves;
    int direction = getDirection();
    
    // Forward one square
    Position forward(position.getRow() + direction, position.getCol());
    if (forward.isValid() && board.isSquareEmpty(forward)) {
        moves.push_back(forward);
        
        // Forward two squares from starting position
        if (!hasMoved) {
            Position forwardTwo(position.getRow() + 2 * direction, position.getCol());
            if (forwardTwo.isValid() && board.isSquareEmpty(forwardTwo)) {
                moves.push_back(forwardTwo);
            }
        }
    }
    
    // Diagonal captures
    for (int colOffset : {-1, 1}) {
        Position diagonal(position.getRow() + direction, position.getCol() + colOffset);
        if (diagonal.isValid()) {
            if (board.isSquareOccupied(diagonal)) {
                Piece* target = board.getPieceAt(diagonal);
                if (target != nullptr && !isSameColor(target)) {
                    moves.push_back(diagonal);
                }
            } else if (canEnPassant(diagonal, board)) {
                moves.push_back(diagonal);
            }
        }
    }
    
    return moves;
}

/**
 * @brief Returns the character symbol representing the Pawn.
 * 
 * Uppercase ('P') for white pawns and lowercase ('p') for black pawns.
 * 
 * @return char The Pawn’s symbol.
 */
char Pawn::getSymbol() const {
    return (color == Color::WHITE) ? 'P' : 'p';
}

/**
 * @brief Returns the name of the piece.
 * @return std::string Always returns "Pawn".
 */
std::string Pawn::getName() const {
    return "Pawn";
}

/**
 * @brief Returns the relative material value of the Pawn.
 * 
 * In standard chess evaluation, the Pawn is worth 1 point.
 * 
 * @return int The value of the Pawn (1).
 */
int Pawn::getValue() const {
    return 1;
}

/**
 * @brief Checks whether the Pawn has reached the promotion rank.
 * 
 * White pawns promote at the top row (row 0), and black pawns at the bottom row (row 7).
 * 
 * @return true If the pawn can be promoted.
 * @return false Otherwise.
 */
bool Pawn::canPromote() const {
    int promotionRow = (color == Color::WHITE) ? 0 : 7;
    return position.getRow() == promotionRow;
}

/**
 * @brief Checks whether the Pawn is still at its starting position.
 * 
 * This is used to determine if the pawn can move two squares forward.
 * 
 * @return true If the pawn is on its original rank.
 * @return false Otherwise.
 */
bool Pawn::isAtStartingPosition() const {
    int startRow = (color == Color::WHITE) ? 6 : 1;
    return position.getRow() == startRow;
}
