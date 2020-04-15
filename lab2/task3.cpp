#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <vector>

#define START_SIZE 10000

using namespace std::chrono;

size_t mass_size = START_SIZE;
int *mass = new int[mass_size]; 

/*
 * Функция ParallelSort() сортирует массив в порядке возрастания
 * begin - указатель на первый элемент исходного массива
 * end - указатель на последний элемент исходного массива
*/
void ParallelSort(int *begin, int *end) {
	if (begin != end) {
		--end;
		int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *middle); 
		cilk_spawn ParallelSort(begin, middle);
		ParallelSort(++middle, ++end);
		cilk_sync;
	}
}

/* Функция resize(sz) изменяет размер глобального массива mass
 * на заданный размер sz;
 * @param sz - новый размер массива
 * @return Возвращает новый размер массива
*/
size_t resize(size_t sz) {
	size_t newSize = START_SIZE * sz;
	int *newArray = new int[newSize];

	for(size_t i = 0; i < newSize; i++) 
		newArray[i] = (rand() % 25000) + 1;

	delete[] mass;
	mass = newArray;
	return newSize;
}

int main() {
	srand((unsigned)time(0));

	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");

	int *mass_begin, *mass_end;
	int resz[3] = {10, 50, 100};

	for(size_t i = 0; i < 3; i++) {
		mass_size = resize(resz[i]);
		std::cout << "Size: " << mass_size << std::endl;
		mass_begin = mass;
		mass_end = mass_begin + mass_size-1;
		auto t1 = system_clock::now();
		ParallelSort(mass_begin, mass_end);
		auto t2 = system_clock::now();
		std::cout << "Duration is: " << std::setprecision(10) << duration<double> (t2 - t1).count() << " seconds" << std::endl;
	}

	delete[] mass;
	return 0;
}