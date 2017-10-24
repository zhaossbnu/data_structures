#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

struct pos
{
	int row;
	int col;
	friend istream& operator>>(istream &in, pos &a)
	{
		in >> a.row >> a.col;
		return in;
	}
};

void print_grid(const vector<vector<int>> &a)
{
	for (size_t i = 1; i < a.size() - 1; ++i)
	{
		for (size_t j = 1; j < a[0].size() - 1; ++j)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


int main(void)
{
	ifstream cin("data.in");
	int m, n;
	cin >> m >> n;
	vector<vector<int>> grid(m + 2);
	grid[0].resize(n + 2);
	grid[m + 1].resize(n + 2);
	for (int i = 0; i < m + 2; ++i)
	{
		grid[0][i] = grid[m + 1][i] = 1;
	}
	for (int i = 1; i <= n; ++i)
	{
		grid[i].resize(n + 2);
		grid[i][0] = grid[i][n + 1] = 1;
		for (int j = 1; j <= n; ++j)
		{
			cin >> grid[i][j];
		}
	}
	print_grid(grid);

	pos start, finish;
	cin >> start >> finish;

	if ((start.row == finish.row) && (start.col == finish.col))
	{
		cout << "起点与终点相同！" << endl;
		return 0;
	}

	vector<pos> offset(4);
	offset[0].row = 0, offset[0].col = 1;	// 右
	offset[1].row = 1, offset[1].col = 0;	// 下
	offset[2].row = 0, offset[2].col = -1;	// 左
	offset[3].row = -1, offset[3].col = 0;	// 上

	pos here = start;
	grid[here.row][here.col] = 2;
	int num_of_nbr = 4;

	deque<pos> q;
	pos nbr;
	for (;;)
	{
		for (int i = 0; i < num_of_nbr; ++i)
		{
			nbr.row = here.row + offset[i].row;
			nbr.col = here.col + offset[i].col;
			if (grid[nbr.row][nbr.col] == 0)
			{
				grid[nbr.row][nbr.col] = grid[here.row][here.col] + 1;
				if ((nbr.row == finish.row) && (nbr.col == finish.col))
				{
					break;
				}
				q.push_back(nbr);
			}
		}
		if ((nbr.row == finish.row) && (nbr.col == finish.col))
		{
			break;
		}
		
		if (q.empty())
		{
			cout << "找不到路径！" << endl;
			return 0;
		}
		here = q.front();
		q.pop_front();
	}
	
	int path_len = grid[finish.row][finish.col] - 2;
	vector<pos> path(path_len);
	here = finish;
	for (int j = path_len - 1; j >= 0; --j)
	{
		path[j] = here;
		for (int i = 0; i < num_of_nbr; ++i)
		{
			nbr.row = here.row + offset[i].row;
			nbr.col = here.col + offset[i].col;
			if (grid[nbr.row][nbr.col] == j + 2)
			{
				break;
			}
		}
		here = nbr;
	}
	for (size_t i = 0; i < path.size(); ++i)
	{
		cout << path[i].row << " " << path[i].col << endl;
	}
	return 0;
}