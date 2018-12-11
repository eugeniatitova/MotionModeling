#include "mainwindow.h"
#include "ui_mainwindow.h"

#define height 364
#define width 464

/*
В конструкторе главного окна создадим робота с радиусом колеса 1 и шириной между
колесами 2. Анимация будет отрисовываться по таймеру с частотой 0,1 мс., привязанному к
методу Robot::advance(), который является слотом родительского класса
QGraphicsPixmapItem. Данные для робота будут загружаться из окошек GUI методом
MainWindow::loadData(). При нажатии кнопки в методе MainWindow::onGoButtonPressed()
будут вычислены функции угла и координат с заданным шагом и нарисованы графики
координат от времени с помощью средств библиотеки QСustomPlot.
*/

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	scene = new QGraphicsScene(0, 0, width, height, this);
	ui->xyGraphicsView->setScene(scene);
	robot = new Robot(1., 2.);
	scene->addItem(robot);
	timer = new QTimer(this);
	timer->start(0.1);
	connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
	connect(ui->goButton, SIGNAL(pressed()), this, SLOT(goButtonPressed()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadData()
{
	QStringList ulString = ui->ulValueLineEdit->text().split(" ");
	QStringList tlString = ui->ulTimeLineEdit->text().split(" ");
	QStringList urString = ui->urValueLineEdit->text().split(" ");
	QStringList trString = ui->urTimeLineEdit->text().split(" ");
	int nl = ulString.size();
	int nr = urString.size();
	double ulArray[nl];
	double tlArray[nl];
	double urArray[nr];
	double trArray[nr];
	for (int i = 0; i< nl; i++)
	{
		ulArray[i] = ulString[i].toDouble();
		tlArray[i] = tlString[i].toDouble();
	}
	for (int j = 0; j< nr; j++)
	{
		urArray[j] = urString[j].toDouble();
		trArray[j] = trString[j].toDouble();
	}
	robot->start = (tlArray[0] >= trArray[0]) ? tlArray[0] : trArray[0];
	robot->finish = (tlArray[nl - 1] >= trArray[nr - 1]) ? tlArray[nl - 1] :
		trArray[nr - 1];
	robot->ul.buildSpline(ulArray, tlArray, nl);
    robot->ur.buildSpline(urArray, trArray, nr);
}

void MainWindow::goButtonPressed()
{
	loadData();
	robot->calculatePath(0.001);
	ui->xtWidget->clearGraphs();
	ui->xtWidget->addGraph();
	ui->xtWidget->graph(0)->setData(robot->time, robot->x);
	ui->xtWidget->xAxis->setRange(robot->time[0], robot->time[robot -
	>time.size() - 1]);
	ui->xtWidget->yAxis->setRange(robot->minX, robot->maxX);
	ui->xtWidget->replot();
	ui->ytWidget->clearGraphs();
	ui->ytWidget->addGraph();
	ui->ytWidget->graph(0)->setData(robot->time, robot->y);
	ui->ytWidget->xAxis->setRange(robot->time[0], robot->time[robot -
	>time.size() - 1]);
	ui->ytWidget->yAxis->setRange(robot->minY, robot->maxY);
	ui->ytWidget->replot();
}