# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -O3 -march=native -msse4.1 -I./libs -I./libs/minilibx-linux -I./libs/math

# Directories
SRC_DIR = src
OBJ_DIR = objs
LIB_DIR = libs
MLX_DIR = $(LIB_DIR)/minilibx-linux
MATH_DIR = $(LIB_DIR)/math

# Source and object files
MAIN_SRC = $(SRC_DIR)/main.cpp
SRCS = $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(MATH_DIR)/*.cpp))
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS += $(SRCS:$(MATH_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MAIN_OBJ = $(OBJ_DIR)/main.o

# Executable name
NAME = rt

# MiniLibX library
MLX = $(MLX_DIR)/libmlx_Linux.a

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(MLX) $(NAME)

# Compile MiniLibX
$(MLX):
	@echo "Compiling MiniLibX..."
	@$(MAKE) -C $(MLX_DIR)

# Linking
$(NAME): $(OBJS) $(MAIN_OBJ)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $^ -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm -lz -o $@

# Compiling main source file
$(MAIN_OBJ): $(MAIN_SRC) | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compiling other source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(MATH_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean object files
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(MLX_DIR) clean

# Clean object files and executable
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)
	@$(MAKE) -C $(MLX_DIR) clean

# Rebuild everything
re: fclean all