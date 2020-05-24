#include "box.h"
#include <cmath>

//------------------------------------------------------------------------------------------
Box::Box(double min_x, double min_y, double x_width, double y_height) :
	x_min(min_x), y_min(min_y), width(x_width), height(y_height) {}

//------------------------------------------------------------------------------------------
void Box::GetParameters(double& min_x, double& min_y, double& x_width, double& y_height) const
{
	min_x = x_min;
	min_y = y_min;
	x_width = width;
	y_height = height;
}

//------------------------------------------------------------------------------------------
void Box::GetWidhtHeight(double& x_width, double& y_height) const
{
	x_width = width;
	y_height = height;
}

//------------------------------------------------------------------------------------------
double Box::GetDiagonal() const
{
	return sqrt(width*width + height * height);
}