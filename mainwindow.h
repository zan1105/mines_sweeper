#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//	#include <QtWidgets>
#include <QMouseEvent>
#include <QtSvg>
#include <QPainter>
#include <QRadioButton>
#include <QInputDialog>
#include <vector>

#include "minemap.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void paintEvent(QPaintEvent *event); //	绘制事件

protected:
	void mousePressEvent(QMouseEvent *event);   //	鼠标按下事件
	void mouseReleaseEvent(QMouseEvent *event); //	鼠标释放事件

private:
	Ui::MainWindow *ui;

	int      n_width;    //	水平方向的格子数
	int      n_height;   //	垂直方向的格子数
	int      mineNum;    //	雷数
	double   time;       //	时间
	int      startY;     //	起始Y坐标
	int      max_width;  //	最大宽度
	int      startX;     //	起始X坐标
	int      cell_width; //	格子宽度
	int      cntWidth;   //	计数器宽度
	int      state;      //	游戏状态
	int      pressX;     //	按下的X坐标
	int      pressY;     //	按下的Y坐标
	bool     start;      //	游戏是否开始
	MineMap *mineMap;    //	雷区

	const QString *texts = new QString[5]{"初级", "中级", "高级", "专家", "自定义"};

	const int cellNums[4][3] = {{9, 9, 10}, {16, 16, 40}, {30, 16, 99}, {46, 22, 208}};

	std::vector<QSvgRenderer *> cellSvgs;     //	单元格图片集
	std::vector<QSvgRenderer *> counterSvgs;  //	计数器数字图片集
	std::vector<QSvgRenderer *> stateSvgs;    //	游戏状态图片集
	std::vector<QRadioButton *> radioButtons; //	单选按钮集
};
#endif //	MAINWINDOW_H
