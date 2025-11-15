# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Target executable
TARGET = $(BIN_DIR)/chess

# Source files (organized by component)
UTILS_SRC = $(SRC_DIR)/utils/Position.cpp

PIECES_SRC = $(SRC_DIR)/pieces/Piece.cpp \
             $(SRC_DIR)/pieces/Pawn.cpp \
             $(SRC_DIR)/pieces/Rook.cpp \
             $(SRC_DIR)/pieces/Knight.cpp \
             $(SRC_DIR)/pieces/Bishop.cpp \
             $(SRC_DIR)/pieces/Queen.cpp \
             $(SRC_DIR)/pieces/King.cpp

BOARD_SRC = $(SRC_DIR)/board/Square.cpp \
            $(SRC_DIR)/board/Board.cpp

GAME_SRC = $(SRC_DIR)/game/Move.cpp \
           $(SRC_DIR)/game/Player.cpp \
           $(SRC_DIR)/game/Game.cpp

MAIN_SRC = $(SRC_DIR)/main.cpp
TEST_SRC = $(SRC_DIR)/test/ChessTests.cpp

# All source files
SOURCES = $(UTILS_SRC) $(PIECES_SRC) $(BOARD_SRC) $(GAME_SRC) $(MAIN_SRC)

# Test source files (all sources except main)
TEST_SOURCES = $(UTILS_SRC) $(PIECES_SRC) $(BOARD_SRC) $(GAME_SRC) $(TEST_SRC)

# Object files (replace src/ with obj/ and .cpp with .o)
OBJECTS = $(UTILS_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(PIECES_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(BOARD_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(GAME_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(MAIN_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test object files
TEST_OBJECTS = $(UTILS_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
               $(PIECES_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
               $(BOARD_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
               $(GAME_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
               $(TEST_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/pieces
	@mkdir -p $(OBJ_DIR)/board
	@mkdir -p $(OBJ_DIR)/game
	@mkdir -p $(OBJ_DIR)/test
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete! Executable: $(TARGET)"

# Compile source files to object files

# Utils
$(OBJ_DIR)/utils/%.o: $(SRC_DIR)/utils/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pieces
$(OBJ_DIR)/pieces/%.o: $(SRC_DIR)/pieces/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Board
$(OBJ_DIR)/board/%.o: $(SRC_DIR)/board/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Game
$(OBJ_DIR)/game/%.o: $(SRC_DIR)/game/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test
$(OBJ_DIR)/test/%.o: $(SRC_DIR)/test/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	@echo "Clean complete!"

# Clean and rebuild
rebuild: clean all

# Run the game
run: all
	@echo "Starting Chess Game..."
	@./$(TARGET)

# Test target
TEST_TARGET = $(BIN_DIR)/chess_test

$(TEST_TARGET): $(TEST_OBJECTS)
	@echo "Linking test: $@"
	$(CXX) $(TEST_OBJECTS) -o $@ $(LDFLAGS)
	@echo "Test build complete! Executable: $(TEST_TARGET)"

test: directories $(TEST_TARGET)
	@echo "Running Chess Game Tests..."
	@./$(TEST_TARGET)

# Debug build (with -g flag)
debug: CXXFLAGS += -g -DDEBUG
debug: clean all
	@echo "Debug build complete!"

# Release build (with optimization)
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all
	@echo "Release build complete!"

# Install (copy to system bin - requires sudo)
install: release
	@echo "Installing chess to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/chess
	@echo "Installation complete! Run with: chess"

# Uninstall
uninstall:
	@echo "Uninstalling chess..."
	sudo rm -f /usr/local/bin/chess
	@echo "Uninstall complete!"

# Help target
help:
	@echo "Chess Game Makefile - Available targets:"
	@echo "  make              - Build the project (default)"
	@echo "  make all          - Build the project"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make run          - Build and run the game"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make release      - Build optimized release version"
	@echo "  make install      - Install to system (requires sudo)"
	@echo "  make uninstall    - Remove from system (requires sudo)"
	@echo "  make help         - Show this help message"

# Print variables (for debugging Makefile)
print-vars:
	@echo "CXX       = $(CXX)"
	@echo "CXXFLAGS  = $(CXXFLAGS)"
	@echo "TARGET    = $(TARGET)"
	@echo "SOURCES   = $(SOURCES)"
	@echo "OBJECTS   = $(OBJECTS)"

# Phony targets (not actual files)
.PHONY: all directories clean rebuild run debug release install uninstall help print-vars
