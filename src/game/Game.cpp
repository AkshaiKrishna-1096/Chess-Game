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
#include <limits>

/**
 * @brief Constructs a new Game object with two players and initializes the board
 * @param player1Name Name of the white player
 * @param player2Name Name of the black player
 */
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

/**
 * @brief Destroys the Game object and frees all allocated memory
 */
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

/**
 * @brief Starts a new chess game by initializing pieces and resetting counters
 */
void Game::start() {
    // Initialize pieces on the board
    initializePieces();
    state = GameState::ACTIVE;
    moveCount = 0;
    halfMoveClock = 0;
}

/**
 * @brief Initializes all 32 chess pieces in their standard starting positions
 */
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

/**
 * @brief Attempts to execute a chess move and updates game state accordingly
 * @param from Starting position of the piece
 * @param to Destination position
 * @return true if move was successful, false if invalid
 */
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

/**
 * @brief Switches the current player to the opponent
 */
void Game::switchPlayer() {
    currentPlayer = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
}

/**
 * @brief Checks if a hypothetical move would leave the player's king in check
 * @param from Starting position of the move
 * @param to Destination position
 * @param playerColor Color of the player making the move
 * @return true if the move would result in check, false otherwise
 */
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

/**
 * @brief Updates the game state by checking for check, checkmate, stalemate, or draw
 */
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

/**
 * @brief Updates the check status for both white and black players
 */
void Game::updateCheckStatus() {
    whitePlayer->setIsInCheck(board->isKingInCheck(Color::WHITE));
    blackPlayer->setIsInCheck(board->isKingInCheck(Color::BLACK));
}

/**
 * @brief Detects if the specified player is in checkmate
 * @param color Color of the player to check
 * @return true if player is in checkmate, false otherwise
 */
bool Game::detectCheckmate(Color color) {
    return board->isKingInCheck(color) && !hasLegalMoves(color);
}

/**
 * @brief Detects if the specified player is in stalemate
 * @param color Color of the player to check
 * @return true if player is in stalemate, false otherwise
 */
bool Game::detectStalemate(Color color) {
    return !board->isKingInCheck(color) && !hasLegalMoves(color);
}

/**
 * @brief Detects if the game should end in a draw (50-move rule or insufficient material)
 * @return true if draw conditions are met, false otherwise
 */
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

/**
 * @brief Checks if the specified player has any legal moves available
 * @param color Color of the player to check
 * @return true if player has at least one legal move, false otherwise
 */
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

/**
 * @brief Handles a captured piece by updating the current player's score
 * @param capturedPiece Pointer to the piece that was captured
 */
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

/**
 * @brief Handles special chess moves including castling, en passant, and pawn promotion
 * @param move Pointer to the move object to check and update
 */
void Game::handleSpecialMoves(Move* move) {
    Piece* piece = move->getMovedPiece();
    if (piece == nullptr) return;
    
    Position from = move->getFrom();
    Position to = move->getTo();
    
    // Check for castling (King moving 2 squares horizontally)
    if (piece->getName() == "King") {
        int colDiff = std::abs(to.getCol() - from.getCol());
        if (colDiff == 2) {
            handleCastling(from, to);
            move->setIsCastling(true);
            return;
        }
    }
    
    // Check for en passant (Pawn capturing diagonally to empty square)
    if (piece->getName() == "Pawn") {
        int colDiff = std::abs(to.getCol() - from.getCol());
        if (colDiff == 1 && board->isSquareEmpty(to)) {
            // This is en passant
            handleEnPassant(from, to);
            move->setIsEnPassant(true);
        }
        
        // Check for promotion (Pawn reaching opposite end)
        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        if (pawn && pawn->canPromote()) {
            handlePromotion(to);
            move->setIsPromotion(true);
        }
    }
}

/**
 * @brief Executes a castling move by moving the rook to its correct position
 * @param from King's starting position
 * @param to King's destination position
 * @return true if castling was successful, false otherwise
 */
bool Game::handleCastling(const Position& from, const Position& to) {
    // Determine if king-side or queen-side castling
    bool kingSide = (to.getCol() > from.getCol());
    int row = from.getRow();
    
    Position rookFrom, rookTo;
    
    if (kingSide) {
        // King-side castling: Rook moves from h-file to f-file
        rookFrom = Position(row, 7);
        rookTo = Position(row, 5);
    } else {
        // Queen-side castling: Rook moves from a-file to d-file
        rookFrom = Position(row, 0);
        rookTo = Position(row, 3);
    }
    
    // Move the rook
    Piece* rook = board->getPieceAt(rookFrom);
    if (rook != nullptr && rook->getName() == "Rook") {
        board->getSquare(rookFrom)->removePiece();
        board->getSquare(rookTo)->setPiece(rook);
        rook->setPosition(rookTo);
        rook->setHasMoved(true);
        return true;
    }
    
    return false;
}

/**
 * @brief Executes an en passant capture by removing the captured pawn
 * @param from Attacking pawn's starting position
 * @param to Attacking pawn's destination position
 * @return true if en passant was successful, false otherwise
 */
bool Game::handleEnPassant(const Position& from, const Position& to) {
    // Get the captured pawn's position (same column as 'to', same row as 'from')
    Position capturedPawnPos(from.getRow(), to.getCol());
    
    Piece* capturedPawn = board->getPieceAt(capturedPawnPos);
    if (capturedPawn != nullptr && capturedPawn->getName() == "Pawn") {
        // Remove the captured pawn
        board->removePiece(capturedPawnPos);
        
        // Add score for capturing the pawn
        handleCapture(capturedPawn);
        
        // Delete the captured pawn
        delete capturedPawn;
        
        return true;
    }
    
    return false;
}

/**
 * @brief Handles pawn promotion by replacing the pawn with a chosen piece
 * @param to Position of the pawn to promote
 * @return true if promotion was successful, false otherwise
 */
bool Game::handlePromotion(const Position& to) {
    Piece* pawn = board->getPieceAt(to);
    if (pawn == nullptr || pawn->getName() != "Pawn") {
        return false;
    }
    
    Color color = pawn->getColor();
    
    // Ask player what piece they want to promote to
    std::cout << "\n*** PAWN PROMOTION ***\n";
    std::cout << "Choose piece to promote to:\n";
    std::cout << "  Q - Queen\n";
    std::cout << "  R - Rook\n";
    std::cout << "  B - Bishop\n";
    std::cout << "  N - Knight\n";
    std::cout << "Enter choice (Q/R/B/N): ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    choice = std::toupper(choice);
    
    // Default to Queen if invalid input
    if (choice != 'Q' && choice != 'R' && choice != 'B' && choice != 'N') {
        std::cout << "Invalid choice. Promoting to Queen by default.\n";
        choice = 'Q';
    }
    
    // Remove the pawn from board
    board->removePiece(to);
    delete pawn;
    
    // Create the new piece
    Piece* newPiece = nullptr;
    switch (choice) {
        case 'Q':
            newPiece = new Queen(color, to);
            std::cout << "Promoted to Queen!\n";
            break;
        case 'R':
            newPiece = new Rook(color, to);
            std::cout << "Promoted to Rook!\n";
            break;
        case 'B':
            newPiece = new Bishop(color, to);
            std::cout << "Promoted to Bishop!\n";
            break;
        case 'N':
            newPiece = new Knight(color, to);
            std::cout << "Promoted to Knight!\n";
            break;
    }
    
    // Add the new piece to the board
    if (newPiece != nullptr) {
        newPiece->setHasMoved(true);
        board->addPiece(newPiece, to);
        return true;
    }
    
    return false;
}

/**
 * @brief Gets the current game state
 * @return Current GameState enum value
 */
GameState Game::getState() const { return state; }

/**
 * @brief Gets the current player
 * @return Pointer to the current Player object
 */
Player* Game::getCurrentPlayer() const { return currentPlayer; }

/**
 * @brief Gets the winner of the game if in checkmate state
 * @return Pointer to winning Player or nullptr if no winner yet
 */
Player* Game::getWinner() const {
    if (state == GameState::CHECKMATE) {
        return (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
    }
    return nullptr;
}

/**
 * @brief Gets the game board (for testing purposes)
 * @return Pointer to the Board object
 */
Board* Game::getBoard() const {
    return board;
}

/**
 * @brief Displays the current state of the chess board
 */
void Game::displayBoard() const {
    if (board != nullptr) {
        board->display();
    }
}
