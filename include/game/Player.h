// ============================================================================
// File: include/game/Player.h
// Description: Class representing a chess player
// Demonstrates: Encapsulation, Data Management
// ============================================================================

#ifndef PLAYER_H
#define PLAYER_H

#include "../utils/Color.h"
#include <string>

class Player {
private:
    std::string name;
    Color color;
    bool isInCheck;
    int score;
    int capturedPieceValue;

public:
    // Constructors
    Player();
    Player(const std::string& playerName, Color playerColor);
    
    // Copy constructor
    Player(const Player& other);
    
    // Assignment operator
    Player& operator=(const Player& other);
    
    // Destructor
    ~Player();
    
    // Getters (Encapsulation)
    std::string getName() const;
    Color getColor() const;
    bool getIsInCheck() const;
    int getScore() const;
    int getCapturedPieceValue() const;
    
    // Setters
    void setName(const std::string& playerName);
    void setIsInCheck(bool check);
    void setScore(int newScore);
    
    // Score management
    void addScore(int points);
    void addCapturedPieceValue(int value);
    void resetScore();
    
    // Utility
    std::string toString() const;
    bool isWhite() const;
    bool isBlack() const;
};

#endif // PLAYER_H