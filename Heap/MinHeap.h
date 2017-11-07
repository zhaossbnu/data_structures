#pragma once
#include <vector>
#include <iostream>
template<typename T>
class MinHeap
{
public:
	MinHeap()
	{
		// 为了使用完全二叉树的特点 序号都从1开始
		heap.resize(1);
	}
	MinHeap(const std::vector<T> &data);
	bool empty();
	int size();
	void push(T &a);
	const T& top();
	void pop();
	
private:
	std::vector<T> heap;

};

template<typename T>
inline MinHeap<T>::MinHeap(const std::vector<T> &data)
{
	heap.resize(data.size() + 1);
	for (size_t i = 0; i < data.size(); ++i)
	{
		heap[i + 1] = data[i];
	}
	for (int root = size() / 2; root >= 1; --root)
	{
		T root_element = heap[root];

		int child = root * 2;
		while (child <= size())
		{
			// 找到最大的孩子
			if (child < size() && heap[child] > heap[child + 1])
			{
				++child;
			}
			// 父亲比孩子小 结束
			if (root_element <= heap[child])
			{
				break;
			}
			// 否则 就把最小的孩子上移
			heap[child / 2] = heap[child];
			child *= 2;
		}
		heap[child / 2] = root_element;
	}
}

template<typename T>
inline bool MinHeap<T>::empty()
{
	return heap.size() == 1;
}

template<typename T>
inline int MinHeap<T>::size()
{
	return heap.size() - 1;
}

template<typename T>
inline void MinHeap<T>::push(T & a)
{
	heap.push_back(a);
	int current = size();
	while (current != 1 && heap[current / 2] > a)
	{
		heap[current] = heap[current / 2];
		current /= 2;
	}
	heap[current] = a;
}

template<typename T>
inline const T& MinHeap<T>::top()
{
	return heap[1];
}

template<typename T>
inline void MinHeap<T>::pop()
{
	if (empty())
	{
		std::cerr << "堆为空" << endl;
		return;
	}
	T temp_root = heap[size()];
	int current = 1, child = 2;
	heap.pop_back();
	if (!empty())
	{
		while (child <= size())
		{
			if (child < size() && heap[child] > heap[child + 1])
			{
				++child;
			}
			if (temp_root <= heap[child])
			{
				break;
			}
			heap[current] = heap[child];
			current = child;
			child *= 2;
		}
		// 只有当堆不为空时才可以设置根
		heap[current] = temp_root;
	}
	
}

