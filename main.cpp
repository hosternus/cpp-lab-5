#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <chrono>

using namespace std;

enum Currency {
    USD,
    RUB,
    EUR,
    AUD,
    BYN,
    AED,
    CNY
};

ostream& operator<<(ostream& out, const Currency curr) {
    switch (curr) {
        case Currency::USD: return out << "Доллары USD";
        case Currency::RUB: return out << "Рубли RUB";
        case Currency::EUR: return out << "Евро EUR";
        case Currency::AUD: return out << "Австралийские доллары AUD";
        case Currency::BYN: return out << "Белорусские рубли BYN";
        case Currency::AED: return out << "Дирхамы AED";
        case Currency::CNY: return out << "Юани CNY";
        default: return out << "Ошибка";
    }
}

enum TransactionType {
    Deposit,
    AnnualPercents,
    Withdraw,
    Fee
};

ostream& operator<<(ostream& out, const TransactionType type) {
    switch (type) {
        case TransactionType::Deposit: return out << "Деопзит";
        case TransactionType::AnnualPercents: return out << "Начисление годовых";
        case TransactionType::Withdraw: return out << "Вывод средств";
        case TransactionType::Fee: return out << "Комиссия";
        default: return out << "Ошибка";
    }
}



class Transaction {

    private:

        size_t id;
        TransactionType Ttype;
        double Amount;
        string Tdate = format("{:%F %T}", chrono::system_clock::now());
};



class BankAccount {

    protected:

        size_t id;
        Currency ACcurrency;
        double Balance;
        vector<Transaction> TransactionsList;
        string DateCreated = format("{:%F %T}", chrono::system_clock::now());
        bool IsActive = true;
};



class Customer {

    private:

        size_t id;
        string Name, Surname;
        long Phone;
        string Email;
        string Address;
        string DateOfBirth;
        bool IsVIP = false;
        vector<BankAccount> Accounts;
};



int main(void) { return 0; }