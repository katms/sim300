CXX=g++
FLAGS = -Wall -std=c++11 -c

build: main.o DHeap.o HashTable.o Soldier.o Thermopylae.o
	$(CXX) -Wall -std=c++11 $^ -o sim300

main.o: main.cpp Thermopylae.o
	$(CXX) $(FLAGS) $^ -o main.o
	
DHeap.o: DHeap.cpp HashTable.o
	$(CXX) $(FLAGS) $^ -o DHeap.o

HashTable.o: HashTable.cpp
	$(CXX) $(FLAGS) HashTable.cpp -o HashTable.o
	
Soldier.o: Soldier.cpp
	$(CXX) $(FLAGS) Soldier.cpp -o Soldier.o

Thermopylae.o: DHeap.o HashTable.o Soldier.o Thermopylae.cpp
	$(CXX) $(FLAGS) $^ -o Thermopylae.o