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
        case TransactionType::Deposit: return out << "Депозит";
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
        const Currency Tcurrency;

    public:

        Transaction(TransactionType type, double amount, Currency currency) : id(IDGenerator()), Ttype(type), Amount(amount), Tdate(format("{:%F %T}", chrono::system_clock::now())), Tcurrency(currency) {}

        void show(void) const {
            cout << "************TRANSACTION INFO************" << endl;
            cout << "***********  " << this->Ttype << "  ***********" << endl;
            cout << "** " << "Номер транзакции: " << this->id << endl;
            cout << "** " << "Сумма: " << this->Amount << " " << this->Tcurrency << endl;
            cout << "** " << "Дата: " << this->Tdate << endl;
            cout << "****************************************" << endl;
        }

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
        const string DateCreated;
        bool IsActive = true;

    public:

        BankAccount(Currency currency, double balance) : id(IDGenerator()), ACcurrency(currency), Balance(balance), DateCreated(format("{:%F %T}", chrono::system_clock::now())) {}

        size_t getID(void) const { return this->id; }
        Currency getCurrency(void) const { return this->ACcurrency; }
        double getBalance(void) const { return this->Balance; }
        vector<Transaction> getTransactions(void) const {return this->TransactionsList; }
        string getDateCreated(void) const { return this->DateCreated; }
        bool isActive(void) const { return this->IsActive; }

        virtual void show(void) const = 0;

        void deactivateAccount(void) {
            this->IsActive = false;
            cout << "********************************" << endl;
            cout << "*** " << "Аккаунт " << this->id << " деактивирован" << " ***" << endl;
            cout << "********************************" << endl;
        }
};



class SavingsAccount : public BankAccount {

    private:

        double AnnualPercent;

    public:

        SavingsAccount(Currency currency, double balance, float annualsP) : BankAccount(currency, balance), AnnualPercent(annualsP) {}

        double getAnnualsPercentInfo(void) const { return this->AnnualPercent; }

        void show(void) const override {
            cout << "********SAVINGS ACCOUNT********" << endl;
            cout << "** " << "Номер: " << this->id << endl;
            cout << "** " << "Баланс: " << this->Balance << " " << this->ACcurrency << endl;
            cout << "** " << "Годовые: " << this->AnnualPercent << " (" << this->Balance * this->AnnualPercent / 12.f << this->ACcurrency << " в месяц)" << endl;
            cout << "** " << "Открыт: " << this->DateCreated << endl;
            cout << "*******************************" << endl;
        }
};



class CheckingAccount : public BankAccount {

    private:

        double WFee;
        size_t FreeWithdrawsLeft;

    public:

        CheckingAccount(Currency currency, double balance, float fee, size_t freewths) : BankAccount(currency, balance), WFee(fee), FreeWithdrawsLeft(freewths) {}

        double getFeeInfo(void) const { return this->WFee; }
        size_t FreeWLeft(void) const { return this->FreeWithdrawsLeft; }
};



class Customer {

    private:

        const size_t id;
        const string Name, Surname;
        long Phone;
        string Email;
        string Address;
        const string DateOfBirth;
        bool IsVIP = false;
        vector<BankAccount> Accounts;

    public:

        Customer(
            string name, string surname, long phone, string email, string address, string dateofbirth
        ) : id(IDGenerator()), Name(name), Surname(surname), Phone(phone), Email(email), Address(address), DateOfBirth(dateofbirth) {}

        long addSAccount() {}

        long addCAccount() {}

        bool removeAccount() {}

        void showCustomerInfo(void) const {
            cout << "******************CUSTOMER INFO******************" << endl;
            cout << "** " <<this->Name << " " << this->Surname << endl;
            cout << "** " << this->Phone << endl;
            cout << "** " << this->Email << endl;
            cout << "** " << this->Address << endl;
            cout << "** " << this->DateOfBirth << endl;
            cout << "** " << (this->IsVIP?"VIP":"nonVIP") << endl;
            cout << "*************************************************" << endl;
        }

        void markAsVIP(void) {
            this->IsVIP = true;
            cout << "*******CUSTOMER STATUS*******" << endl;
            cout << "** " << "Клиент № " << this->id << " теперь VIP" << endl;
            cout << "******************************" << endl;
        }

        void updateContactInfo(long phone, string email, string address) {
            this->Phone = phone;
            this->Email = email;
            this->Address = address;
            cout << "*******CUSTOMER DETAILS UPDATED*******" << endl;
            cout << "** " << "Номер клиента: " << this->id << endl;
            cout << "** " << "Телефон: " << this->Phone << endl;
            cout << "** " << "Почта: " << this->Email << endl;
            cout << "** " << "Адрес: " << this->Address << endl;
            cout << "**************************************" << endl;
        }

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