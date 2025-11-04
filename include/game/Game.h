
#ifndef GAME_H
#define GAME_H

#include "../board/Board.h"
#include "Player.h"
#include "Move.h"
#include "../utils/GameState.h"
#include <vector>
#include <string>

class Game {
private:
    Board* board;
    Player* whitePlayer;
    Player* blackPlayer;
    Player* currentPlayer;
    std::vector<Move*> moveHistory;
    GameState state;
    int moveCount;
    int halfMoveClock;  
    
    
    void initializePieces();
    void switchPlayer();
    bool isValidGameMove(const Position& from, const Position& to);
    bool wouldBeInCheck(const Position& from, const Position& to, Color playerColor);
    void updateGameState();
    bool hasLegalMoves(Color color);
    void handleCapture(Piece* capturedPiece);
    void handleSpecialMoves(Move* move);
    bool handleCastling(const Position& from, const Position& to);
    bool handleEnPassant(const Position& from, const Position& to);
    bool handlePromotion(const Position& to);
    
    
    void updateCheckStatus();
    bool detectCheckmate(Color color);
    bool detectStalemate(Color color);
    bool detectDraw();

public:
    
    Game(const std::string& player1Name, const std::string& player2Name);
    ~Game();
    
   
    void start();
    bool makeMove(const Position& from, const Position& to);
    void undoMove(); 
    void reset();
    
    
    GameState getState() const;
    Player* getCurrentPlayer() const;
    Player* getWinner() const;
    Player* getWhitePlayer() const;
    Player* getBlackPlayer() const;
    Board* getBoard() const;
    int getMoveCount() const;
    
    
    bool isCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    bool isDraw() const;
    
    
    std::vector<Move*> getMoveHistory() const;
    Move* getLastMove() const;
    
    
    void displayBoard() const;
    void displayGameInfo() const;
    void displayMoveHistory() const;
    
    
    std::string getGameStatus() const;
};

#endif 