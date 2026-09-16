# =========================================
# Date: 2026
# CROSS-PLATFORM MAKEFILE (C11 & C23)
# Refactored for Robust OS Detection & CI/CD
# =========================================

export MAKE

# --- 1. OS DETECTION (POSIX & WINDOWS COMPATIBLE) ---
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    OS_NAME := windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
        OS_NAME := linux
    else ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := Darwin
        OS_NAME := macos
    endif
endif

# --- 2. COMPILER SELECTION ---
CC  := gcc
CXX := g++

# --- 3. DIRECTORY CONFIGURATION ---
SRC_DIR  := src
INC_DIR  := inc
OBJ_DIR  := lib/$(OS_NAME)
BIN_DIR  := bin/$(OS_NAME)
APP_NAME := xloc

# --- 4. FLAGS ---
CFLAGS           := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -g
CXXFLAGS         := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c++23 -O3 -g -flto=auto
CXXFLAGS_RELEASE := -I$(INC_DIR) -std=c++23 -O3 -DNDEBUG -flto=auto
COMMON_LDFLAGS   := -lstdc++exp -flto=auto

# --- 5. FILE DETECTION ---
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS   := $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS     := $(C_OBJS) $(CXX_OBJS)

# --- 6. OS-SPECIFIC CONFIGURATION ---
ifeq ($(DETECTED_OS),Windows)
    TARGET_EXT  := .exe
    SHELL       := cmd.exe
    LDFLAGS     := -static -static-libgcc -static-libstdc++ -Wl,-Bstatic -lwinpthread $(COMMON_LDFLAGS)
    .SHELLFLAGS := /D /C
    TARGET      := $(BIN_DIR)/$(APP_NAME)$(TARGET_EXT)

    WIN_OBJ_DIR := $(subst /,\,$(OBJ_DIR))
    WIN_BIN_DIR := $(subst /,\,$(BIN_DIR))
    
    MKDIR_OBJ   := if not exist $(WIN_OBJ_DIR) mkdir $(WIN_OBJ_DIR)
    MKDIR_BIN   := if not exist $(WIN_BIN_DIR) mkdir $(WIN_BIN_DIR)
    CLEAN_OBJ   := if exist $(WIN_OBJ_DIR) rd /s /q $(WIN_OBJ_DIR)
    CLEAN_BIN   := if exist $(WIN_BIN_DIR) rd /s /q $(WIN_BIN_DIR)
    
    SCREEN_CLEAR   := cls
    SLEEP_CMD      := timeout /t 1 /nobreak > NUL
    UP_TO_DATE_MSG := @if "$(WAS_REBUILT)"=="" echo --- [INFO] Project Is Up To Date. Compile Stopped. ---

else ifeq ($(DETECTED_OS),Linux)
    TARGET     := $(BIN_DIR)/$(APP_NAME)
    LDFLAGS    := -static-libgcc -static-libstdc++ -pthread $(COMMON_LDFLAGS)

    MKDIR_OBJ  := mkdir -p $(OBJ_DIR)
    MKDIR_BIN  := mkdir -p $(BIN_DIR)
    CLEAN_OBJ  := rm -rf $(OBJ_DIR)
    CLEAN_BIN  := rm -rf $(BIN_DIR)
    
    SCREEN_CLEAR   := clear
    SLEEP_CMD      := sleep 1
    UP_TO_DATE_MSG := @if [ -z "$(WAS_REBUILT)" ]; then echo "--- [INFO] Project Is Up To Date. Compile Stopped. ---"; fi

else ifeq ($(DETECTED_OS),Darwin)
    TARGET     := $(BIN_DIR)/$(APP_NAME)
    LDFLAGS    := $(COMMON_LDFLAGS)

    MKDIR_OBJ  := mkdir -p $(OBJ_DIR)
    MKDIR_BIN  := mkdir -p $(BIN_DIR)
    CLEAN_OBJ  := rm -rf $(OBJ_DIR)
    CLEAN_BIN  := rm -rf $(BIN_DIR)
    
    SCREEN_CLEAR   := clear
    SLEEP_CMD      := sleep 1
    UP_TO_DATE_MSG := @if [ -z "$(WAS_REBUILT)" ]; then echo "--- [INFO] Project Is Up To Date. Compile Stopped. ---"; fi
endif

# --- 7. COMPILING RULES ---
compile: prepare $(TARGET)
	$(UP_TO_DATE_MSG)

prepare:
	@echo --- [PREPARE] Checking Folders. ---
	@$(MKDIR_OBJ)
	@$(MKDIR_BIN)

$(TARGET): $(OBJS)
	@echo --- [LINK] Linking Program: $@ ---
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo --- [SUCCESS] Compiling Completed ---
	$(eval WAS_REBUILT := 1)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | prepare
	@echo [C++] Compile: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | prepare
	@echo [C]   Compile: $<
	@$(CC) $(CFLAGS) -c $< -o $@

run: compile
	@echo --- [RUN] Executing Program ---
	@$(TARGET) $(ARGS)
	@echo --- [SUCCESS] End Execution ---

# --- Release Target Specific Variables ---
release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: CFLAGS   := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -O3 -DNDEBUG -flto=auto

release: clean compile
	@echo --- [RELEASE] Stripping Debug Symbols ---
	@strip $(TARGET)
	@echo --- [SUCCESS] Release Build Completed: $(TARGET) ---

# --- 8. CLEANING ---
clean:
	@echo --- [CLEAN] Resetting Project ---
	@$(CLEAN_OBJ)
	@$(CLEAN_BIN)
	@echo --- [SUCCESS] Reset Done ---
	@-$(SLEEP_CMD) 
	@-$(SCREEN_CLEAR)

.PHONY: compile prepare run release clean