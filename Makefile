CFLAGS := -Wall -Wextra -Wpedantic  -std=c++20

commun := src/planet.cpp src/packet.cpp src/unix_socket.cpp 
INCLUDE := -I./include
MKDIR_CMD = mkdir -p "$(1)"

rover:
	@$(call MKDIR_CMD,rover)
	g++ $(INCLUDE) -o rover/rover src/rover.cpp src/roverMain.cpp $(commun) $(CFLAGS)

mission:
	@$(call MKDIR_CMD,mission_control)
	g++ $(INCLUDE) -o mission_control/mission_control src/mission_control.cpp src/mission_controlMain.cpp $(commun) $(CFLAGS)

all :
	@$(call MKDIR_CMD,rover)
	@$(call MKDIR_CMD,mission_control)
	g++ $(INCLUDE) -o rover/rover src/rover.cpp src/roverMain.cpp $(commun) $(CFLAGS)
	g++ $(INCLUDE) -o mission_control/mission_control src/mission_control.cpp src/mission_controlMain.cpp $(commun) $(CFLAGS)

.PHONY: clean all rover mission


TEST_DIR := tests
TEST_BIN_DIR := tests_bin
TEST_MS := $(TEST_DIR)/mission_control_test.cpp src/mission_control.cpp src/planet.cpp src/unix_socket.cpp src/packet.cpp 
TEST_PLANET := $(TEST_DIR)/planet_test.cpp src/planet.cpp
TEST_ROVER := $(TEST_DIR)/rover_test.cpp src/rover.cpp src/planet.cpp src/unix_socket.cpp src/packet.cpp


all_tests:	
	@$(call MKDIR_CMD,$(TEST_BIN_DIR))
	$(MAKE) mission_control_tests
	$(MAKE) planet_tests
	$(MAKE) rover_tests	

mission_control_tests:
	@$(call MKDIR_CMD,$(TEST_BIN_DIR))
	g++ $(INCLUDE) $(CFLAGS) \
		$(TEST_MS) \
		-lgtest -lgtest_main -pthread \
		-o $(TEST_BIN_DIR)/mission_control_tests
	./$(TEST_BIN_DIR)/mission_control_tests

planet_tests:
	@$(call MKDIR_CMD,$(TEST_BIN_DIR))
	g++ $(INCLUDE) $(CFLAGS) \
		$(TEST_PLANET) \
		-lgtest -lgtest_main -pthread \
		-o $(TEST_BIN_DIR)/planet_tests
	./$(TEST_BIN_DIR)/planet_tests

rover_tests:
	@$(call MKDIR_CMD,$(TEST_BIN_DIR))
	g++ $(INCLUDE) $(CFLAGS) \
		$(TEST_ROVER) \
		-lgtest -lgtest_main -pthread \
		-o $(TEST_BIN_DIR)/rover_tests
	./$(TEST_BIN_DIR)/rover_tests


clean :
	-@rm -f mission_control/mission_control rover/rover tests_bin/mission_control_tests tests_bin/planet_tests tests_bin/rover_tests
	-@rmdir --ignore-fail-on-non-empty mission_control rover tests_bin

