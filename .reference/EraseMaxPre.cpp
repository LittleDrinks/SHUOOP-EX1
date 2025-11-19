// EraseMaxPre.cpp
/******************************************************
 *  给定两个同类单向链表，要求删除相同的最大前缀结点  *
 ******************************************************/
#include "LinkList.h"
#include <cstring>

template <typename LINK> void EraseMaxPre(LINK &linkA, LINK &linkB)
{
	if(linkA.isEmpty() || linkB.isEmpty())
		return;

	linkA.GoTop();
	linkB.GoTop();
	while(linkA.CurNode()!=NULL && linkB.CurNode() && linkA.CurData() == linkB.CurData())
	{
		linkA.DeleteCurNode();
		linkB.DeleteCurNode();
	}
}

int EraseMaxPre()
{
	typedef LinkList<char> LINK;			// 定义标识符 LINK 为类名
	char str1[] = "xyyzxz";
	char str2[] = "xyyzyxxz";
	LINK linkA(str1, strlen(str1)), linkB(str2, strlen(str2));

	cout << "原始链表 A："; linkA.ShowList();
	cout << "原始链表 B："; linkB.ShowList();

	LINK linkA1 = linkA;					// 深拷贝构造链表
	LINK linkB1 = linkB;					// 深拷贝构造链表
	cout << "A1："; linkA1.ShowList();
	cout << "B1："; linkB1.ShowList();

	cout << "\n删除 A1，B1 的最大相同前缀结点" << endl;
	EraseMaxPre(linkA1, linkB1);
	cout << "A1："; linkA1.ShowList();
	cout << "B1："; linkB1.ShowList();
	
	cout << "\n赋值 A1 = A;" << endl;
	linkA1 = linkA;
	cout << "A ："; linkA.ShowList();
	cout << "A1："; linkA1.ShowList();
	
	cout << "\n删除 A，A1 的最大相同前缀结点" << endl;
	EraseMaxPre(linkA, linkA1);
	cout << "结果链表A ："; linkA.ShowList();
	cout << "结果链表A1："; linkA1.ShowList();
	return 0;
}
