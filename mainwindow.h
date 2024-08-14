#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//	#include <QtWidgets>
#include <QMouseEvent>
#include <QtSvg>
#include <QPainter>
#include <QRadioButton>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>
#include <QTimer>
#include <vector>
#include <chrono>

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

private slots:
	void showGameInfo();          //	游戏玩法按钮点击事件
	void customParams(int index); //	自定义按钮点击事件
	void updateTime();            //	更新计时器

private:
	Ui::MainWindow *ui;

	const int menuHeight = 30; //	菜单栏高度
	int       n_width;         //	水平方向的格子数
	int       n_height;        //	垂直方向的格子数
	int       mineNum;         //	雷数
	int       startY;          //	起始Y坐标
	int       startX;          //	起始X坐标
	int       cell_width;      //	格子宽度
	int       cntWidth;        //	计数器宽度
	int       state;           //	游戏状态
	int       pressX;          //	按下的X坐标
	int       pressY;          //	按下的Y坐标
	bool      start;           //	游戏是否开始
	int       lastTime;        //	上一次的时间
	bool      showNumber;      //	踩雷后当前绘制被标记格子是否显示数字
	MineMap  *mineMap;         //	雷区地图
	QTimer   *timer;           //	计时器

	std::chrono::time_point<std::chrono::system_clock> startTime; //	游戏开始时间

	const int     cellNums[4][3] = {{9, 9, 10}, {16, 16, 40}, {30, 16, 99}, {46, 22, 208}};
	const QString GameInfo =
	    "第一次点击不会是雷。格子里的数字表示它周围有几个雷。游戏目标是找出所有雷。“触雷”则输。点击"
	    "表情重新开始。二选一留到最后，可任选，需先清完其他方块。\n操作说明：鼠标右键点击原始方块，"
	    "以小旗标记为雷，再点去除标记。标记的雷数和数字匹配时，点击数字（左右键均可）翻开周围的安全"
	    "区域。鼠标左键点击无标记方块则直接打开。";

	const QString *texts = new QString[5]{"初级", "中级", "高级", "专家", "自定义"};
	QPushButton   *GameInfoButton;            //	游戏玩法按钮
	std::vector<QSvgRenderer *> cellSvgs;     //	单元格图片集
	std::vector<QSvgRenderer *> counterSvgs;  //	计数器数字图片集
	std::vector<QSvgRenderer *> stateSvgs;    //	游戏状态图片集
	std::vector<QRadioButton *> radioButtons; //	单选按钮集
};
#endif //	MAINWINDOW_H
