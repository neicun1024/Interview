#include <iostream>
#include <thread>
#include <mutex>
#include <set>
using namespace std;

const double MAX_MONEY = 100;

class Account
{
private:
    string name;
    double money;
    mutex _mutex;

public:
    Account(string name, double money) : name(name), money(money) {}

    string getName()
    {
        return name;
    }

    double getMoney()
    {
        return money;
    }

    void changeMoney(double money)
    {
        this->money += money;
    }

    mutex &getMutex()
    {
        return _mutex;
    }
};

class Bank
{
private:
    set<Account *> accounts;

public:
    void add_account(Account *account)
    {
        accounts.insert(account);
    }

    bool transfer_money(double money, Account *a1, Account *a2)
    {
        // lock(a1->getMutex(), a2->getMutex());
        // lock_guard guardA(a1->getMutex(), adopt_lock);
        // lock_guard guardB(a2->getMutex(), adopt_lock);
        scoped_lock lockAll(a1->getMutex(), a2->getMutex());

        if (a1->getMoney() < money)
        {
            return false;
        }
        else
        {
            a1->changeMoney(-money);
            a2->changeMoney(money);
            return true;
        }
    }

    double getTotalMoney()
    {
        double sum = 0;
        for (auto a : accounts)
        {
            sum += a->getMoney();
        }
        return sum;
    }
};

mutex cout_mutex;

void random_transfer(Bank *bank, Account *a1, Account *a2)
{
    while (true)
    {
        double money = (double)rand() / RAND_MAX * MAX_MONEY;
        if (bank->transfer_money(money, a1, a2))
        {
            cout_mutex.lock();
            cout << a1->getName() << " transfer " << money << " to " << a2->getName()
                 << ", bank's total money = " << bank->getTotalMoney() << endl;
            cout_mutex.unlock();
        }
        else
        {
            cout_mutex.lock();
            cout << a1->getName() << " wants to transfer " << money << " to " << a2->getName()
                 << ", but has only " << a1->getMoney() << endl;
            cout_mutex.unlock();
        }
    }
}

int main()
{
    Account *a1 = new Account("Bob", MAX_MONEY);
    Account *a2 = new Account("Lin", MAX_MONEY);
    Bank *bank = new Bank();
    bank->add_account(a1);
    bank->add_account(a2);
    thread t1(random_transfer, bank, a1, a2);
    thread t2(random_transfer, bank, a2, a1);
    t1.join();
    t2.join();

    return 0;
}