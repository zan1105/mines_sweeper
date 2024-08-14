#ifndef MINEMAP_H
#define MINEMAP_H

#include <vector>

// #include "mainwindow.h"

using std::vector;

class MineMap {
public:
	MineMap(int width = 9, int height = 9, int mines = 10)
	    : mapWidth(width), mapHeight(height), mineNum(mines) {
		m_map = vector(width, vector<int>(height, 0));
		restart(width, height, mines);
	}

public:
	void restart(int width, int height, int mines);           // 重置游戏
	int  getVal(int x, int y) { return m_map[x][y]; }         // 获取指定块的值
	void setVal(int x, int y, int val) { m_map[x][y] = val; } // 设置指定块的值
	void leftClick(int x, int y);                             // 左键点击
	void rightClick(int x, int y);                            // 右键点击
	int  getGameStatus();                                     // 获取游戏状态
	int  getMarkedCellNum() { return _markedCellNum; }        // 获取标记的格子数

private:
	void _generate(int mines);                     // 生成雷区
	void _expandEmptyArea(int x, int y);           // 递归展开空白区域
	void _calculateNumbers();                      // 计算范围内的雷数
	void _calculateCellNumber(int x, int y);       // 计算指定块周围的雷数
	void _remainingTips(int x, int y);             // 提示范围内未翻开的块
	bool _helper1(int x, int y, bool honrizontal); // _tryChooseOne 使用的辅助函数
	bool _tryChooseOne(int x, int y);              // 二选一

private:
	bool _gameOver;
	int  _markedCellNum; //	标记的格子数
	int  _openedCellNum; //	翻开的格子数
	int  mapWidth;       //	地图高度（水平方向的格子数）
	int  mapHeight;      //	地图宽度（垂直方向的格子数）
	int  mineNum;        //	雷数

	vector<vector<int>> m_map; // 地图格子状态值
};

#endif // MINEMAP_H
