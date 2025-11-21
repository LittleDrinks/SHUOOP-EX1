#include "LinkList.hpp"
#include <gtest/gtest.h>

// 简单的测试示例
TEST(LinkListTest, CanAppendNode) {
    LinkList<int> list;
    list.Append(10);
    list.Go(0);
    EXPECT_EQ(list.CurData(), 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}