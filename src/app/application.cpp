#include "app/application.hpp"
#include "app/BankModels.hpp"

#include "app/application.hpp"
#include "app/BankModels.hpp"
#include <fstream>
#include <sstream>

void Application::loadData() {
    std::cout << "Loading data from bank_data.txt..." << std::endl;
    std::ifstream file("bank_data.txt");
    if (!file.is_open()) {
        std::cout << "Error: Cannot open bank_data.txt" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "C") {
            std::string id;
            std::string name;
            iss >> id >> name;
            Customer c(id, name);
            bankSystem.Append(c);
        } else if (type == "A") {
            std::string cid;
            std::string aid;
            std::string password;
            iss >> cid >> aid >> password;

            bankSystem.GoTop();
            while (bankSystem.CurNode() != nullptr) {
                if (bankSystem.CurData().getId() == cid) {
                    Account a(cid, aid, password);
                    bankSystem.CurData().getAccountList().Append(a);
                    break;
                }
                bankSystem.Skip(1);
            }
        } else if (type == "T") {
            Transaction trans = Transaction::loadFromStream(iss);
            std::string aid = trans.getCurrentAccountId();

            bankSystem.GoTop();
            bool found = false;
            while (bankSystem.CurNode() != nullptr) {
                LinkList<Account>& accList = bankSystem.CurData().getAccountList();
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
                bankSystem.Skip(1);
            }
        }
    }

    file.close();
    std::cout << "Data loaded successfully." << std::endl;
}

void Application::saveData() {
    std::cout << "Saving data to bank_data.txt..." << std::endl;
    std::ofstream file("bank_data.txt");
    if (!file.is_open()) {
        std::cout << "Error: Cannot open bank_data.txt for writing" << std::endl;
        return;
    }

    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        Customer& cust = bankSystem.CurData();
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
        bankSystem.Skip(1);
    }
    file.close();
    std::cout << "Data saved successfully." << std::endl;
}

void Application::addCustomer() {
    std::string id, name;
    std::cout << "Enter Customer ID: ";
    std::cin >> id;
    std::cout << "Enter Customer Name: ";
    std::cin >> name;

    bool exists = false;
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        if (bankSystem.CurData().getId() == id) {
            exists = true;
            break;
        }
        bankSystem.Skip(1);
    }

    if (exists) {
        std::cout << "Customer with ID " << id << " already exists." << std::endl;
        system("pause");
        return;
    }

    Customer c(id, name);
    bankSystem.Append(c);
    std::cout << "Customer added successfully." << std::endl;
    system("pause");
}

void Application::addAccount() {
    std::string cid;
    std::cout << "Enter Customer ID: ";
    std::cin >> cid;

    bool found = false;
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        if (bankSystem.CurData().getId() == cid) {
            found = true;
            break;
        }
        bankSystem.Skip(1);
    }

    if (!found) {
        std::cout << "Customer not found." << std::endl;
        system("pause");
        return;
    }

    std::string aid, password;
    std::cout << "Enter Account ID: ";
    std::cin >> aid;
    std::cout << "Enter Password: ";
    std::cin >> password;

    // 检查账户 ID 是否全局唯一
    bool accExists = false;
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        LinkList<Account>& accList = bankSystem.CurData().getAccountList();
        accList.GoTop();
        while (accList.CurNode() != nullptr) {
            if (accList.CurData().getAid() == aid) {
                accExists = true;
                break;
            }
            accList.Skip(1);
        }
        if (accExists) break;
        bankSystem.Skip(1);
    }

    if (accExists) {
        std::cout << "Account ID " << aid << " already exists." << std::endl;
        system("pause");
        return;
    }

    // 添加账户到找到的客户
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        if (bankSystem.CurData().getId() == cid) {
            break;
        }
        bankSystem.Skip(1);
    }

    Account a(cid, aid, password);
    bankSystem.CurData().getAccountList().Append(a);
    std::cout << "Account added successfully." << std::endl;
    system("pause");
}

void Application::addTransaction() {
    std::string aid;
    std::cout << "Enter Account ID: ";
    std::cin >> aid;

    bool found = false;
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        LinkList<Account>& accounts = bankSystem.CurData().getAccountList();
        accounts.GoTop();
        while (accounts.CurNode() != nullptr) {
            if (accounts.CurData().getAid() == aid) {
                found = true;
                break;
            }
            accounts.Skip(1);
        }
        if (found) break;
        bankSystem.Skip(1);
    }

    if (!found) {
        std::cout << "Account not found." << std::endl;
        system("pause");
        return;
    }

    double amount;
    std::string note, targetId;
    std::cout << "Enter Amount: ";
    std::cin >> amount;
    std::cout << "Enter Note: ";
    std::cin >> note;
    std::cout << "Enter Target Account ID (0 for cash): ";
    std::cin >> targetId;

    static int transCounter = 1000;
    std::string tid = "T" + std::to_string(++transCounter); 

    Transaction t(tid, aid, std::chrono::system_clock::now(), amount, note, targetId);
    // bankSystem.CurData() is customer, accounts.CurData() is account
    bankSystem.CurData().getAccountList().CurData().getTransactionList().Append(t);
    std::cout << "Transaction added successfully." << std::endl;
    system("pause");
}

void Application::showAllData() {
    bankSystem.GoTop();
    while (bankSystem.CurNode() != nullptr) {
        Customer& cust = bankSystem.CurData();
        std::cout << "Customer: " << cust.getName() << " (" << cust.getId() << ")" << std::endl;

        LinkList<Account>& accounts = cust.getAccountList();
        accounts.GoTop();
        while (accounts.CurNode() != nullptr) {
            Account& acc = accounts.CurData();
            std::cout << "  Account: " << acc.getAid() << " (Pwd: " << acc.getPassword() << ")" << std::endl;

            LinkList<Transaction>& transList = acc.getTransactionList();
            transList.GoTop();
            while (transList.CurNode() != nullptr) {
                Transaction& trans = transList.CurData();
                std::cout << "    " << trans << std::endl;
                transList.Skip(1);
            }
            accounts.Skip(1);
        }
        bankSystem.Skip(1);
    }
    system("pause");
}

void Application::showUI() {
    while (true) {
        system("cls"); // 清屏
        std::cout << R"(
 _                 _                    _
| |__   __ _ _ __ | | __  ___ _   _ ___| |_ ___ _ __ ___
| '_ \ / _` | '_ \| |/ / / __| | | / __| __/ _ \ '_ ` _ \
| |_) | (_| | | | |   <  \__ \ |_| \__ \ ||  __/ | | | | |
|_.__/ \__,_|_| |_|_|\_\ |___/\__, |___/\__\___|_| |_| |_|
                              |___/
        )" << "\n";

        std::cout << "=====================================\n";
        std::cout << "1. 加载数据\n";
        std::cout << "2. 保存数据\n";
        std::cout << "3. 新增客户\n";
        std::cout << "4. 新增账户\n";
        std::cout << "5. 登记流水\n";
        std::cout << "6. 查看所有数据\n";
        std::cout << "0. 退出系统\n";
        std::cout << "=====================================\n";
        std::cout << "请选择操作: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                loadData();
                system("pause");
                break;
            case 2:
                saveData();
                system("pause");
                break;
            case 3:
                addCustomer();
                break;
            case 4:
                addAccount();
                break;
            case 5:
                addTransaction();
                break;
            case 6:
                showAllData();
                break;
            case 0:
                std::cout << "退出系统...\n";
                return; // 退出循环，结束程序
            default:
                std::cout << "无效输入，请重新选择。\n";
                system("pause");
        }
    }
}