#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <ppl.h>

#pragma comment(lib, "winmm")

int CountPrimesSerial(int first, int last);
int CountPrimesParallel(int first, int last);
int Measure(std::function<int()> func, int& result);

int _tmain(int argc, _TCHAR* argv[]) {
	int result;
	int time = Measure([]() {
		return CountPrimesSerial(3, 10000000);
		}, result);
	std::cout << "Serial elapsed: " << time << " msec. result: " << result << std::endl;

	time = Measure([]() {
		return CountPrimesParallel(3, 10000000);
		}, result);
	std::cout << "Parallel elapsed: " << time << " msec. result: " << result << std::endl;

	return 0;
}

int Measure(std::function<int()> func, int& result) {
	auto start = ::timeGetTime();
	result = func();
	return ::timeGetTime() - start;
}

int CountPrimesSerial(int first, int last) {
	int count = 0;
	for (int i = first; i <= last; i++) {
		int limit = (int)::sqrt(i);
		bool prime = true;
		for (int j = 2; j <= limit; j++)
			if (i % j == 0) {
				prime = false;
				break;
			}
		if (prime)
			++count;
	}
	return count;
}

int CountPrimesParallel(int first, int last) {
	int count = 0;
	concurrency::parallel_for(first, last + 1, [&count](int i) {
		int limit = (int)::sqrt(i);
		bool prime = true;
		for (int j = 2; j <= limit; j++)
			if (i % j == 0) {
				prime = false;
				break;
			}
		if (prime)
			::InterlockedIncrement((unsigned*)&count);
		});
	return count;
}
