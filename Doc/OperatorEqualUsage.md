# Operator= (Assignment) Usage in Chess Game

This document lists all locations where the overloaded `=` (assignment) operator for the `Position` class is used.

## Definition

The `operator=` is defined in the `Position` class:
- **Header**: `include/utils/Position.h` (line 20)
- **Implementation**: `src/utils/Position.cpp` (lines 16-22)

```cpp
Position& Position::operator=(const Position& other) {
    if (this != &other) {  // Check for self-assignment
        row = other.row;
        col = other.col;
    }
    return *this;
}
```

## Usage Locations

### 1. **Move Class** (`src/game/Move.cpp`)
- **Line 35-36**: Assignment in `Move::operator=` when assigning Move objects
  ```cpp
  Move& Move::operator=(const Move& other) {
      if (this != &other) {
          from = other.from;      // Uses Position::operator=
          to = other.to;          // Uses Position::operator=
          // ... other assignments
      }
      return *this;
  }
  ```

### 2. **Piece Class** (`src/pieces/Piece.cpp`)
- **Line 46**: Assignment in `Piece::setPosition()` method
  ```cpp
  void Piece::setPosition(const Position& pos) {
      position = pos;  // Uses Position::operator=
  }
  ```

### 3. **Main Function** (`src/main.cpp`)
- **Line 230-231**: Assignment when parsing input to Position objects
  ```cpp
  from = Position(fromRow, fromCol);  // Uses Position::operator=
  to = Position(toRow, toCol);        // Uses Position::operator=
  ```

## Summary

The `operator=` is used **5 times** in the codebase:

1. **2 times** in `Move::operator=` (lines 35-36) - assigns `from` and `to` Position members
2. **1 time** in `Piece::setPosition()` (line 46) - updates the piece's position
3. **2 times** in `main.cpp::parseInput()` (lines 230-231) - assigns parsed positions to variables

**Total: 5 direct uses of Position::operator=**

## Notes

- The assignment operator is also used implicitly in copy constructors and when returning Position objects from functions, but those use the copy constructor, not the assignment operator.
- The `Move` class's assignment operator uses Position's assignment operator when assigning Move objects to each other.
- The `Piece::setPosition()` method uses the assignment operator to update a piece's position on the board.

## Related Classes

Other classes that also define `operator=`:
- **Move class** (`include/game/Move.h`, `src/game/Move.cpp`) - Uses Position's `operator=` internally
- **Player class** (`include/game/Player.h`, `src/game/Player.cpp`) - Does not contain Position members

