#pragma once
#include <vector>

template <typename T>
class Node
{
private:
	T data;
	std::vector<Node<T> *> children;
	Node<T> *parent;

public:
	Node(T data);
	Node(T data, Node<T> *parent = nullptr);
	~Node();

	void createNode(const T &data);
	Node<T> *searchNode(const T &data);
	Node<T> *getParent() const;
	T getData() const;
};

template <typename T>
Node<T>::Node(T data) : data(data)
{
}

template <typename T>
Node<T>::Node(T data, Node<T> *parent) : data(data), parent(parent)
{
}

template <typename T>
Node<T>::~Node()
{
	for (Node<T> *child : children)
		delete child;
}

template <typename T>
void Node<T>::createNode(const T &data)
{
	children.push_back(new Node<T>(data, this));
}

template <typename T>
T Node<T>::getData() const
{
	return (data);
}

template <typename T>
Node<T> *Node<T>::searchNode(const T &data)
{
	if (this->data == data)
		return this;

	for (Node<T> *child : children)
	{
		Node<T> *result = child->searchNode(data);
		if (result != nullptr)
			return result;
	}
	return nullptr;
}
template <typename T>
Node<T>* Node<T>::getParent() const
{
	return parent;
}
