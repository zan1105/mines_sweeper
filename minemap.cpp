#include <stdlib.h>
#include "minemap.h"
#include <iostream>

void MineMap::restart(int width, int height, int mines) {
	if (width != mapWidth || height != mapHeight) {
		mapWidth = width;
		mapHeight = height;
		m_map = vector(width, vector<int>(height, 0));
	} else {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				m_map[i][j] = 0; // 重置地图
			}
		}
	}
	_gameOver = false;
	_markedCellNum = 0;
	_openedCellNum = 0;
	mineNum = mines;
	_generate(mines);
}

void MineMap::leftClick(int x, int y) {
	if (m_map[x][y] >= 20) { // 已标记
		m_map[x][y] -= 20;   // 取消标记
		_markedCellNum--;
	} else if (m_map[x][y] >= 10) { // 已翻开
		_remainingTips(x, y);
	} else if (m_map[x][y] == 9) { // 踩雷
		if (_openedCellNum == 0) { // 首次容错
			while (true) {         // 将雷与一个非雷交换
				int i = rand() % mapWidth;
				int j = rand() % mapHeight;
				if (m_map[i][j] != 9) {
					m_map[i][j] = 9;
					break;
				}
			}
			m_map[x][y] = 0;
			_calculateNumbers(); // 重新计算数字
			leftClick(x, y);     // 重新点击

		} else {                                 // 游戏结束
			for (int i = 0; i < mapWidth; i++) { // 翻开所有雷
				for (int j = 0; j < mapHeight; j++) {
					if (m_map[i][j] == 9) {
						m_map[i][j] += 10;
					}
				}
			}
			m_map[x][y] = -1; // 标记踩到的雷
			_markedCellNum = 0;
			_gameOver = true;
		}
	} else if (m_map[x][y] == 0) { // 空白块
		_expandEmptyArea(x, y);    // 展开空白区域

	} else if (m_map[x][y] > 0) { // 数字区域
		m_map[x][y] += 10;
		_openedCellNum++;
		if (_openedCellNum == 1) { // 首次点击时将附近的非雷格子翻开
			for (int i = x - 1; i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					if (i == x && j == y) continue;
					if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight && m_map[i][j] < 9) {
						leftClick(i, j);
					}
				}
			}
		}
	}
}

void MineMap::rightClick(int x, int y) {
	if (m_map[x][y] >= 20) { // 已标记
		m_map[x][y] -= 20;   // 取消标记
		_markedCellNum--;

	} else if (m_map[x][y] >= 10) { // 已翻开
		_remainingTips(x, y);       // 提示 9 格内未翻开的块

	} else {               // 未标记
		m_map[x][y] += 20; // 标记
		_markedCellNum++;
	}
}

void MineMap::_generate(int mines) {
	// 布雷
	while (mines) {
		int x = rand() % mapWidth;
		int y = rand() % mapHeight;
		if (m_map[x][y] != 9) {
			m_map[x][y] = 9;
			mines--;
		}
	}

	_calculateNumbers(); // 计算数字
}

void MineMap::_expandEmptyArea(int x, int y) {
	m_map[x][y] += 10;
	_openedCellNum++;
	for (int i = x - 1; i <= x + 1; i++) { // 展开空白区域
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) continue;
			if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight && m_map[i][j] < 10) {
				leftClick(i, j);
			}
		}
	}
}

void MineMap::_calculateNumbers() {
	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			if (m_map[i][j] % 10 == 9 || (m_map[i][j] >= 10 && m_map[i][j] < 20)) {
				continue;
			}
			m_map[i][j] = 0; // 重置数字
			_calculateCellNumber(i, j);
		}
	}
}

void MineMap::_calculateCellNumber(int x, int y) {
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight && m_map[i][j] % 10 == 9) {
				m_map[x][y]++;
			}
		}
	}
}

void MineMap::_remainingTips(int x, int y) {
	int tag = 0;
	using std::pair;
	vector<pair<int, int>> tips;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i < 0 || i >= mapWidth || j < 0 || j >= mapHeight || (i == x && j == y))
				continue;
			else if (m_map[i][j] < 10) {
				tips.push_back({i, j});
			} else if (m_map[i][j] >= 20) {
				tag++;
			}
		}
	}
	if (tag >= m_map[x][y] - 10) {
		for (auto tip : tips) {
			if (m_map[tip.first][tip.second] == 19) { // 标红猜错的雷
				m_map[tip.first][tip.second] = -1;
				_markedCellNum = 0;
			} else
				leftClick(tip.first, tip.second); // 翻开不是雷的块
		}
	} else {
		for (auto tip : tips) {
			m_map[tip.first][tip.second] += 30; // 提示未翻开的块
		}
	}
}

int MineMap::getGameStatus() {
	if (_gameOver) {
		return 2;
	} else if (_markedCellNum == mapWidth * mapHeight - _openedCellNum) {
		return 3;
	} else {
		return 0;
	}
}

bool MineMap::_judgeGameOver(int x, int y) { return true; }
