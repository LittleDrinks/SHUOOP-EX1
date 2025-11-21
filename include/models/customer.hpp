#pragma once

#include <string>
#include "account.hpp"

class Customer {
private:
    std::string id;
    std::string name;
    LinkList<Account> accounts;

public:
    Customer(const std::string& cid, const std::string& cname = "")
        : id(cid), name(cname) {}

    friend bool operator==(const Customer& c1, const Customer& c2) {
        return c1.id == c2.id;
    }

    std::string getName() const {
        return name;
    }

    std::string getId() const {
        return id;
    }

    void setName(const std::string& cname) {
        name = cname;
    }

    LinkList<Account> &getAccountList() {
        return accounts;
    }
};