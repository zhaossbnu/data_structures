#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
using namespace std;

struct pos
{
    int row, col;
};

void print_pixel(vector<vector<int>> &a)
{
    for(int i = 1; i < a.size() - 1; ++i)
    {
        for(int j = 1; j < a[0].size() - 1; ++j)
        {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int main(void)
{
    ifstream cin("data.in");
    int m, n;
    cin >> m >> n;
    vector<vector<int>> pixel(m + 2);
    pixel[0].resize(n + 2);
    pixel[m + 1].resize(n + 2);
    for(int i = 1; i <= m; ++i)
    {
        pixel[i].resize(n + 2);
        pixel[i][0] = pixel[i][n + 1] = 0;
        for(int j = 1; j <= n; ++j)
        {
            cin >> pixel[i][j];
        }
    }
    print_pixel(pixel);

    vector<pos> offset(4);
    offset[0].row = 0, offset[0].col = 1;
    offset[1].row = 1, offset[1].col = 0;
    offset[2].row = 0, offset[2].col = -1;
    offset[3].row = -1, offset[3].col = 0;

    int flag = 1;
    int num_of_nbr = 4;
    deque<pos> q;
    pos here, nbr;
    for(int i = 1; i < pixel.size() - 1; ++i)
    {
        for(int j = 1; j < pixel[0].size() - 1; ++j)
        {
            if(pixel[i][j] == 1)
            {
                pixel[i][j] = ++flag;
                here.row = i;
                here.col = j;
                for(;;)
                {
                    for(int k = 0; k < num_of_nbr; ++k)
                    {
                        nbr.row = here.row + offset[k].row;
                        nbr.col = here.col + offset[k].col;
                        if(pixel[nbr.row][nbr.col] == 1)
                        {
                            pixel[nbr.row][nbr.col] = flag;
                            q.push_back(nbr);
                        }
                    }
                    if(q.empty())
                    {
                        break;
                    }
                    here = q.front();
                    q.pop_front();
                }
            }
        }
    }
    print_pixel(pixel);
    return 0;
}
