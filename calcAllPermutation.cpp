/**
* 输入一个字符串，打印该字符串中字符的所有排列。
* 如输入字符串abc，则输出为：
* abc, acb, bac, bca, cab, cba
*/

//法一：递归
//从集合中依次选出一个元素，作为排列的第一个元素，然后对剩余的
//元素进行全排列，如此递归处理，从而得到所有元素的全排列。

#include <iostream>
#include <algorithm>
using namespace std;

void calcAllPermutation(string perm, int from, int to) {
	if (to <= 1)
		return;
	if (from == to) {
		for (int i = 0; i <= to; ++i)
			cout << perm[i];
		cout << endl;
	}
	else {
		for (int j = from; j <= to; ++j) {
			swap(perm[from], perm[j]);
			calcAllPermutation(perm, from + 1, to);
			swap(perm[from], perm[j]);		//因为要保持递归时perm相同
		}
	}
}	

int main() {
	string s;
	cout << "请输入字符串：";
	cin >> s;
	int to = s.length() - 1;
	calcAllPermutation(s, 0, to);
	return 0;
}
