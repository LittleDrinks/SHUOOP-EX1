#include "LinkList.hpp"
#include <gtest/gtest.h>

// 测试基本追加和访问
TEST(LinkListBasicTest, AppendAndAccess) {
    LinkList<int> list;
    list.Append(10);
    list.Go(0);
    EXPECT_EQ(list.CurData(), 10);
    list.Append(20);
    list.Go(1);
    EXPECT_EQ(list.CurData(), 20);
}

// 测试删除当前节点
TEST(LinkListBasicTest, DeleteCurNode) {
    LinkList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    list.Go(1); // 指向2
    list.DeleteCurNode();
    list.Go(0);
    EXPECT_EQ(list.CurData(), 1);
    list.Go(1);
    EXPECT_EQ(list.CurData(), 3);
    EXPECT_EQ(list.NumNodes(), 2);
}

// 测试反转
TEST(LinkListBasicTest, Reverse) {
    LinkList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    list.Reverse();
    list.Go(0);
    EXPECT_EQ(list.CurData(), 3);
    list.Go(1);
    EXPECT_EQ(list.CurData(), 2);
    list.Go(2);
    EXPECT_EQ(list.CurData(), 1);
}

// 测试排序
TEST(LinkListBasicTest, Sort) {
    LinkList<int> list;
    list.Append(3);
    list.Append(1);
    list.Append(2);
    // 假设Sort使用默认比较
    list.Sort(0); // 使用int作为T1
    list.Go(0);
    EXPECT_EQ(list.CurData(), 1);
    list.Go(1);
    EXPECT_EQ(list.CurData(), 2);
    list.Go(2);
    EXPECT_EQ(list.CurData(), 3);
}

// 测试空链表
TEST(LinkListBasicTest, EmptyList) {
    LinkList<int> list;
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.NumNodes(), 0);
    list.Go(0);
    EXPECT_EQ(list.CurNode(), nullptr);
}

// 测试保存和加载
TEST(LinkListIOTest, SaveAndLoad) {
    LinkList<int> list;
    list.Append(5);
    list.Append(6);
    list.Save("test_data.txt");
    LinkList<int> newList;
    newList.Load("test_data.txt");
    newList.Go(0);
    EXPECT_EQ(newList.CurData(), 5);
    newList.Go(1);
    EXPECT_EQ(newList.CurData(), 6);
}