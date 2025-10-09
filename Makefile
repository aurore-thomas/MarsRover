CFLAGS := -Wall -Wextra -Wpedantic  -std=c++20


client:
	g++ -o rover src/rover.cpp 
# 	$(CFLAGS)

server:
	g++ -o mission_control src/rover.cpp 
# 	$(CFLAGS)

all : 
	g++ -o rover src/rover.cpp 
	g++ -o mission_control src/rover.cpp 