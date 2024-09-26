# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -O3 -march=native -msse4.1 -I./libs -I./libs/minilibx-linux -I./libs/math
LDFLAGS = -L./libs/minilibx-linux -lmlx -lX11 -lXext -lm -lz -lpthread

# Directories
SRC_DIR = src
OBJ_DIR = objs
LIB_DIR = libs
MLX_DIR = $(LIB_DIR)/minilibx-linux
MATH_DIR = $(LIB_DIR)/math

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable name
NAME = rt

# MiniLibX library
MLX = $(MLX_DIR)/libmlx.a

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(MLX) $(NAME)

# Compile MiniLibX
$(MLX):
	@echo "Compiling MiniLibX..."
	@$(MAKE) -C $(MLX_DIR)

# Linking
$(NAME): $(OBJS) $(MLX)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@ -L$(MLX_DIR) -lmlx -lX11 -lXext -lm -lz -lpthread

# Compiling source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
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