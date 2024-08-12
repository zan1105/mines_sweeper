#include <stdlib.h>
#include "minemap.h"
#include <iostream>

void MineMap::restart(int width, int height, int mines) {
	if (width != m_map.size() || height != m_map[0].size())
		m_map = vector(width, vector<int>(height, 0));
	else {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				m_map[i][j] = 0;
			}
		}
	}
	_fault_tolerance = true;
	_gameOver = false;
	_markedCellNum = 0;
	_openedCellNum = 0;
	_generate(mines);
}

void MineMap::leftClick(int x, int y) {
	if (m_map[x][y] >= 20) { // 已标记
		m_map[x][y] -= 20;   // 取消标记
		_markedCellNum--;
	} else if (m_map[x][y] >= 10) { // 已翻开
		_remainingTips(x, y);
	} else if (m_map[x][y] == 9) { // 踩雷
		if (_fault_tolerance) {    // 容错
			int num = m_map.size() * m_map[0].size();
			while (num--) {
				int i = rand() % m_map.size();
				int j = rand() % m_map[0].size();
				if ((i != x || j != y) && m_map[i][j] < 9) {
					m_map[i][j] = 9;
					break;
				}
			}
			m_map[x][y] = 0;
			_fault_tolerance = false;
			_calculateNumbers();
			leftClick(x, y);
		} else { // 游戏结束
			for (int i = 0; i < m_map.size(); i++) {
				for (int j = 0; j < m_map[0].size(); j++) {
					if (m_map[i][j] == 9) {
						m_map[i][j] += 10;
					}
				}
			}
			m_map[x][y] = -1;
			_gameOver = true;
		}
	} else if (m_map[x][y] == 0) { // 空白块
		_expandEmptyArea(x, y);
	} else { // 数字区域
		m_map[x][y] += 10;
		_openedCellNum++;
		if (_fault_tolerance) {
			_fault_tolerance = false;
			for (int i = x - 1; i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					if (i == x && j == y) continue;
					if (i >= 0 && i < m_map.size() && j >= 0 && j < m_map[0].size() &&
					    m_map[i][j] < 9) {
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
		_remainingTips(x, y);
	} else { // 未标记
		m_map[x][y] += 20;
		_markedCellNum++;
	}
}

void MineMap::_generate(int mines) {
	int width = m_map.size();
	int height = m_map[0].size();
	// 布雷
	while (mines) {
		int x = rand() % width;
		int y = rand() % height;
		if (m_map[x][y] != 9) {
			m_map[x][y] = 9;
			mines--;
		}
	}

	_calculateNumbers(); // 计算数字
}

void MineMap::_expandEmptyArea(int x, int y) {
	_fault_tolerance = false;
	m_map[x][y] += 10;
	_openedCellNum++;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) continue;
			if (i >= 0 && i < m_map.size() && j >= 0 && j < m_map[0].size() && m_map[i][j] < 10) {
				leftClick(i, j);
			}
		}
	}
}

void MineMap::_calculateNumbers() {
	for (int i = 0; i < m_map.size(); i++) {
		for (int j = 0; j < m_map[0].size(); j++) {
			if (m_map[i][j] % 10 == 9 || (m_map[i][j] >= 10 && m_map[i][j] < 20)) {
				continue;
			}
			m_map[i][j] = 0;
			for (int x = i - 1; x <= i + 1; x++) {
				for (int y = j - 1; y <= j + 1; y++) {
					if (x >= 0 && x < m_map.size() && y >= 0 && y < m_map[0].size() &&
					    m_map[x][y] % 10 == 9) {
						m_map[i][j]++;
					}
				}
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
			if (i < 0 || i >= m_map.size() || j < 0 || j >= m_map[0].size() || (i == x && j == y))
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
	} else if (_markedCellNum == m_map.size() * m_map[0].size() - _openedCellNum) {
		return 3;
	} else {
		return 0;
	}
}

bool MineMap::_judgeGameOver(int x, int y) { return true; }
