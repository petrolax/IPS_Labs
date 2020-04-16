#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std::chrono;

/*
 * Функция ReducerMaxTest() определяет максимальный элемент массива,
 * переданного ей в качестве аргумента, и его позицию
 * @param mass_pointer - указатель исходный массив целых чисел
 * @param size - количество элементов в массиве
*/
void ReducerMaxTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_max_index<long, int>> maximum;
	cilk_for(long i = 0; i < size; ++i)
	{
		maximum->calc_max(i, mass_pointer[i]);
	}
	printf("Maximal element = %d has index = %d\n",
		maximum->get_reference(), maximum->get_index_reference());
}
/*
 * Task2
 * Функция ReducerMinTest определяет минимальный элемент массива
 * @param mass_pointer - указатель на целочисленный массив
 * @param size - кол-во элементов массива
*/
void ReducerMinTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_min_index<long, int>> minimum;
	cilk_for(long i = 0; i < size; ++i)
	{
		minimum->calc_min(i, mass_pointer[i]);
	}
	printf("Minimal element = %d has index = %d\n",
		minimum->get_reference(), minimum->get_index_reference());
}

/*
 * Функция ParallelSort() сортирует массив в порядке возрастания
 * @param begin - указатель на первый элемент исходного массива
 * @param end - указатель на последний элемент исходного массива
*/
void ParallelSort(int *begin, int *end)
{
	if (begin != end) 
	{
		--end;
		int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *middle); 
		cilk_spawn ParallelSort(begin, middle);
		ParallelSort(++middle, ++end);
		cilk_sync;
	}
}


int main()
{
	srand((unsigned)time(0));

	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");

	long i;
	const long mass_size = 10000;
	int *mass_begin, *mass_end;
	int *mass = new int[mass_size]; 

	for(i = 0; i < mass_size; ++i)
		mass[i] = (rand() % 25000) + 1;
	
	mass_begin = mass;
	mass_end = mass_begin + mass_size;
	
	std::cout << "Size: " << mass_size << endl;
	std::cout << std::endl;

	std::cout << "Before sorting: " << std::endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);

	std::cout << std::endl;

	ParallelSort(mass_begin, mass_end);

	std::cout << "After sorting: " << std::endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);

	delete[]mass;
	return 0;
}
