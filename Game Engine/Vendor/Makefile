INC_DIR := ./include
SRC_DIR := ./src
OBJ_DIR := ./Build

CC       := g++
CPPFLAGS := -I$(INC_DIR) -std=c++11 -g

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SOURCES))

.PHONY: clean

library : $(OBJECTS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

clean: ; rm -f $(OBJ_DIR)/*.o

#em++ -std=c++11 Sources/*.cpp -O2 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 --preload-file res -o index.html