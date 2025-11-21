#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

class Transaction {
private:
    std::string id;                     // 流水号
    std::string current_account_id;     // 当前账户ID
    std::chrono::system_clock::time_point t; // 日期时间
    double amount;                      // 金额
    std::string note;                   // 备注
    std::string to_account_id;          // 对方账户ID

public:
    // 构造函数
    Transaction(const std::string& id, const std::string& current_account_id,
                const std::chrono::system_clock::time_point& t, double amount,
                const std::string& note, const std::string& to_account_id)
        : id(id), current_account_id(current_account_id), t(t),
          amount(amount), note(note), to_account_id(to_account_id) {}

    // 默认构造函数
    Transaction() : id(""), current_account_id(""), t(std::chrono::system_clock::now()),
                    amount(0.0), note(""), to_account_id("") {}

    // 获取日期时间为字符串
    std::string getDateAsString() const {
        std::time_t time = std::chrono::system_clock::to_time_t(t);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // 从字符串解析日期时间
    static std::chrono::system_clock::time_point parseDate(const std::string& date_str) {
        std::tm tm = {};
        std::istringstream ss(date_str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    // 保存到输出流
    void saveToStream(std::ostream& os) const {
        os << id << " " << current_account_id << " "
           << getDateAsString() << " " << amount << " "
           << note << " " << to_account_id << "\n";
    }

    // 从输入流加载
    static Transaction loadFromStream(std::istream& is) {
        std::string id, current_account_id, date_str, time_str, note, to_account_id;
        double amount;
        // Expecting: ID AccountID Date Time Amount Note ToAccountID
        is >> id >> current_account_id >> date_str >> time_str >> amount >> note >> to_account_id;
        auto t = parseDate(date_str + " " + time_str);
        return Transaction(id, current_account_id, t, amount, note, to_account_id);
    }

    std::string getCurrentAccountId() const {
        return current_account_id;
    }

    std::string getId() const {
        return id;
    }

    double getAmount() const {
        return amount;
    }

    // 重载 << 运算符用于打印
    friend std::ostream& operator<<(std::ostream& os, const Transaction& t) {
        os << "Transaction ID: " << t.id
           << ", Account ID: " << t.current_account_id
           << ", Date: " << t.getDateAsString()
           << ", Amount: " << t.amount
           << ", Note: " << t.note
           << ", To Account ID: " << t.to_account_id;
        return os;
    }

    // 重载 == 运算符用于比较
    bool operator==(const Transaction& other) const {
        return id == other.id && current_account_id == other.current_account_id &&
               t == other.t && amount == other.amount &&
               note == other.note && to_account_id == other.to_account_id;
    }
};