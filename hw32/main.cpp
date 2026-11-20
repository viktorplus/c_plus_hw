#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                    ДОМАШНЄ ЗАВДАННЯ 32 тема: успадкування — одиночне, множинне, віртуальний базовий клас.
==========================================================================================================================================

  Тема: IT-компанія. Усі працівники мають ім'я і базову ставку, але зарплату кожен рахує по-своєму:
  розробник отримує бонус за виконані задачі, менеджер — за розмір команди, а тімлід — і те, і інше.

  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  Можна користуватися ШІ, щоб розібратися в незрозумілому. Але на розборі вас можуть попросити пояснити
  будь-який рядок свого коду і відповісти на запитання «що буде, якщо цей рядок змінити».

  ---------------------------------------------------------------
  РІВЕНЬ 1. Працівник і розробник — одиночне успадкування (обов'язковий)
  ---------------------------------------------------------------

  Клас Employee — працівник:
    - поля (protected): string name; int baseSalary;
    - конструктор Employee(const string& name, int baseSalary). Конструктора за замовчуванням НЕМАЄ;
    - string getName() const;
    - int getSalary() const — повертає baseSalary;
    - void print() const — формат: Олена (ставка: 30000 грн).

  Клас Developer : public Employee — розробник:
    - поля (private): string language; int tasksDone;
    - конструктор Developer(const string& name, int baseSalary, const string& language) — tasksDone = 0;
    - void completeTask() — збільшує tasksDone на 1;
    - int getTasksDone() const;
    - int getSalary() const — ставка + 1000 грн за кожну виконану задачу;
    - void print() const — викликає Employee::print() і дописує мову, кількість задач і зарплату:
      Олена (ставка: 30000 грн), C++, задач: 3, зарплата: 33000 грн.

  У main:
    1. Створіть двох розробників з різними мовами.
    2. Першому зарахуйте 3 виконані задачі, другому — 1.
    3. Виведіть обох через print().
    4. Виведіть ім'я першого через getName() — метод, який Developer успадкував від Employee.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Менеджер з командою — успадкування + агрегація (бажаний)
  ---------------------------------------------------------------

  Клас Manager : public Employee — менеджер (команда — як Group з уроку про агрегацію):
    - поля (private): Developer** team — динамічний масив покажчиків на розробників; int teamSize;
    - конструктор Manager(const string& name, int baseSalary) — команда порожня: team = nullptr, teamSize = 0;
    - копіювання заборонене: конструктор копіювання і operator= — = delete;
    - деструктор — звільняє ЛИШЕ масив покажчиків, а не самих розробників (їх створює main);
    - bool addToTeam(Developer* developer) — false, якщо developer == nullptr. Інакше створює новий масив
      на один елемент більший, копіює в нього покажчики, додає розробника, звільняє старий масив і повертає true;
    - int getTeamTasks() const — сума виконаних задач усієї команди;
    - int getSalary() const — ставка + 2000 грн за кожного розробника в команді;
    - void showTeam() const — виводить кожного розробника команди з нового рядка і загальну кількість задач команди;
    - void print() const — дані менеджера (через Employee::print()) і зарплата, а нижче — команда через showTeam().

  У main:
    1. Створіть менеджера і додайте в команду розробників з рівня 1 і ще одного нового.
    2. Новому розробнику зарахуйте 2 задачі ПІСЛЯ того, як додали його в команду.
    3. Виведіть менеджера. Задачі нового розробника мають порахуватися в задачах команди.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Тімлід — множинне успадкування і ромб (бонус)
  ---------------------------------------------------------------

  Тімлід пише код і водночас керує командою, тому він і Developer, і Manager.

    - Змініть Developer і Manager так, щоб вони успадковували Employee віртуально.
    - Клас TeamLead : public Developer, public Manager:
        - конструктор TeamLead(const string& name, int baseSalary, const string& language) —
          сам викликає конструктор Employee, а також конструктори Developer і Manager;
        - int getSalary() const — бонус за задачі + бонус за команду + ставка.
          Ставка має врахуватися ОДИН раз (підказка: скористайтеся Developer::getSalary() і Manager::getSalary());
        - void print() const — дані тімліда, його задачі і зарплата, а нижче — команда через showTeam().

  У main:
    1. Створіть тімліда, зарахуйте йому 2 задачі, додайте в його команду двох розробників.
    2. Виведіть тімліда.
    3. Виведіть його ім'я через getName().

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Чому поля Employee — protected, а не private? Що доведеться змінити в Developer, якщо зробити їх private?
  2. Що станеться, якщо в конструкторі Developer не викликати Employee(name, baseSalary) у списку ініціалізації? Чому?
  3. У якому порядку викликаються конструктори Employee і Developer під час створення розробника? А деструктори?
  4. Developer::getSalary() і Employee::getSalary() мають однакове ім'я. Яка версія викличеться для developer.getSalary()?
     Як усередині Developer викликати версію Employee?
  5. (Рівень 2) Manager «є» Employee і «має» розробників. Яке з цих відношень — успадкування, а яке — агрегація?
     Чому деструктор Manager робить delete[] team, але НЕ видаляє самих розробників?
  6. (Рівень 3) Скільки полів name було б у TeamLead без virtual? Чому без virtual рядок teamLead.getName()
     не компілюється?
  7. (Рівень 3) Чому TeamLead сам викликає конструктор Employee? Що станеться з викликами Employee(...)
     у конструкторах Developer і Manager під час створення тімліда?
*/

// ===== Ваш код =====

class Employee
{
protected:
    string name;
    int baseSalary;

public:
    Employee(const string& name, int baseSalary) : name(name), baseSalary(baseSalary)
    {
    }

    string getName() const
    {
        return name;
    }

    int getSalary() const
    {
        return baseSalary;
    }

    void print() const
    {
        cout << name << " (ставка: " << baseSalary << " грн)";
    }
};

// рівень 3: успадкування віртуальне, щоб у TeamLead був лише один Employee
class Developer : virtual public Employee
{
private:
    string language;
    int tasksDone;

public:
    Developer(const string& name, int baseSalary, const string& language)
        : Employee(name, baseSalary), language(language), tasksDone(0)
    {
    }

    void completeTask()
    {
        tasksDone++;
    }

    int getTasksDone() const
    {
        return tasksDone;
    }

    int getSalary() const
    {
        return Employee::getSalary() + tasksDone * 1000;
    }

    void print() const
    {
        Employee::print();
        cout << ", " << language << ", задач: " << tasksDone << ", зарплата: " << getSalary() << " грн";
    }
};

class Manager : virtual public Employee
{
private:
    Developer** team;   // агрегація: розробників створює main
    int teamSize;

public:
    Manager(const string& name, int baseSalary) : Employee(name, baseSalary), team(nullptr), teamSize(0)
    {
    }

    Manager(const Manager& other) = delete;
    Manager& operator=(const Manager& other) = delete;

    ~Manager()
    {
        delete[] team;   // тільки масив покажчиків
    }

    bool addToTeam(Developer* developer)
    {
        if (developer == nullptr)
        {
            return false;
        }
        Developer** temp = new Developer*[teamSize + 1];
        for (int i = 0; i < teamSize; i++)
        {
            temp[i] = team[i];
        }
        temp[teamSize] = developer;
        delete[] team;
        team = temp;
        teamSize++;
        return true;
    }

    int getTeamTasks() const
    {
        int total = 0;
        for (int i = 0; i < teamSize; i++)
        {
            total += team[i]->getTasksDone();
        }
        return total;
    }

    int getSalary() const
    {
        return Employee::getSalary() + teamSize * 2000;
    }

    void showTeam() const
    {
        cout << "  Команда:" << endl;
        if (teamSize == 0)
        {
            cout << "    (порожня)" << endl;
        }
        for (int i = 0; i < teamSize; i++)
        {
            cout << "    ";
            team[i]->print();
            cout << endl;
        }
        cout << "  Усього задач команди: " << getTeamTasks() << endl;
    }

    void print() const
    {
        Employee::print();
        cout << ", менеджер, зарплата: " << getSalary() << " грн" << endl;
        showTeam();
    }
};

class TeamLead : public Developer, public Manager
{
public:
    // Employee створює сам TeamLead, бо він віртуальний базовий клас
    TeamLead(const string& name, int baseSalary, const string& language)
        : Employee(name, baseSalary), Developer(name, baseSalary, language), Manager(name, baseSalary)
    {
    }

    int getSalary() const
    {
        // у кожній з двох сум є ставка, тому одну віднімаємо
        return Developer::getSalary() + Manager::getSalary() - baseSalary;
    }

    void print() const
    {
        Employee::print();
        cout << ", тімлід, власних задач: " << getTasksDone() << ", зарплата: " << getSalary() << " грн" << endl;
        showTeam();
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;
    Developer olena("Олена", 30000, "C++");
    Developer bohdan("Богдан", 27000, "Java");

    olena.completeTask();
    olena.completeTask();
    olena.completeTask();
    bohdan.completeTask();

    olena.print();
    cout << endl;
    bohdan.print();
    cout << endl;
    cout << "Ім'я першого розробника (getName з Employee): " << olena.getName() << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "===== РІВЕНЬ 2 =====" << endl;
    Developer sofia("Софія", 25000, "Python");

    Manager dmytro("Дмитро", 40000);
    dmytro.addToTeam(&olena);
    dmytro.addToTeam(&bohdan);
    dmytro.addToTeam(&sofia);
    if (!dmytro.addToTeam(nullptr))
    {
        cout << "nullptr у команду не додано" << endl;
    }

    // задачі зараховуємо вже ПІСЛЯ додавання в команду
    sofia.completeTask();
    sofia.completeTask();

    dmytro.print();
    // Manager copy = dmytro;   // помилка: копіювання заборонене

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3 =====" << endl;
    TeamLead ivan("Іван", 45000, "C#");
    ivan.completeTask();
    ivan.completeTask();
    ivan.addToTeam(&bohdan);
    ivan.addToTeam(&sofia);

    ivan.print();
    cout << "Ім'я тімліда: " << ivan.getName() << endl;

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. protected означає, що поля закриті для main, але нащадки (Developer, Manager) можуть до них звертатися
     напряму. Якщо зробити їх private, то в Developer вже не можна написати baseSalary чи name — треба буде
     брати їх через публічні методи Employee, наприклад getName() і Employee::getSalary(), або додати геттер для ставки.

  2. Буде помилка компіляції. Перед Developer обов'язково створюється його частина Employee, і якщо в списку
     ініціалізації нічого не вказано, компілятор пробує викликати Employee() без параметрів. А такого конструктора
     у Employee немає.

  3. Спочатку конструктор Employee (базова частина), потім конструктор Developer. Деструктори навпаки: спочатку
     ~Developer, потім ~Employee.

  4. Для developer.getSalary() викличеться версія Developer, бо вона «ховає» однойменний метод базового класу.
     Щоб усередині Developer викликати версію Employee, треба вказати клас явно: Employee::getSalary().

  5. «Є» — це успадкування (Manager : public Employee), «має» — агрегація (масив покажчиків team). Розробників
     створює main, і вони можуть бути одночасно в кількох командах, тому менеджер їх не володіє і не видаляє.
     Він звільняє тільки свій масив покажчиків, який сам виділив у addToTeam.

  6. Без virtual у TeamLead було б два поля name (і два baseSalary): одне з частини Developer, друге з частини
     Manager. Тоді teamLead.getName() неоднозначний — компілятор не знає, з якого саме Employee брати метод, і
     видає помилку ambiguous.

  7. При віртуальному успадкуванні Employee в TeamLead лише один, і його створює найпохідніший клас, тобто сам
     TeamLead. Виклики Employee(...) у списках ініціалізації Developer і Manager під час створення тімліда просто
     ігноруються. Вони спрацьовують лише тоді, коли окремо створюють Developer чи Manager.

*/
