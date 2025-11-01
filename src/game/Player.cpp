
// ============================================================================
// File: src/game/Player.cpp
// Description: Implementation of Player class
// ============================================================================

#include "../../include/game/Player.h"
#include <sstream>

// Default constructor
Player::Player() 
    : name("Player"), color(Color::WHITE), isInCheck(false), 
      score(0), capturedPieceValue(0) {}

// Parameterized constructor
Player::Player(const std::string& playerName, Color playerColor)
    : name(playerName), color(playerColor), isInCheck(false), 
      score(0), capturedPieceValue(0) {}

// Copy constructor
Player::Player(const Player& other)
    : name(other.name), color(other.color), isInCheck(other.isInCheck),
      score(other.score), capturedPieceValue(other.capturedPieceValue) {}

// Assignment operator
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;
        color = other.color;
        isInCheck = other.isInCheck;
        score = other.score;
        capturedPieceValue = other.capturedPieceValue;
    }
    return *this;
}

// Destructor
Player::~Player() {}

// Getters
std::string Player::getName() const { return name; }
Color Player::getColor() const { return color; }
bool Player::getIsInCheck() const { return isInCheck; }
int Player::getScore() const { return score; }
int Player::getCapturedPieceValue() const { return capturedPieceValue; }

// Setters
void Player::setName(const std::string& playerName) { name = playerName; }
void Player::setIsInCheck(bool check) { isInCheck = check; }
void Player::setScore(int newScore) { score = newScore; }

// Score management
void Player::addScore(int points) {
    score += points;
}

void Player::addCapturedPieceValue(int value) {
    capturedPieceValue += value;
    score += value;
}

void Player::resetScore() {
    score = 0;
    capturedPieceValue = 0;
}

// String representation
std::string Player::toString() const {
    std::ostringstream oss;
    oss << name << " (" << (color == Color::WHITE ? "White" : "Black") << ")";
    oss << " - Score: " << score;
    if (isInCheck) {
        oss << " [IN CHECK]";
    }
    return oss.str();
}

// Color checks
bool Player::isWhite() const {
    return color == Color::WHITE;
}

bool Player::isBlack() const {
    return color == Color::BLACK;
}