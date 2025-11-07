#include "../../include/pieces/Pawn.h"
#include "../../include/board/Board.h"
#include "../../include/game/Move.h"
#include <cmath>

Pawn::Pawn(Color c, const Position& pos) : Piece(c, pos) {}

Pawn::~Pawn() {}

int Pawn::getDirection() const {
    return (color == Color::WHITE) ? -1 : 1; // White moves up (decreasing row), Black moves down
}

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

char Pawn::getSymbol() const {
    return (color == Color::WHITE) ? 'P' : 'p';
}

std::string Pawn::getName() const {
    return "Pawn";
}

int Pawn::getValue() const {
    return 1;
}

bool Pawn::canPromote() const {
    int promotionRow = (color == Color::WHITE) ? 0 : 7;
    return position.getRow() == promotionRow;
}

bool Pawn::isAtStartingPosition() const {
    int startRow = (color == Color::WHITE) ? 6 : 1;
    return position.getRow() == startRow;
}