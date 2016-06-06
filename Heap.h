#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<vector>
#include<assert.h>
//#include"HaffmanTree.h"
using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};


template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

template<class T>
struct Less<T*>
{
	bool operator()(const T*Nodel, const T*Noder)
	{
		return Nodel->_wight < Noder->_wight;
	}
};

template<class T,class Continer = Less<T>>//默认为小堆
class Heap
{
public:
	Heap(){};
	Heap(const T* a, size_t size,const T& invalid);
	Heap(vector<T> a);
	Heap(const vector<T>& v);
	void Push(const T& x);
	void Pop();
	T& GetTop();
	bool Empty();
	size_t Size();
	void HeapSort(T* a, size_t size);
protected:
	void _AdjustDown(size_t parent);
	void _AdjustUp(int child);
protected:
	vector<T> _a;
};

template<class T, class Continer = Less<T>>
Heap<T, Continer>::Heap(const T* a, size_t size,const T& invalid)
{
	_a.reserve(size);

	for (size_t i = 0; i < size; ++i)
	{
		if (a[i] != invalid)
		{
			_a.push_back(a[i]);
		}
	}

	//建堆
	for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		//从第一个非叶子结点开始下调，叶子结点可以看作是一个大堆或小堆
	{

		_AdjustDown(i);
	}
}
template<class T, class Continer = Less<T>>
Heap<T, Continer>::Heap(vector<T> a)
{
	_a.swap(a);

	// 建堆
	for (int i = (_a.size() - 2) / 2; i >= 0; --i)
	{
		_AdjustDown(i);
	}
}
template<class T, class Continer = Less<T>>
Heap<T, Continer>::Heap(const vector<T>& v)
	:_a(v)
{
	//_a.resize(v.size());
}
template<class T, class Continer = Less<T>>
void Heap<T, Continer>::Push(const T& x)
{
	_a.push_back(x);
	_AdjustUp(_a.size() - 1);
}
template<class T, class Continer = Less<T>>
void Heap<T, Continer>::Pop()
{
	assert(!_a.empty());
	size_t size = _a.size();
	swap(_a[0], _a[size - 1]);
	_a.pop_back();
	_AdjustDown(0);
}
template<class T, class Continer = Less<T>>
T& Heap<T, Continer>::GetTop()
{
	return _a[0];
}
template<class T, class Continer = Less<T>>
bool Heap<T, Continer>::Empty()
{
	return _a.empty();
}
template<class T, class Continer = Less<T>>
size_t Heap<T, Continer>::Size()
{
	return _a.size();
}

template<class T, class Continer = Less<T>>
void Heap<T, Continer>::_AdjustDown(size_t parent)
{
	Continer _con;
	size_t child = parent * 2 + 1;
	size_t size = _a.size();
	while (child < size)
	{
		if (child + 1 < size&&_con(_a[child + 1], _a[child]))
			//注意这必须是child+1更大或更小，所以把child+1放在前面
			++child;
		if (/*_a[parent] < _a[child]*/_con(_a[child], _a[parent]))
		{
			swap(_a[parent], _a[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
			break;
	}
}


template<class T, class Continer = Less<T>>
void Heap<T, Continer>::_AdjustUp(int child)
{
	Continer _con;
	int parent = (child - 1) / 2;
	while (child > 0)
	{
		if (_con(_a[child], _a[parent]))
		{
			swap(_a[child], _a[parent]);
			child = parent;
			parent = (child - 1) / 2;
		}
		else
			break;
	}
}


////堆排序
//template<class T>
//void AdjustDown(T* a, size_t size, size_t parent)
//{
//	size_t child = parent * 2 + 1;
//	while (child < size)
//	{
//		if (child + 1 < size&&a[child] < a[child + 1])
//			++child;
//		if (a[parent] < a[child])
//		{
//			swap(a[parent], a[child]);
//			parent = child;
//			child = parent * 2 + 1;
//		}
//		else
//			break;
//	}
//}
//
//template<class T>
//void Heap<T>::HeapSort(T* a, size_t size)
//{
//	//建堆
//	for (int i = (size - 2) / 2; i >= 0; --i)
//		//i不能定义为size_t除法会溢出，翻转成一个很大的数
//	{
//		AdjustDown(a, size, i);
//	}
//	for (size_t i = 0; i < size; ++i)
//	{
//		swap(a[0], a[size - 1 - i]);
//		AdjustDown(a, size - i - 1, 0);
//	}
//}

//
//void test()
//{
//	int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//	int size = sizeof(arr) / sizeof(arr[0]);
//	Heap<int> h(arr, size, -1);
//	int top = h.GetTop();
//	h.Pop();
//	h.Pop();
//	top = h.GetTop();
//	h.Push(1);
//	h.Push(0);
//}
