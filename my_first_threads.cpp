// Kevin Tang
// 8/1/2019

// Learning how multithreading works and behaves
// Compile using g++ -std=c++11 -pthread

#include <iostream>
#include <thread>
using namespace std;

void eggs(int x) {
	for (int i = 0; i < x; ++i) {
		printf("I like eggs #%d\n", i);
	}
}

void ham(int x) {
	for (int i = 0; i < x; ++i) {
		printf("I like ham #%d\n", i);
	}
}

class bacon {
public:
	void operator()(int x) {
		for (int i = 0; i < x; ++i) {
			printf("I like bacon #%d\n", i);
		}
	}
}

int main() {

	// Threads are created with a function pointer and its arguments
	// Threads are launched after calling
	thread t1(eggs, 3);
	cout << "Thread 1 started" << endl;

	thread t2(ham, 3);
	cout << "Thread 2 started" << endl;

	thread t3(bacon(), 3);
	cout << "Thread 3 started" << endl;

	// To make program wait until a thread to finish, use thread::join()
	t1.join();
	t2.join();
	t3.join();
	
	return 0;
	
}	

