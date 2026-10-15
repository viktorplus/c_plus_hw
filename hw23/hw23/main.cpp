#include <iostream>
#include <cstring>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                        ДОМАШНЄ ЗАВДАННЯ 23 тема: статичні члени, покажчик this.
==========================================================================================================================================

  Тема: банківський рахунок (клас Account).
  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус. Частина «ПОЯСНИ» обов'язкова для всіх.

  ВАЖЛИВЕ ОБМЕЖЕННЯ
  Об'єкти Account передавайте у функції та методи ЛИШЕ за посиланням (Account&). Не пишіть  Account b = a;
  і не передавайте Account за значенням: програма впаде (пам'ятаєте приклад ShallowCopy із теми про строки в стилі С?).
  * Чому так і як це виправити — тема наступного уроку.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Клас Account (обов'язковий)
  ---------------------------------------------------------------

  Поля (private):
    - char* owner        — ім'я власника, динамічний рядок;
    - double balance     — баланс у гривнях;
    - const int id       — унікальний номер рахунку.

  Статичні поля (private):
    - int count          — скільки рахунків існує ЗАРАЗ;
    - int nextId         — номер для наступного рахунку, починається з 1001;
    - double totalMoney  — сума грошей на ВСІХ рахунках разом.
    *   Не забудьте визначити їх поза класом (чи зробити inline).

  Конструктори:
    - Account(const char* owner, double balance) — головний конструктор:
        * id отримує значення nextId, після чого nextId збільшується на 1 (у списку ініціалізації);
        * ім'я встановлюється через setOwner;
        * початковий баланс приймається, лише якщо він більший за 0, інакше баланс = 0;
        * збільшує count і додає баланс до totalMoney;
        * виводить повідомлення:  [+] Рахунок #1001 (Олена)
    - Account(const char* owner) — делегує головному з балансом 0;
    - Account() — делегує головному з ім'ям "Без імені" і балансом 0.

  Деструктор:
    - виводить повідомлення:  [-] Рахунок #1001 (Олена)
    - віднімає свій баланс від totalMoney, зменшує count;
    - звільняє пам'ять owner.

  Методи:
    - void setOwner(const char* owner) — як setName у Student. Параметр має ТЕ САМЕ ім'я, що й поле: використовуйте this->.
    - double getBalance(), int getId();
    - void deposit(double amount) — поповнення; сума має бути більшою за 0;
    - void withdraw(double amount) — зняття; сума більша за 0 і не більша за баланс, інакше вивести «Відмова: недостатньо коштів»;
    - void print() — виводить:  #1001 Олена: 1000 грн
    - static int getCount(), static double getTotalMoney().
    Увага: deposit і withdraw мають змінювати і balance, і totalMoney.

  main:
    1. До створення будь-якого рахунку виведіть Account::getCount().
    2. Створіть три рахунки, використавши всі три конструктори.
    3. Поповніть і зніміть гроші, спробуйте зняти більше, ніж є.
    4. Виведіть усі рахунки, getCount() і getTotalMoney().

  ---------------------------------------------------------------
  РІВЕНЬ 2. Ланцюжок викликів і переказ (бажаний)
  ---------------------------------------------------------------

  1. Змініть deposit і withdraw так, щоб вони повертали Account& (return *this;). У main виконайте одним рядком:
         a.deposit(500).withdraw(200).print();

  2. Додайте метод  bool transfer(Account& to, double amount):
     - переказ суми з поточного рахунку на рахунок to;
     - якщо to — це той самий рахунок if(this == &to), вивести «Відмова: переказ самому собі» і повернути false;
     - якщо сума не більша за 0 або більша за баланс — вивести «Відмова: некоректна сума» і повернути false;
     - інакше перевести гроші і повернути true. У main перевірте всі три випадки: успішний переказ, переказ самому собі, переказ завеликої суми.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Масив рахунків (бонус)
  ---------------------------------------------------------------

  1. Створіть динамічний масив з n рахунків:  new Account[n]; (n введіть з клавіатури). Які конструктори викликались?
  2. Поповніть кілька рахунків масиву різними сумами.
  3. Знайдіть і виведіть рахунок з найбільшим балансом.
  4. Видаліть масив і переконайтеся, що getCount() і getTotalMoney() повернулися до значень, які були до створення масиву.

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Чому count і totalMoney статичні, а balance — ні?
  2. Навіщо в setOwner потрібен this->? Що станеться, якщо його прибрати?
  3. Чому getCount() можна викликати ще до створення будь-якого рахунку?
  4. (Рівень 2) Що станеться, якщо в transfer прибрати перевірку if(this == &to)? Чи зміниться totalMoney після переказу? Чому?
  5. (Рівень 2) Чому deposit повертає Account&, а не Account?
*/

// ===== Ваш код =====

class Account
{
    inline static int count = 0;            //— скільки рахунків існує ЗАРАЗ;
    inline static int nextId = 1000;        //— номер для наступного рахунку, починається з 1001;
    inline static double totalMoney = 0;    //— сума грошей на ВСІХ рахунках разом.

    char* owner;       //— ім'я власника, динамічний рядок;
    double balance;    //— баланс у гривнях;
    const int id;      //— унікальний номер рахунку.

public:

    Account(const char* owner, double balance) : id(++nextId), owner(nullptr)
    {
        setOwner(owner);
        this->balance = (balance > 0) ? balance : 0;
        count++;
        totalMoney += this->balance;
        cout << "[+] Рахунок #" << id << " (" << this->owner << ")" << endl;
    }

    Account(const char* owner) : Account(owner, 0) {}

    Account() : Account("Без імені", 0) {}

    ~Account()
    {
        cout << "[-] Рахунок #" << id << " (" << this->owner << ")" << endl;
        totalMoney -= balance;
        count--;
        delete[] owner;
    }

    void setOwner(const char* owner)
    {
        delete[] this->owner;
        size_t length = strlen(owner) + 1;
        this->owner = new char[length];
        strcpy_s(this->owner, length, owner);
    }

    double getBalance() const { return balance; }

    int getId() const { return id; }

    Account& deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            totalMoney += amount;
        }
		return *this;

    }

    Account& withdraw(double amount) 
    {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            totalMoney -= amount;
        }
        else {
            cout << "Відмова: недостатньо коштів" << endl;
        }
		return *this;
    }

    void print() const {
        cout << "#" << id << " " << owner << ": " << balance << " грн" << endl;
    }

    inline static int getCount() { return count; }

    inline static double getTotalMoney() { return totalMoney; }

    bool transfer(Account& to, double amount) {
		if (this == &to) {
			cout << "Відмова: переказ самому собі" << endl;
			return false;
		}
		if (amount <= 0 || amount > balance) {
			cout << "Відмова: некоректна сума" << endl;
			return false;
		}
		to.deposit(amount);
		this->withdraw(amount);
		return true;
    };

};


int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "Accounts: " << Account::getCount() << endl;
    cout << "Total: " << Account::getTotalMoney() << endl;

    Account a1("Олена", 1000);
    Account a2("Ігор");
    Account a3{};

    //a1.deposit(500);
    //a1.print();
    //a1.withdraw(200);
    //a1.print();
    //a1.withdraw(2000); // спроба зняти більше, ніж є

    a1.deposit(500).withdraw(200).print();

    cout << "Accounts: " << Account::getCount() << endl;
    cout << "Total: " << Account::getTotalMoney() << "грн" << endl;

	a1.transfer(a2, 300); // успішний переказ
	a1.transfer(a1, 100); // переказ самому собі
	a2.transfer(a1, 2000); // переказ завеликої суми

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1.

  2.

  3.

  4.

  5.

*/
