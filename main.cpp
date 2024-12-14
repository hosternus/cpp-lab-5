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
    USD = 0,
    RUB = 1,
    EUR = 2,
    AUD = 3,
    BYN = 4,
    AED = 5,
    CNY = 6,
    CurrencyCount 
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

        bool withdrawAllowed(double amount) { return (this->Balance - amount) >= 0; }

    public:

        BankAccount(Currency currency, double balance) : id(IDGenerator()), ACcurrency(currency), Balance(balance), DateCreated(format("{:%F %T}", chrono::system_clock::now())) {}

        virtual ~BankAccount() {}

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

// Currency?
        void deposit(double amount) {
            CHECK_ACTIVE_ACCOUNT
            Transaction depObj (TransactionType::Deposit, amount, this->ACcurrency);
            this->Balance += amount;
            this->TransactionsList.push_back(depObj);
        }

        virtual void withdraw(double amount) {
            CHECK_ACTIVE_ACCOUNT
            if (this->withdrawAllowed(amount)) {
                Transaction wthObj (TransactionType::Withdraw, amount, this->ACcurrency);
                this->Balance -= amount;
                this->TransactionsList.push_back(wthObj);
            } else { NoMONEY }
        }
};



class SavingsAccount : public BankAccount {

    private:

        double AnnualPercent;

    public:

        SavingsAccount(Currency currency, double balance, float annualsP) : BankAccount(currency, balance), AnnualPercent(annualsP) {}

        double getAnnualsPercentInfo(void) const { return this->AnnualPercent; }

        void show(void) const override {
            cout << "****************SAVINGS ACCOUNT****************" << endl;
            cout << "** " << "Номер: " << this->id << endl;
            cout << "** " << "Баланс: " << this->Balance << " " << this->ACcurrency << endl;
            cout << "** " << "Годовые: " << this->AnnualPercent << " (" << this->Balance * this->AnnualPercent / 12.f << " " << this->ACcurrency << " в месяц)" << endl;
            cout << "** " << "Открыт: " << this->DateCreated << endl;
            cout << "***********************************************" << endl;
        }

        void doProfit(void) {
            CHECK_ACTIVE_ACCOUNT
            double amount = this->Balance * AnnualPercent;
            Transaction annObj (TransactionType::AnnualPercents, amount, this->ACcurrency);
            this->Balance += amount;
            this->TransactionsList.push_back(annObj);
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

        void withdraw(double amount) override {
            CHECK_ACTIVE_ACCOUNT
            if (this->withdrawAllowed(amount)) {
                Transaction wthObj (TransactionType::Withdraw, amount, this->ACcurrency);
                if (FreeWithdrawsLeft > 0) {
                    this->Balance -= amount;
                    FreeWithdrawsLeft--;
                } else {
                    Transaction feeObj (TransactionType::Fee, amount * WFee, this->ACcurrency);
                    this->Balance -= amount * ( 1 + WFee );
                    this->TransactionsList.push_back(feeObj);
                }
                this->TransactionsList.push_back(wthObj);
            } else { NoMONEY }
        }

        void show(void) const override {
            cout << "***************CHECKING ACCOUNT***************" << endl;
            cout << "** " << "Номер: " << this->id << endl;
            cout << "** " << "Баланс: " << this->Balance << " " << this->ACcurrency << endl;
            cout << "** " << "Комиссия: " << this->WFee << "%" << endl;
            cout << "** " << "Осталось бесплатных выводов: " << this->FreeWithdrawsLeft << endl;        
            cout << "** " << "Открыт: " << this->DateCreated << endl;
            cout << "**********************************************" << endl;
        }
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
        vector<BankAccount*> Accounts;

    public:

        Customer(
            string name, string surname, long phone, string email, string address, string dateofbirth
        ) : id(IDGenerator()), Name(name), Surname(surname), Phone(phone), Email(email), Address(address), DateOfBirth(dateofbirth) {}

        ~Customer() {
            for (size_t i = 0; i < Accounts.size(); i++) {
                delete Accounts[i];
            }
        }

        size_t addSAccount(Currency currency, double balance, float annuals) {
            Accounts.push_back(new SavingsAccount(currency, balance, annuals));
            return Accounts[Accounts.size() - 1]->getID();
        }

        size_t addCAccount(Currency currency, double balance, size_t freeWithdraws, float fee) {
            Accounts.push_back(new CheckingAccount(currency, balance, fee, freeWithdraws));
            return Accounts[Accounts.size() - 1]->getID();
        }

        bool removeAccount(size_t id) {
            for (size_t i = 0; i < Accounts.size(); i++) {
                if (Accounts[i]->getID() == id) {
                    delete Accounts[i];
                    Accounts.erase(Accounts.begin() + i);
                    return true;
                }
            }
            return false;
        }

        void showCustomerInfo(void) const {
            cout << "******************CUSTOMER INFO******************" << endl;
            cout << "** " <<this->Name << " " << this->Surname << endl;
            cout << "** " << this->Phone << endl;
            cout << "** " << this->Email << endl;
            cout << "** " << this->Address << endl;
            cout << "** " << this->DateOfBirth << endl;
            cout << "** " << (this->IsVIP?"VIP":"nonVIP") << endl;
            cout << "** " << this->id << endl;
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
        vector<BankAccount*> getAccounts(void) const { return this->Accounts; }
};



int main(void) {

    cout << "******COMMANDS******" << endl;
    cout << "** " << "/addCustomer" << endl;
    cout << "** " << "/addCAccount" << endl;
    cout << "** " << "/addSAccount" << endl;
    cout << "** " << "#exit" << endl;
    
    string inpt;
    vector<Customer*> clients;
    
    while (true) {
        cin >> inpt;

        if (inpt == "/addCustomer") {
            string name, surname, email, address, dateofbirth;
            long phone;
            cout << "** Имя:" << endl;
            cin >> name;
            cout << "** Фамилия:" << endl;
            cin >> surname;
            cout << "** Почта:" << endl;
            cin >> email;
            cout << "** Aдрес:" << endl;
            cin >> address;
            cout << "** Телефон:" << endl;
            cin >> phone;
            cout << "** Дата рождения:" << endl;
            cin >> dateofbirth;
            clients.push_back(new Customer(name, surname, phone, email, address, dateofbirth));
            cout << "************NEW************" << endl;
            clients[clients.size() - 1]->showCustomerInfo();
            continue;
        }

        if (inpt == "/addCAccount" || inpt == "/addSAccount") {
            size_t id;
            cout << "Номер клиента:" << endl;
            cin >> id;
            bool flag = false;
            for (size_t i = 0; i < clients.size(); i++) {
                if (clients[i]->getID() == id) { id = i; flag = true; break; }
            }
            if (flag) {
                size_t currency;
                double balance;
                cout << "*** Введите номер валюты в соотвествии с этим списком ***" << endl;
                for (size_t cr = 0; cr < Currency::CurrencyCount; cr++) { cout << static_cast<Currency>(cr) << " = " << cr << endl; }
                cin >> currency;
                cout << "*** Введите изначальный баланс ***" << endl;
                cin >> balance;
                if (inpt == "/addCAccount") {
                    size_t freew;
                    float fee;
                    cout << "*** Введите количество бесплатных выводов ***" << endl;
                    cin >> freew;
                    cout << "*** Введите ставвку для вывода средств ***" << endl;
                    cin >> fee;
                    clients[id]->addCAccount(static_cast<Currency>(currency), balance, freew, fee);
                } else if (inpt == "/addSAccount") {
                    float annuals;
                    cout << "*** Введи ставку годовых ***" << endl;
                    cin >> annuals;
                    clients[id]->addSAccount(static_cast<Currency>(currency), balance, annuals);
                }
                cout << "********ГОТОВО********" << endl;
            } else { cout << "*** Нет такого клиента ***" << endl; }
            continue;
        }

        if (inpt == "#exit") { break; }

        cout << "*** Нет такой команды ***" << endl;
    }

    for (size_t i = 0; i < clients.size(); i++) { delete clients[i]; }

    return 0; }