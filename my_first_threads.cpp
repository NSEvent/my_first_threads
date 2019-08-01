// Kevin Tang
// 8/1/2019

// Learning how multithreading works and behaves
// Compile using g++ -std=c++11 -pthread

#include <iostream>
#include <thread>
using namespace std;

void eggs(int x) {
	printf("Eggs function started\n");
	for (int i = 0; i < x; ++i) {
		printf("Cooking eggs #%d\n", i);
	}
}

void ham(int x) {
	printf("Ham function started\n");
	for (int i = 0; i < x; ++i) {
		printf("Cooking ham #%d\n", i);
	}
}

class bacon {
public:
	void operator()(int x) {
		printf("Bacon functor started\n");
		for (int i = 0; i < x; ++i) {
			printf("Cooking bacon #%d\n", i);
		}
	}
};

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("usage: ./my_first_threads <number>\n");
		return 1;
	}
	int num = atoi(argv[1]);

	// thread instances are created with a function pointer and its arguments
	// thread starts running immediately after instantiation
	// after running, thread is joinable; must be joined or detached before going out of scope
	thread t1(eggs, num);

	thread t2(ham, num);

	// Threads can be created with a functor
	thread t3(bacon(), num);

	// To make program wait until a thread to finish, use thread::join()
	// thread will throw an exception if join is not used to ensure the thread has stopped executing
	t1.join();
	t2.join();
	t3.join();

	// Threads can be created with a lambda function
	// Since clean_up comes after the other threads join, it will execute afterwards
	auto clean_up = [](int x) {
		printf("Clean up lambda function started\n");
		for (int i = 0; i < x; ++i) {
			printf("Washing dishes #%d\n", i);
		}
		printf("Finished cleaning up, about to exit labmda function\n");
	};

	thread t4(clean_up, num);
	t4.detach();
	
	printf("About to exit main function\n");
	return 0;
}	

