#include <iostream>
#include <vector>
#include <thread>
#include <exception>
#include <functional>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

using namespace std::chrono;

#define NUMB_ROWS 2
#define NUMB_COLS 3


/*
 * перечисление, определ€ющее как будет происходить вычисление
 * средних значений матрицы: по строкам или по столбцам
*/
#define BY_ROWS 0
#define BY_COLS 1

/**
 * ‘ункци€ инициализации матрицы.
 * @param matrix - исходна€ матрица дл€ заполнени€
*/
void InitMatrix(std::vector<std::vector<double>> &matrix) {
	for(size_t i = 0; i < matrix.size(); ++i) {
		for(size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] = rand() % 5 + 1;
		}
	}
}

/**
 * ‘ункци€ PrintMatrix() печатает элементы матрицы <i>matrix</i> на консоль;
 * @param numb_rows - количество строк в исходной матрице <i>matrix</i>;
 * @param numb_cols - количество столбцов в исходной матрице <i>matrix</i>.
*/
void PrintMatrix(std::vector<std::vector<double>> matrix) {
	printf( "Generated matrix:\n" );
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			printf( "%lf ", matrix[i][j] );
		}
		printf( "\n" );
	}
}

/*
 * ‘ункци€ FindAverageValues() находит средние значени€ в матрице <i>matrix</i>
 * по строкам, либо по столбцам в зависимости от значени€ параметра <i>proc_type</i>;
 * @param proc_type - признак, в зависимости от которого средние значени€ вычисл€ютс€ 
 * либо по строкам, либо по стобцам исходной матрицы <i>matrix</i>;
 * @param matrix - исходна€ матрица;
 * @param numb_rows - количество строк в исходной матрице <i>matrix</i>;
 * @param numb_cols - количество столбцов в исходной матрице <i>matrix</i>.;
 * @param average_vals - массив, куда сохран€ютс€ вычисленные средние значени€.
*/
void FindAverageValues(int mode, std::vector<std::vector<double>> matrix, std::vector<double> &average_vals) {
	switch (mode) {
		case BY_ROWS: {
			for(size_t i = 0; i < matrix.size(); ++i) {
				cilk::reducer_opadd<double> sum( 0.0 );
				cilk_for(size_t j = 0; j < matrix[0].size(); ++j) {
					sum += matrix[i][j];
				}
				average_vals[i] = sum.get_value() / matrix[0].size();
			}
			break;
		}
		case BY_COLS: {
			for(size_t j = 0; j < matrix[0].size(); ++j) {
				cilk::reducer_opadd<double> sum( 0.0 );
				cilk_for(size_t i = 0; i < matrix.size(); ++i) {
					sum += matrix[i][j];
				}
				average_vals[j] = sum.get_value() / matrix.size();
			}
			break;
		}
		default: {
			throw("Incorrect value for parameter 'mode' in function FindAverageValues() call!");
		}
	}
}

/*
 * ‘ункци€ PrintAverageVals() печатает элементы массива <i>average_vals</i> на консоль;
 * @param proc_type - признак, отвечающий за то, как были вычислены 
 * средние значени€ исходной матрицы по строкам или по столбцам
 * @param average_vals - массив, хран€щий средние значени€ исходной матрицы,
 * вычисленные по строкам или по столбцам
 * @param dimension - количество элементов в исходной массиве <i>average_vals</i> 
*/
void PrintAverageVals(int mode, std::vector<double> average_vals) {
	switch (mode) {
		case BY_ROWS: {
			printf("\nAverage values in rows:\n");
			for(size_t i = 0; i < average_vals.size(); ++i) {
				printf("Row %u: %lf\n", i, average_vals[i]);
			}
			break;
		}
		case BY_COLS: {
			printf("\nAverage values in columns:\n");
			for(size_t i = 0; i < average_vals.size(); ++i) {
				printf("Column %u: %lf\n", i, average_vals[i]);
			}
			break;
		}
		default: {
			throw("Incorrect value for parameter 'mode' in function PrintAverageVals() call!");
		}
	}
}


int main() {
	srand( (unsigned) time( 0 ) );

	std::vector<std::vector<double>> matrix (NUMB_ROWS, std::vector<double>(NUMB_COLS));

	std::vector<double> average_vals_in_rows (NUMB_ROWS);
	std::vector<double> average_vals_in_cols (NUMB_COLS);
	try {
		InitMatrix(matrix);
		PrintMatrix(matrix); 

		std::thread first_thr(FindAverageValues, BY_ROWS, matrix, std::ref(average_vals_in_rows));
		std::thread second_thr(FindAverageValues, BY_COLS, matrix, std::ref(average_vals_in_cols));

		first_thr.join();
		second_thr.join();

		PrintAverageVals(BY_ROWS, average_vals_in_rows);
		PrintAverageVals(BY_COLS, average_vals_in_cols);
	}
	catch (std::exception& except) {
		std::cerr << "Error: " << except.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}