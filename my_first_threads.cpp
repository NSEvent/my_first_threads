// Kevin Tang
// 8/1/2019

// Learning how multithreading works and behaves
// Compile using g++ -std=c++11 -pthread

#include <iostream>
#include <thread>
#include <assert.h>

void eggs(int x) {
	printf("Eggs function started\n");
	for (int i = 0; i < x; ++i) {
		printf("Cooking eggs #%d\n", i);
	}
	printf("Finished cooking eggs, about to exit function\n");
}

void ham(int x) {
	printf("Ham function started\n");
	for (int i = 0; i < x; ++i) {
		printf("Cooking ham #%d\n", i);
	}
	printf("Finished cooking ham, about to exit function\n");
}

class bacon {
public:
	void operator()(int x) {
		printf("Bacon functor started\n");
		for (int i = 0; i < x; ++i) {
			printf("Cooking bacon #%d\n", i);
		}
	printf("Finished cooking bacon, about to exit functor\n");
	}
};

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("usage: ./my_first_threads <number>\n");
		return 1;
	}
	int num = atoi(argv[1]);

	printf("Started main function, starting threads\n");

	// Thread instances are created with a function pointer and its arguments
	// Thread starts running immediately after instantiation
	// If thread is constructed with a callable, it is joinable;
	// must be joined or detached before going out of scope
	// Default constructed threads are not joinable
	std::thread t1(eggs, num);

	std::thread t2(ham, num);

	// Threads can be created with a functor
	std::thread t3(bacon(), num);

	// To make program wait until a thread to finish, use std::thread::join()
	// thread will throw an exception if join is not used to ensure the thread has stopped executing
	// Once join is called, thread is no longer joinable
	// To run threads concurrently, construct them first, and then join them
	t1.join();
	t2.join();
	t3.join();

	// Threads can be created with a lambda function
	// Since clean_up comes after the other threads join, it will execute afterwards
	auto clean_up = [](int x) {
		printf("Clean up lambda function started\n");
		for (int i = 0; i < x; ++i) {
			printf("Cleaning up kitchen #%d\n", i);
		}
		printf("Finished cleaning up, about to exit lambda function\n");
	};

	std::thread t4(clean_up, num);

	// Ownership of a thread can be changed with std::move(thread t)
	std::thread t5(std::move(t4)); //t5 takes over thread, t5 is joinable now, t4 is no longer joinable
	assert(t5.joinable());

	t5.join();
	

	// All child threads must be non joinable by the time the parent thread terminates or else error thrown
	printf("About to exit main function\n");
	return 0;
}	

