#include <iostream>
#include <cstring>
#include <string>
#include <initializer_list>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                   ДОМАШНЄ ЗАВДАННЯ 29 тема: шаблони класів, std::initializer_list, спеціалізація
==========================================================================================================================================

  Тема: шаблон класу Backpack<T> — рюкзак героя, у який можна покласти предмети будь-якого типу. Рюкзак сам збільшується,
  коли місце закінчується, — так влаштований vector, з яким ми познайомимось пізніше в курсі.
  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Шаблон класу Backpack<T> (обов'язковий)
  ---------------------------------------------------------------

  Поля (private):
    - T* items      — динамічний масив предметів;
    - int count     — скільки предметів лежить у рюкзаку;
    - int capacity  — на скільки предметів виділено пам'ять.

  Допоміжний private-метод void grow() — збільшує місткість УДВІЧІ: виділити новий масив на capacity * 2 елементів,
  скопіювати в нього count предметів, звільнити старий масив.

  Конструктори і деструктор:
    - Backpack() — порожній рюкзак, capacity = 2;
    - Backpack(initializer_list<T> list) — capacity = кількість елементів списку (але не менше 2),
      скопіювати всі елементи списку (цикл for по діапазону);
    - правило трьох: конструктор копіювання, operator= (з перевіркою самоприсвоєння, повертає Backpack&), деструктор.

  Методи і оператори:
    - void add(const T& item) — якщо count == capacity, спочатку викликати grow(); потім покласти предмет у кінець;
    - bool contains(const T& item) const — чи є такий предмет у рюкзаку (порівняння через ==);
    - int getCount() const та int getCapacity() const;
    - [] — дві версії: T& operator[](int index) і const T& operator[](int index) const;
    - дружній operator<< — пишіть ВСЕРЕДИНІ класу. Виводить предмети у квадратних дужках через кому: [10, 25, 5]

  main:
    1. Backpack<int> coins = { 10, 25, 5 }; — виведіть рюкзак, кількість і місткість. Додайте 50 і 100,
       виведіть знову. Як змінилася місткість?
    2. Перевірте coins.contains(25) і coins.contains(7).
    3. Створіть порожній Backpack<double> supplies; додайте 1.5, 2.25, 0.5 і виведіть рюкзак та місткість.
    4. Backpack<int> saved = coins; змініть coins[0] = 15 і додайте до coins ще одну монету. Виведіть обидва рюкзаки:
       копія має лишитися незмінною.
    5. Перевірте ланцюжок a = b = saved; і самоприсвоєння saved = saved;

  ---------------------------------------------------------------
  РІВЕНЬ 2. Свої предмети в рюкзаку (бажаний)
  ---------------------------------------------------------------

  1. Клас Item:
       - поля (private): string name; int weight;
       - конструктор Item(const string& name, int weight);
       - конструктор за замовчуванням Item() — порожня назва, вага 0 (навіщо він — запитання 4 у «ПОЯСНИ»);
       - int getWeight() const;
       - operator== — предмети однакові, якщо однакові назви;
       - дружній operator<< — виводить: Меч (5 кг)
  2. У main: Backpack<Item> hero = { Item("Меч", 5), Item("Щит", 8) }; додайте Item("Зілля", 1) і виведіть рюкзак.
     Перевірте, чи є в рюкзаку "Щит" і "Лук".
  3. Функція int totalWeight(const Backpack<Item>& backpack) — загальна вага предметів.
  4. Шаблон функції template <typename T> T total(const Backpack<T>& backpack) — сума всіх елементів
     (рюкзак вважайте непорожнім). Порахуйте суму монет у coins і вагу припасів у supplies.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Записки в рюкзаку (бонус)
  ---------------------------------------------------------------

  1. Backpack<const char*> notes = { "ключ від підвалу", "мапа" };
     char query[32];
     strcpy_s(query, "мапа");          // текст в ІНШОМУ масиві — так буває після введення з клавіатури
     Виведіть notes.contains(query). Результат вас здивує (запитання 5).
  2. Напишіть повну спеціалізацію методу contains для const char*, яка порівнює рядки через strcmp:
         template <>
         bool Backpack<const char*>::contains(const char* const& item) const
     (const T& при T = const char* — це саме const char* const&). Перевірте знову.
  3. Створіть Backpack<string> з тими самими записками і перевірте contains(query) — без жодної спеціалізації.

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Backpack<int> і Backpack<double> — це один клас чи два? Чому не компілюється supplies = coins; ?
  2. Чому add і contains приймають const T&, а не просто T?
  3. Навіщо рюкзаку два поля, count і capacity? Чому місткість збільшується вдвічі, а не на 1?
  4. (Рівень 2) Закоментуйте в Item конструктор за замовчуванням. Де компілятор покаже помилку і чому?
     Поверніть конструктор назад.
  5. (Рівень 3) Чому без спеціалізації contains не знаходить "мапа", хоча така записка в рюкзаку є?
     І чому для Backpack<string> спеціалізація не потрібна?
*/

// ===== Ваш код =====

// ---------------- РІВЕНЬ 1: шаблон Backpack<T> ----------------

template <typename T>
class Backpack
{
private:
    T* items;
    int count;
    int capacity;

    // збільшуємо місткість удвічі
    void grow()
    {
        int newCapacity = capacity * 2;
        T* bigger = new T[newCapacity];
        for (int i = 0; i < count; i++)
        {
            bigger[i] = items[i];
        }
        delete[] items;
        items = bigger;
        capacity = newCapacity;
    }

public:
    Backpack() : items(nullptr), count(0), capacity(2)
    {
        items = new T[capacity];
    }

    Backpack(initializer_list<T> list) : items(nullptr), count(0), capacity(0)
    {
        capacity = (int)list.size();
        if (capacity < 2)
        {
            capacity = 2;
        }
        items = new T[capacity];
        for (const T& item : list)
        {
            items[count] = item;
            count++;
        }
    }

    Backpack(const Backpack& other) : items(nullptr), count(other.count), capacity(other.capacity)
    {
        items = new T[capacity];
        for (int i = 0; i < count; i++)
        {
            items[i] = other.items[i];
        }
    }

    Backpack& operator=(const Backpack& other)
    {
        if (this == &other)
        {
            return *this;
        }
        T* copyItems = new T[other.capacity];
        for (int i = 0; i < other.count; i++)
        {
            copyItems[i] = other.items[i];
        }
        delete[] items;
        items = copyItems;
        count = other.count;
        capacity = other.capacity;
        return *this;
    }

    ~Backpack()
    {
        delete[] items;
    }

    void add(const T& item)
    {
        if (count == capacity)
        {
            grow();
        }
        items[count] = item;
        count++;
    }

    bool contains(const T& item) const
    {
        for (int i = 0; i < count; i++)
        {
            if (items[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    int getCount() const
    {
        return count;
    }

    int getCapacity() const
    {
        return capacity;
    }

    T& operator[](int index)
    {
        return items[index];
    }

    const T& operator[](int index) const
    {
        return items[index];
    }

    friend ostream& operator<<(ostream& out, const Backpack& backpack)
    {
        out << "[";
        for (int i = 0; i < backpack.count; i++)
        {
            if (i > 0)
            {
                out << ", ";
            }
            out << backpack.items[i];
        }
        out << "]";
        return out;
    }
};

// ---------------- РІВЕНЬ 3: спеціалізація contains для записок ----------------

// для const char* порівнюємо не адреси, а самі тексти
template <>
bool Backpack<const char*>::contains(const char* const& item) const
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(items[i], item) == 0)
        {
            return true;
        }
    }
    return false;
}

// ---------------- РІВЕНЬ 2: свої предмети ----------------

class Item
{
private:
    string name;
    int weight;

public:
    Item(const string& name, int weight) : name(name), weight(weight) {}

    // потрібен, бо Backpack робить new T[capacity]
    Item() : name(""), weight(0) {}

    int getWeight() const
    {
        return weight;
    }

    bool operator==(const Item& other) const
    {
        return name == other.name;
    }

    friend ostream& operator<<(ostream& out, const Item& item)
    {
        out << item.name << " (" << item.weight << " кг)";
        return out;
    }
};

int totalWeight(const Backpack<Item>& backpack)
{
    int sum = 0;
    for (int i = 0; i < backpack.getCount(); i++)
    {
        sum += backpack[i].getWeight();
    }
    return sum;
}

// рюкзак непорожній, тому починаємо з першого елемента
template <typename T>
T total(const Backpack<T>& backpack)
{
    T sum = backpack[0];
    for (int i = 1; i < backpack.getCount(); i++)
    {
        sum += backpack[i];
    }
    return sum;
}

// виводить "так" / "ні" замість 1 / 0
const char* yesNo(bool value)
{
    return value ? "так" : "ні";
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;

    cout << "1. Монети" << endl;
    Backpack<int> coins = { 10, 25, 5 };
    cout << "  " << coins << " кількість: " << coins.getCount() << ", місткість: " << coins.getCapacity() << endl;
    coins.add(50);
    coins.add(100);
    cout << "  " << coins << " кількість: " << coins.getCount() << ", місткість: " << coins.getCapacity() << endl;
    cout << "  (при додаванні 50 місце скінчилося, місткість стала 3 * 2 = 6)" << endl;

    cout << "2. contains" << endl;
    cout << "  є 25? " << yesNo(coins.contains(25)) << endl;
    cout << "  є 7?  " << yesNo(coins.contains(7)) << endl;

    cout << "3. Припаси" << endl;
    Backpack<double> supplies;
    cout << "  порожній: " << supplies << ", місткість: " << supplies.getCapacity() << endl;
    supplies.add(1.5);
    supplies.add(2.25);
    supplies.add(0.5);
    cout << "  " << supplies << ", місткість: " << supplies.getCapacity() << endl;

    // запитання 1: Backpack<int> і Backpack<double> — різні типи
    // supplies = coins;

    cout << "4. Копія" << endl;
    Backpack<int> saved = coins;
    coins[0] = 15;
    coins.add(1);
    cout << "  coins: " << coins << endl;
    cout << "  saved: " << saved << endl;

    cout << "5. Присвоєння" << endl;
    Backpack<int> a;
    Backpack<int> b = { 1, 2 };
    a = b = saved;
    saved = saved;
    cout << "  a: " << a << endl;
    cout << "  b: " << b << endl;
    cout << "  saved: " << saved << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "===== РІВЕНЬ 2 =====" << endl;

    Backpack<Item> hero = { Item("Меч", 5), Item("Щит", 8) };
    hero.add(Item("Зілля", 1));
    cout << "  Рюкзак героя: " << hero << endl;
    cout << "  є Щит? " << yesNo(hero.contains(Item("Щит", 0))) << endl;
    cout << "  є Лук? " << yesNo(hero.contains(Item("Лук", 0))) << endl;
    cout << "  Загальна вага: " << totalWeight(hero) << " кг" << endl;

    cout << "  Сума монет у coins: " << total(coins) << endl;
    cout << "  Вага припасів у supplies: " << total(supplies) << endl;

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3 =====" << endl;

    Backpack<const char*> notes = { "ключ від підвалу", "мапа" };
    char query[32];
    strcpy_s(query, "мапа");
    cout << "  Записки: " << notes << endl;

    // так порівнював би загальний contains — через ==, тобто адреси
    cout << "  notes[1] == query (як у загальному contains): " << yesNo(notes[1] == query) << endl;
    // а тут уже працює спеціалізація зі strcmp
    cout << "  notes.contains(query) зі спеціалізацією: " << yesNo(notes.contains(query)) << endl;

    Backpack<string> stringNotes = { "ключ від підвалу", "мапа" };
    cout << "  Backpack<string>: " << stringNotes << endl;
    cout << "  stringNotes.contains(query): " << yesNo(stringNotes.contains(query)) << endl;

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Це два різні класи. Шаблон — це тільки заготовка, а компілятор по ній генерує окремий клас для int і окремий
     для double. Між собою вони ніяк не пов'язані, тому operator= у Backpack<int> приймає тільки Backpack<int>,
     і supplies = coins не компілюється (немає перетворення з одного типу в інший).

  2. Бо T може бути чим завгодно, наприклад Item зі string всередині або великий об'єкт. Якщо приймати T,
     то при кожному виклику робилася б копія. const T& передає сам об'єкт без копіювання, а const гарантує,
     що метод його не змінить. Для int різниці майже немає, але шаблон пишемо одразу для всіх типів.

  3. count — скільки предметів реально лежить, а capacity — скільки місця вже виділено. Завдяки цьому add
     не виділяє пам'ять кожного разу, а просто кладе предмет у вільне місце. Якщо збільшувати на 1, то кожен
     add робив би new, копіювання всіх елементів і delete[] — дуже повільно. Коли збільшуємо вдвічі,
     перевиділення бувають рідко, а запас росте разом з рюкзаком.

  4. Помилка буде всередині шаблону Backpack, там де new T[capacity] (у конструкторах і в grow), коли компілятор
     створює Backpack<Item>. new T[n] має створити n об'єктів одразу, а без конструктора за замовчуванням
     Item не можна створити без параметрів. Компілятор пише, що немає відповідного конструктора за замовчуванням.

  5. Бо загальний contains порівнює через ==, а для const char* це порівняння адрес, а не тексту. Записка "мапа"
     у рюкзаку — це літерал в одному місці пам'яті, а query — окремий масив з тими самими буквами, адреси різні,
     тому false. Для string оператор == порівнює самі символи, тому все працює без спеціалізації
     (а query сам перетворюється на string).

*/
