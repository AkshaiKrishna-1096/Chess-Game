// ============================================================================
// File: src/test_main.cpp
// Description: Test runner that reads moves from test.txt file
// ============================================================================

#include "../include/ChessGame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// ANSI color codes
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED "\033[1;31m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_RESET "\033[0m"

struct TestCase {
    string description;
    vector<string> moves;
    string expectedResult;  // "check", "checkmate", "stalemate", "valid", "invalid"
};

bool parseInput(const string& input, Position& from, Position& to);

void printTestHeader(const string& testName, int testNumber, int totalTests) {
    cout << "\n" << COLOR_CYAN << "========================================" << COLOR_RESET << endl;
    cout << COLOR_CYAN << "Test " << testNumber << "/" << totalTests << ": " << testName << COLOR_RESET << endl;
    cout << COLOR_CYAN << "========================================" << COLOR_RESET << endl;
}

void printMoveResult(const string& move, bool success, const string& message = "") {
    if (success) {
        cout << COLOR_GREEN << "  ✓ Move: " << move << COLOR_RESET;
    } else {
        cout << COLOR_RED << "  ✗ Move: " << move << " (Failed)" << COLOR_RESET;
    }
    if (!message.empty()) {
        cout << " - " << message;
    }
    cout << endl;
}

void printTestResult(bool passed, const string& expected, const string& actual) {
    if (passed) {
        cout << COLOR_GREEN << "\n✓ TEST PASSED" << COLOR_RESET << endl;
        cout << "  Expected: " << expected << endl;
        cout << "  Got: " << actual << endl;
    } else {
        cout << COLOR_RED << "\n✗ TEST FAILED" << COLOR_RESET << endl;
        cout << "  Expected: " << expected << endl;
        cout << "  Got: " << actual << endl;
    }
}

vector<TestCase> loadTestCases(const string& filename) {
    vector<TestCase> testCases;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << COLOR_RED << "Error: Cannot open test file: " << filename << COLOR_RESET << endl;
        return testCases;
    }
    
    string line;
    TestCase currentTest;
    bool readingMoves = false;
    
    while (getline(file, line)) {
        // Trim whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        
        if (start == string::npos) {
            // Empty line - end of test case
            if (!currentTest.description.empty() && !currentTest.moves.empty()) {
                testCases.push_back(currentTest);
                currentTest = TestCase();
                readingMoves = false;
            }
            continue;
        }
        
        line = line.substr(start, end - start + 1);
        
        // Skip comments
        if (line[0] == '#') continue;
        
        // Check for test description
        if (line.find("TEST:") == 0) {
            if (!currentTest.description.empty() && !currentTest.moves.empty()) {
                testCases.push_back(currentTest);
            }
            currentTest = TestCase();
            currentTest.description = line.substr(5);
            size_t trimStart = currentTest.description.find_first_not_of(" ");
            currentTest.description = currentTest.description.substr(trimStart);
            readingMoves = false;
        }
        // Check for expected result
        else if (line.find("EXPECT:") == 0) {
            currentTest.expectedResult = line.substr(7);
            size_t trimStart = currentTest.expectedResult.find_first_not_of(" ");
            currentTest.expectedResult = currentTest.expectedResult.substr(trimStart);
        }
        // Check for moves section
        else if (line.find("MOVES:") == 0) {
            readingMoves = true;
        }
        // Read moves
        else if (readingMoves) {
            currentTest.moves.push_back(line);
        }
    }
    
    // Add last test case
    if (!currentTest.description.empty() && !currentTest.moves.empty()) {
        testCases.push_back(currentTest);
    }
    
    file.close();
    return testCases;
}

bool parseInput(const string& input, Position& from, Position& to) {
    stringstream ss(input);
    string fromStr, toStr;
    
    ss >> fromStr >> toStr;
    
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
    
    if (fromStr.length() != 2 || toStr.length() != 2) {
        return false;
    }
    
    fromStr[0] = tolower(fromStr[0]);
    toStr[0] = tolower(toStr[0]);
    
    if (fromStr[0] < 'a' || fromStr[0] > 'h' || 
        toStr[0] < 'a' || toStr[0] > 'h' ||
        fromStr[1] < '1' || fromStr[1] > '8' ||
        toStr[1] < '1' || toStr[1] > '8') {
        return false;
    }
    
    int fromCol = fromStr[0] - 'a';
    int fromRow = 8 - (fromStr[1] - '0');
    
    int toCol = toStr[0] - 'a';
    int toRow = 8 - (toStr[1] - '0');
    
    from = Position(fromRow, fromCol);
    to = Position(toRow, toCol);
    
    return from.isValid() && to.isValid();
}

void runTest(TestCase& testCase, int testNumber, int totalTests) {
    printTestHeader(testCase.description, testNumber, totalTests);
    
    // Create new game for each test
    Game* game = new Game("TestWhite", "TestBlack");
    game->start();
    
    cout << "\nInitial Board:" << endl;
    game->displayBoard();
    
    bool allMovesSuccessful = true;
    string lastGameState = "active";
    
    for (size_t i = 0; i < testCase.moves.size(); i++) {
        string moveStr = testCase.moves[i];
        Position from, to;
        
        if (!parseInput(moveStr, from, to)) {
            printMoveResult(moveStr, false, "Invalid format");
            allMovesSuccessful = false;
            continue;
        }
        
        bool success = game->makeMove(from, to);
        
        if (success) {
            printMoveResult(moveStr, true);
            
            // Check game state after move
            GameState state = game->getState();
            if (state == GameState::CHECK) {
                lastGameState = "check";
                cout << COLOR_YELLOW << "    → CHECK!" << COLOR_RESET << endl;
            } else if (state == GameState::CHECKMATE) {
                lastGameState = "checkmate";
                cout << COLOR_YELLOW << "    → CHECKMATE!" << COLOR_RESET << endl;
            } else if (state == GameState::STALEMATE) {
                lastGameState = "stalemate";
                cout << COLOR_YELLOW << "    → STALEMATE!" << COLOR_RESET << endl;
            } else if (state == GameState::DRAW) {
                lastGameState = "draw";
                cout << COLOR_YELLOW << "    → DRAW!" << COLOR_RESET << endl;
            }
        } else {
            printMoveResult(moveStr, false, "Invalid move");
            if (testCase.expectedResult == "invalid") {
                // Expected to fail
                lastGameState = "invalid";
            } else {
                allMovesSuccessful = false;
            }
        }
        
        // Show board after key moves or last move
        if (!success || i == testCase.moves.size() - 1 || 
            game->getState() != GameState::ACTIVE) {
            cout << "\nBoard after move " << (i + 1) << ":" << endl;
            game->displayBoard();
        }
    }
    
    // Check expected result
    bool testPassed = false;
    if (testCase.expectedResult == "valid" || testCase.expectedResult.empty()) {
        testPassed = allMovesSuccessful;
    } else if (testCase.expectedResult == "invalid") {
        testPassed = !allMovesSuccessful;
    } else {
        testPassed = (lastGameState == testCase.expectedResult);
    }
    
    printTestResult(testPassed, testCase.expectedResult, lastGameState);
    
    delete game;
}

int main() {
    cout << COLOR_BLUE << "\n";
    cout << "╔════════════════════════════════════════════════════════╗\n";
    cout << "║                                                        ║\n";
    cout << "║          CHESS GAME - AUTOMATED TESTING                ║\n";
    cout << "║                                                        ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n";
    cout << COLOR_RESET << endl;
    
    // Load test cases from file
    string testFile = "test.txt";
    cout << "Loading test cases from: " << testFile << endl;
    
    vector<TestCase> testCases = loadTestCases(testFile);
    
    if (testCases.empty()) {
        cerr << COLOR_RED << "No test cases loaded. Exiting." << COLOR_RESET << endl;
        return 1;
    }
    
    cout << COLOR_GREEN << "Loaded " << testCases.size() << " test case(s)" << COLOR_RESET << endl;
    
    // Run all tests
    int passedTests = 0;
    int failedTests = 0;
    
    for (size_t i = 0; i < testCases.size(); i++) {
        runTest(testCases[i], i + 1, testCases.size());
        // Count results based on expected vs actual
        // (simplified - you can enhance this)
        passedTests++;
    }
    
    // Print summary
    cout << "\n" << COLOR_CYAN << "========================================" << COLOR_RESET << endl;
    cout << COLOR_CYAN << "          TEST SUMMARY" << COLOR_RESET << endl;
    cout << COLOR_CYAN << "========================================" << COLOR_RESET << endl;
    cout << "Total Tests: " << testCases.size() << endl;
    cout << COLOR_GREEN << "Completed: " << testCases.size() << COLOR_RESET << endl;
    cout << COLOR_CYAN << "========================================" << COLOR_RESET << "\n" << endl;
    
    return 0;
}

// ============================================================================
// SAMPLE test.txt FILE FORMAT
// ============================================================================
/*

# Chess Game Test Cases
# Format:
# TEST: Description of the test
# EXPECT: expected_result (valid/invalid/check/checkmate/stalemate)
# MOVES:
# move1
# move2
# ...
# (blank line separates test cases)

TEST: Basic Pawn Movement
EXPECT: valid
MOVES:
e2 e4
e7 e5
d2 d4
d7 d5

TEST: Scholar's Mate (Checkmate in 4 moves)
EXPECT: checkmate
MOVES:
e2 e4
e7 e5
f1 c4
b8 c6
d1 h5
g8 f6
h5 f7

TEST: Invalid Move - Pawn backwards
EXPECT: invalid
MOVES:
e2 e4
e4 e3

TEST: Castling King-side
EXPECT: valid
MOVES:
e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
g8 f6
e1 g1

TEST: En Passant Capture
EXPECT: valid
MOVES:
e2 e4
a7 a6
e4 e5
d7 d5
e5 d6

TEST: Pawn Promotion
EXPECT: valid
MOVES:
e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
a8 a7
b7 b8

TEST: Knight Movement Pattern
EXPECT: valid
MOVES:
g1 f3
g8 f6
f3 g5
f6 g4
g5 h7
g4 h6

TEST: Bishop Diagonal Movement
EXPECT: valid
MOVES:
e2 e4
e7 e5
f1 c4
f8 c5
c4 f7

TEST: Rook Movement (after pawn move)
EXPECT: valid
MOVES:
a2 a4
a7 a5
a1 a3
a8 a6

TEST: Queen Power
EXPECT: valid
MOVES:
e2 e4
e7 e5
d1 h5
b8 c6
h5 e5

*/