#include "robot.h"
#include "math.h"

#define pi 3.141592653589793238462643383279
#define height 364
#define width 464
#define sizeOfPixmap 64.

/*
Класс Robot, наследуемый от класса QGraphicsPixmapItem, во входных параметрах конструктора
принимает значения радиуса колеса r и расстояния между колесами h. Ему нужно задать
угловые скорости колес ul и ur сплайнами, по которым с помощью метода Рунге — Кутты 4
порядка будут вычислены приближенные значения координат центра х, у и theta — угла
поворота как решение задачи Коши с нулевыми начальными условиями. Также прописаны методы 
для преобразования координат анимации его движения по плоскости.
*/

Robot::Robot(double r, double h) : QGraphicsPixmapItem(0)
{
	this->r = r;
	this->h = h;
	setPixmap(QPixmap(":/robot.png"));
	setTransformOriginPoint(sizeOfPixmap / 2, sizeOfPixmap / 2);
}

void Robot::calculatePath(double step)
{
	this->step = step;
	x.clear();
	y.clear();
	theta.clear();
	time.clear();
	position = 0;
	RungeKutta();
}

//Метод отрисовки анимации

void Robot::advance(int phase)
{
	if (phase && position<x.size())
	{
		setRotation(180 * theta[position] / pi);
		setPos(xToPixels(x[position]), yToPixels(y[position]));
		position++;
	}
}

//Методы линейного преобразования координат

double Robot::xToPixels(double x)
{
	if (maxX == minX) return x;
	return (x * stretchX) - shiftX;
}

double Robot::yToPixels(double y)
{
	if (maxY == minY) return y;
	return (y * stretchY) - shiftY;
}

//Метод преобразования габаритов робота

void Robot::transform()
{
	double buffer = sqrt(4 * r * r + h * h);
	stretchX = width / (maxX - minX + 2 * buffer);
	shiftX = stretchX * (minX - buffer);
	stretchY = height / (maxY - minY + 2 * buffer);
	shiftY = stretchY * (minY - buffer);
	heightOfRobot = stretchY * h / sizeOfPixmap;
	widthOfRobot = stretchX * 2 * r / sizeOfPixmap;
	trans.setMatrix(stretchY * h, 0, 0, 0, stretchX * 2 * r, 0, 0, 0, sizeOfPixmap);
	setTransform(trans);
}

//Дифференциальные уравнения, описывающие закон движения робота

double Robot::dx_dt(double ul, double ur, double theta)
{
	return (ur + ul) * cos(theta) / 2.;
}
double Robot::dy_dt(double ul, double ur, double theta)
{
	return (ur + ul) * sin(theta) / 2.;
}
double Robot::dtheta_dt(double ul, double ur)
{
	return (ur - ul) * r / h;
}

//Метод Рунге-Кутты 4 порядка

void Robot::RungeKutta() 
{
	double tempUr, tempUl;
	double tempX = 0, tempY = 0, tempTheta = 0;
	minX = INT32_MAX, minY = INT32_MAX;
	maxX = INT32_MIN, maxY = INT32_MIN;
	double k11, k12, k13, k14;
	double k21, k22, k23, k24;
	double k31, k32, k33, k34;
	for (double x = start; x <= finish; x += step)
	{
		tempUr = ur.interpolate(x);
		tempUl = ul.interpolate(x);
		k11 = dx_dt(tempUl, tempUr, tempTheta);
		k12 = dx_dt(tempUl + step / 2., tempUr + step / 2., tempTheta + step / 2.);
		k13 = k12;
		k14 = dx_dt(tempUl + step, tempUr + step, tempTheta + step);
		k21 = dy_dt(tempUl, tempUr, tempTheta);
		k22 = dy_dt(tempUl + step / 2., tempUr + step / 2., tempTheta + step / 2.);
	    k23 = k22;
		k24 = dy_dt(tempUl + step, tempUr + step, tempTheta + step);
		k31 = dtheta_dt(tempUl, tempUr);
		k32 = dtheta_dt(tempUl + step / 2., tempUr + step / 2.);
		k33 = k32;
		k34 = dtheta_dt(tempUl + step, tempUr + step);
		tempTheta += step * (k31 + 2 * k32 + 2 * k33 + k34) / 6.;
		tempX += step * (k11 + 2 * k12 + 2 * k13 + k14) / 6.;
		tempY += step * (k21 + 2 * k22 + 2 * k23 + k24) / 6.;
		x.push_back(tempX);
		y.push_back(tempY);
		theta.push_back(tempTheta);
		time.push_back(x);
		if (tempX < minX) minX = tempX;
		if (tempX > maxX) maxX = tempX;
		if (tempY < minY) minY = tempY;
		if (tempY > maxY) maxY = tempY;
	}
	transform();
}