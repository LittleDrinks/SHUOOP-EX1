#pragma once

#include <iostream>
#include <string>
#include "BankModels.hpp"

// 单例模式
class Application {
private:
    Application() { }
    LinkList<Customer> bankSystem; // 全局数据存储
public:
    static Application& getInstance() {
        static Application instance;
        return instance;
    }
    void loadData();
    void saveData();
    void showUI();
    LinkList<Customer>& getBankSystem() { return bankSystem; } // 提供访问接口

private:
    void addCustomer();
    void addAccount();
    void addTransaction();
    void showAllData();
};