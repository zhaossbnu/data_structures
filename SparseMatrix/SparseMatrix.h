#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
struct MatrixTerm
{
	size_t row, col;
	int value;
	bool operator<(MatrixTerm &b)
	{
		if (row < b.row)
		{
			return true;
		}
		else if (row == b.row)
		{
			if (col < b.col)
			{
				return true;
			}
		}
		return false;
	}
};

class SparseMatrix
{
	friend std::istream& operator>>(std::istream &in, SparseMatrix &a);
	friend std::ostream& operator<<(std::ostream &out, SparseMatrix &a);
public:
	SparseMatrix();
	~SparseMatrix();
	void transpose1(SparseMatrix &b);
	void transpose2(SparseMatrix &b);
	void transpose3(SparseMatrix &b);
	void add(SparseMatrix &b, SparseMatrix &c);
	void mul(SparseMatrix &b, SparseMatrix &c);
private:
	size_t rows = 0, cols = 0;
	std::vector<MatrixTerm> terms;
};
