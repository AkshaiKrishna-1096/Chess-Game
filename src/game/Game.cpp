#include "../../include/game/Game.h"
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/Pawn.h"
#include "../../include/pieces/Rook.h"
#include "../../include/pieces/Knight.h"
#include "../../include/pieces/Bishop.h"
#include "../../include/pieces/Queen.h"
#include "../../include/pieces/King.h"
#include <iostream>
#include <algorithm>

// Constructor
Game::Game(const std::string& player1Name, const std::string& player2Name)
    : board(nullptr), whitePlayer(nullptr), blackPlayer(nullptr), 
      currentPlayer(nullptr), state(GameState::ACTIVE), 
      moveCount(0), halfMoveClock(0) {
    
    // Create board (Composition)
    board = new Board();
    
    // Create players (Composition)
    whitePlayer = new Player(player1Name, Color::WHITE);
    blackPlayer = new Player(player2Name, Color::BLACK);
    
    // White starts first
    currentPlayer = whitePlayer;
}

// Destructor
Game::~Game() {
    // Delete board
    if (board != nullptr) {
        delete board;
        board = nullptr;
    }
    
    // Delete players
    if (whitePlayer != nullptr) {
        delete whitePlayer;
        whitePlayer = nullptr;
    }
    if (blackPlayer != nullptr) {
        delete blackPlayer;
        blackPlayer = nullptr;
    }
    
    // Delete all pieces from both colors
    std::vector<Piece*> allPieces = board ? board->getAllPieces() : std::vector<Piece*>();
    for (Piece* piece : allPieces) {
        delete piece;
    }
    
    // Delete move history
    for (Move* move : moveHistory) {
        delete move;
    }
    moveHistory.clear();
}

// Start the game
void Game::start() {
    // Initialize pieces on the board
    initializePieces();
    board->initialize();
    state = GameState::ACTIVE;
    moveCount = 0;
    halfMoveClock = 0;
}

// Initialize all pieces
void Game::initializePieces() {
    // Clear any existing pieces
    board->clear();
    
    // Create and place White pieces
    // Pawns
    for (int col = 0; col < 8; col++) {
        Piece* pawn = new Pawn(Color::WHITE, Position(6, col));
        board->addPiece(pawn, Position(6, col));
    }
    
    // Rooks
    board->addPiece(new Rook(Color::WHITE, Position(7, 0)), Position(7, 0));
    board->addPiece(new Rook(Color::WHITE, Position(7, 7)), Position(7, 7));
    
    // Knights
    board->addPiece(new Knight(Color::WHITE, Position(7, 1)), Position(7, 1));
    board->addPiece(new Knight(Color::WHITE, Position(7, 6)), Position(7, 6));
    
    // Bishops
    board->addPiece(new Bishop(Color::WHITE, Position(7, 2)), Position(7, 2));
    board->addPiece(new Bishop(Color::WHITE, Position(7, 5)), Position(7, 5));
    
    // Queen
    board->addPiece(new Queen(Color::WHITE, Position(7, 3)), Position(7, 3));
    
    // King
    board->addPiece(new King(Color::WHITE, Position(7, 4)), Position(7, 4));
    
    // Create and place Black pieces
    // Pawns
    for (int col = 0; col < 8; col++) {
        Piece* pawn = new Pawn(Color::BLACK, Position(1, col));
        board->addPiece(pawn, Position(1, col));
    }
    
    // Rooks
    board->addPiece(new Rook(Color::BLACK, Position(0, 0)), Position(0, 0));
    board->addPiece(new Rook(Color::BLACK, Position(0, 7)), Position(0, 7));
    
    // Knights
    board->addPiece(new Knight(Color::BLACK, Position(0, 1)), Position(0, 1));
    board->addPiece(new Knight(Color::BLACK, Position(0, 6)), Position(0, 6));
    
    // Bishops
    board->addPiece(new Bishop(Color::BLACK, Position(0, 2)), Position(0, 2));
    board->addPiece(new Bishop(Color::BLACK, Position(0, 5)), Position(0, 5));
    
    // Queen
    board->addPiece(new Queen(Color::BLACK, Position(0, 3)), Position(0, 3));
    
    // King
    board->addPiece(new King(Color::BLACK, Position(0, 4)), Position(0, 4));
}

// Make a move
bool Game::makeMove(const Position& from, const Position& to) {
    // Validate positions
    if (!board->isValidPosition(from) || !board->isValidPosition(to)) {
        return false;
    }
    
    // Get piece at from position
    Piece* piece = board->getPieceAt(from);
    if (piece == nullptr) {
        return false;
    }
    
    // Check if it's the current player's piece
    if (piece->getColor() != currentPlayer->getColor()) {
        return false;
    }
    
    // Check if move is valid for this piece
    if (!piece->isValidMove(to, *board)) {
        return false;
    }
    
    // Check if move would leave king in check
    if (wouldBeInCheck(from, to, currentPlayer->getColor())) {
        return false;
    }
    
    // Get captured piece (if any)
    Piece* capturedPiece = board->getPieceAt(to);
    
    // Create move object
    Move* move = new Move(from, to, piece, capturedPiece);
    
    // Handle special moves (castling, en passant, promotion)
    handleSpecialMoves(move);
    
    // Execute the move
    board->movePiece(from, to);
    
    // Handle capture
    if (capturedPiece != nullptr) {
        handleCapture(capturedPiece);
        move->setCapturedPiece(capturedPiece);
    }
    
    // Add move to history
    moveHistory.push_back(move);
    board->setLastMove(move);
    
    // Update move counters
    moveCount++;
    if (capturedPiece != nullptr || piece->getName() == "Pawn") {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }
    
    // Switch player
    switchPlayer();
    
    // Update game state
    updateGameState();
    
    return true;
}

// Switch current player
void Game::switchPlayer() {
    currentPlayer = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
}

// Check if move would result in check
bool Game::wouldBeInCheck(const Position& from, const Position& to, Color playerColor) {
    // Temporarily make the move
    Piece* movingPiece = board->getPieceAt(from);
    Piece* capturedPiece = board->getPieceAt(to);
    
    // Execute temporary move
    board->getSquare(from)->removePiece();
    board->getSquare(to)->setPiece(movingPiece);
    movingPiece->setPosition(to);
    
    // Check if king is in check
    bool inCheck = board->isKingInCheck(playerColor);
    
    // Undo the move
    board->getSquare(to)->removePiece();
    board->getSquare(from)->setPiece(movingPiece);
    movingPiece->setPosition(from);
    if (capturedPiece != nullptr) {
        board->getSquare(to)->setPiece(capturedPiece);
    }
    
    return inCheck;
}

// Update game state
void Game::updateGameState() {
    updateCheckStatus();
    
    Color currentColor = currentPlayer->getColor();
    
    if (detectCheckmate(currentColor)) {
        state = GameState::CHECKMATE;
    } else if (detectStalemate(currentColor)) {
        state = GameState::STALEMATE;
    } else if (detectDraw()) {
        state = GameState::DRAW;
    } else if (currentPlayer->getIsInCheck()) {
        state = GameState::CHECK;
    } else {
        state = GameState::ACTIVE;
    }
}

// Update check status for both players
void Game::updateCheckStatus() {
    whitePlayer->setIsInCheck(board->isKingInCheck(Color::WHITE));
    blackPlayer->setIsInCheck(board->isKingInCheck(Color::BLACK));
}

// Detect checkmate
bool Game::detectCheckmate(Color color) {
    return board->isKingInCheck(color) && !hasLegalMoves(color);
}

// Detect stalemate
bool Game::detectStalemate(Color color) {
    return !board->isKingInCheck(color) && !hasLegalMoves(color);
}

// Detect draw
bool Game::detectDraw() {
    // 50-move rule
    if (halfMoveClock >= 100) {  // 50 moves per player = 100 half-moves
        return true;
    }
    
    // Insufficient material (simplified check)
    std::vector<Piece*> allPieces = board->getAllPieces();
    if (allPieces.size() <= 3) {
        // Only kings, or kings with one minor piece
        return true;
    }
    
    return false;
}

// Check if player has legal moves
bool Game::hasLegalMoves(Color color) {
    std::vector<Piece*> pieces = board->getPieces(color);
    
    for (Piece* piece : pieces) {
        std::vector<Position> possibleMoves = piece->getPossibleMoves(*board);
        for (const Position& move : possibleMoves) {
            if (!wouldBeInCheck(piece->getPosition(), move, color)) {
                return true;
            }
        }
    }
    return false;
}

// Handle captured piece
void Game::handleCapture(Piece* capturedPiece) {
    if (capturedPiece == nullptr) return;
    
    // Add score to current player based on piece value
    int pieceValue = 0;
    std::string pieceName = capturedPiece->getName();
    
    if (pieceName == "Pawn") pieceValue = 1;
    else if (pieceName == "Knight" || pieceName == "Bishop") pieceValue = 3;
    else if (pieceName == "Rook") pieceValue = 5;
    else if (pieceName == "Queen") pieceValue = 9;
    
    currentPlayer->addCapturedPieceValue(pieceValue);
}

// Handle special moves
void Game::handleSpecialMoves(Move* move) {
    // Check for castling, en passant, promotion
    // Implementation depends on specific piece logic
}

bool Game::handleCastling(const Position& from, const Position& to) {
    // Castling logic
    return false;
}

bool Game::handleEnPassant(const Position& from, const Position& to) {
    // En passant logic
    return false;
}

bool Game::handlePromotion(const Position& to) {
    // Promotion logic
    return false;
}

// Getters
GameState Game::getState() const { return state; }
Player* Game::getCurrentPlayer() const { return currentPlayer; }
Player* Game::getWhitePlayer() const { return whitePlayer; }
Player* Game::getBlackPlayer() const { return blackPlayer; }
Board* Game::getBoard() const { return board; }
int Game::getMoveCount() const { return moveCount; }

Player* Game::getWinner() const {
    if (state == GameState::CHECKMATE) {
        return (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
    }
    return nullptr;
}

// Check queries
bool Game::isCheck(Color color) const {
    return board->isKingInCheck(color);
}

bool Game::isCheckmate(Color color) const {
    return state == GameState::CHECKMATE;
}

bool Game::isStalemate(Color color) const {
    return state == GameState::STALEMATE;
}

bool Game::isDraw() const {
    return state == GameState::DRAW;
}

// Move history
std::vector<Move*> Game::getMoveHistory() const {
    return moveHistory;
}

Move* Game::getLastMove() const {
    if (moveHistory.empty()) return nullptr;
    return moveHistory.back();
}

// Display methods
void Game::displayBoard() const {
    if (board != nullptr) {
        board->display();
    }
}

void Game::displayGameInfo() const {
    std::cout << "\n=== Game Information ===\n";
    std::cout << whitePlayer->toString() << "\n";
    std::cout << blackPlayer->toString() << "\n";
    std::cout << "Move Count: " << moveCount << "\n";
    std::cout << "Current Player: " << currentPlayer->getName() << "\n";
    std::cout << "Game State: " << getGameStatus() << "\n";
}

void Game::displayMoveHistory() const {
    std::cout << "\n=== Move History ===\n";
    for (size_t i = 0; i < moveHistory.size(); i++) {
        std::cout << (i + 1) << ". " << moveHistory[i]->toString() << "\n";
    }
}

// Game status string
std::string Game::getGameStatus() const {
    switch (state) {
        case GameState::ACTIVE: return "Active";
        case GameState::CHECK: return "Check";
        case GameState::CHECKMATE: return "Checkmate";
        case GameState::STALEMATE: return "Stalemate";
        case GameState::DRAW: return "Draw";
        default: return "Unknown";
    }
}

// Reset game
void Game::reset() {
    // Clear board and reinitialize
    board->clear();
    start();
    
    // Reset players
    whitePlayer->resetScore();
    blackPlayer->resetScore();
    whitePlayer->setIsInCheck(false);
    blackPlayer->setIsInCheck(false);
    
    // Clear move history
    for (Move* move : moveHistory) {
        delete move;
    }
    moveHistory.clear();
    
    // Reset state
    currentPlayer = whitePlayer;
    state = GameState::ACTIVE;
    moveCount = 0;
    halfMoveClock = 0;
}

// Undo last move (optional implementation)
void Game::undoMove() {
    // Implementation for undo functionality
    // This is complex and requires storing board states
}