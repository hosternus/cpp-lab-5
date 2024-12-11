#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <chrono>

#define HANDLE_ERROR(msg) cerr << (msg) << endl; exit(1);
#define NoMONEY HANDLE_ERROR("Недостаточно средств")
#define NonACTIVE HANDLE_ERROR("Аккаунт неактивен")
#define CHECK_ACTIVE_ACCOUNT if (!this->isActive()) { NonACTIVE; }

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

long IDGenerator(void) {
    static long counter = 0;
    return counter++;
}



class Transaction {

    private:

        const size_t id;
        const TransactionType Ttype;
        const double Amount;
        const string Tdate;

    public:

        Transaction(TransactionType type, double amount) : id(IDGenerator()), Ttype(type), Amount(amount), Tdate(format("{:%F %T}", chrono::system_clock::now())) {}

        void quickView(void) const { cout << this->Ttype << 't' << this->Amount << endl; }

        size_t getID(void) const { return this->id; }
        TransactionType getType(void) const { return this->Ttype; }
        double getAmount(void) const { return this->Amount; }
        string getDate(void) const {return this->Tdate; }
};



class BankAccount {

    protected:

        const size_t id;
        const Currency ACcurrency;
        double Balance;
        vector<Transaction> TransactionsList;
        const string DateCreated = format("{:%F %T}", chrono::system_clock::now());
        bool IsActive = true;

    public:

        size_t getID(void) const { return this->id; }
        Currency getCurrency(void) const { return this->ACcurrency; }
        double getBalance(void) const { return this->Balance; }
        vector<Transaction> getTransactions(void) const {return this->TransactionsList; }
        string getDateCreated(void) const { return this->DateCreated; }
        bool isActive(void) const { return this->IsActive; }  
};



class SavingsAccount : public BankAccount {

    private:

        double AnnualPercent;

    public:

        double getAnnualsPercentInfo(void) const { return this-> AnnualPercent; }
};



class CheckingAccount : public BankAccount {

    private:

        double WFee;
        size_t FreeWithdrawsLeft;

    public:

        double getFeeInfo(void) const { return this-> WFee; }
        size_t FreeWLeft(void) const {return this->FreeWithdrawsLeft; }
};



class Customer {

    private:

        const size_t id;
        string Name, Surname;
        long Phone;
        string Email;
        string Address;
        const string DateOfBirth;
        bool IsVIP = false;
        vector<BankAccount> Accounts;

    public:

        size_t getID(void) const { return this->id; }
        string getName(void) const { return (this->Name + this->Surname); }
        long getPhone(void) const { return this->Phone; }
        string getEmail(void) const { return this->Email; }
        string getAddredd(void) const { return this->Address; }
        string getDateBirth(void) const { return this-> DateOfBirth; }
        bool isVIP(void) const { return this-> IsVIP; }
        vector<BankAccount> getAccounts(void) const { return this->Accounts; }
};



int main(void) { return 0; }