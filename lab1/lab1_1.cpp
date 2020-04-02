#include <iostream>
#include <iomanip>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std::chrono;

double f(double x) {
	return 6 / (sqrt(x * (2 - x)));
}

void compute(double a, double b, int n) {
	auto t1 = high_resolution_clock::now();
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;
	
	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}
	auto t2 = high_resolution_clock::now();
	std::cout << std::setprecision(25) << h * s << std::endl;
	duration<double> duration = (t2 - t1);
	std::cout << "Duration is: " << std::setprecision(5) << duration.count() << " seconds" << std::endl;
}

int main() {
	for (int i = 100; i < 10000001; i *= 10) {
		compute(0.5, 1, i);
	}
	
	return EXIT_SUCCESS;
}