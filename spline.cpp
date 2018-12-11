#include "spline.h"

/*
Класс кубического сплайна, интерполирующий поточечно заданную функцию,
содержит массив из «кусочков» полиномов степени не выше третьей между узлами и
вычисляет значение функции в произвольной точке. Кратные узлы запрещены.
Сплайны будут использоваться для задания входных функций угловых скоростей колес робота.
*/

Spline::Spline() { }

Spline::~Spline()
{
	freeMem();
}

void Spline::buildSpline(const double *x, const double *y, std::size_t n)
{
	freeMem();
	this->n = n;
	splines = new splineTuple[n];
	for (std::size_t i = 0; i < n; ++i)
	{
		splines[i].x = x[i];
		splines[i].a = y[i];
	}
	splines[0].c = 0.;

	//Коэффициенты могут быть найдены с помощью прогонки трехдиагональной матрицы.

	double *alpha = new double[n - 1];
	double *beta = new double[n - 1];
	double A, B, C, F, h_i, h_i1, z;
	alpha[0] = beta[0] = 0.;
	for (std::size_t i = 1; i < n - 1; ++i)
	{
		h_i = x[i] - x[i - 1], h_i1 = x[i + 1] - x[i];
		A = h_i;
		C = 2. * (h_i + h_i1);
		13
			B = h_i1;
		F = 6. * ((y[i + 1] - y[i]) / h_i1 - (y[i] - y[i - 1]) / h_i);
		z = (A * alpha[i - 1] + C);
		alpha[i] = -B / z;
		beta[i] = (F - A * beta[i - 1]) / z;
	}
	splines[n - 1].c = (F - A * beta[n - 2]) / (C + A * alpha[n - 2]);
	for (int i = n - 2; i > 0; --i)
		splines[i].c = alpha[i] * splines[i + 1].c + beta[i];
	delete[] beta;
	delete[] alpha;
	for (int i = n - 1; i > 0; --i)
	{
		double h_i = x[i] - x[i - 1];
		splines[i].d = (splines[i].c - splines[i - 1].c) / h_i;
		splines[i].b = h_i * (2. * splines[i].c + splines[i - 1].c) / 6. + (y[i]
			- y[i - 1]) / h_i;
	}
}

double Spline::interpolate(double x) const //Функция, выдающая значение функции по заданной точке
{
	if (!splines)
		return std::numeric_limits<double>::quiet_NaN();
	splineTuple *s;
	if (x <= splines[0].x)
		s = splines + 1;
	else if (x >= splines[n - 1].x)
		s = splines + n - 1;
	else
	{
		std::size_t i = 0, j = n - 1;
		while (i + 1 < j)
		{
			std::size_t k = i + (j - i) / 2;
			if (x <= splines[k].x)
				j = k;
			else
				i = k;
		}
		s = splines + j;
	}
	double dx = (x - s->x);
	return s->a + (s->b + (s->c / 2. + s->d * dx / 6.) * dx) * dx;
}

void Spline::freeMem()
{
	delete[] splines;
	splines = NULL;
}