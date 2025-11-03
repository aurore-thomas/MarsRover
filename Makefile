# # ==========================
# # Project Settings
# # ==========================
# CXX := g++
# CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude


# # ==========================
# # Project Structure
# # ==========================
# SRC_DIR := src
# OBJ_DIR := build
# BIN_DIR := .

# # Detect platform
# ifeq ($(OS),Windows_NT)
#     PLATFORM := WINDOWS
#     CXXFLAGS += -D_WIN32
#     LIBS := -lws2_32
#     RM := del /Q
#     MKDIR := mkdir
#     EXE_EXT := .exe

#     # Only include the Windows socket file
#     COMMON_SRC := $(SRC_DIR)/common/win_socket.cpp \
#                   $(SRC_DIR)/common/packet.cpp
# else
#     PLATFORM := UNIX
#     CXXFLAGS += -D_UNIX
#     LIBS :=
#     RM := rm -f
#     MKDIR := mkdir -p
#     EXE_EXT :=

#     # Only include the Unix socket file
#     COMMON_SRC := $(SRC_DIR)/common/unix_socket.cpp \
#                   $(SRC_DIR)/common/packet.cpp
# endif

# # COMMON_SRC := $(wildcard $(SRC_DIR)/common/*.cpp)
# COMMON_OBJ := $(COMMON_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# MISSION_SRC := $(SRC_DIR)/mission_control.cpp $(SRC_DIR)/console.cpp $(SRC_DIR)/planet.cpp
# MISSION_OBJ := $(MISSION_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ROVER_SRC := $(SRC_DIR)/rover.cpp $(SRC_DIR)/console.cpp $(SRC_DIR)/planet.cpp
# ROVER_OBJ := $(ROVER_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# # Final executables
# MISSION_BIN := mission_control/mission_control$(EXE_EXT)
# ROVER_BIN := rover/rover$(EXE_EXT)

# # ==========================
# # Default target
# # ==========================
# all: $(MISSION_BIN) $(ROVER_BIN)

# # ==========================
# # Build rules
# # ==========================
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@$(MKDIR) $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(MISSION_BIN): $(MISSION_OBJ) $(COMMON_OBJ)
# 	@$(MKDIR) $(dir $@)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
# 	@echo "Built: $@"

# $(ROVER_BIN): $(ROVER_OBJ) $(COMMON_OBJ)
# 	@$(MKDIR) $(dir $@)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
# 	@echo "Built: $@"

# # ==========================
# # Cleaning rules
# # ==========================
# .PHONY: all clean rover mission run_mission run_rover

# rover: $(ROVER_BIN)

# mission: $(MISSION_BIN)

# clean:
# ifeq ($(OS),Windows_NT)
#     -@cmd /C rmdir /S /Q $(OBJ_DIR) 2>nul || exit 0
#     -@if exist "$(MISSION_BIN)" del /Q "$(MISSION_BIN)" >nul 2>&1 || exit 0
#     -@if exist "$(ROVER_BIN)" del /Q "$(ROVER_BIN)" >nul 2>&1 || exit 0
# else
#     -@rm -rf $(OBJ_DIR) $(MISSION_BIN) $(ROVER_BIN)
# endif

# run_mission: $(MISSION_BIN)
#     @./$(MISSION_BIN)

# run_rover: $(ROVER_BIN)
#     @./$(ROVER_BIN)



CFLAGS := -Wall -Wextra -Wpedantic  -std=c++20

commun := src/planet.cpp src/common/packet.cpp src/common/unix_socket.cpp src/common/win_socket.cpp
INCLUDE := -I./include

# Platform-safe directory creation: use POSIX 'mkdir -p' on Unix, and a Windows-safe
# 'if not exist "dir" mkdir "dir"' on cmd.exe so creating an existing dir doesn't fail.
ifeq ($(OS),Windows_NT)
	MKDIR_CMD = if not exist "$(1)" mkdir "$(1)"
else
	MKDIR_CMD = mkdir -p "$(1)"
endif

rover:
	@$(call MKDIR_CMD,rover)
	g++ $(INCLUDE) -o rover/rover src/rover.cpp $(commun) $(CFLAGS)

mission:
	@$(call MKDIR_CMD,mission_control)
	g++ $(INCLUDE) -o mission_control/mission_control src/mission_control.cpp src/console.cpp $(commun) $(CFLAGS)

all :
	@$(call MKDIR_CMD,rover)
	@$(call MKDIR_CMD,mission_control)
	g++ $(INCLUDE) -o rover/rover src/rover.cpp $(commun) $(CFLAGS)
	g++ $(INCLUDE) -o mission_control/mission_control src/mission_control.cpp src/console.cpp $(commun) $(CFLAGS)

