all: my_first_threads.cpp
	g++ -std=c++11 -pthread my_first_threads.cpp -o my_first_threads

clean: 
	rm my_first_threads
