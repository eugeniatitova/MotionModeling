#ifndef SPLINE_H
#define SPLINE_H
#include <cstdlib>
#include <cmath>
#include <limits>

class Spline
{
public:
	Spline();
	~Spline();
	void buildSpline(const double *x, const double *y, std::size_t n);
	double interpolate(double x) const;
private:
	struct splineTuple
	{
		double a, b, c, d, x;
	};
	splineTuple *splines;
	std::size_t n;
	void freeMem();
};

#endif // SPLINE_H
#endif // SPLINE_H
