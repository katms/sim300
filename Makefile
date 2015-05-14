CXX=g++
FLAGS = -Wall -std=c++11 -c

build: main.o BinaryHeap.o HashTable.o Soldier.o Thermopylae.o
	$(CXX) -Wall -std=c++11 $^ -o sim300

main.o: main.cpp Thermopylae.o
	$(CXX) $(FLAGS) $^ -o main.o
	
BinaryHeap.o: BinaryHeap.cpp HashTable.o
	$(CXX) $(FLAGS) $^ -o BinaryHeap.o

HashTable.o: HashTable.cpp
	$(CXX) $(FLAGS) HashTable.cpp -o HashTable.o
	
Soldier.o: Soldier.cpp
	$(CXX) $(FLAGS) Soldier.cpp -o Soldier.o

Thermopylae.o: BinaryHeap.o HashTable.o Soldier.o Thermopylae.cpp
	$(CXX) $(FLAGS) $^ -o Thermopylae.o