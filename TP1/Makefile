CXX = g++ # Compiler
CXXFLAGS = -Wall # Flags

# Directories
DATA_DIR = ./data
INCLUDE_DIR = ./include
OBJ_DIR = ./obj
SRC_DIR = ./src
##

SRCS = $(wildcard $(SRC_DIR)/*.cpp) # Searches for all the .cpp files in (SRC_DIR) and lists them.
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS)) # Takes the text from (SRCS) and substitues the .cpp pattern with the .o pattern.
INCLUDES = $(wildcard $(INCLUDE_DIR)/*.h) # Searches for all the .h files in (INCLUDE_DIR) and lists them.

TARGET = tp1 # Executable name.

# Rules
all : main

main : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CXX) $(FLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)
##