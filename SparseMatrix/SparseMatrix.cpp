#include "SparseMatrix.h"


std::istream& operator>>(std::istream &in, SparseMatrix &a)
{
	in >> a.rows >> a.cols;
	for (size_t i = 0; i < a.rows; ++i)
	{
		for (size_t j = 0; j < a.cols; ++j)
		{
			int temp;
			in >> temp;
			if (temp != 0)
			{	
				a.terms.push_back({i, j, temp});
			}
		}
	}
	return in;
}

std::ostream& operator<<(std::ostream &out, SparseMatrix &a)
{
	/*for (auto it = a.terms.cbegin(); it != a.terms.cend(); ++it)
	{
		out << it->row << " " << it->col << " " << it->value << std::endl;
	}*/
	auto it = a.terms.cbegin();
	for (size_t i = 0; i < a.rows; ++i)
	{
		for (size_t j = 0; j < a.cols; ++j)
		{
			if (it != a.terms.cend() && i == it->row && j == it->col)
			{
				out << std::setw(3) << it->value << " ";
				++it;
			}
			else
			{
				out << std::setw(3) << 0 <<  " ";
			}
		}
		out << std::endl << std::endl;
	}
	return out;
}

SparseMatrix::SparseMatrix()
{
}


SparseMatrix::~SparseMatrix()
{
}

//将原矩阵直接转置 然后按照转置后的序列按照 行优先进行排序
void SparseMatrix::transpose1(SparseMatrix & b)
{
	b.rows = cols;
	b.cols = rows;
	b.terms.clear();
	b.terms.resize(terms.size());
	for(std::vector<MatrixTerm>::size_type i = 0; i < terms.size(); ++i)
	{
		b.terms[i].row = terms[i].col;
		b.terms[i].col = terms[i].row;
		b.terms[i].value = terms[i].value;
	}
	std::sort(b.terms.begin(), b.terms.end());
}


// 清大数据结构 稀疏矩阵的转置方法2
void SparseMatrix::transpose2(SparseMatrix &b)
{
	b.rows = cols;
	b.cols = rows;
	b.terms.clear();
	for (size_t i = 0; i < cols; ++i)
	{
		for_each(terms.cbegin(), terms.cend(), [i, &b](const MatrixTerm &a) {
			if (a.col == i)
			{
				b.terms.push_back({ a.col, a.row, a.value });
			}
		});
	}
}

// 快速转置方法
void SparseMatrix::transpose3(SparseMatrix & b)
{
	b.rows = cols;
	b.cols = rows;
	b.terms.clear();
	b.terms.resize(terms.size());
	// 存放原矩阵各列非0元素个数 也就是转置后各行非0元素个数
	std::vector<size_t> row_size(cols, 0);
	for_each(terms.cbegin(), terms.cend(), [&row_size](const MatrixTerm &a) { ++row_size[a.col]; });

	//存放事先计算出来的稀疏矩阵各行非零元素在转置矩阵的三元组表中应当存放的位置
	std::vector<size_t> row_start(cols, 0);
	row_start[0] = 0;
	for (size_t i = 1; i < cols; ++i)
	{
		// 上一行开始位置 上一行元素个数
		row_start[i] = row_start[i - 1] + row_size[i - 1];
	}
	for (size_t i = 0; i < terms.size(); ++i)
	{
		b.terms[row_start[terms[i].col]] = { terms[i].col, terms[i].row, terms[i].value };
		++row_start[terms[i].col];
	}
}

void SparseMatrix::add(SparseMatrix & b, SparseMatrix & c)
{
	c.rows = rows;
	c.cols = cols;
	c.terms.clear();

	auto ia = terms.cbegin();
	auto ib = b.terms.cbegin();

	while (ia != terms.cend() && ib != b.terms.cend())
	{
		size_t index_a = ia->row * cols + ia->col;
		size_t index_b = ib->row * cols + ib->col;

		if (index_a < index_b)
		{
			c.terms.push_back(*ia++);
		}
		else if (index_b < index_a)
		{
			c.terms.push_back(*ib++);
		}
		else
		{
			if (ia->value + ib->value != 0)
			{
				c.terms.push_back({ ia->row, ia->col, ia->value + ib->value });
				++ia, ++ib;
			}
		}
	}
	while (ia != terms.cend())
	{
		c.terms.push_back(*ia++);
	}
	while (ib != b.terms.cend())
	{
		c.terms.push_back(*ib++);
	}
}

// 快速乘法
void SparseMatrix::mul(SparseMatrix & b, SparseMatrix & c)
{
	c.rows = rows;
	c.cols = b.cols;
	c.terms.clear();
	// row_size 是每一行非0元素的个数 row_start 是每一行元素的开始
	std::vector<size_t> row_size(b.rows, 0), row_start(b.rows + 1, 0);
	for_each(b.terms.cbegin(), b.terms.cend(), [&row_size](const MatrixTerm &a) { ++row_size[a.row]; });
	row_start[0] = 0;
	for (size_t i = 1; i < b.rows; ++i)
	{
		row_start[i] = row_start[i - 1] + row_size[i - 1];
	}
	// 要多一个  因为后面的循环会用到
	row_start[b.rows] = b.terms.size();

	std::vector<int> temp(b.cols, 0);
	for (size_t i = 0; i < terms.size();)
	{
		// 记录左边矩阵当前正在计算的行
		size_t row_a = terms[i].row;
		// 当个数未越界  且 还在当前行的时候
		while (i < terms.size() && terms[i].row == row_a)
		{	
			// 对于a中与当前正在进行元素的列号相同的行号的b中元素进行乘法 并累加
			for (size_t j = row_start[terms[i].col]; j < row_start[terms[i].col + 1]; ++j)
			{
				temp[b.terms[j].col] += terms[i].value * b.terms[j].value;
			}
			++i;
		}
		// 说明当前行已经结束了 b矩阵的所有列也都乘过了 c矩阵的该行所有元素均已行成
		for (size_t k = 0; k < b.cols; ++k)
		{
			c.terms.push_back({ row_a, k, temp[k] });
			temp[k] = 0;
		}
	}

}
