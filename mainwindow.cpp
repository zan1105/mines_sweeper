#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

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

	// 初始化游戏玩法参数
	start = false;
	n_width = 9;
	n_height = 9;
	mineNum = 10;
	mineMap = new MineMap(n_width, n_height, mineNum);
	state = mineMap->getGameStatus();

	// 初始化按钮
	// 点击按钮显示游戏玩法
	GameInfoButton = new QPushButton("玩法说明", this);
	GameInfoButton->setGeometry(0, 0, 80, menuHeight);
	connect(GameInfoButton, &QPushButton::clicked, this, &MainWindow::showGameInfo);

	// 单选按钮选择游戏难度
	for (int i = 0; i < 5; i++) {
		QRadioButton *radioButton = new QRadioButton(this);
		radioButton->setGeometry(80 + 75 * i, 0, 75, menuHeight); // 按钮位置
		radioButton->setText(texts[i]);                           // 按钮文本
		radioButton->setCheckable(true);                          // 设置是否可选
		radioButton->setChecked(i == 0);                          // 设置默认选中
		radioButton->show();
		radioButtons.push_back(radioButton);
		connect(radioButton, &QRadioButton::clicked, this, [=]() { customParams(i); });
	}

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
}

MainWindow::~MainWindow() {

	delete timer;                         // 释放计时器对象
	delete GameInfoButton;                // 释放按钮对象
	for (auto radioButton : radioButtons) // 释放单选按钮对象
		delete radioButton;
	for (auto cellSvg : cellSvgs) // 释放单元格图片对象
		delete cellSvg;
	for (auto counterSvg : counterSvgs) // 释放计数器图片对象
		delete counterSvg;
	for (auto stateSvg : stateSvgs) // 释放状态图片对象
		delete stateSvg;

	delete mineMap; // 释放雷区对象
	delete[] texts; // 释放游戏难度文本
	delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 更新窗口参数
	cell_width = std::min(width() / n_width, (height() - menuHeight) / (n_height + 1));
	startY = menuHeight + cell_width;
	startX = (width() - cell_width * n_width) / 2;
	cntWidth = cell_width * 13 / 23;
	// 绘制背景
	// painter.setBrush(QBrush(QColor(192, 0, 0)));
	// painter.drawRect(0, 0, 800, 600);

	// 绘制雷数
	int cnt = std::max(0, mineNum - mineMap->getMarkedCellNum());
	for (int i = 2; i >= 0; i--) {
		counterSvgs[cnt % 10]->render(
		    &painter, QRect(startX + i * cntWidth, menuHeight, cntWidth, cell_width));
		cnt /= 10;
	}

	// 绘制时间
	cnt = start ? std::chrono::duration_cast<std::chrono::seconds>(
	                  std::chrono::system_clock::now() - startTime)
	                  .count()
	            : 0;
	for (int i = 1; i <= 3; i++) {
		counterSvgs[cnt % 10]->render(
		    &painter, QRect(width() - startX - i * cntWidth, menuHeight, cntWidth, cell_width));
		cnt /= 10;
	}

	// 绘制状态
	stateSvgs[state]->render(&painter,
	                         QRect((width() - cell_width) / 2, menuHeight, cell_width, cell_width));

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
			else // 提示格子
				cellSvgs[0]->render(&painter, rect);
		}
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton) return;
	pressX = event->position().x();
	pressY = event->position().y();
	if (pressY < startY && pressX > (width() - cell_width) / 2 &&
	    pressX < (width() + cell_width) / 2) { // 点击笑脸刷新游戏
		state = 1;
		start = false;
		mineMap->restart(n_width, n_height, mineNum);
	} else if (state > 1) { // 游戏结束，不响应点击事件
		return;

	} else if (pressX > startX && pressX < startX + n_width * cell_width && pressY > startY &&
	           pressY < startY + n_height * cell_width) { // 点击雷区
		int x = (pressX - startX) / cell_width;
		int y = (pressY - startY) / cell_width;
		if (event->button() == Qt::LeftButton) {
			mineMap->leftClick(x, y);
		} else {
			mineMap->rightClick(x, y);
		}
		state = state == 0 ? mineMap->getGameStatus() : state;
		update();
		if (start == false) { // 开始计时
			start = true;
			startTime = std::chrono::system_clock::now();
			timer->start(100);
		}
	}

	update(); // 重绘
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton || state > 1)
		return;
	if (state == 1) state = 0; // 恢复笑脸

	// 撤销提示
	if (pressX > startX && pressX < startX + n_width * cell_width && pressY > startY &&
	    pressY < startY + n_height * cell_width) {
		int x = (pressX - startX) / cell_width;
		int y = (pressY - startY) / cell_width;
		if (mineMap->getVal(x, y) >= 10 && mineMap->getVal(x, y) < 20) {
			for (int i = x - 1; i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					if (i >= 0 && i < n_width && j >= 0 && j < n_height &&
					    mineMap->getVal(i, j) >= 30) {
						mineMap->setVal(i, j, mineMap->getVal(i, j) - 30);
					}
				}
			}
		}
	}

	update();
}

void MainWindow::showGameInfo() { QMessageBox::information(this, "游戏玩法说明", GameInfo); }

void MainWindow::customParams(int index) {

	if (index < 4) {
		n_width = cellNums[index][0];
		n_height = cellNums[index][1];
		mineNum = cellNums[index][2];
	} else {
		bool ok;
		int new_numX = QInputDialog::getInt(this, "自定义", "请输入水平格子数", 9, 9, 100, 1, &ok);
		if (!ok) return;
		int new_numY = QInputDialog::getInt(this, "自定义", "请输入垂直格子数", 9, 9, 100, 1, &ok);
		if (!ok) return;
		int n = QInputDialog::getInt(this, "自定义", "请输入雷数", 10, 10, new_numX * new_numY / 2,
		                             1, &ok);
		if (!ok) return;
		n_width = new_numX;
		n_height = new_numY;
		mineNum = n;
	}
	start = false;
	state = 0;
	mineMap->restart(n_width, n_height, mineNum);
	update();
}

void MainWindow::updateTime() {
	if (start && state < 2) {
		update();
		timer->start(100);
	}
}
