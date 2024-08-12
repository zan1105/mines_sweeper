#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// 1 : 1
	cellSvgs = {new QSvgRenderer(QString(":/img/celldown.svg")), // 0
	            new QSvgRenderer(QString(":/img/cell1.svg")),
	            new QSvgRenderer(QString(":/img/cell2.svg")),
	            new QSvgRenderer(QString(":/img/cell3.svg")),
	            new QSvgRenderer(QString(":/img/cell4.svg")),
	            new QSvgRenderer(QString(":/img/cell5.svg")),
	            new QSvgRenderer(QString(":/img/cell6.svg")),
	            new QSvgRenderer(QString(":/img/cell7.svg")),
	            new QSvgRenderer(QString(":/img/cell8.svg")),
	            new QSvgRenderer(QString(":/img/cellmine.svg")), // 9
	            new QSvgRenderer(QString(":/img/cellup.svg")),   // 10
	            new QSvgRenderer(QString(":/img/cellflag.svg")), // 11
	            new QSvgRenderer(QString(":/img/blast.svg"))};   // 12

	// 13 : 23
	counterSvgs = {new QSvgRenderer(QString(":/img/counter0.svg")),
	               new QSvgRenderer(QString(":/img/counter1.svg")),
	               new QSvgRenderer(QString(":/img/counter2.svg")),
	               new QSvgRenderer(QString(":/img/counter3.svg")),
	               new QSvgRenderer(QString(":/img/counter4.svg")),
	               new QSvgRenderer(QString(":/img/counter5.svg")),
	               new QSvgRenderer(QString(":/img/counter6.svg")),
	               new QSvgRenderer(QString(":/img/counter7.svg")),
	               new QSvgRenderer(QString(":/img/counter8.svg")),
	               new QSvgRenderer(QString(":/img/counter9.svg"))};

	// 1 : 1
	stateSvgs = {new QSvgRenderer(QString(":/img/smileface.svg")),
	             new QSvgRenderer(QString(":/img/smilefacedown.svg")),
	             new QSvgRenderer(QString(":/img/lostface.svg")),
	             new QSvgRenderer(QString(":/img/winface.svg"))};

	n_width = 9;
	n_height = 9;
	mineNum = 10;
	start = false;
	time = 0;
	mineMap = new MineMap(n_width, n_height, mineNum);
	state = mineMap->getGameStatus();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	startY = 0.1 * height();
	max_width = std::min(width(), height() - startY);
	startX = (width() - max_width) / 2;
	cell_width = std::min(max_width / n_width, max_width / n_height);
	cntWidth = cell_width * 13 / 23;
	// 绘制背景
	// painter.setBrush(QBrush(QColor(192, 0, 0)));
	// painter.drawRect(0, 0, 800, 600);

	// 绘制雷数
	int cnt = std::max(0, mineNum - mineMap->getMarkedCellNum());
	for (int i = 2; i >= 0; i--) {
		counterSvgs[cnt % 10]->render(&painter,
		                              QRect(startX + i * cntWidth, 0, cntWidth, cell_width));
		cnt /= 10;
	}

	// 绘制时间
	cnt = int(time);
	for (int i = 1; i <= 3; i++) {
		counterSvgs[cnt % 10]->render(
		    &painter, QRect(width() - startX - i * cntWidth, 0, cntWidth, cell_width));
		cnt /= 10;
	}

	// 绘制状态
	state = state == 0 ? mineMap->getGameStatus() : state;
	stateSvgs[state]->render(&painter,
	                         QRect((width() - cell_width) / 2, 0, cell_width, cell_width));

	// 绘制网格
	for (int i = 0; i < n_width; i++) {
		for (int j = 0; j < n_height; j++) {
			int   val = mineMap->getVal(i, j);
			QRect rect(startX + i * cell_width, startY + j * cell_width, cell_width, cell_width);
			if (val < 0) // 踩雷格子
				cellSvgs[12]->render(&painter, rect);
			else if (val < 10) // 未翻开格子
				cellSvgs[10]->render(&painter, rect);
			else if (val < 20) // 翻开格子
				cellSvgs[val - 10]->render(&painter, rect);
			else if (val < 30) // 标记格子
				cellSvgs[11]->render(&painter, rect);
			else { // 提示格子
				cellSvgs[0]->render(&painter, rect);
				mineMap->setVal(i, j, val - 30);
			}
		}
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton) return;
	pressX = event->position().x();
	pressY = event->position().y();
	if (pressY < startY && pressX > (width() - cell_width) / 2 &&
	    pressX < (width() + cell_width) / 2) {
		state = 1;
		start = false;
		mineMap->restart(n_width, n_height, mineNum);
	} else if (state > 1) {
		return;

	} else if (pressX > startX && pressX < startX + n_width * cell_width && pressY > startY &&
	           pressY < startY + n_height * cell_width) {
		int x = (pressX - startX) / cell_width;
		int y = (pressY - startY) / cell_width;
		if (event->button() == Qt::LeftButton) {
			mineMap->leftClick(x, y);
		} else {
			mineMap->rightClick(x, y);
		}
		update();
		if (start == false) {
			start = true;
			time = 0;
		}
	}

	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton || state > 1)
		return;
	if (state == 1) state = 0;

	update();
}
