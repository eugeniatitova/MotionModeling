#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include "robot.h"
#include <math.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void goButtonPressed();
private:
	Ui::MainWindow *ui;
	Robot* robot;
	QGraphicsScene *scene;
	QTimer *timer;
	void loadData();
};

#endif // MAINWINDOW_H