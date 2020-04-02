#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <tbb/tbb.h>

using namespace std::chrono;

double f(double x) {
	return 6 / (sqrt(x * (2 - x)));
}

double compute(double a, double b,int n) {
	double s = (f(a) + f(b)) / 2;
	double h = (b - a) / n;
	for (int i = 1; i <= n - 1; i++) {
		s += f(a + i * h);
	}
	return h * s;
}

int main () {
  double a = 0.5;
	double b = 1;
	std::vector<double> borders = {a, 0.6, 0.7, 0.8, 0.9, b};
	std::cout.setf(std::ios::scientific); 	
	std::cout << "\t Let's start!" << std::endl;

  for (int i = 100; i < 10000001; i *= 10) {
		double res{0.0};
		auto t1 = high_resolution_clock::now();

		tbb::parallel_for_each(Borders.begin(), Borders.end()-1,[&](double n) {
			res += compute(n, n+0.1, i);
		});

		auto t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);

		std::cout << "result = " << res
							<< " time = " << duration.count()
							<< " step = " << i << std::endl << std::endl;
	}

	return EXIT_SUCCESS;
}
