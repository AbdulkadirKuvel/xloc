# =========================================
# Date: 23.02.2026
# CROSS-PLATFORM MAKEFILE (C11 & C23)
# Written by: Gemini 3.1 Pro & Abdulkadir 
# =========================================

# --- 1. COMPILER SELECTION ---
CC := gcc
CXX := g++

# --- 2. DIRECTORY CONFIGURATION ---
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := lib
BIN_DIR := bin
APP_NAME := xloc

# --- 3. FLAGS ---
CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -g
CXXFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c++23 -O3 -g
CXXFLAGS_RELEASE := -I$(INC_DIR) -std=c++23 -O3 -DNDEBUG -flto
LDFLAGS := -lstdc++exp

# --- 4. FILE DETECTION ---
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS := $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS := $(C_OBJS) $(CXX_OBJS)

# --- 5. OS CONTROL AND COMMANDS ---
ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    # /D: Disables auto run commands (isolation).
    # /C: runs the commands and closes the shell.
    .SHELLFLAGS := /D /C
    TARGET := $(BIN_DIR)/$(APP_NAME).exe
    
    # Instead of fragile '&' operator, the commands are parted.
    MKDIR_OBJ := if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    MKDIR_BIN := if not exist $(BIN_DIR) mkdir $(BIN_DIR)
    CLEAN_OBJ := if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)
    CLEAN_BIN := if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
    
    SCREEN_CLEAR := cls
    SLEEP_CMD := timeout /t 1 /nobreak > NUL
else
    TARGET := $(BIN_DIR)/$(APP_NAME)
    
    MKDIR_OBJ := mkdir -p $(OBJ_DIR)
    MKDIR_BIN := mkdir -p $(BIN_DIR)
    CLEAN_OBJ := rm -rf $(OBJ_DIR)
    CLEAN_BIN := rm -rf $(BIN_DIR)
    
    SCREEN_CLEAR := clear
    SLEEP_CMD := sleep 1
endif

# --- 6. COMPILING RULES ---
compile: prepare $(TARGET)
	@if "$(WAS_REBUILT)"=="" @echo --- [INFO] Project Is Up To Date. Compile Stopped. ---

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
release: CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -O3 -DNDEBUG -flto

release: clean compile
	@echo --- [RELEASE] Stripping Debug Symbols ---
	@strip $(TARGET)
	@echo --- [SUCCESS] Release Build Completed: $(TARGET) ---

# --- 7. CLEANING ---
clean:
	@echo --- [CLEAN] Resetting Project ---
	@$(CLEAN_OBJ)
	@$(CLEAN_BIN)
	@echo --- [SUCCESS] Reset Done ---
	@$(SLEEP_CMD) 
	@$(SCREEN_CLEAR)

.PHONY: compile prepare run release clean