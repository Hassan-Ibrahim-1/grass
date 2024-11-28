# Thomas Daley
# September 13, 2021

# A generic build template for C/C++ programs

# executable name
EXE = main

# C compiler
CC = clang
# C++ compiler
CXX = clang++
# linker
LD = clang++

# C++ flags

ASSIMP_INCLUDE = dependencies/assimp/include/
ASSIMP_LIBS = dependencies/assimp/bin/

LIBJPEG = dependencies/libjpeg/
NOISE = dependencies/noise/

GLADSRC = dependencies/glad/src/*.c
GLADPATH =  dependencies/glad/include
STBPATH = dependencies/stb/

GLFW_INCLUDE = dependencies/glfw/include/
GLFW_LIBS = dependencies/glfw/lib

GLM_INCLUDE = dependencies/glm/

# GLLIBS = -L/opt/homebrew/Cellar/glfw/3.4/lib -lglfw -framework Cocoa -framework IOKit -framework CoreFoundation
GLLIBS = -L$(GLFW_LIBS) -lglfw -framework Cocoa -framework IOKit -framework CoreFoundation

# C flags
CFLAGS = -Wall -W -O3 -D_THREAD_SAFE -I$(GLFW_INCLUDE) -I$(GLADPATH)

IMGUI_DIR = dependencies/imgui
# CXXFLAGS = -Wall -W --debug -g -D_THREAD_SAFE -I/opt/homebrew/include -I/opt/homebrew/Cellar/glfw/3.4/include --std=c++17 -I$(GLADPATH) -I$(STBPATH) -I/opt/homebrew/Cellar/glm/1.01/include/glm -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(ASSIMP_INCLUDE) -I$(NOISE)
CXXFLAGS = -Wall -W --debug -g -D_THREAD_SAFE -I$(GLFW_INCLUDE) --std=c++17 -I$(GLADPATH) -I$(STBPATH) -I$(GLM_INCLUDE) -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(ASSIMP_INCLUDE) -I$(NOISE) -Idependencies/libjpeg

# CXXFLAGS = -Wall -W -O4 -D_THREAD_SAFE -I/opt/homebrew/include -I/opt/homebrew/Cellar/glfw/3.4/include --std=c++17 -I$(GLADPATH) -I$(STBPATH) -I/opt/homebrew/Cellar/glm/1.01/include/glm -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(ASSIMP_INCLUDE) -I$(NOISE)
# C/C++ flags
CPPFLAGS = -Wall
# dependency-generation flags
DEPFLAGS = -MMD -MP
# linker flags
LDFLAGS = -lSDL2 -lSDL2_test -L/opt/homebrew/lib $(GLLIBS) -L/opt/homebrew/lib -lassimp -L$(ASSIMP_LIBS) -L$(LIBJPEG) -ljpeg
# LDFLAGS = $(GLLIBS) -lassimp -L$(ASSIMP_LIBS) -L$(LIBJPEG) -ljpeg
# library flags
LDLIBS = 

# build directories
BIN = bin
OBJ = obj
SRC = src

SOURCES := $(wildcard $(SRC)/*.c $(SRC)/*.cc $(SRC)/*.cpp $(SRC)/*.cxx $(SRC)/blocks/*.c $(GLADSRC)) $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# imgui stuff

# CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

OBJECTS := \
	$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c)) \
	$(patsubst $(SRC)/blocks/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/blocks/*.c)) \
	$(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(wildcard $(SRC)/*.cc)) \
	$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(wildcard $(SRC)/*.cpp)) \
	$(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(wildcard $(SRC)/*.cxx)) \
	$(patsubst $(IMGUI_DIR)/%.cpp, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/*.cpp)) \
	$(patsubst $(IMGUI_DIR)/backends/%.cpp, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/backends/*.cpp)) \
	$(patsubst $(IMGUI_DIR)/%.cxx, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/*.cxx)) \
	$(patsubst $(IMGUI_DIR)/backends/%.cxx, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/backends/*.cxx)) \
	$(patsubst $(IMGUI_DIR)/%.c, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/*.c)) \
	$(patsubst $(IMGUI_DIR)/backends/%.cxx, $(OBJ)/%.o, $(wildcard $(IMGUI_DIR)/backends/*.cxx)) \
	$(patsubst dependencies/glad/src/%.c, $(OBJ)/%.o, $(wildcard dependencies/glad/src/*.c))

# include compiler-generated dependency rules
DEPENDS := $(OBJECTS:.o=.d)

# compile C source
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@
# compile C++ source
COMPILE.cxx = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
# link objects
LINK.o = $(LD) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

.DEFAULT_GOAL = all

.PHONY: all
all: $(BIN)/$(EXE)

$(BIN)/$(EXE): $(SRC) $(OBJ) $(BIN) $(OBJECTS)
	$(LINK.o)

$(SRC):
	mkdir -p $(SRC)

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

$(OBJ)/%.o:	$(SRC)/%.c
	$(COMPILE.c) $<

$(OBJ)/%.o: ./dependencies/glad/src/%.c
	$(COMPILE.c) $<

$(OBJ)/%.o:	$(IMGUI_DIR)/%.cpp
	$(COMPILE.cxx) $<
$(OBJ)/%.o:	$(IMGUI_DIR)/backends/%.cpp
	$(COMPILE.cxx) $<

$(OBJ)/%.o:	$(SRC)/%.cc
	$(COMPILE.cxx) $<

$(OBJ)/%.o:	$(SRC)/%.cpp
	$(COMPILE.cxx) $<

$(OBJ)/%.o:	$(SRC)/%.cxx
	$(COMPILE.cxx) $<

# force rebuild
.PHONY: remake
remake:	clean $(BIN)/$(EXE)

# execute the program
.PHONY: run
run: $(BIN)/$(EXE)
	./$(BIN)/$(EXE)

# remove previous build and objects
.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPENDS)
	$(RM) $(BIN)/$(EXE)

# remove everything except source
.PHONY: reset
reset:
	$(RM) -r $(OBJ)
	$(RM) -r $(BIN)

-include $(DEPENDS)
