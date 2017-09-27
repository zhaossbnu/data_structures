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

//��ԭ����ֱ��ת�� Ȼ����ת�ú�����а��� �����Ƚ�������
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


// ������ݽṹ ϡ������ת�÷���2
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

// ����ת�÷���
void SparseMatrix::transpose3(SparseMatrix & b)
{
	b.rows = cols;
	b.cols = rows;
	b.terms.clear();
	b.terms.resize(terms.size());
	// ���ԭ������з�0Ԫ�ظ��� Ҳ����ת�ú���з�0Ԫ�ظ���
	std::vector<size_t> row_size(cols, 0);
	for_each(terms.cbegin(), terms.cend(), [&row_size](const MatrixTerm &a) { ++row_size[a.col]; });

	//������ȼ��������ϡ�������з���Ԫ����ת�þ������Ԫ�����Ӧ����ŵ�λ��
	std::vector<size_t> row_start(cols, 0);
	row_start[0] = 0;
	for (size_t i = 1; i < cols; ++i)
	{
		// ��һ�п�ʼλ�� ��һ��Ԫ�ظ���
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

// ���ٳ˷�
void SparseMatrix::mul(SparseMatrix & b, SparseMatrix & c)
{
	c.rows = rows;
	c.cols = b.cols;
	c.terms.clear();
	// row_size ��ÿһ�з�0Ԫ�صĸ��� row_start ��ÿһ��Ԫ�صĿ�ʼ
	std::vector<size_t> row_size(b.rows, 0), row_start(b.rows + 1, 0);
	for_each(b.terms.cbegin(), b.terms.cend(), [&row_size](const MatrixTerm &a) { ++row_size[a.row]; });
	row_start[0] = 0;
	for (size_t i = 1; i < b.rows; ++i)
	{
		row_start[i] = row_start[i - 1] + row_size[i - 1];
	}
	// Ҫ��һ��  ��Ϊ�����ѭ�����õ�
	row_start[b.rows] = b.terms.size();

	std::vector<int> temp(b.cols, 0);
	for (size_t i = 0; i < terms.size();)
	{
		// ��¼��߾���ǰ���ڼ������
		size_t row_a = terms[i].row;
		// ������δԽ��  �� ���ڵ�ǰ�е�ʱ��
		while (i < terms.size() && terms[i].row == row_a)
		{	
			// ����a���뵱ǰ���ڽ���Ԫ�ص��к���ͬ���кŵ�b��Ԫ�ؽ��г˷� ���ۼ�
			for (size_t j = row_start[terms[i].col]; j < row_start[terms[i].col + 1]; ++j)
			{
				temp[b.terms[j].col] += terms[i].value * b.terms[j].value;
			}
			++i;
		}
		// ˵����ǰ���Ѿ������� b�����������Ҳ���˹��� c����ĸ�������Ԫ�ؾ����г�
		for (size_t k = 0; k < b.cols; ++k)
		{
			c.terms.push_back({ row_a, k, temp[k] });
			temp[k] = 0;
		}
	}

}
