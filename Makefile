CFLAGS := -Wall -Wextra -Wpedantic  -std=c++20

commun := src/planet.cpp src/communication.cpp

client:
	g++ -o rover src/rover.cpp $(commun) $(CFLAGS)

server:
	g++ -o mission_control src/mission_control.cpp src/console.cpp $(commun) $(CFLAGS)

all : 
	g++ -o rover src/rover.cpp $(commun) $(CFLAGS)
	g++ -o mission_control src/mission_control.cpp src/console.cpp $(commun) $(CFLAGS)