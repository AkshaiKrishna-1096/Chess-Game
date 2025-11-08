#include "../../include/game/Player.h"
#include <sstream>

/**
 * @brief Default constructor for the Player class.
 * 
 * Initializes a player with the default name "Player", white color,
 * zero score, and not in check.
 */
Player::Player() 
    : name("Player"), color(Color::WHITE), isInCheck(false), 
      score(0), capturedPieceValue(0) {}

/**
 * @brief Parameterized constructor for the Player class.
 * 
 * @param playerName The name of the player.
 * @param playerColor The color assigned to the player (WHITE or BLACK).
 */
Player::Player(const std::string& playerName, Color playerColor)
    : name(playerName), color(playerColor), isInCheck(false), 
      score(0), capturedPieceValue(0) {}

/**
 * @brief Copy constructor for the Player class.
 * 
 * @param other The Player object to copy data from.
 */
Player::Player(const Player& other)
    : name(other.name), color(other.color), isInCheck(other.isInCheck),
      score(other.score), capturedPieceValue(other.capturedPieceValue) {}

/**
 * @brief Assignment operator for the Player class.
 * 
 * Copies all data members from another Player object.
 * 
 * @param other The Player object to assign from.
 * @return Player& Reference to the updated Player object.
 */
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

/**
 * @brief Destructor for the Player class.
 */
Player::~Player() {}

/**
 * @brief Gets the player's name.
 * 
 * @return std::string The name of the player.
 */
std::string Player::getName() const { return name; }

/**
 * @brief Gets the player's color.
 * 
 * @return Color The color of the player (WHITE or BLACK).
 */
Color Player::getColor() const { return color; }

/**
 * @brief Checks if the player is currently in check.
 * 
 * @return true If the player is in check.
 * @return false Otherwise.
 */
bool Player::getIsInCheck() const { return isInCheck; }

/**
 * @brief Retrieves the player’s total score.
 * 
 * @return int The current score of the player.
 */
int Player::getScore() const { return score; }

/**
 * @brief Retrieves the total value of pieces captured by the player.
 * 
 * @return int The sum of captured piece values.
 */
int Player::getCapturedPieceValue() const { return capturedPieceValue; }

/**
 * @brief Sets the player's name.
 * 
 * @param playerName The new name to assign.
 */
void Player::setName(const std::string& playerName) { name = playerName; }

/**
 * @brief Updates the player's "in check" status.
 * 
 * @param check True if the player is in check, false otherwise.
 */
void Player::setIsInCheck(bool check) { isInCheck = check; }

/**
 * @brief Updates the player's score.
 * 
 * @param newScore The new score value to assign.
 */
void Player::setScore(int newScore) { score = newScore; }

/**
 * @brief Adds points to the player’s current score.
 * 
 * @param points The number of points to add.
 */
void Player::addScore(int points) {
    score += points;
}

/**
 * @brief Adds the value of a captured piece to the player's total.
 * 
 * This function also increases the player's overall score by the same value.
 * 
 * @param value The point value of the captured piece.
 */
void Player::addCapturedPieceValue(int value) {
    capturedPieceValue += value;
    score += value;
}

/**
 * @brief Resets the player’s score and captured piece total to zero.
 */
void Player::resetScore() {
    score = 0;
    capturedPieceValue = 0;
}

/**
 * @brief Returns a string representation of the player's current status.
 * 
 * Includes the player’s name, color, score, and “IN CHECK” status if applicable.
 * 
 * @return std::string A formatted description of the player.
 */
std::string Player::toString() const {
    std::ostringstream oss;
    oss << name << " (" << (color == Color::WHITE ? "White" : "Black") << ")";
    oss << " - Score: " << score;
    if (isInCheck) {
        oss << " [IN CHECK]";
    }
    return oss.str();
}

/**
 * @brief Checks if the player is playing as white.
 * 
 * @return true If the player’s color is WHITE.
 * @return false Otherwise.
 */
bool Player::isWhite() const {
    return color == Color::WHITE;
}

/**
 * @brief Checks if the player is playing as black.
 * 
 * @return true If the player’s color is BLACK.
 * @return false Otherwise.
 */
bool Player::isBlack() const {
    return color == Color::BLACK;
}
