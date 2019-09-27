#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <mutex>
using namespace std;

int counter = 0; // Global var
mutex mtx; // Mutex object

void increment(int iterations) {
	for (int i = 0; i < iterations; ++i) {
		unique_lock<mutex> lock(mtx); // Lock the mutex
		counter++;
	}
}
int main() {
	std::thread t1(increment, 100000); // Execute 'increment' in a thread
	std::thread t2(increment, 100000); // Execute 'increment' in a thread
	t1.join(); // Wait for t1
	t2.join(); // Wait for t2
	std::cout << "Counter = " << counter << std::endl;

	system("pause");
	return 0;
}

