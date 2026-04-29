SHELL := cmd.exe
.SHELLFLAGS := /C

# ======================================
# 2D Game Engine Makefile
# Supports Windows (MinGW), Linux, and macOS
# ======================================

# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++23 -Wall -Wextra
CFLAGS = -w -Wextra
DEBUGFLAGS = -g -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin
LIBDIR = lib
VENDORDIR = vendor
ASSETSDIR = assets
OBJDIR = $(BINDIR)/obj

# Source files
CPP_SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
C_SOURCES = $(wildcard $(SRCDIR)/SDL2Ext/*.c)
IMGUI_SOURCES = \
    $(VENDORDIR)/imgui/imgui.cpp \
    $(VENDORDIR)/imgui/imgui_demo.cpp \
    $(VENDORDIR)/imgui/imgui_draw.cpp \
    $(VENDORDIR)/imgui/imgui_tables.cpp \
    $(VENDORDIR)/imgui/imgui_widgets.cpp \
    $(VENDORDIR)/imgui/imgui_impl_sdl2.cpp \
	$(VENDORDIR)/imgui/imgui_impl_sdlrenderer2.cpp

PUGIXML_SOURCES = $(VENDORDIR)/pugixml/pugixml.cpp

CPP_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES))
C_OBJECTS = $(C_SOURCES:$(SRCDIR)/SDL2Ext/%.c=$(OBJDIR)/SDL2Ext/%.o)
IMGUI_OBJECTS = $(IMGUI_SOURCES:$(VENDORDIR)/imgui/%.cpp=$(OBJDIR)/imgui/%.o)
PUGIXML_OBJECTS = $(PUGIXML_SOURCES:$(VENDORDIR)/pugixml/%.cpp=$(OBJDIR)/pugixml/%.o)
ALL_OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS) $(IMGUI_OBJECTS) $(PUGIXML_OBJECTS)
TARGET = $(BINDIR)/game

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    SDL2_INCDIR = $(VENDORDIR)/SDL2/include
    SDL2_LIBDIR = $(LIBDIR)
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua53

    # Windows commands
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM = del /Q /F
    RMDIR = rmdir /S /Q
    COPY = copy
    PATH_SEP = \\
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        SDL2_FLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)
        LIBS = $(SDL2_FLAGS) -llua
    endif
    ifeq ($(UNAME_S),Darwin)
        SDL2_FLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)
        LIBS = $(SDL2_FLAGS) -llua
    endif
    MKDIR = mkdir -p $(1)
    RM = rm -f
    RMDIR = rm -rf
    COPY = cp
    PATH_SEP = /
endif

# Include directories
INCLUDES = -I$(INCDIR)
INCLUDES += -I$(VENDORDIR)/glm
INCLUDES += -I$(VENDORDIR)/imgui
INCLUDES += -I$(VENDORDIR)/imgui/backends
INCLUDES += -I$(VENDORDIR)/lua
INCLUDES += -I$(VENDORDIR)/sol2/include
INCLUDES += -I$(VENDORDIR)/pugixml
ifneq ($(SDL2_INCDIR),)
    INCLUDES += -I$(SDL2_INCDIR)
endif

# Library directories
LIBFLAGS =
ifneq ($(SDL2_LIBDIR),)
    LIBFLAGS += -L$(SDL2_LIBDIR)
endif

# Default target
.PHONY: all debug release clean run copy-dlls setup help

all: debug

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(RELEASEFLAGS)
release: CFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

# Link target
$(TARGET): $(ALL_OBJECTS) | $(BINDIR)
	$(CXX) $(ALL_OBJECTS) -o $@ $(LIBFLAGS) $(LIBS)
	@echo Build complete: $(TARGET)

# Compile C++ source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile C source files from SDL2Ext
$(OBJDIR)/SDL2Ext/%.o: $(SRCDIR)/SDL2Ext/%.c
	@$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile ImGui source files
$(OBJDIR)/imgui/%.o: $(VENDORDIR)/imgui/%.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile pugixml source files
$(OBJDIR)/pugixml/%.o: $(VENDORDIR)/pugixml/%.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create directories
$(BINDIR):
	$(call MKDIR,$(BINDIR))
	$(call MKDIR,$(OBJDIR))

# Clean build files
clean:
ifeq ($(OS),Windows_NT)
	@echo Cleaning project...
	@if exist "$(OBJDIR)" rmdir /S /Q "$(OBJDIR)" >nul 2>&1
	@if exist "$(BINDIR)\game.exe" del /Q /F "$(BINDIR)\game.exe" >nul 2>&1
	@if exist "$(BINDIR)\*.o" del /Q /F "$(BINDIR)\*.o" >nul 2>&1
	@if exist "$(BINDIR)\*.d" del /Q /F "$(BINDIR)\*.d" >nul 2>&1
else
	@echo Cleaning project...
	@rm -rf "$(OBJDIR)"
	@rm -f "$(TARGET)"
endif
	@echo Clean complete

# Copy SDL2 DLLs (Windows only)
copy-dlls:
ifeq ($(OS),Windows_NT)
# 	@echo Copying SDL2 DLLs...
	@if exist "$(subst /,\,$(VENDORDIR))\SDL2\bin\*.dll" $(COPY) "$(subst /,\,$(VENDORDIR))\SDL2\bin\*.dll" "$(subst /,\,$(BINDIR))\" >nul
# 	@echo SDL2 DLLs copied to bin directory
endif

# Set up the project (create directories, copy DLLs)
setup: $(BINDIR) copy-dlls
	@echo Project setup complete

# Run the program
run: $(TARGET) copy-dlls
ifeq ($(OS),Windows_NT)
	@cd $(BINDIR) && game.exe
else
	@cd $(BINDIR) && ./game
endif

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build debug version (default)"
	@echo "  debug     - Build debug version with debugging symbols"
	@echo "  release   - Build optimized release version"
	@echo "  clean     - Remove build files"
	@echo "  run       - Build and run the game"
	@echo "  setup     - Set up project directories and copy DLLs"
	@echo "  copy-dlls - Copy SDL2 DLLs to bin (Windows only)"
	@echo "  help      - Show this help message"

# Dependencies (auto-generated)
-include $(ALL_OBJECTS:.o=.d)
