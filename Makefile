# ============================================================================
# Chess Game Makefile
# ============================================================================
# This Makefile compiles the Chess Game with proper dependency management
# Place this file in the root directory: ChessGame/Makefile
# ============================================================================

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

# All source files
SOURCES = $(UTILS_SRC) $(PIECES_SRC) $(BOARD_SRC) $(GAME_SRC) $(MAIN_SRC)

# Object files (replace src/ with obj/ and .cpp with .o)
OBJECTS = $(UTILS_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(PIECES_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(BOARD_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(GAME_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
          $(MAIN_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/pieces
	@mkdir -p $(OBJ_DIR)/board
	@mkdir -p $(OBJ_DIR)/game
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

# ============================================================================
# COMPILATION INSTRUCTIONS
# ============================================================================
#
# PLACEMENT:
#   Place this Makefile in the root directory of your project:
#   ChessGame/Makefile
#
# PREREQUISITES:
#   - g++ compiler (or any C++ compiler, modify CXX variable)
#   - make utility
#   - Linux/Unix/MacOS (or Windows with MinGW/Cygwin/WSL)
#
# BASIC USAGE:
#
# 1. Navigate to project directory:
#    $ cd ChessGame
#
# 2. Build the project:
#    $ make
#    or
#    $ make all
#
# 3. Run the game:
#    $ make run
#    or
#    $ ./bin/chess
#
# 4. Clean build files:
#    $ make clean
#
# 5. Rebuild from scratch:
#    $ make rebuild
#
# ADVANCED USAGE:
#
# Build with debug symbols (for debugging with gdb):
#    $ make debug
#    $ gdb ./bin/chess
#
# Build optimized release version:
#    $ make release
#
# Install system-wide (requires sudo):
#    $ make install
#    $ chess  (can now run from anywhere)
#
# Uninstall:
#    $ make uninstall
#
# Get help:
#    $ make help
#
# TROUBLESHOOTING:
#
# If compilation fails:
# 1. Check that all source files exist in correct directories
# 2. Ensure g++ is installed: $ g++ --version
# 3. Ensure make is installed: $ make --version
# 4. Check file permissions: $ ls -la
# 5. Try clean rebuild: $ make rebuild
#
# On Windows (using MinGW):
#    $ mingw32-make
#    $ mingw32-make run
#
# CUSTOMIZATION:
#
# Change compiler:
#    CXX = clang++
#
# Add compiler flags:
#    CXXFLAGS = -std=c++17 -Wall -Wextra -O2
#
# Change executable name:
#    TARGET = $(BIN_DIR)/my_chess_game
#
# ============================================================================