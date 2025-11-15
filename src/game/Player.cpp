#include "../../include/game/Player.h"

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
 * @brief Updates the player's "in check" status.
 * 
 * @param check True if the player is in check, false otherwise.
 */
void Player::setIsInCheck(bool check) { isInCheck = check; }

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
