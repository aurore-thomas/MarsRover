CFLAGS := -Wall -Wextra -Wpedantic  -std=c++20

commun := src/planet.cpp src/packet.cpp src/unix_socket.cpp 
INCLUDE := -I./include
MKDIR_CMD = mkdir -p "$(1)"

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

.PHONY: clean all rover mission

clean :
	-@rm -f mission_control/mission_control rover/rover

