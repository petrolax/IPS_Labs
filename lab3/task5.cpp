#include <iostream>
#include <ctime>
#include <vector>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <iomanip>
#include <chrono>

using namespace std::chrono;

// количество строк в исходной квадратной матрице
const int MATRIX_SIZE = 4;//1500;

/// Функция InitMatrix() заполняет переданную в качестве 
/// параметра квадратную матрицу случайными значениями
/// matrix - исходная матрица СЛАУ
void InitMatrix( std::vector<std::vector<double>> &matrix ) {
	for ( int i = 0; i < MATRIX_SIZE; ++i ) {
		for ( int j = 0; j <= MATRIX_SIZE; ++j ) {
			matrix[i][j] = rand() % 2500 + 1;
		}
	}
}

/// Функция SerialGaussMethod() решает СЛАУ методом Гаусса 
/// matrix - исходная матрица коэффиициентов уравнений, входящих в СЛАУ,
/// последний столбей матрицы - значения правых частей уравнений
/// rows - количество строк в исходной матрице
/// result - массив ответов СЛАУ
duration<double> SerialGaussMethod( std::vector<std::vector<double>> matrix, const int rows, std::vector<double> &result ) {
	double koef;
  auto t1 = system_clock::now();
	// прямой ход метода Гаусса
	for (int k = 0; k < rows; ++k)	{
		for (int i = k + 1; i < rows; ++i) {
			koef = -matrix[i][k] / matrix[k][k];

			for (int j = k; j <= rows; ++j) {
				matrix[i][j] += koef * matrix[k][j];
			}
		}
	}
  auto t2 = system_clock::now();
  //duration<double> timer = (t2-t1);
	// обратный ход метода Гаусса
	result[rows - 1] = matrix[rows - 1][rows] / matrix[rows - 1][rows - 1];

	for (int k = rows - 2; k >= 0; --k) {
		result[k] = matrix[k][rows];

		for (int j = k + 1; j < rows; ++j) {
			result[k] -= matrix[k][j] * result[j];
		}

		result[k] /= matrix[k][k];
	}

  return (duration<double>)(t2-t1);
}

duration<double> ParallelGaussMethod( std::vector<std::vector<double>> matrix, const int rows, std::vector<double> &result ) {
	auto t1 = system_clock::now();
	// прямой ход метода Гаусса
	for (int k = 0; k < rows; ++k)	{
		cilk_for (int i = k + 1; i < rows; ++i) {
			double koef = -matrix[i][k] / matrix[k][k];

			for (int j = k; j <= rows; ++j) {
				matrix[i][j] += koef * matrix[k][j];
			}
		}
	}
  auto t2 = system_clock::now();
  
	// обратный ход метода Гаусса
	result[rows - 1] = matrix[rows - 1][rows] / matrix[rows - 1][rows - 1];

	for (int k = rows - 2; k >= 0; --k) {
		cilk::reducer_opadd<double> resK(matrix[k][rows]);

		cilk_for (int j = k + 1; j < rows; ++j) {
			resK -= matrix[k][j] * result[j];
		}

		result[k] = resK.get_value() / matrix[k][k];
	}

  return (duration<double>)(t2-t1);
}

int main() {
  srand( (unsigned) time( 0 ) );

  // массив решений СЛАУ
  std::vector<double> result (MATRIX_SIZE);
  // исходный массив
  std::vector<std::vector<double>> test_mx (MATRIX_SIZE, std::vector<double> (MATRIX_SIZE+1));

  //InitMatrix(test_mx);

  test_mx[0][0] = 2; test_mx[0][1] = 5;  test_mx[0][2] = 4;  test_mx[0][3] = 1;  test_mx[0][4] = 20;
	test_mx[1][0] = 1; test_mx[1][1] = 3;  test_mx[1][2] = 2;  test_mx[1][3] = 1;  test_mx[1][4] = 11;
	test_mx[2][0] = 2; test_mx[2][1] = 10; test_mx[2][2] = 9;  test_mx[2][3] = 7;  test_mx[2][4] = 40;
	test_mx[3][0] = 3; test_mx[3][1] = 8;  test_mx[3][2] = 9;  test_mx[3][3] = 2;  test_mx[3][4] = 37;

  duration<double> timer = SerialGaussMethod( test_mx, MATRIX_SIZE, result );
  duration<double> timer = ParallelGaussMethod( test_mx, MATRIX_SIZE, result );

  std::cout << "Solution:" << std::endl;
  for (int i = 0; i < MATRIX_SIZE; ++i ) {
		std::cout << "x(" << i << ") = " << result[i] << std::endl;
	}

  std::cout << "Serial time: " << std::setprecision(10) << timer.count() << std::endl;
  std::cout << "Parallel time: " << std::setprecision(10) << timer1.count() << std::endl;

	return 0;
}