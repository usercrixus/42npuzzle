#pragma once
#include "Node.hpp"

template <typename T>
class Tree
{
private:
	Node<T>* head;

public:
	Tree(const T& rootData);
	~Tree();

	Node<T>* getRoot();
	Node<T>* searchNode(const T& data);
};

template <typename T>
Tree<T>::Tree(const T& rootData)
{
	head = new Node<T>(rootData, nullptr);
}


template <typename T>
Tree<T>::~Tree()
{
	delete head;
}

template <typename T>
Node<T>* Tree<T>::getRoot()
{
	return head;
}

template <typename T>
Node<T>* Tree<T>::searchNode(const T& data)
{
	if (!head)
		return nullptr;
	return head->searchNode(data);
}
