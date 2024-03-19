#pragma once

#include "treeNode.h"

namespace graph
{
	template<typename T, size_t Size>
	TreeNode<T, Size>::TreeNode(const std::initializer_list<T>& init)
	{
		std::copy(std::begin(init), std::end(init), std::begin(keys));
	}

	template<typename T, size_t Size>
	TreeNode<T, Size>* TreeNode<T, Size>::addChild()
	{
		children.push_back(std::make_unique<TreeNode<T, Size>>());
		return children.back().get();
	}

	template<typename T, size_t Size>
	T TreeNode<T, Size>::getKey(int position)
	{
		return keys.at(position);
	}

	template<typename T, size_t Size>
	void TreeNode<T, Size>::setKey(int position, T key)
	{
		keys.at(position) = key;
	}

	template<typename T, size_t Size>
	bool TreeNode<T, Size>::isLeaf()
	{
		return children.size() == 0;
	}
}
