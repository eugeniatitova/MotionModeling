#ifndef ROBOT_H
#define ROBOT_H
#include <QGraphicsPixmapItem>
#include "math.h"
#include "spline.h"

class Robot : public QGraphicsPixmapItem
{
public:
	Robot(double r, double h);
	Spline ul, ur;
	QVector<double> x, y, theta, time;
	int position;
	void calculatePath(double step);
	double maxX, minX, maxY, minY;
	double start, finish;
	virtual void advance(int phase);
private:
    double r, h;
	QTransform trans;
	double step;
	double stretchX, stretchY, shiftX, shiftY;
	double heightOfRobot, widthOfRobot;
	double xToPixels(double x);
	double yToPixels(double y);
	void transform();
	double dx_dt(double ul, double ur, double theta);
	double dy_dt(double ul, double ur, double theta);
	double dtheta_dt(double ul, double ur);
	void RungeKutta();
};

#endif // ROBOT_H