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
	// 获取文件名前缀
	output_file = input_file.substr(0, input_file.find('.')) + ".zzz"; 
	out.open(output_file, ios::binary);
}

void output(long pcode)
{
	int c, d;
	// 如果上一次有剩余位数的话
	if (bits_left)
	{
		// 先取当前pcode的后八位
		d = (int)(pcode & MASK1);
		// 上次剩余的左移四位按位或拼凑的右移8位
		// 拼成了一个 12 位
		c = (int)((left_over << EXCESS) | (pcode >> BYTE_SIZE));
		// 要先输出
		out.put(c);
		out.put(d);
		bits_left = false;
	}
	else
	{
		//先把本次剩余的四位取出来
		left_over = pcode & MASK2;
		// 获取高12
		c = (int)(pcode >> EXCESS);
		out.put(c);
		bits_left = true;
	}
}

void compress()
{
	map<const long, int> h;
	//  将扩展ASCII码先放入映射表中
	for (int i = 0; i < 256; ++i)
	{
		h.insert({ i, i });
	}
	// 标识当前最大码值
	int code_used = 256;
	int c = in.get();
	if (c != EOF)
	{
		long pcode = c;
		while ((c = in.get()) != EOF)
		{
			long key = (pcode << BYTE_SIZE) + c;
			// 寻找当前串在不在表中
			map<const long, int>::iterator the_pair = h.find(key);
			// 如果不存在 说明当前串不在里面
			if (the_pair == h.end())
			{
				// 输出当前code
				output(pcode);
				// 将当前串和其码值放在表中
				if (code_used < MAX_CODES)
				{
					h.insert({ key, code_used++ });
				}
				pcode = c;
			}
			else // 如果存在 则将当前pcode设置为 当前串的码值 为下次配置更长串做准备
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