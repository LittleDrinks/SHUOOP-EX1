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