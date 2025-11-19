// Main.cpp
#include <iostream>
#include <conio.h>
using namespace std;

int EraseMaxPre();				// 删除两条链表的最大相同前缀
int JosephusTest();				// 约瑟夫问题
int BisectionTest();			// 链表奇偶二分

int main()
{
	int choice=1;

	while(choice)
	{
		cout << "\n1 -- 约瑟夫问题"
			 << "\n2 -- 链表奇偶二分"
			 << "\n3 -- 删除两条链表中的最大相同前缀"
			 << "\n0 -- 退出" << endl;
		choice = getch() - '0';

		switch(choice)
		{
		case 1: JosephusTest();		break;
		case 2: BisectionTest();	break;
		case 3: EraseMaxPre();		break;
		}
	}

	return 0;
}
