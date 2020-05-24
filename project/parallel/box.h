#pragma once
#include <cmath>

/// —труктура Box описывает конкретный экземпл€р box-а
struct Box
{
protected:
	/// лева€ координата x box-a
	double x_min;
	/// нижн€€ координата y box-a
	double y_min;
	/// ширина box-a
	double width;
	/// высота box-a
	double height;
public:
	Box() {}

	Box(double min_x, double min_y, double x_width, double y_height);

	~Box() {}

	/// функци€ GetParameters() возвращает актуальные параметры box-а
	void GetParameters(double& min_x, double& min_y, double& x_width, double& y_height) const;

	/// функци€ GetWidhtHeight() возвращает актуальные значени€ ширины и высоты box-a
	void GetWidhtHeight(double& x_width, double& y_height) const;

	/// функци€ GetDiagonal() возвращает диагональ box-a
	double GetDiagonal() const;
};