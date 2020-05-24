#pragma once
#include "box.h"
#include <vector>

extern const double g_l1_max;
extern const double g_l2_max;
extern const double g_l1_min;
extern const double g_l2_min;
extern const double g_l0;


extern const double g_precision;


typedef std::pair< std::vector<double>, std::vector<double> > min_max_vectors;
typedef std::pair<Box, Box> boxes_pair;




class low_level_fragmentation
{
protected:
	/// анализируемый в данный момент box
	Box current_box;
	/// Функция VerticalSplitter() разбивает переданный в качестве параметра box по ширине
	void VerticalSplitter(const Box& box, boxes_pair& vertical_splitter_pair);
	/// Функция HorizontalSplitter() разбивает переданный в качестве параметра box по высоте
	void HorizontalSplitter(const Box& box, boxes_pair& horizontal_splitter_pair);
	/// Функция GetNewBoxes() разбивает переданный в качестве параметра box по большей стороне,
	/// вызывая VerticalSplitter() или HorizontalSplitter()
	void GetNewBoxes(const Box& box, boxes_pair& new_pair_of_boxes);
	/// Функция FindTreeDepth() возвращает глубину двоичного дерева, 
	/// которая соответствует количеству итераций алгоритма
	unsigned int FindTreeDepth();
	/// функция ClasifyBox() анализирует box и классифицирует его
	int ClasifyBox(const min_max_vectors& vects);
	/// Функция GetBoxType() добавляет классифицированный ранее box во множество решений, 
	/// или удаляет его из анализа, или добавляет его к граничной области, 
	/// или относит его к тем, что подлежат дальнейшему анализу
	void GetBoxType(const Box& box);
	/// Функция GetMinMax() определяет максимумы минимумы функций gj на box-e
	/// Внимание! В рамках данного класса определения функции GetMinMax() быть не должно
	virtual void GetMinMax(const Box& box, min_max_vectors& min_max_vector) = 0;

public:
	low_level_fragmentation() {}

	low_level_fragmentation(double& min_x, double& min_y, double& x_width, double& y_height);

	low_level_fragmentation(const Box& box);

	/// виртуальный деструктор
	virtual ~low_level_fragmentation() {}
};


class high_level_analysis : public low_level_fragmentation
{
protected:
	/// Переопределенная в дочернем классе функция GetMinMax()
	void GetMinMax(const Box& box, min_max_vectors& min_max_vector) override;

public:
	high_level_analysis() {}

	high_level_analysis(double& min_x, double& min_y, double& x_width, double& y_height);

	high_level_analysis(Box& box);

	///  Функция GetSolution() запускает алгоритм нахождения рабочей области манипулятора
	void GetSolution();
};


/// Функция WriteResults() записывает параметры полученных box-ов (относящихся к рабочему
/// пространству, к граничной области и ко множеству, не являющемуся решением) в выходные 
/// файлы для дальнейшей визуализации
void WriteResults(const char* file_names[]);