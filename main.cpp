#include <iostream>
#include <string>
#include <vector>

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

enum TransactionType {
    Deposit,
    Withdrawal,
    AnnualPercents,
    Fee
};



class BankAccount {

    private:
        size_t id;
        Currency Currency;
        string DateCreated;
        float Balance;
        bool IsActive = true;
        vector<Transaction> TransactionsList;



};

class Transaction {
    private:
        size_t id;
        TransactionType Type;
        string Date;
        float Amount;

};

class Customer {
    
    private:
        size_t id;
        long PhoneNumber;
        string email;
        string Address;
        string Name, Surname;
        string DateOfBirth;
        bool IsVIP = false;
        vector<BankAccount> Accounts;


};


class SavingsAccount : BankAccount {

    private:
        float Annual;

};

class CheckingAccount : BankAccount {

    private:
        double Fee;

};








int main(void) {



    return 0;
}