#ifndef _RBTREE_H_
#define _RBTREE_H_

#include<iostream>
#include<vector>
#include<queue>
#include<stack>
using namespace std;
//节点定义
#define RED 0
#define BLACK 1
struct TreeNode
{
	int val;
	int color;
	TreeNode* right;
	TreeNode* left;
	TreeNode* parent;
	TreeNode(int x = 0) :color(RED), val(x), right(nullptr), left(nullptr), parent(nullptr)
	{}
};
class CRBTree
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
	void RotateLeft(TreeNode* node)
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
	void RotateRight(TreeNode* node)
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
	void TransPlant(TreeNode* &child1, TreeNode* &child2)
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
		_deleteNode(key);
	}
	void  _deleteNode(int y)
	{
		TreeNode* node = findKey(y);
		if (node == nullptr) return;
		//记录删除节点的颜色
		int DeleteNodeColor = node->color;
		//需要调整的节点
		TreeNode* FixNode = nullptr;
		if (node->right == nullptr)
		{
			FixNode = node->left;
			TransPlant(node, node->left);
		}
		else if (node->left == nullptr)
		{
			FixNode = node->right;
			TransPlant(node, node->right);
		}
		else
		{
			//查找后继节点
			TreeNode* tmp = increment(node);
			DeleteNodeColor = tmp->color;
			FixNode = tmp->right;
			if (tmp->parent == node)
			{
				FixNode->parent = tmp;
			}
			if (tmp->parent != node)// && tmp->right!=nullptr)
			{
				TransPlant(tmp, tmp->right);
				tmp->right = node->right;
				tmp->right->parent = tmp;
			}
			TransPlant(node, tmp);
			tmp->left = node->left;
			tmp->left->parent = tmp;
			tmp->color = node->color;
		}
		if (DeleteNodeColor == BLACK)
		{
			//处理平衡
			RBDeleteFixUp(FixNode);
		}
		resetNode(node);
	}
	void RBDeleteFixUp(TreeNode* node)
	{
		while (node != nullptr && node->color == BLACK && node != _root)
		{
			if (node == node->parent->left)
			{
				TreeNode* bnode = node->parent->right;
				//如果node为黑色节点，怎为了保持红黑树性质怎，其兄弟节点必定存在，即bnode不为空
				if (bnode->color == RED)
				{
					//此时父节点一定为黑色
					bnode->color = BLACK;
					node->parent->color = RED;
					RotateLeft(node->parent);
					bnode = node->parent->right;
				}
				if (bnode->left->color == BLACK && bnode->right->color == BLACK)
				{
					bnode->color = RED;
					node = node->parent;
				}
				else
				{
					if (bnode->right->color == BLACK)
					{
						//根据上一个条件判断，则此时左节点必为红色,父节点必为黑色
						bnode->left->color = BLACK;
						bnode->parent->color = RED;
						RotateRight(bnode);
						bnode = node->parent->right;
					}
					//如果兄弟节点的右节点为红色
					bnode->color = node->parent->color;
					node->parent->color = BLACK;
					bnode->right->color = BLACK;
					RotateLeft(node->parent);
					node = _root;//结束循环，达到平衡
				}
			}
			else if (node == node->parent->right)
			{
				TreeNode* bnode = node->parent->left;
				if (bnode->color == RED)
				{
					bnode->color = BLACK;
					node->parent->color = RED;
					RotateRight(node->parent);
					bnode = node->parent->left;
				}
				if (bnode->left->color == BLACK && bnode->right->color == BLACK)
				{
					bnode->color = RED;
					node = node->parent;
				}
				else
				{
					if (bnode->left->color == BLACK)
					{
						bnode->color = RED;
						bnode->right->color = BLACK;
						RotateLeft(bnode);
						bnode = node->parent->left;
					}
					bnode->color = node->parent->color;
					node->parent->color = BLACK;
					bnode->left->color = BLACK;
					RotateLeft(node->parent);
					node = _root;
				}
			}
		}
		if (node != nullptr)
		{
			node->color = BLACK;
		}
	}
	void resetNode(TreeNode* &node)
	{

		node->left = nullptr;
		node->right = nullptr;
		node->parent = nullptr;
		delete node;
	}
	TreeNode* findKey(int y)
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
		_Insert(key);
	}
	void _Insert(int key)
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
		else if (pNode->val < key)
		{
			pNode->right = pInsertNode;
			pInsertNode->parent = pNode;
		}
		else
		{
			pNode->left = pInsertNode;
			pInsertNode->parent = pNode;
		}
		RBInsertFixup(pInsertNode);
	}
	//修正插入红黑性质
	void RBInsertFixup(TreeNode* node)
	{
		//当前插入节点 ，其父节点为红色
		while (node->parent != nullptr && node->parent->color == RED)
		{
			if (node->parent->parent->left == node->parent)
			{
				TreeNode* uncle = node->parent->parent->right;
				if (uncle!=nullptr && uncle->color == RED)
				{
					//若父节点为红色或叔父节点为红色，则祖父节点一定为黑色
					node->parent->color = BLACK;
					uncle->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				}
				else
				{
					if (node->parent->right == node)
					{
						node = node->parent;
						RotateLeft(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					RotateRight(node->parent->parent);
				}
			}
			else if(node->parent->parent->right==node->parent)
			{
				TreeNode* uncle = node->parent->parent->left;
				if (uncle != nullptr && uncle->color == RED)
				{
					node->parent->color = BLACK;
					uncle->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				}
				else 
				{
					if (node == node->parent->left)
					{
						node = node->parent;
						RotateRight(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					RotateLeft(node->parent->parent);
				}
			}
		}
		_root->color = BLACK;
	}

public:
	CRBTree() :_root(nullptr) {}
	~CRBTree()
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
	void levelOrder()
	{
		result.clear();
		TreeNode* p = _root;
		queue<TreeNode*> q;
		int levelcount = 1;
		int nextlevelcount = 0;
		q.push(p);
		while (!q.empty())
		{
			TreeNode* tmp = q.front();
			result.push_back(tmp);
			q.pop();
			levelcount--;
			if (tmp->left != nullptr)
			{
				q.push(tmp->left);
				nextlevelcount++;
			}
			if (tmp->right != nullptr)
			{
				q.push(tmp->right);
				nextlevelcount++;
			}
			if (levelcount == 0)
			{
				resultlevel.push_back(result);
				result.clear();
				levelcount = nextlevelcount;
				nextlevelcount = 0;
			}
		}
	}
	void printRBTree()
	{
		inorderRBTeee();
		for (int i = 0;i < result.size();i++)
		{
			cout << result[i]->val <<"->"<<result[i]->color<< endl;
		}
		cout << "*******************************" << endl;
		levelOrder();
		for (int i = 0;i < resultlevel.size();i++)
		{
			for (int j = 0;j < resultlevel[i].size();j++)
			{
				cout << resultlevel[i][j]->val << "->" << resultlevel[i][j]->color << "\t";
			}
			cout << endl;
		}
		cout << "*******************************" << endl;
		result.clear();
		resultlevel.clear();
	}
private:
	TreeNode* _root;
	std::vector<TreeNode*> result;
	std::vector< vector<TreeNode*>> resultlevel;
};
#endif // !_RBTREE_H_
