

#include <iostream>
#include <tchar.h>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

DWORD WINAPI CalcPrimes(PVOID p);

const int NumThreads = 2;


struct PrimeData {
	int First, Last;
	int Count;
};
int _tmain(int argc, _TCHAR* argv[]) {
	int first = 3, last = 20000000;

	PrimeData data[NumThreads];
	HANDLE hThread[NumThreads];

	int delta = (last - first + 1) / NumThreads;

	DWORD start = timeGetTime();
	for (int i = 0; i < NumThreads; i++) {
		data[i].First = first + delta * i;
		data[i].Last = i == NumThreads -1 ? last:  (first + delta * (1 + 1) - 1);

		hThread[i] = CreateThread(nullptr, 0, CalcPrimes, &data[i], 0, nullptr);
	}

	WaitForMultipleObjects(NumThreads, hThread, TRUE, INFINITE);
	DWORD elapsed = timeGetTime() - start;

	int count = 0;
	for (int i = 0; i < NumThreads; i++) {
		std::cout << "Thread " << i << " count: " << data[i].Count << std::endl;
		count += data[i].Count;
	}

	std::cout << "Elapsed: " << elapsed << std::endl << "Total: " << count << std::endl;
	return 0;
}

bool IsPrime(int number) {
	int limit = (int)sqrt((float)number);
	for (int j = 2; j <= limit; j++)
		if (number % j == 0)
			return false;
	return true;
}

DWORD WINAPI CalcPrimes(PVOID p) {
	int count = 0;
	PrimeData* data = (PrimeData*)p;
	for (int i = data->First; i <= data->Last; ++i)
		if (IsPrime(i))
			count++;

	data->Count = count;

	return 0;
}