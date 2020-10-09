//红黑树测试用例;二叉搜索树实例自己实现
#include"RBTree.h"
using namespace std;

int main()
{
	CRBTree btTree;
	btTree.Insert(19);
	btTree.Insert(25);
	btTree.Insert(20);
	btTree.Insert(30);
	btTree.Insert(10);
	btTree.Insert(14);
	btTree.Insert(12);
	btTree.Insert(11);
	btTree.Insert(17);
	btTree.Insert(13);
	btTree.Insert(4);
	btTree.Insert(2);
	btTree.printRBTree();
	btTree.deleteNode(20);
	btTree.printRBTree();
	return 0;

}
