#pragma once

#include <string>
#include "transaction.hpp"
#include "../LinkList.hpp"

class Account {
private:
    std::string cid;
    std::string aid;
    std::string password;
    LinkList<Transaction> transactions;

public:
    Account(const std::string &cid, const std::string &aid, const std::string &pass="")
        : cid(cid), aid(aid), password(pass) {}

    friend bool operator==(const Account& a1, const Account& a2) {
        return a1.aid == a2.aid;
    }

    std::string getAid() const {
        return aid;
    }

    std::string getCid() const {
        return cid;
    }

    std::string getPassword() const {
        return password;
    }

    void setPassword(const std::string& pass) {
        password = pass;
    }

    LinkList<Transaction>& getTransactionList() {
        return transactions;
    }
};