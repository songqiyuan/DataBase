#include"HashTable.h"
int main()
{
	CHashTable<string, HashFunChar<string> > hash(50, HashFunChar<string>());
	string s1 = "123";
	string s2 = "456";
	string s3 = "204";
	string s4 = "102";
	string s5 = "63";
	string s6 = "53";
	string s7 = "108";
	hash.setBunique(false);
	hash.insert(s1);
	hash.insert(s2);
	hash.insert(s3);
	hash.insert(s4);
	hash.insert(s5);
	hash.insert(s6);
	hash.insert(s7);
	hash.printfElements();

	CHashTable<int, HashFunInt<int> > hashint(50, HashFunInt<int>());
	for (int i = 0;i < 50;i++)
	{
		hashint.insert(i);
	}
	hashint.printfElements();
	return 0;

}
