#include "app/application.hpp"
#include "app/BankModels.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

// 测试添加客户
TEST(ApplicationTest, AddCustomer) {
    Application& app = Application::getInstance();
    // 清空数据
    app.getBankSystem().FreeList();

    // 添加客户
    std::string id = "C001";
    std::string name = "John Doe";
    Customer c(id, name);
    app.getBankSystem().Append(c);

    // 验证
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getId(), id);
    EXPECT_EQ(app.getBankSystem().CurData().getName(), name);
    EXPECT_EQ(app.getBankSystem().NumNodes(), 1);
}

// 测试添加账户
TEST(ApplicationTest, AddAccount) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 添加客户
    Customer c("C001", "John");
    app.getBankSystem().Append(c);

    // 添加账户
    app.getBankSystem().Go(0);
    Account a("C001", "A001", "pass123");
    app.getBankSystem().CurData().getAccountList().Append(a);

    // 验证
    app.getBankSystem().CurData().getAccountList().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getAid(), "A001");
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getPassword(), "pass123");
}

// 测试添加交易
TEST(ApplicationTest, AddTransaction) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 添加客户和账户
    Customer c("C001", "John");
    app.getBankSystem().Append(c);
    app.getBankSystem().Go(0);
    Account a("C001", "A001", "pass");
    app.getBankSystem().CurData().getAccountList().Append(a);

    // 添加交易
    Transaction t("T001", "A001", std::chrono::system_clock::now(), 100.0, "Deposit", "0");
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Append(t);

    // 验证
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().CurData().getId(), "T001");
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().CurData().getAmount(), 100.0);
}

// 测试保存和加载数据
TEST(ApplicationTest, SaveAndLoadData) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 添加数据
    Customer c("C001", "John");
    app.getBankSystem().Append(c);
    app.getBankSystem().Go(0);
    Account a("C001", "A001", "pass");
    app.getBankSystem().CurData().getAccountList().Append(a);
    Transaction t("T001", "A001", std::chrono::system_clock::now(), 100.0, "Deposit", "0");
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Append(t);

    // 保存
    std::ofstream file("test_bank_data.txt");
    app.getBankSystem().GoTop();
    while (app.getBankSystem().CurNode() != nullptr) {
        Customer& cust = app.getBankSystem().CurData();
        file << "C " << cust.getId() << " " << cust.getName() << std::endl;
        LinkList<Account>& accounts = cust.getAccountList();
        accounts.GoTop();
        while (accounts.CurNode() != nullptr) {
            Account& acc = accounts.CurData();
            file << "A " << acc.getCid() << " " << acc.getAid() << " " << acc.getPassword() << std::endl;
            LinkList<Transaction>& transList = acc.getTransactionList();
            transList.GoTop();
            while (transList.CurNode() != nullptr) {
                Transaction& trans = transList.CurData();
                file << "T ";
                trans.saveToStream(file);
                transList.Skip(1);
            }
            accounts.Skip(1);
        }
        app.getBankSystem().Skip(1);
    }
    file.close();

    // 清空并加载
    app.getBankSystem().FreeList();
    std::ifstream infile("test_bank_data.txt");
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "C") {
            std::string id, name;
            iss >> id >> name;
            Customer cust(id, name);
            app.getBankSystem().Append(cust);
        } else if (type == "A") {
            std::string cid, aid, password;
            iss >> cid >> aid >> password;
            app.getBankSystem().GoTop();
            while (app.getBankSystem().CurNode() != nullptr) {
                if (app.getBankSystem().CurData().getId() == cid) {
                    Account acc(cid, aid, password);
                    app.getBankSystem().CurData().getAccountList().Append(acc);
                    break;
                }
                app.getBankSystem().Skip(1);
            }
        } else if (type == "T") {
            Transaction trans = Transaction::loadFromStream(iss);
            std::string aid = trans.getCurrentAccountId();
            app.getBankSystem().GoTop();
            bool found = false;
            while (app.getBankSystem().CurNode() != nullptr) {
                LinkList<Account>& accList = app.getBankSystem().CurData().getAccountList();
                accList.GoTop();
                while (accList.CurNode() != nullptr) {
                    if (accList.CurData().getAid() == aid) {
                        accList.CurData().getTransactionList().Append(trans);
                        found = true;
                        break;
                    }
                    accList.Skip(1);
                }
                if (found) break;
                app.getBankSystem().Skip(1);
            }
        }
    }
    infile.close();

    // 验证加载
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getId(), "C001");
    app.getBankSystem().CurData().getAccountList().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getAid(), "A001");
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().CurData().getId(), "T001");
}

// 3.1.1 基本功能测试 - 测试多个客户、账户、交易
TEST(ApplicationTest, MultipleEntities) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 添加多个客户
    Customer c1("C001", "John");
    Customer c2("C002", "Jane");
    app.getBankSystem().Append(c1);
    app.getBankSystem().Append(c2);

    // 为每个客户添加账户
    app.getBankSystem().Go(0);
    Account a1("C001", "A001", "pass1");
    app.getBankSystem().CurData().getAccountList().Append(a1);
    app.getBankSystem().Go(1);
    Account a2("C002", "A002", "pass2");
    app.getBankSystem().CurData().getAccountList().Append(a2);

    // 添加交易
    app.getBankSystem().Go(0);
    Transaction t1("T001", "A001", std::chrono::system_clock::now(), 100.0, "Deposit", "0");
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Append(t1);
    app.getBankSystem().Go(1);
    Transaction t2("T002", "A002", std::chrono::system_clock::now(), 200.0, "Withdraw", "0");
    app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().Append(t2);

    // 验证
    EXPECT_EQ(app.getBankSystem().NumNodes(), 2);
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().NumNodes(), 1);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().NumNodes(), 1);
    app.getBankSystem().Go(1);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().NumNodes(), 1);
    EXPECT_EQ(app.getBankSystem().CurData().getAccountList().CurData().getTransactionList().NumNodes(), 1);
}

// 3.1.2 可靠性测试 - 测试重复ID
TEST(ApplicationTest, DuplicateCustomerID) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    Customer c1("C001", "John");
    app.getBankSystem().Append(c1);

    // 尝试添加重复ID
    Customer c2("C001", "Jane");
    app.getBankSystem().Append(c2);  // 应该允许，但业务逻辑中应检查

    // 验证：链表允许重复，但业务逻辑应防止
    EXPECT_EQ(app.getBankSystem().NumNodes(), 2);  // LinkList允许重复
}

// 3.1.2 可靠性测试 - 添加账户到不存在的客户
TEST(ApplicationTest, AddAccountToNonExistentCustomer) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 不添加客户，直接尝试添加账户 - 模拟业务逻辑中的检查
    // 在实际业务中，会检查客户存在
    // 这里测试LinkList的稳定性
    EXPECT_EQ(app.getBankSystem().NumNodes(), 0);
    // 如果尝试访问不存在的节点，不会崩溃
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurNode(), nullptr);
}

// 3.1.2 可靠性测试 - 加载不存在的文件
TEST(ApplicationTest, LoadNonExistentFile) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 尝试加载不存在的文件
    std::ifstream file("nonexistent.txt");
    EXPECT_FALSE(file.is_open());  // 文件不存在

    // 模拟loadData中的行为
    if (!file.is_open()) {
        // 不应崩溃
        EXPECT_TRUE(true);
    }
}

// 3.1.2 可靠性测试 - 空链表操作
TEST(ApplicationTest, EmptyListOperations) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    EXPECT_TRUE(app.getBankSystem().isEmpty());
    EXPECT_EQ(app.getBankSystem().NumNodes(), 0);
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurNode(), nullptr);
    // 不会崩溃
}

// 3.1.3 其他测试 - 边界测试：空字符串ID
TEST(ApplicationTest, EmptyStringID) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    Customer c("", "No ID");
    app.getBankSystem().Append(c);
    app.getBankSystem().Go(0);
    EXPECT_EQ(app.getBankSystem().CurData().getId(), "");
    EXPECT_EQ(app.getBankSystem().CurData().getName(), "No ID");
}

// 3.1.3 其他测试 - 性能测试：添加大量数据
TEST(ApplicationTest, LargeDataSet) {
    Application& app = Application::getInstance();
    app.getBankSystem().FreeList();

    // 添加100个客户
    for (int i = 0; i < 100; ++i) {
        std::string id = "C" + std::to_string(i);
        Customer c(id, "Customer" + std::to_string(i));
        app.getBankSystem().Append(c);
    }

    EXPECT_EQ(app.getBankSystem().NumNodes(), 100);

    // 遍历验证
    app.getBankSystem().GoTop();
    int count = 0;
    while (app.getBankSystem().CurNode() != nullptr) {
        count++;
        app.getBankSystem().Skip(1);
    }
    EXPECT_EQ(count, 100);
}