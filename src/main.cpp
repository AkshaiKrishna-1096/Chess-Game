// ============================================================================
// File: src/main.cpp
// Description: Entry point for the Chess Game application
// Demonstrates OOP concepts and game flow
// ============================================================================

#include "../include/ChessGame.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

// Function prototypes
void displayWelcomeMessage();
void displayHelp();
bool parseInput(const string& input, Position& from, Position& to);
string positionToString(const Position& pos);
void clearInputBuffer();
void displayGameResult(Game* game);

int main() {
    // Display welcome message
    displayWelcomeMessage();
    
    // Get player names
    string player1Name, player2Name;
    cout << "\nEnter White player name: ";
    getline(cin, player1Name);
    if (player1Name.empty()) player1Name = "White Player";
    
    cout << "Enter Black player name: ";
    getline(cin, player2Name);
    if (player2Name.empty()) player2Name = "Black Player";
    
    // Create game object (demonstrates Object Creation and Composition)
    Game* game = new Game(player1Name, player2Name);
    
    cout << "\nGame starting!\n";
    cout << "Type 'help' for commands, 'quit' to exit\n\n";
    
    // Start the game
    game->start();
    
    // Main game loop
    string input;
    bool gameRunning = true;
    
    while (gameRunning) {
        // Display current board state
        game->displayBoard();
        
        // Check game state
        GameState state = game->getState();
        
        if (state == GameState::CHECKMATE) {
            displayGameResult(game);
            break;
        } else if (state == GameState::STALEMATE) {
            cout << "\n=================================\n";
            cout << "        STALEMATE - DRAW!        \n";
            cout << "=================================\n";
            break;
        } else if (state == GameState::DRAW) {
            cout << "\n=================================\n";
            cout << "            DRAW GAME!           \n";
            cout << "=================================\n";
            break;
        } else if (state == GameState::CHECK) {
            cout << "\n*** CHECK! ***\n";
        }
        
        // Display current player
        Player* currentPlayer = game->getCurrentPlayer();
        cout << "\n" << currentPlayer->getName() << "'s turn (";
        cout << (currentPlayer->getColor() == Color::WHITE ? "White" : "Black") << ")\n";
        cout << "Enter move (e.g., 'e2 e4') or command: ";
        
        // Get user input
        getline(cin, input);
        
        // Trim whitespace
        size_t start = input.find_first_not_of(" \t\n\r");
        size_t end = input.find_last_not_of(" \t\n\r");
        if (start != string::npos && end != string::npos) {
            input = input.substr(start, end - start + 1);
        }
        
        // Convert to lowercase for command checking
        string lowerInput = input;
        for (char& c : lowerInput) c = tolower(c);
        
        // Handle commands
        if (lowerInput == "quit" || lowerInput == "exit" || lowerInput == "q") {
            cout << "\nThanks for playing! Goodbye.\n";
            gameRunning = false;
            break;
        } else if (lowerInput == "help" || lowerInput == "h") {
            displayHelp();
            continue;
        } else if (lowerInput == "board" || lowerInput == "b") {
            // Just redisplay board (will happen at next loop)
            continue;
        } else {
            // Parse move input
            Position from, to;
            if (parseInput(input, from, to)) {
                // Attempt to make the move (demonstrates Polymorphism and Encapsulation)
                bool moveSuccess = game->makeMove(from, to);
                
                if (!moveSuccess) {
                    cout << "\nInvalid move! Try again.\n";
                } else {
                    cout << "\nMove executed: " << positionToString(from) 
                         << " -> " << positionToString(to) << "\n";
                }
            } else {
                cout << "\nInvalid input format! Use format like 'e2 e4' or type 'help'\n";
            }
        }
    }
    
    // Clean up (demonstrates proper memory management)
    delete game;
    
    cout << "\nPress Enter to exit...";
    cin.get();
    
    return 0;
}

/**
 * @brief Display the Welcome Message
 * 
 */
void displayWelcomeMessage() {
    cout << "=======================================================\n";
    cout << "                                                       \n";
    cout << "    ♔ ♕ ♖ ♗ ♘ ♙  CHESS GAME  ♟ ♞ ♝ ♜ ♛ ♚              \n";
    cout << "                                                       \n";
    cout << "           Object-Oriented Programming Demo           \n";
    cout << "                                                       \n";
    cout << "=======================================================\n";
}

/**
 * @brief Display Help Menu
 * 
 */
void displayHelp() {
    cout << "\n=======================================================\n";
    cout << "                    HELP MENU                          \n";
    cout << "=======================================================\n";
    cout << "\nMOVE FORMAT:\n";
    cout << "  Use algebraic notation: [from] [to]\n";
    cout << "  Examples: e2 e4, g1 f3, e7 e5\n";
    cout << "  - Columns: a-h (left to right)\n";
    cout << "  - Rows: 1-8 (bottom to top for White)\n";
    cout << "\nCOMMANDS:\n";
    cout << "  help/h    - Display this help menu\n";
    cout << "  board/b   - Redisplay the board\n";
    cout << "  quit/q    - Exit the game\n";
    cout << "\nPIECE SYMBOLS:\n";
    cout << "  K/k = King    Q/q = Queen   R/r = Rook\n";
    cout << "  B/b = Bishop  N/n = Knight  P/p = Pawn\n";
    cout << "  (Uppercase = White, Lowercase = Black)\n";
    cout << "\nGAME RULES:\n";
    cout << "  - Standard chess rules apply\n";
    cout << "  - Move pieces according to their type\n";
    cout << "  - Capture opponent's king to win\n";
    cout << "  - Avoid leaving your king in check\n";
    cout << "=======================================================\n";
}

/**
 * @brief Convert the given String into 2 substring (from and to)
 * 
 * @details 
 * This function does two process. This function will take input String and address of String to and from.
 * The String input is broken down and converted into Position object after checking if this is valid.
 * Return whether the position are valid or not
 * 
 * @param input String
 * @param from String
 * @param to String
 * @return true 
 * @return false 
 */
bool parseInput(const string& input, Position& from, Position& to) {
    // Expected format: "e2 e4" or "e2e4" or "e2-e4"
    stringstream ss(input);
    string fromStr, toStr;
    
    // Try to parse with space separator
    ss >> fromStr >> toStr;
    
    // If no space, try to split at middle or dash
    if (toStr.empty()) {
        size_t dashPos = fromStr.find('-');
        if (dashPos != string::npos) {
            toStr = fromStr.substr(dashPos + 1);
            fromStr = fromStr.substr(0, dashPos);
        } else if (fromStr.length() == 4) {
            toStr = fromStr.substr(2, 2);
            fromStr = fromStr.substr(0, 2);
        }
    }
    
    // Validate format: should be like "e2" (letter + number)
    if (fromStr.length() != 2 || toStr.length() != 2) {
        return false;
    }
    
    // Convert to lowercase for column
    fromStr[0] = tolower(fromStr[0]);
    toStr[0] = tolower(toStr[0]);
    
    // Validate column (a-h) and row (1-8)
    if (fromStr[0] < 'a' || fromStr[0] > 'h' || 
        toStr[0] < 'a' || toStr[0] > 'h' ||
        fromStr[1] < '1' || fromStr[1] > '8' ||
        toStr[1] < '1' || toStr[1] > '8') {
        return false;
    }
    
    // Convert algebraic notation to array indices
    // 'a' = column 0, '1' = row 7 (from bottom), '8' = row 0 (from top)
    int fromCol = fromStr[0] - 'a';
    int fromRow = 8 - (fromStr[1] - '0');  // Invert: algebraic 1 = array row 7
    
    int toCol = toStr[0] - 'a';
    int toRow = 8 - (toStr[1] - '0');
    
    from = Position(fromRow, fromCol);
    to = Position(toRow, toCol);
    
    return from.isValid() && to.isValid();
}

/**
 * @brief 
 * Convert the Position into rank and file from Position
 * The value of Position is in Integer
 * 
 * @param pos // use 'int' to get the position
 * @return string 
 */
string positionToString(const Position& pos) {
    // Convert array indices back to algebraic notation
    char col = 'a' + pos.getCol();
    char row = '1' + (7 - pos.getRow());  // Invert back
    return string(1, col) + string(1, row);
}

/**
 * @brief Remove any unwanted buffer still persisting in the buffer space
 * 
 */
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * @brief Show the Game result
 * 
 * @param game 
 */
void displayGameResult(Game* game) {
    Player* winner = game->getWinner();
    cout << "\n=======================================================\n";
    cout << "                   CHECKMATE!                          \n";
    cout << "=======================================================\n";
    if (winner != nullptr) {
        cout << "\n🏆 " << winner->getName() << " (";
        cout << (winner->getColor() == Color::WHITE ? "White" : "Black");
        cout << ") WINS! 🏆\n\n";
    }
    cout << "=======================================================\n";
}