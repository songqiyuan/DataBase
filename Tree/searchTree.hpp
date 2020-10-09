#ifndef _SEARCHTREE_H_
#define _SEARCHTREE_H_

#include<iostream>
#include<vector>
#include<queue>
#include<stack>
using namespace std;
struct TreeNode
{
	int val;
	TreeNode* right;
	TreeNode* left;
	TreeNode* parent;
	TreeNode(int x=0) :val(x), right(nullptr), left(nullptr),parent(nullptr)
	{}
};
class CSearchTree
{
public:
	//后继节点
	TreeNode* increment(TreeNode* node)
	{
		if (node == nullptr) return nullptr;
		if (node->right != nullptr)
		{
			node = node->right;
			while (node->left != nullptr)
			{
				node = node->left;
			}
			return node;
		}
		else
		{
			TreeNode* y = node->parent;
			while (node == y->right)
			{
				node = y;
				y = y->parent;
			}
			if (y != nullptr) //若y==nullptr,则说明已经回溯到了根节点，因此无后继节点
			{
				return y;
			}
		}
		return nullptr;
	}
	//前驱节点
	TreeNode* decrement(TreeNode* node)
	{
		if (node == nullptr) return nullptr;
		if (node->left != nullptr)
		{
			node = node->left;
			while (node->right != nullptr)
			{
				node = node->right;
			}
			return node;
		}
		else
		{
			TreeNode* y = node->parent;
			while (node == y->left)
			{
				node = y;
				y = y->parent;
			}
			if (y != nullptr) //若y==nullptr,则说明已经回溯到了根节点，因此无前驱节点
			{
				return y;
			}
		}
		return nullptr;
	}

	//左旋
	void RotateLeft( TreeNode* node)
	{
		TreeNode* y = node->right;
		node->right = y->left;
		if (y->left != nullptr)
		{
			y->left->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == nullptr)
		{
			_root = y;
		}
		else if (node == node->parent->left)
		{
			node->parent->left = y;
		}
		else if (node == node->parent->right)
		{
			node->parent->right = y;
		}
		y->left = node;
		node->parent = y;
	}
	//右旋
	void RotateRight( TreeNode* node)
	{
		TreeNode* y = node->left;
		node->left = y->right;
		if (y->right != nullptr)
		{
			y->right->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == nullptr)
		{
			_root = y;
		}
		else if (node == node->parent->left)
		{
			node->parent->left = y;
		}
		else if (node == node->parent->right)
		{
			node->parent->right = y;
		}
		y->right = node;
		node->parent = y;

	}
	//子树替换
	void TransPlant(TreeNode* &child1,TreeNode* &child2)
	{
		if (child1->parent == nullptr)
		{
			_root = child2;
		}
		else if (child1 == child1->parent->left)
		{
			child1->parent->left = child2;
			//child2->parent = child1->parent;
		}
		else
		{
			child1->parent->right = child2;
			//child2->parent = child1->parent;
		}

		if (child2 != nullptr)
		{
			child2->parent = child1->parent;
		}
		//resetNode(child1);
	}
	void deleteNode(int key)
	{
		_deleteNode( key);
	}
	void  _deleteNode(int y)
	{
		TreeNode* node = findKey(y);
		if (node == nullptr) return ;
	
		if (node->right == nullptr)
		{
			TransPlant( node, node->left);
		}
		else if (node->left == nullptr)
		{
			TransPlant( node, node->right);
		}
		else
		{
			//查找后继节点
			TreeNode* tmp=increment(node);
			if (tmp->parent != node)// && tmp->right!=nullptr)
			{
				TransPlant(tmp, tmp->right);
				tmp->right = node->right;
				tmp->right->parent = tmp;
			}
			TransPlant(node, tmp);
			tmp->left = node->left;
			tmp->left->parent = tmp;
		}
		resetNode(node);
	}
	void resetNode(TreeNode* &node)
	{
		
		node->left = nullptr;
		node->right = nullptr;
		node->parent = nullptr;
		delete node;
	}
	TreeNode* findKey( int y)
	{
		TreeNode* node = _root;
		while (node != nullptr && node->val != y)
		{
			if (node->val > y) node = node->left;
			else if (node->val < y) node = node->right;
		}
		return node;
	}
	//最小值
	TreeNode* MinVal()
	{
		MinImum(_root);
	}
	//最大值
	TreeNode* MaxVal()
	{
		MaxImum(_root);
	}
	//当前节点最大关键字节点
	TreeNode* MaxImum(TreeNode* &node)
	{
		while (node->left != nullptr)
		{
			node = node->left;
		}
		return node;
	}
	//当前节点最小关键字节点
	TreeNode* MinImum(TreeNode* &node)
	{
		while (node->right != nullptr)
		{
			node = node->right;
		}
		return node;
	}
	//插入节点
	void Insert(int key)
	{
		_Insert( key);
	}
	void _Insert( int key)
	{
		TreeNode* node = _root;
		TreeNode* pNode = nullptr;

		while (node != nullptr)
		{
			pNode = node;
			if (key > node->val)
			{
				node = node->right;
			}
			else
			{
				node = node->left;
			}
		}
		TreeNode* pInsertNode = new TreeNode(key);
		//根节点为空，此时无数据
		if (pNode == nullptr)
		{
			
			_root = pInsertNode;
		}
		else if(pNode->val < key)
		{
			pNode->right=pInsertNode;
			pInsertNode->parent = pNode;
		}
		else
		{
			pNode->left = pInsertNode;
			pInsertNode->parent = pNode;
		}
	}
	//替换节点

public:
	CSearchTree() :_root(nullptr) {}
	~CSearchTree()
	{
		freeNode();
	}
	void freeNode()
	{
		queue<TreeNode*> q;
		q.push(_root);
		while (!q.empty())
		{
			TreeNode* p = q.front();
			q.pop();
			if (p->left != nullptr) q.push(p->left);
			if (p->right != nullptr) q.push(p->right);
			resetNode(p);
		}
	}
	void inorderRBTeee()
	{
		//中序遍历
		TreeNode* p = _root;
		stack<TreeNode*> s;
		while (!s.empty() || p != nullptr)
		{
			while (p != nullptr)
			{
				s.push(p);
				p = p->left;
			}
			TreeNode* t = s.top();
			result.push_back(t);
			s.pop();
			if (t->right != nullptr)
			{
				p = t->right;
			}
		}
	}
	void printRBTree()
	{
		inorderRBTeee();
		for (int i = 0;i < result.size();i++)
		{
			cout << result[i]->val << endl;
		}
	}
private:
	TreeNode* _root;
	std::vector<TreeNode*> result;
};
#endif // ! _SEARCHTREE_H_

#pragma once

