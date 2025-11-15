#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position {
private:
    int row;  // 0-7, where 0 is top (rank 8) and 7 is bottom (rank 1)
    int col;  // 0-7, where 0 is 'a' and 7 is 'h'

public:
    // Constructors
    Position();
    Position(int r, int c);
    
    // Copy constructor (demonstrates Rule of Three)
    Position(const Position& other);
    
    // Assignment operator
    Position& operator=(const Position& other);
    
    // Destructor
    ~Position();
    
    // Getters (Encapsulation)
    int getRow() const;
    int getCol() const;
    
    // Setters (Encapsulation with Validation)
    void setPosition(int r, int c);
    
    // Validation
    bool isValid() const;
    static bool isValid(int row, int col);
    
    // Comparison operators (Operator Overloading)
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    
    std::string toString() const;  // Returns algebraic notation (e.g., "e4")
    
    // Check if positions are diagonal
    bool isDiagonal(const Position& other) const;
};

#endif // POSITION_H