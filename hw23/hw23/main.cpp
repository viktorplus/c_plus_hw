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
как быстро растет   ---------------------------------------------------------------

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

	Account& withdraw(double amount) // Account& тому що ми хочемо повертати посилання на поточний об'єкт, щоб дозволити ланцюжок викликів методів
    {
		if (amount > 0 && amount <= balance) { // перевірка, що сума більша за 0 і не більша за баланс 
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
		if (this == &to) { // перевірка, чи рахунок, на який ми переказуємо, є тим самим рахунком
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

    //как быстро растет-------------------------------------------------------------- -

    //    1. Створіть динамічний масив з n рахунків : new Account[n]; (n введіть з клавіатури).Які конструктори викликались ?
    //    2. Поповніть кілька рахунків масиву різними сумами.
    //    3. Знайдіть і виведіть рахунок з найбільшим балансом.
    //    4. Видаліть масив і переконайтеся, що getCount() і getTotalMoney() повернулися до значень, які були до створення масиву.


    cout << "Accounts: " << Account::getCount() << endl;
    cout << "Total: " << Account::getTotalMoney() << endl;

    Account* accounts = nullptr;
    size_t  n;
    cout << "Введіть кількість рахунків: ";
    cin >> n;
	n = 3; // для тестування
    accounts = new Account[n];
    accounts[0].setOwner("Олена");
    accounts[0].deposit(1000);
    accounts[1].setOwner("Олег");
    accounts[1].deposit(2000);
    accounts[2].setOwner("Ірина");
    accounts[2].deposit(3000);

    cout << "Рахунок з найбільшим балансом: " << endl;
    Account* maxAccount = &accounts[0];
    for (int i = 1; i < n; i++) {
        if (accounts[i].getBalance() > maxAccount->getBalance()) {
            maxAccount = &accounts[i];
        }
    }
    maxAccount->print();

    delete[] accounts;

    cout << "Accounts: " << Account::getCount() << endl;
    cout << "Total: " << Account::getTotalMoney() << endl;
}

//    cout << "Accounts: " << Account::getCount() << endl;
//    cout << "Total: " << Account::getTotalMoney() << endl;
//
//    Account a1("Максим", 1000);
//    Account a2("Святослав");
//    Account a3{};
//
//    a1.deposit(500).withdraw(200).print();
//
//    cout << "Accounts: " << Account::getCount() << endl;
//    cout << "Total: " << Account::getTotalMoney() << "грн" << endl;
//
//	a1.transfer(a2, 300); // успішний переказ
//	a1.transfer(a1, 100); // переказ самому собі
//	a2.transfer(a1, 2000); // переказ завеликої суми
//
//    a1.print();
//    a2.print();
//    a3.print();
//
//    cout << "Accounts: " << Account::getCount() << endl;
//    cout << "Total: " << Account::getTotalMoney() << " грн" << endl;
//
//    return 0;
//}

/*

/*
ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

1. count і totalMoney статичні, тому що вони спільні для всіх рахунків:
   зберігають кількість рахунків і загальну суму грошей.
   balance не статичний, бо кожен рахунок має власний баланс.

2. Параметр owner має таке саме ім'я, як і поле класу.
   this->owner означає поле поточного об'єкта, а owner — параметр.
   Якщо прибрати this->, звернення буде до параметра замість поля.
   У нашому коді присвоєння пам'яті параметру owner також спричинить
   помилку компіляції, оскільки він має тип const char*.

3. getCount() — статичний метод, який належить класу.
   Для його виклику не потрібен об'єкт. Він читає статичне поле count,
   яке існує ще до створення першого рахунку.

4. Якщо прибрати перевірку this == &to, переказ допустимої суми самому
   собі спочатку поповнить рахунок, а потім зніме ту саму суму.
   Баланс залишиться незмінним, але метод поверне true.
   Після успішного переказу totalMoney не змінюється: скільки грошей
   знімається з одного рахунку, стільки ж додається на інший.

5. deposit повертає Account&, тобто посилання на поточний об'єкт,
   без створення копії. Це дозволяє будувати ланцюжок викликів:
   a.deposit(500).withdraw(200).
   Повернення Account за значенням створило б копію. У цьому класі
   копія мала б той самий покажчик owner, що може призвести
   до повторного звільнення тієї самої пам'яті.
*/