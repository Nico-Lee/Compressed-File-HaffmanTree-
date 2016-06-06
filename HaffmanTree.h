#pragma once
#include<iostream>
#include"Heap.h"
#include"FileComparess.h"

using namespace std;


template<class T>
struct HaffmanNode
{
	HaffmanNode<T>* _left;
	HaffmanNode<T>* _right;
	T _wight;
	HaffmanNode(const T& wight)
		:_left(NULL)
		, _right(NULL)
		, _wight(wight)
	{}
};


template<class T>
class HaffmanTree
{
public:
	typedef HaffmanNode<T> Node;
	HaffmanTree(const T* a, size_t size, const T& invalid)
	{
		_root = _CreatHaffmanTree(a, size, invalid);
	}
	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _CreatHaffmanTree(const T* a,size_t size, const T& invalid)
	{
		Heap<Node*, Less<Node*>> minHeap;
		for (size_t i = 0; i < size; ++i)
		{
			if (a[i] != invalid)
			{
				Node* tmp = new Node(a[i]);
				minHeap.Push(tmp);
			}
		}
		while (!minHeap.Empty())
		{
			Node* left = minHeap.GetTop();
			minHeap.Pop();
			Node* right = NULL;
			if (!minHeap.Empty())
			{
				right = minHeap.GetTop();
				minHeap.Pop();
			}
			Node* parent = NULL;
			if (right)
			{
				parent = new Node(left->_wight + right->_wight);
			}
			else
			{
				parent = new Node(left->_wight);
			}
			parent->_left = left;
			parent->_right = right;
			if (minHeap.Empty())
			{
				return parent;
			}
			minHeap.Push(parent);
		}
		return NULL;
	}
protected:
	Node* _root;
};

