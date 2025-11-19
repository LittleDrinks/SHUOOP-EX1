// Bisection.cpp
/**********************************************************************************
 *     给定一条单向链表，要求按结点的顺序的奇偶性，创建两条链表：偶链表，奇链表。 *
 * 并且原链表保持不变。                                                           *
 **********************************************************************************/
#include "LinkList.h"
#include <cstring>

template <typename TYPE>
void Bisection(LinkList<TYPE> &linkA, LinkList<TYPE> &linkEven, LinkList<TYPE> &linkOdd)
{
	int n=0, cur_pos = linkA.CurPos();
	TYPE x;

	linkA.GoTop();
	while(linkA.CurNode())
	{
		x = linkA.CurData();
		if(n%2==0)
			linkEven.Append(x);
		else
			linkOdd.Append(x);
		n++;
		linkA.Skip();
	}
	linkA.Go(cur_pos);
}

int BisectionTest()
{
	int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	LinkList<int> linkA(array, sizeof(array)/sizeof(*array));
	LinkList<int> linkA0, linkA1;
	LinkList<char> linkC(str, strlen(str));
	LinkList<char> linkC0, linkC1;

	Bisection(linkA, linkA0, linkA1);
	linkA.ShowList();
	linkA0.ShowList();
	linkA1.ShowList();
	cout << endl;

	Bisection(linkC, linkC0, linkC1);
	linkC.ShowList();
	linkC0.ShowList();
	linkC1.ShowList();
	return 0;
}
