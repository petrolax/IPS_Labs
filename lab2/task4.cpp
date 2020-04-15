#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std::chrono;

/*
 * Функция fillVector заполняет вектор из стандартной библиотеки,
 * размером sz, рандомными значениями
 * @param sz - размер вектора
*/
void fillVector(size_t sz) {
  std::vector<int> arr;
   
  for(size_t i = 0; i < sz; i++) {
    arr.push_back(std::rand() % 20000 + 1);
  }
}

/*
 * Функция fillIntelMagic заполняет магический вектор из библиотеки Intel,
 * размером sz, рандомными значениями
 * @param sz - размер вектора
*/
void fillIntelMagic(size_t sz) {
  cilk::reducer<cilk::op_vector<int>>red_vec; 

  cilk_for(size_t i = 0; i < sz; i++) {
    red_vec->push_back(std::rand() % 20000 + 1);
  }
}

/*
 * Функция CompareForAndCilk_For выводит размер векторов
 * и время выполнения функций fillVector и fillIntelMagic
 * @param sz - размер векторов, который передаётся в функции fillVector и fillIntelMagic
*/
void CompareForAndCilk_For(size_t sz) {
  std::cout << "Size: " << sz << std::endl;
  auto t1 = system_clock::now();
  fillVector(sz);
  auto t2 = system_clock::now();
  std::cout << "Time to fill Standard vector: " <<
               std::setprecision(10) << duration<double> (t2 - t1).count() <<
               " seconds" << std::endl;t1 = system_clock::now();

  fillIntelMagic(sz);
  t2 = system_clock::now();
  std::cout << "Time to fill Intel Magic vector: " <<
               std::setprecision(10) << duration<double> (t2 - t1).count() <<
               " seconds" << std::endl;
  
  std::cout << std::endl;
}


int main() {
	std::srand((unsigned)std::time(NULL));

	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");

  size_t resz[8] = {1000000, 100000, 10000, 1000, 500, 100, 50, 10};

  for(size_t i = 0; i < 8; i++)
    CompareForAndCilk_For(resz[i]);
  
	return 0;
}