#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iterator>
#include <Windows.h>
using namespace std;


struct pos
{
	int row;
	int col;
};

void goto_xy(int x, int y)      
{
	COORD c;
	c.X = 2 * x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void print_maze(vector<vector<bool>> &maze)
{
	for (size_t i = 1; i < maze.size() - 1; ++i)
	{
		for (size_t j = 1; j < maze[i].size() - 1; ++j)
		{
			cout << maze[i][j] << " ";
		}
		cout << endl;
	}
}


bool find_path(vector<vector<bool>> &maze, stack<pos> &path)
{
	vector<pos> offset(4);
	offset[0].row = 0, offset[0].col = 1;	// ср
	offset[1].row = 1, offset[1].col = 0;	// об
	offset[2].row = 0, offset[2].col = -1;	// вС
	offset[3].row = -1, offset[3].col = 0;	// ио

	pos here = { 1, 1 };
	maze[1][1] = true;
	int option = 0;
	int last_option = 3;
	int m = maze.size() - 2;
	int n = maze[0].size() - 2;
	while (here.row != m || here.col != n) 
	{
		int r, c;
		while (option <= last_option)
		{
			r = here.row + offset[option].row;
			c = here.col + offset[option].col;
			if (!maze[r][c])
			{
				break;
			}
			++option;
		}
		if (option <= last_option)
		{
			path.push(here);
			here.row = r;
			here.col = c;
			maze[r][c] = true;
			option = 0;
		}
		else
		{
			if (path.empty())
			{
				return false;
			}
			pos next = path.top();
			path.pop();
			if (next.row == here.row)
			{
				option = 2 + next.col - here.col;
			}
			else
			{
				option = 3 - next.row - here.row;
			}
			here = next;
		}
	}
	return true;

}

int main(void)
{
	fstream cin("data.in");
	int m, n;
	cin >> m >> n;
	vector<vector<bool>> maze(m + 2);
	maze[0].resize(n + 2);
	maze[m + 1].resize(n + 2);
	for (int i = 1; i <= m; ++i)
	{
		maze[i].resize(n + 2);
		for (int j = 1; j <= n; ++j)
		{
			bool t;
			cin >> t;
			maze[i][j] = t;
		}	
	}
	
	for (int i = 0; i < m + 2; ++i)
	{
		maze[i][0] = maze[i][n + 1] = true;
	}
	for (int i = 0; i < n + 2; ++i)
	{
		maze[0][i] = maze[m + 1][i] = true;
	}
	print_maze(maze);
	cout << endl;
	stack<pos> result;
	if (find_path(maze, result))
	{
		stack<pos> path;
		while (!result.empty())
		{
			path.push(result.top());
			result.pop();
		}
		CONSOLE_SCREEN_BUFFER_INFO c;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
		while (!path.empty())
		{
		
			goto_xy(c.dwCursorPosition.X + path.top().col - 1 ,c.dwCursorPosition.Y + path.top().row - 1);
			cout << "1";
			path.pop();
		}
	}
	else
	{
		cout << "Can not find path!" << endl;
	}
	cout << endl;
	return 0;
}