all: manager

manager: main.cpp src/controller.cpp
	g++ main.cpp src/controller.cpp -o manager
	

