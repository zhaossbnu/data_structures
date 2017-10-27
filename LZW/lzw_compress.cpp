#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <iomanip>
using namespace std;

ifstream in;
ofstream out;
const unsigned int MASK1 = 255;
const unsigned int MASK2 = 15;
const unsigned int EXCESS = 4;
const unsigned int BYTE_SIZE = 8;
const unsigned int MAX_CODES = 4096;

bool bits_left = false;
int left_over;

void set_files(int argc, char* argv[])
{
	string input_file, output_file;
	if (argc >= 2)
	{
		input_file = argv[1];
	}
	else
	{
		cerr << "Enter file name!" << endl;
		cin >> input_file;
	}
	in.open(input_file);
	if (in.fail())
	{
		cerr << "Cannot open file " << input_file << endl;
		exit(1);
	}
	// ��ȡ�ļ���ǰ׺
	output_file = input_file.substr(0, input_file.find('.')) + ".zzz"; 
	out.open(output_file, ios::binary);
}

void output(long pcode)
{
	int c, d;
	// �����һ����ʣ��λ���Ļ�
	if (bits_left)
	{
		// ��ȡ��ǰpcode�ĺ��λ
		d = (int)(pcode & MASK1);
		// �ϴ�ʣ���������λ��λ��ƴ�յ�����8λ
		// ƴ����һ�� 12 λ
		c = (int)((left_over << EXCESS) | (pcode >> BYTE_SIZE));
		// Ҫ�����
		out.put(c);
		out.put(d);
		bits_left = false;
	}
	else
	{
		//�Ȱѱ���ʣ�����λȡ����
		left_over = pcode & MASK2;
		// ��ȡ��12
		c = (int)(pcode >> EXCESS);
		out.put(c);
		bits_left = true;
	}
}

void compress()
{
	map<const long, int> h;
	//  ����չASCII���ȷ���ӳ�����
	for (int i = 0; i < 256; ++i)
	{
		h.insert({ i, i });
	}
	// ��ʶ��ǰ�����ֵ
	int code_used = 256;
	int c = in.get();
	if (c != EOF)
	{
		long pcode = c;
		while ((c = in.get()) != EOF)
		{
			long key = (pcode << BYTE_SIZE) + c;
			// Ѱ�ҵ�ǰ���ڲ��ڱ���
			map<const long, int>::iterator the_pair = h.find(key);
			// ��������� ˵����ǰ����������
			if (the_pair == h.end())
			{
				// �����ǰcode
				output(pcode);
				// ����ǰ��������ֵ���ڱ���
				if (code_used < MAX_CODES)
				{
					h.insert({ key, code_used++ });
				}
				pcode = c;
			}
			else // ������� �򽫵�ǰpcode����Ϊ ��ǰ������ֵ Ϊ�´����ø�������׼��
			{
				pcode = the_pair->second;
			}
		}
		output(pcode);
		if (bits_left)
		{
			out.put(left_over << EXCESS);
		}
	}
	out.close();
	in.close();
}
int main(int argc, char* argv[])
{
	set_files(argc, argv);
	compress();
	return 0;
}