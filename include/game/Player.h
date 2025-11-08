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
    
    Player();
    Player(const std::string& playerName, Color playerColor);
    
    
    Player(const Player& other);
    
    
    Player& operator=(const Player& other);
    
    
    ~Player();
    
    
    std::string getName() const;
    Color getColor() const;
    bool getIsInCheck() const;
    int getScore() const;
    int getCapturedPieceValue() const;
    
    
    void setName(const std::string& playerName);
    void setIsInCheck(bool check);
    void setScore(int newScore);
    
    
    void addScore(int points);
    void addCapturedPieceValue(int value);
    void resetScore();
    
    
    std::string toString() const;
    bool isWhite() const;
    bool isBlack() const;
};

#endif 