// Kevin Tang
// 8/1/2019

// Learning how multithreading works and behaves
// Compile using g++ -std=c++11 -pthread

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <assert.h>

std::mutex mu;

void eggs(int x, int* count) {

	printf("Eggs function started\n");
	for (int i = 0; i < x; ++i) {
		// lock_guard acquires the mutex and allows for safe data access
		// When lock_guard goes out of scope, mutex is released
		// In this example, all three food-making functions are cooking at the same time
		// To ensure food_count is accurate, lock_guard is needed
		// Multiple threads competing for a shared resource (write) may result in undefined behavior
		std::lock_guard<std::mutex> lock(mu);
		printf("Cooking eggs #%d\n", i);
		++(*count);
	}
	printf("Finished cooking eggs, about to exit function\n");
}

void ham(int x, int* count) {
	printf("Ham function started\n");
	for (int i = 0; i < x; ++i) {
		std::lock_guard<std::mutex> lock(mu);
		printf("Cooking ham #%d\n", i);
		++(*count);
	}
	printf("Finished cooking ham, about to exit function\n");
}

class bacon {
public:
	void operator()(int x, int* count) {
		printf("Bacon functor started\n");
		for (int i = 0; i < x; ++i) {
			std::lock_guard<std::mutex> lock(mu);
			printf("Cooking bacon #%d\n", i);
			++(*count);
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

	// use std::atomic for primitive data types if it will be shared between threads
	// mutexes are much slower for primitives
	int num_food = 0;

	printf("Started main function, starting threads\n");

	// Thread instances are created with a function pointer and its arguments
	// Thread starts running immediately after instantiation
	// If thread is constructed with a callable, it is joinable;
	// must be joined or detached before going out of scope
	// Default constructed threads are not joinable
	std::thread t1(eggs, num, &num_food);

	std::thread t2(ham, num, &num_food);

	// Threads can be created with a functor
	std::thread t3(bacon(), num, &num_food);

	// To make program wait until a thread to finish, use std::thread::join()
	// thread will throw an exception if join is not used to ensure the thread has stopped executing
	// Once join is called, thread is no longer joinable
	// To run threads concurrently, construct them first, and then join them
	t1.join();
	t2.join();
	t3.join();

	printf("Cooked %d items of food total\n", num_food);

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

