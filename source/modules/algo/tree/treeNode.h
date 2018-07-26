#pragma once
#include <vector>
#include <memory>
#include <functional>

namespace graph
{
	template<typename T, size_t Size>
	class TreeNode
	{
	protected:
		std::array<T, Size> keys;
		std::vector<std::unique_ptr<TreeNode<T, Size>>> children;

	public:
		TreeNode() = default;
		TreeNode(const std::initializer_list<T>& init);
		virtual ~TreeNode() = default;

		TreeNode<T, Size>* addChild();

		virtual T foldDepthFirst(const T& accumulator) = 0;

		T getKey(int position);
		void setKey(int position, T key);

		bool isLeaf();
	};
}

#include "treeNode.tpp"
