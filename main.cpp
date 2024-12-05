#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

#define HANDLE_ERROR(msg) cerr << (msg) << endl; exit(1);
#define NoMONEY HANDLE_ERROR("Недостаточно средств")
#define NonACTIVE HANDLE_ERROR("Аккаунт неактивен")

#define CHECK_ACTIVE_ACCOUNT if (!this->IsActive) { NonACTIVE; }



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
    AnnualPercents,
    Withdraw,
    Fee
};

string typeToString(TransactionType type) {
    switch (type) {
        case Deposit: return "Deposit";
        case AnnualPercents: return "Annual Percents";
        case Withdraw: return "Withdraw";
        case Fee: return "Fee";
        default: return "Unknown";
    }
}



class BankAccount {

    protected:
        size_t id;
        Currency AccCurrency;
        string DateCreated;
        double Balance;
        vector<Transaction> TransactionsList;
        bool IsActive = true;

        bool withdrawAllowed(double amount) { return (bool)((this->Balance - amount) >= 0); }

    public:
        BankAccount(size_t id, Currency currency, double balance, string dateOfCreation) {
            if (balance < 0) { cerr << "Баланс не может быть отрицательным" << endl; exit(1); }
            this->id = id;
            this->AccCurrency = currency;
            this->Balance = balance;
            this->DateCreated = dateOfCreation;
        }

        virtual void deposit(double amount) {
                CHECK_ACTIVE_ACCOUNT
                Transaction depObj(this->id + this->TransactionsList.size(), amount, asctime(std::localtime(nullptr)), TransactionType::Deposit);
                this->Balance += amount;
                this->TransactionsList.push_back(depObj);

        }

        virtual void withdraw(double amount) {
                CHECK_ACTIVE_ACCOUNT
                if (this->withdrawAllowed(amount)) {
                    Transaction wthObj(this->id + this->TransactionsList.size(), amount, asctime(std::localtime(nullptr)), TransactionType::Withdraw);
                    this->Balance -= amount;
                    this->TransactionsList.push_back(wthObj);
                } else { NoMONEY }
        }

        virtual void display(void) const {
            cout << "Account number:" << '\t' << this->id << endl;
            cout << "Balance:" << '\t' << this->Balance << endl;
            cout << "Active:" << '\t' << (this->IsActive ? "yes" : "false") << endl;
        }

        size_t getNumber(void) const { return this->id; }

        double getBalance(void) const { return this->Balance; }
};


class Transaction {

    private:
        size_t id;
        TransactionType Type;
        string Date;
        double Amount;

    public:
        Transaction(size_t id, double amount, string date, TransactionType type) {
            this->id = id;
            this->Amount = amount;
            this->Date = date;
            this->Type = type;
        }

        void quickView(void) const { cout << typeToString(this->Type) << 't' << this->Amount << endl; }
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

class SavingsAccount : public BankAccount {

    private:
        double AnnualPercent;

    public:
        SavingsAccount(size_t id, Currency currency, double balance, string dateOfCreation, double annualPrc) {
            if (balance < 0) { cerr << "Баланс не может быть отрицательным" << endl; exit(1); }
            this->id = id;
            this->AccCurrency = currency;
            this->AnnualPercent = annualPrc;
            this->Balance = balance;
            this->DateCreated = dateOfCreation;
        }

        void doProfit(void) {
            CHECK_ACTIVE_ACCOUNT
            double amount = this->AnnualPercent * this->Balance;
            Transaction prcObj(this->id + this->TransactionsList.size(), amount, asctime(std::localtime(nullptr)), TransactionType::AnnualPercents);
            this->Balance *= this->AnnualPercent;
            this->TransactionsList.push_back(prcObj);
        }

    void display() const override {
        cout << "Account number:" << '\t' << this->id << endl;
        cout << "Balance:" << '\t' << this->Balance << endl;
        cout << "Annuals:" << '\t' << this->AnnualPercent;
        cout << "Active:" << '\t' << (this->IsActive ? "yes" : "false") << endl;
    }
};

class CheckingAccount : public BankAccount {

    private:
        double Fee;

    public:
        CheckingAccount(size_t id, Currency currency, double balance, string dateOfCreation, double fee) {
            if (balance < 0) { cerr << "Баланс не может быть отрицательным" << endl; exit(1); }
            this->id = id;
            this->Fee = fee;
            this->AccCurrency = currency;
            this->Balance = balance;
            this->DateCreated = dateOfCreation;
        }

        void withdraw(double amount) override {
                CHECK_ACTIVE_ACCOUNT
                if (this->withdrawAllowed(amount * (1 + this->Fee))) {
                    Transaction wthObj(this->id + this->TransactionsList.size(), amount, asctime(std::localtime(nullptr)), TransactionType::Withdraw);
                    Transaction feeObj(this->id + this->TransactionsList.size() + 1, amount * this->Fee, asctime(std::localtime(nullptr)), TransactionType::Fee);
                    this->Balance -= amount;
                    this->Balance -= (amount * this->Fee);
                    this->TransactionsList.push_back(wthObj);
                    this->TransactionsList.push_back(feeObj);
                } else { NoMONEY }
        }

        void display() const override {
            cout << "Account number:" << '\t' << this->id << endl;
            cout << "Balance:" << '\t' << this->Balance << endl;
            cout << "Withdraw fee:" << '\t' << this->Fee;
            cout << "Active:" << '\t' << (this->IsActive ? "yes" : "false") << endl;
        }

};







int main(void) {



    return 0;
}