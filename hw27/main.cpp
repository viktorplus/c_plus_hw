#include <iostream>
#include <cstring>
#include <utility>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                ДОМАШНЄ ЗАВДАННЯ 27 тема: перевантаження операторів (підсумок модуля 17).
==========================================================================================================================================

  Тема: власний клас рядка MyString — спрощена версія того, як влаштований std::string (модуль 18).
  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Клас MyString (обов'язковий)
  ---------------------------------------------------------------

  Поля (private):
    - char* data   — динамічний рядок;
    - int length   — кількість символів (без '\0').

  Допоміжний private-метод void assign(const char* text) — як setName у Student: звільнити старий рядок, виділити нову пам'ять,
  скопіювати text, оновити length.

  Конструктори і деструктор:
    - MyString(const char* text) — БЕЗ explicit (чому — запитання 1 у «ПОЯСНИ»); якщо text == nullptr, вважайте його "";
    - MyString() — порожній рядок "" (делегування конструктору з const char*);
    - правило трьох: конструктор копіювання, operator= (з перевіркою самоприсвоєння, повертає MyString&), деструктор.

  Методи і оператори:
    - int getLength() const;
    - [] — дві версії: char& operator[](int index) і char operator[](int index) const;
    - operator+=(const MyString& other) — дописує other у кінець:
        * виділіть новий буфер на length + other.length + 1 символів, скопіюйте в нього старий рядок (strcpy_s),
          допишіть other (strcat_s), і лише ПІСЛЯ цього звільніть старий буфер;
        * повертає MyString&;
    - operator+(const MyString& other) const — метод-член, НОВИЙ рядок; реалізуйте через += (як у Fraction);
    - operator== та operator!= (порівняння через strcmp);
    - дружній operator<< — виводить рядок. Якщо data == nullptr (знадобиться на рівні 2), нічого не виводить.

  main:
    1. MyString greeting("Hello"); MyString copy = greeting; змініть copy[0] = 'J'; виведіть обидва рядки.
    2. Створіть порожній рядок, виведіть його в квадратних дужках [ ] і його довжину.
    3. MyString name("World"); MyString message = greeting + ", " + name + "!"; — виведіть message і його довжину.
       Допишіть до name "!!!" через += і виведіть.
    4. Перевірте greeting == "Hello" і copy != greeting.
    5. Перевірте ланцюжок a = b = c; і самоприсвоєння c = c;
    6. Створіть const MyString title("OOP"); виведіть title[0]. Спробуйте title[0] = 'o'; — прочитайте помилку і закоментуйте.
    7. Виведіть довжину MyString("Привіт"). Поясніть результат (запитання 4).

  ---------------------------------------------------------------
  РІВЕНЬ 2. Перенесення і сортування (бажаний)
  ---------------------------------------------------------------

  1. Конструктор перенесення і operator= перенесенням (правило п'яти): забрати data і length, в other — data = nullptr, length = 0.
  2. operator< (через strcmp) — для сортування.
  3. У main: MyString names[5] = { "Олена", "Марко", "Анна", "Тарас", "Богдан" };
     відсортуйте масив бульбашкою. Обмін двох рядків — через std::move:
         MyString temp = std::move(names[j]);
         names[j] = std::move(names[j + 1]);
         names[j + 1] = std::move(temp);
     Виведіть відсортований масив.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Рядок ліворуч, >> і bool (бонус)
  ---------------------------------------------------------------

  1. Спробуйте MyString hello = "Hello, " + name; — не компілюється. Замініть метод operator+ ДРУЖНЬОЮ функцією
     MyString operator+(const MyString& left, const MyString& right) і перевірте знову.
  2. Дружній operator>> — читає одне слово в буфер char buffer[256], потім присвоює його рядку: s = buffer;
  3. explicit operator bool() const — true, якщо рядок не порожній. Перевірте if (!empty) і if (input).

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. У модулі 16 радили робити конструктор з одним параметром explicit. Чому MyString(const char*) — без explicit?
     Які рядки вашого main перестануть компілюватися, якщо його додати?
  2. Що станеться при b = a, якщо не написати власний operator=? Коли саме програма «зламається»?
  3. Навіщо дві версії []? Що буде, якщо залишити лише неконстантну?
  4. Чому довжина "Привіт" не 6?
  5. (Рівень 2) Чому обмін рядків у сортуванні зроблено через std::move? Що відбувалося б при кожному обміні,
     якби в класі не було методів перенесення?
  6. (Рівень 3) Чому "Hello, " + name не компілювалося, коли + був методом, і компілюється з дружньою функцією?
*/

// ===== Ваш код =====

class MyString
{
private:
    char* data;
    int length;

    void assign(const char* text)
    {
        int newLength = (int)strlen(text);
        char* newData = new char[newLength + 1];
        strcpy_s(newData, newLength + 1, text);
        delete[] data;
        data = newData;
        length = newLength;
    }

public:
    //   1. У модулі 16 радили робити конструктор з одним параметром explicit. Чому MyString(const char*) — без explicit?
    //   Які рядки вашого main перестануть компілюватися, якщо його додати ?
    //   
	//   тому що ми хочемо, щоб рядки могли автоматично перетворюватися на MyString, наприклад, у виразі greeting == "Hello"
    //   або при створенні об'єкта MyString з рядка літерала. Якщо додати explicit, ці автоматичні перетворення не будуть працювати,
    //   і рядки main, де використовується порівняння з рядком літералом або створення об'єкта MyString з рядка, перестануть компілюватися.

    MyString(const char* text) : data(nullptr), length(0)
    {
        if (text == nullptr)
        {
            text = "";
        }
        assign(text);
    }

    MyString() : MyString("") {}

    MyString(const MyString& other) : data(nullptr), length(0)
    {
        assign(other.data);
    }

    //  2. Що станеться при b = a, якщо не написати власний operator=? Коли саме програма «зламається»?
    //
	//  Якщо не написати власний operator=, компілятор згенерує стандартний оператор присвоєння, який виконає поверхневе копіювання полів.
	//  Це призведе до того, що обидва об'єкти будуть вказувати на один і той же динамічно виділений рядок. Коли один з об'єктів буде знищено,
	//  його деструктор звільнить пам'ять, і другий об'єкт залишиться з вказівником на вже звільнену пам'ять. Це призведе до помилок доступу до пам'яті та потенційних аварій програми.

    MyString& operator=(const MyString& other)
    {
        if (this == &other)
        {
            return *this;
        }
        assign(other.data);
        return *this;
    }

    ~MyString()
    {
        delete[] data;
    }

    // ===== Рівень 2: перенесення (правило п'яти) =====
    MyString(MyString&& other) : data(other.data), length(other.length)
    {
        other.data = nullptr;
        other.length = 0;
    }

    MyString& operator=(MyString&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete[] data;
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
        return *this;
    }

    int getLength() const
    {
        return length;
    }

    //  3. Навіщо дві версії []? Що буде, якщо залишити лише неконстантну?
	//  Дві версії [] потрібні для того, щоб забезпечити доступ до елементів рядка як для константних, так і для неконстантних об'єктів.
	//  Неконстантна версія повертає посилання на символ, що дозволяє змінювати його, тоді як константна версія повертає копію символу, що дозволяє лише читати його.
    char& operator[](int index)
    {
        return data[index];
    }

    char operator[](int index) const
    {
        return data[index];
    }

    MyString& operator+=(const MyString& other)
    {
        int newLength = length + other.length;
        char* newData = new char[newLength + 1];
        strcpy_s(newData, newLength + 1, data);
        strcat_s(newData, newLength + 1, other.data);
        delete[] data;
        data = newData;
        length = newLength;
        return *this;
    }

    // замінено дружньою функцією
    // MyString operator+(const MyString& other) const
    // {
    //     MyString result = *this;
    //     result += other;
    //     return result;
    // }

    bool operator==(const MyString& other) const
    {
        return strcmp(data, other.data) == 0;
    }

    bool operator!=(const MyString& other) const
    {
        return !(*this == other);
    }

    bool operator<(const MyString& other) const
    {
        return strcmp(data, other.data) < 0;
    }

    explicit operator bool() const
    {
        return length > 0;
    }

    friend ostream& operator<<(ostream& out, const MyString& s);
    friend istream& operator>>(istream& in, MyString& s);
    friend MyString operator+(const MyString& left, const MyString& right);
};

ostream& operator<<(ostream& out, const MyString& s)
{
    if (s.data != nullptr)
    {
        out << s.data;
    }
    return out;
}

istream& operator>>(istream& in, MyString& s)
{
    char buffer[256];
    in >> buffer;
    s = buffer;
    return in;
}

// 6. (Рівень 3) Чому "Hello, " + name не компілювалося, коли + був методом, і компілюється з дружньою функцією?
//
// Коли operator+ був методом-членом, він міг бути викликаний лише на об'єкті MyString, тобто у вигляді name + "Hello, ".
MyString operator+(const MyString& left, const MyString& right)
{
    MyString result = left;
    result += right;
    return result;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "1. Копія незалежна від оригіналу" << endl;
    MyString greeting("Hello");
    MyString copy = greeting;
    copy[0] = 'J';
    cout << "  " << greeting << " / " << copy << endl;                     // Hello / Jello

    cout << "2. Порожній рядок" << endl;
    MyString empty;
    cout << "  [" << empty << "], довжина " << empty.getLength() << endl;  // [], 0

    cout << "3. Склеювання" << endl;
    MyString name("World");
    MyString message = greeting + ", " + name + "!";
    cout << "  " << message << ", довжина " << message.getLength() << endl; // Hello, World!, 13
    name += "!!!";
    cout << "  " << name << endl;                                           // World!!!

    cout << "4. Порівняння з неявним перетворенням" << endl;
    if (greeting == "Hello")
    {
        cout << "  greeting == \"Hello\"" << endl;
    }
    if (copy != greeting)
    {
        cout << "  copy != greeting" << endl;
    }

    cout << "5. Присвоєння" << endl;
    MyString a("one");
    MyString b("two");
    MyString c("three");
    a = b = c;
    c = c;
    cout << "  " << a << " " << b << " " << c << endl;

    cout << "6. Константний рядок" << endl;
    const MyString title("OOP");
    cout << "  title[0] = " << title[0] << endl;

	cout << "7. Довжина кирилиці" << endl;      // 4. Чому довжина "Привіт" не 6? - 
    // тому що в кодуванні UTF-8 деякі символи кирилиці займають більше одного байта, тому 
   //  strlen повертає кількість байтів, а не символів.
    MyString word("Привіт");
    cout << "  \"" << word << "\" має довжину " << word.getLength() << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    // 5. (Рівень 2) Чому обмін рядків у сортуванні зроблено через std::move? Що відбувалося б при кожному обміні,
    // якби в класі не було методів перенесення ?

	// Обмін рядків у сортуванні зроблено через std::move, щоб скористатися конструктором перенесення та оператором присвоєння перенесенням.
	// Це дозволяє ефективно переміщувати ресурси (в даному випадку динамічно виділену пам'ять) з одного об'єкта в інший без необхідності копіювання даних.
	// Якщо б у класі не було методів перенесення, кожен обмін призводив би до копіювання рядків, що значно зменшило б продуктивність сортування, особливо для великих рядків.

    cout << "8. Рівень 2: сортування з перенесенням" << endl;
    MyString names[5] = { "Олена", "Марко", "Анна", "Тарас", "Богдан" };
    for (int i = 0; i < 5 - 1; i++)
    {
        for (int j = 0; j < 5 - 1 - i; j++)
        {
            if (names[j + 1] < names[j])
            {
                MyString temp = std::move(names[j]);
                names[j] = std::move(names[j + 1]);
                names[j + 1] = std::move(temp);
            }
        }
    }
    cout << "  ";
    for (int i = 0; i < 5; i++)
    {
        cout << names[i] << " ";
    }
    cout << endl;

    // ---------------- РІВЕНЬ 3 ----------------
    cout << "9. Рівень 3: рядок ліворуч" << endl;
    MyString hello = "Hello, " + name;
    cout << "  " << hello << endl;

    cout << "10. Рівень 3: operator bool і >>" << endl;
    if (!empty)
    {
        cout << "  empty порожній" << endl;
    }
    MyString input;
    cout << "  Введіть слово: ";
    cin >> input;
    if (input)
    {
        cout << "  Ви ввели: " << input << " (довжина " << input.getLength() << ")" << endl;
    }

    return 0;
}

/*
  1. У модулі 16 радили робити конструктор з одним параметром explicit. Чому MyString(const char*) — без explicit?
     Які рядки вашого main перестануть компілюватися, якщо його додати?
  2. Що станеться при b = a, якщо не написати власний operator=? Коли саме програма «зламається»?
  3. Навіщо дві версії []? Що буде, якщо залишити лише неконстантну?
  4. Чому довжина "Привіт" не 6?
  5. (Рівень 2) Чому обмін рядків у сортуванні зроблено через std::move? Що відбувалося б при кожному обміні,
     якби в класі не було методів перенесення?
  6. (Рівень 3) Чому "Hello, " + name не компілювалося, коли + був методом, і компілюється з дружньою функцією?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Тому що ми хочемо, щоб рядки могли автоматично перетворюватися на MyString, наприклад, у виразі greeting == "Hello"
	 або при створенні об'єкта MyString з рядка літерала. Якщо додати explicit, ці автоматичні перетворення не будуть працювати,
	 і рядки main, де використовується порівняння з рядком літералом або створення об'єкта MyString з рядка, перестануть компілюватися.

  2. Написаний власний operator= виконує глибоке копіювання рядка, виділяючи нову пам'ять і копіюючи дані. Якщо його не написати,
     компілятор згенерує стандартний оператор присвоєння, який виконає поверхневе копіювання полів. Це призведе до того, що обидва
     об'єкти будуть вказувати на один і той же динамічно виділений рядок. Коли один з об'єктів буде знищено, його деструктор звільнить пам'ять,
     і другий об'єкт залишиться з вказівником на вже звільнену пам'ять. Це призведе до помилок доступу до пам'яті та потенційних аварій програми.

  3. Кожна версія [] потрібна для того, щоб забезпечити доступ до елементів рядка як для константних, так і для неконстантних об'єктів.
	 Неконстантна версія повертає посилання на символ, що дозволяє змінювати його, тоді як константна версія повертає копію символу,
	 що дозволяє лише читати його. Якщо залишити лише неконстантну версію, то при спробі доступу до елементів константного об'єкта
	 виникне помилка компіляції.

  4. Тому що в кодуванні UTF-8 деякі символи кирилиці займають більше одного байта, тому strlen повертає кількість байтів, а не символів.

  5. Обмін рядків у сортуванні зроблено через std::move для оптимізації продуктивності. Якщо б використовувався звичайний оператор присвоєння,
     відбувалося б копіювання рядків, що може бути повільніше, особливо для великих рядків. Використання std::move дозволяє перенести ресурси
     без копіювання, що робить обмін більш ефективним.

  6. Тому що operator+ був методом-членом, він міг бути викликаний лише на об'єкті MyString, тобто у вигляді name + "Hello, ".
*/
