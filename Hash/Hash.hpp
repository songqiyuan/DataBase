#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__
#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
using namespace std;

//仿函数，计算元素的落脚位置
template<typename T>
struct HashFunInt //计算int类型
{
	size_t operator()(T key, int mod)
	{
		return (key%mod);
	}
};
template<typename T>
struct HashFunChar //计算string类型
{
	size_t operator()(const T& key, int mod)
	{
		size_t h = 0;
		int i = 0;
		while (key[i])
		{
			h = (h << 4) + key[i++];
			size_t g = h & 0xF0000000L;
			if (g)
			{
				h ^= g >> 24;
				h &= ~g;
			}
		}
		return (h % mod);
	}
};
static const int num_primes = 28;
static const unsigned long prime_list[num_primes] = {
	53,97,193,389,769,1543,3097,6151,12289,24539,49157,98317,
	196613,393241,786433,1572869,3145739,6291469,12582917,25165843,
	50331653,100663319,203126611,402653189,805306457,161012741,3221225473ul,
	4294967291ul
};
//hashtable节点定义
template<typename T>
struct HashTableNode
{
	HashTableNode* next;
	T val;
	HashTableNode(T key) :val(key), next(nullptr) {}
};
template<typename T,typename HashFun>
class CHashTable
{
private:
	typedef HashTableNode<T> _hashnode;//哈希节点
	vector<_hashnode*> _buckets;//哈希桶
	size_t num_elements;//元素个数
	bool bunique;
	HashFun _hashfun;
public:
	//bucket个数
	size_t bucket_count() const { return _buckets.size(); }
public:
	size_t next_prime(unsigned long n)
	{
		const unsigned long * first = prime_list;
		const unsigned long * last = prime_list+ num_primes;
		const unsigned long * pos = lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}
	CHashTable(size_t n,HashFun hashfun) :num_elements(n),bunique(true),_hashfun(hashfun)
	{
		initialize_buckets(n);
	}
	~CHashTable()
	{
		clear();
	}
	void initialize_buckets(size_t n)
	{
		const size_t n_buckets = next_prime(n);
		_buckets.reserve(n_buckets);
		_buckets.insert(_buckets.end(), n_buckets, (_hashnode*)0);
		num_elements = 0;
	}
public:
	void insert(const T key)
	{
		resize(num_elements + 1);
		insert_noresize(key);
	}
	void setBunique(bool unique)
	{
		bunique = unique;
	}
	bool getBunique()
	{
		return bunique;
	}
	void insert_noresize(const T key)
	{
		const size_t pos = hashFunc(key, _buckets.size());
		_hashnode* first = _buckets[pos];
		
		while (first!=nullptr)
		{
			if (first->val == key && bunique == true)
			{
				//不允许重复插入
				return;
			}
			else if (first->val == key && bunique == false)
			{
				//允许重复插入，遇见相同立即插入
				_hashnode* tmp = new _hashnode(key);
				tmp->next = first->next;
				first->next = tmp;
				num_elements++;
				return;
			}
			first = first->next;
		}
		_hashnode* node = new _hashnode(key);
		//这个位置从头插入
		node->next = _buckets[pos];
		_buckets[pos] = node;
		num_elements++;
		return;
	}
	//判断表格是否需要重建
	void resize(size_t elementsCount)
	{
		//用元素个数与_buckets的容量相比，如果元素个数>_buckets.size()则重建
		//可自行设定扩容条件
		const size_t old_n = _buckets.size();
		if (elementsCount > old_n)
		{
			const size_t new_n = next_prime(elementsCount);
			if (new_n > old_n)
			{
				vector<_hashnode*> tmp(new_n, (_hashnode*)0);
				//以下处理每一个旧的元素
				for (size_t i = 0;i < _buckets.size();i++)
				{
					_hashnode* firstNode = _buckets[i];
					while (firstNode!=nullptr)
					{
						//进行元素移动的时候一定要重新哈希不要直接拷贝
						//重新哈希
						//const T key = firstNode->val;
						const size_t new_buckets_pos = hashFunc(firstNode->val, new_n);
						_buckets[i] = firstNode->next;
						firstNode->next = tmp[new_buckets_pos]!=nullptr?tmp[new_buckets_pos]->next:nullptr;
						tmp[new_buckets_pos] = firstNode;
						firstNode = _buckets[i];
					}
				}
				//最后交换空间,作用域结束释放临时变量tmp
				_buckets.swap(tmp);
				cout << "扩容完成!" << endl;
			}
		}
	}
	size_t hashFunc(const T key,size_t n)
	{
		return _hashfun(key,n);
	}
	void clear()
	{
		//整体删除并释放内存
		for (size_t i = 0;i < _buckets.size();i++)
		{
			_hashnode* first = _buckets[i];
			while (first!=nullptr)
			{
				_buckets[i] = first->next;
				delete first;
				first = _buckets[i];
			}
			_buckets[i] = 0;
		}
		//元素个数清0
		num_elements = 0;
	}
	//按桶打印
	void printfElements()
	{
		//整体删除并释放内存
		for (size_t i = 0;i < _buckets.size();i++)
		{
			_hashnode* first = _buckets[i]; 
			bool bempty = first == nullptr ? true : false;
			if (!bempty)
			{
				cout << i << "号桶：";
			}
			while (first != nullptr)
			{
				_buckets[i] = first->next;
				// T1 val = first->val;
				cout << first->val << "\t";
				first = _buckets[i];
			}
			if (bempty == false)
			{
				cout << endl;
			}
			_buckets[i] = 0;
		}
	}
};
#endif // !__HASHTABLE_H__



