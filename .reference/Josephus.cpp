// Josephus.cpp
/***********************************************************************************
 *     约瑟夫问题                                                                  *
 *     有n个小孩(编号1～n)围成一圈，从第 1 个小该开始按顺时针方向从 1～m 循环报数，*
 * 报到 m 的小孩出圈。最后剩下的小孩为胜利者。这便是约瑟夫问题。                   *
 * 给定 n, m 编程计算获胜者的号码（函数形参 num 为小孩数，interval 表示 m）。      *
 ***********************************************************************************/
#include "LinkList.h"
#include <conio.h>

int Josephus(int num, int interval)
{
	LinkList<int> Jose;		// LinkList<int>为类名，Jose为对象名
	int i, j, win=0;

	for(i=1; i<=num; i++)	// 各小孩依次入圈，编号从 1 开始
		Jose.Append(i);

	cout << " 初始状态 ";
	for(Jose.GoTop(); Jose.NumNodes()>1; )
	{
		Jose.ShowList();	// 输出剩下的小孩号码(注意当前结点)
		getch();
		for(j=1; j<interval; j++)	// 循环报数（当前结点指针移动）
		{
			if(Jose.isLast()) Jose.GoTop();
			else			  Jose.Skip();
		}
		Jose.ShowCurData();
		cout << " 号出圈。";		// 输出被删除的编号
		Jose.DeleteCurNode();		// 删除当前结点
									// 若删除尾结点，则无当前结点
		if(Jose.CurNode()==NULL) Jose.GoTop();
	}
	Jose.ShowList();
	win = Jose.CurData();
	return win;
}

int JosephusTest()
{
	int n, m;
	cout << "约瑟夫(Josephus)问题："
		 << "请输入小孩数 间隔数（例如： 10 3）" << flush;
	cin >> n >> m;
	cout << "获胜者的号码：" << Josephus(n, m) << endl;
	return 0;
}
