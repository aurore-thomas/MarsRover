CFLAGS := -Wall -Wextra -Wpedantic -std=c++20

# Modules present in the repo
MODULES := communication mission_control planet rover

# Build include directories from modules and a single INCLUDES variable
INCLUDE_DIRS := $(patsubst %,%/include,$(MODULES))
INCLUDES := $(patsubst %,-I%,$(INCLUDE_DIRS))


MKDIR_CMD = mkdir -p "$(1)"

# Where to put compiled executables (binaries and tests)
BIN_DIR := bin

# Communication implementation files used by multiple targets
COMM_SRCS := communication/src/packet_serializer.cpp communication/src/unix_socket.cpp

# Planet implementation (needed by rover and mission binaries)
PLANET_SRCS := planet/src/planet.cpp

rover:
	@$(call MKDIR_CMD,$(BIN_DIR))
	g++ $(INCLUDES) -o $(BIN_DIR)/rover rover/src/rover.cpp rover/src/rover_main.cpp $(COMM_SRCS) $(PLANET_SRCS) $(CFLAGS)

mission:
	@$(call MKDIR_CMD,$(BIN_DIR))
	g++ $(INCLUDES) -o $(BIN_DIR)/mission_control mission_control/src/mission_control.cpp mission_control/src/mission_control_main.cpp $(COMM_SRCS) $(PLANET_SRCS) $(CFLAGS)

all : rover mission

.PHONY: clean all rover mission

# Test sources and binaries
TEST_MS := mission_control/tests/mission_control_test.cpp mission_control/src/mission_control.cpp planet/src/planet.cpp communication/src/packet_serializer.cpp communication/src/unix_socket.cpp
TEST_PLANET := planet/tests/planet_test.cpp planet/src/planet.cpp
TEST_ROVER := rover/tests/rover_test.cpp rover/src/rover.cpp planet/src/planet.cpp communication/src/packet_serializer.cpp communication/src/unix_socket.cpp

all_tests:
	@$(call MKDIR_CMD,$(BIN_DIR))
	$(MAKE) mission_control_tests
	$(MAKE) planet_tests
	$(MAKE) rover_tests

mission_control_tests:
	@$(call MKDIR_CMD,$(BIN_DIR))
	g++ $(INCLUDES) $(CFLAGS) \
		$(TEST_MS) \
		-lgtest -lgtest_main -pthread \
		-o $(BIN_DIR)/mission_control_tests
	./$(BIN_DIR)/mission_control_tests

planet_tests:
	@$(call MKDIR_CMD,$(BIN_DIR))
	g++ $(INCLUDES) $(CFLAGS) \
		$(TEST_PLANET) \
		-lgtest -lgtest_main -pthread \
		-o $(BIN_DIR)/planet_tests
	./$(BIN_DIR)/planet_tests

rover_tests:
	@$(call MKDIR_CMD,$(BIN_DIR))
	g++ $(INCLUDES) $(CFLAGS) \
		$(TEST_ROVER) \
		-lgtest -lgtest_main -pthread \
		-o $(BIN_DIR)/rover_tests
	./$(BIN_DIR)/rover_tests


clean :
	-@rm -f $(BIN_DIR)/mission_control $(BIN_DIR)/rover $(BIN_DIR)/mission_control_tests $(BIN_DIR)/planet_tests $(BIN_DIR)/rover_tests
	-@rmdir --ignore-fail-on-non-empty $(BIN_DIR) || true
