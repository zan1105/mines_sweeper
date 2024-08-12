#ifndef MINEMAP_H
#define MINEMAP_H

#include <vector>

// #include "mainwindow.h"

using std::vector;

class MineMap {
public:
	MineMap(int width = 9, int height = 9, int mines = 10) { restart(width, height, mines); }

public:
	void restart(int width, int height, int mines);
	int  getVal(int x, int y) { return m_map[x][y]; }
	void setVal(int x, int y, int val) { m_map[x][y] = val; }
	void leftClick(int x, int y);
	void rightClick(int x, int y);
	int  getGameStatus();
	int  getMarkedCellNum() { return _markedCellNum; }

private:
	void _generate(int mines);           // 生成雷区
	void _expandEmptyArea(int x, int y); // 递归展开空白区域
	void _calculateNumbers();            // 计算范围内的雷数
	void _remainingTips(int x, int y);   // 提示范围内未翻开的块
	bool _judgeGameOver(int x, int y);   // 判断游戏是否结束

private:
	bool _fault_tolerance;
	bool _gameOver;
	int  _markedCellNum; //	标记的格子数
	int  _openedCellNum; //	翻开的格子数

	vector<vector<int>> m_map;
};

#endif // MINEMAP_H
