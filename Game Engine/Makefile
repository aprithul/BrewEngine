INC_DIR := ./include
SRC_DIR := ./src
OBJ_DIR := ./Build
VENDOR := ./Vendor/Build

CC       := g++
CPPFLAGS := -I$(INC_DIR) -I./Vendor/include -std=c++11 -O0
ifeq ($(OS),Windows_NT)
LDLIBS   := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lglew32 -lOpenGL32
else
LDLIBS   := -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -framework OpenGL
endif

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SOURCES)) $(wildcard $(VENDOR)/*.o)

.PHONY: clean

PrEngine: $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

clean: ; rm -f PrEngine $(OBJ_DIR)/*.o

run: PrEngine
	./PrEngine

#em++ -std=c++11 Sources/*.cpp -O2 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 --preload-file res -o index.html
