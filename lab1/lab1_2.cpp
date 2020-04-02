//Интервал разбиения: 100, 1000, 10000, 100000, 1000000
#include <iostream>
#include <iomanip>
#include <locale>
#include <chrono>
#include <thread>
#include <mutex>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std::chrono;

double tmr = 0;
double value;
std::mutex gmutex;
double f(double x) {
	return 6 / (sqrt(x * (2 - x)));
}

void compute(int n) {
	double a = 0.5, b = 1;
	auto t1 = high_resolution_clock::now();
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;

	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}

	auto t2 = high_resolution_clock::now();
	std::cout << std::setprecision(25) << h * s - M_PI << std::endl;
	duration<double> duration = (t2 - t1);
	std::cout << "Duration is: " << std::setprecision(5) << duration.count() << " seconds" << std::endl;
}

void compute_qpar(int n) {
	double a = 0.5, b = 1;
	auto t1 = high_resolution_clock::now();
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;

	#pragma loop(hint_parallel(4))
	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}

	auto t2 = high_resolution_clock::now();
	std::cout << std::setprecision(25) << h * s - M_PI << std::endl;
	duration<double> duration = (t2 - t1);
	std::cout << "Duration is: " << std::setprecision(5) << duration.count() << " seconds" << std::endl;
}

void compute_novector(int n) {
	double a = 0.5, b = 1;
	auto t1 = high_resolution_clock::now();
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;

	#pragma loop(no_vector)
	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}

	auto t2 = high_resolution_clock::now();
	std::cout << std::setprecision(25) << h * s - M_PI << std::endl;
	duration<double> duration = (t2 - t1);
	std::cout << "Duration is: " << std::setprecision(5) << duration.count() << " seconds" << std::endl;
}

void compute4threads(double a, double b, int n) {
	auto t1 = high_resolution_clock::now();
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;
	
	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}

	auto t2 = high_resolution_clock::now();
	gmutex.lock();
	value += (h * s);
	duration<double> duration = (t2 - t1);
	tmr += duration.count();
	gmutex.unlock();
}

int main() {
	setlocale(LC_ALL, "Rus");

	std::cout << "\t=== WinAPI Parallelization ===" << std::endl;
	for (int i = 100; i < 10000001; i *= 10) {
		std::cout << "No threads:" << std::endl;
		compute(i);
		std::cout << "Qpar threads:" << std::endl;
		compute_qpar(i);
		std::cout << "No vector:" << std::endl;
		compute_novector(i);
		std::cout << "============================================================" << std::endl;
	}

	std::cout << "\t=== Normal Parallelization ===" << std::endl;
	for (int i = 100; i < 10000001; i *= 10) {
		value = 0;
		tmr = 0;
		std::cout << "[" << i << "]\t";
		std::cout << "Без потоков: ";
		compute(i);
		std::cout << "С потоками: ";
		std::thread t1 (compute4threads, 0.5, 0.6, i);
		std::thread t2(compute4threads, 0.6, 0.7, i);
		std::thread t3(compute4threads, 0.7, 0.8, i);
		std::thread t4(compute4threads, 0.8, 0.9, i);
		std::thread t5(compute4threads, 0.9, 1, i);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		std::cout << value - M_PI << std::endl;
		std::cout << "Duration is:" << tmr << " seconds" << std::endl;
	}

	return EXIT_SUCCESS;
}
