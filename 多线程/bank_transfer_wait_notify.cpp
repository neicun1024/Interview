#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <set>
using namespace std;

const double MAX_MONEY = 100;

class Account
{
private:
  string name;
  double money;
  mutex _mutex;
  condition_variable mConditionVar;

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

  void changeMoney(double change)
  {
    unique_lock ulock(_mutex);
    mConditionVar.wait(ulock, [this, change]()
                       { return this->money + change > 0; });
    this->money += change;
    mConditionVar.notify_all();
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
    a1->changeMoney(-money);
    a2->changeMoney(money);
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
    bank->transfer_money(money, a1, a2);
    lock_guard guard(cout_mutex);
    cout << a1->getName() << " transfer " << money << " to " << a2->getName()
         << ", bank's total money = " << bank->getTotalMoney() << endl;
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