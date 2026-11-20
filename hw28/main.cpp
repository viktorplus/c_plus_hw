#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                             ДОМАШНЄ ЗАВДАННЯ 28 тема: клас string
==========================================================================================================================================

  Тема: два маленькі класи, які щодня потрібні в реальних програмах: робота з іменами файлів і розсилка повідомлень
  за шаблоном. З char* це були б десятки рядків з new, strcpy_s і strcat_s — зі string усе пишеться операторами і методами.
  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  Новий метод для цього завдання: s.rfind('.') — як find, але шукає з КІНЦЯ рядка, тобто знаходить ОСТАННЄ входження.
  Якщо не знайдено — так само повертає string::npos.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Клас FileName (обов'язковий)
  ---------------------------------------------------------------

  Поле (private): string name — повне ім'я файлу, наприклад "report.final.docx".
  Деструктор, конструктор копіювання і operator= НЕ пишіть (чому — запитання 3 у «ПОЯСНИ»).

  Методи:
    - конструктор FileName(const string& name);
    - string getName() const;
    - string getExtension() const — усе після ОСТАННЬОЇ крапки (rfind + substr): "report.final.docx" → "docx".
      Якщо крапки немає — повертає порожній рядок "";
    - string getBaseName() const — усе до останньої крапки: "report.final.docx" → "report.final".
      Якщо крапки немає — усе ім'я;
    - void changeExtension(const string& newExtension) — нове ім'я: getBaseName() + "." + newExtension;
    - bool isImage() const — true, якщо розширення "png", "jpg" або "gif" (порівняння через ==);
    - void print() const — виводить, наприклад:
          report.final.docx | ім'я: report.final | розширення: [docx]
      а для зображень додає в кінці: | зображення

  main:
    1. Створіть файли "report.final.docx", "photo.png", "README" і виведіть усі три.
    2. Змініть розширення report на "pdf", а README — на "md". Виведіть їх знову.
    3. FileName copy = report; змініть у copy розширення на "txt". Виведіть імена обох: оригінал не має змінитися.
    4. Порівняйте імена photo і report оператором < і виведіть, яке з них стоїть у списку раніше.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Розсилка за шаблоном (бажаний)
  ---------------------------------------------------------------

  Так працюють розсилки і Telegram-боти: один текст-шаблон, у який підставляються дані кожного отримувача.

  Клас MessageTemplate:
    - поле (private): string text — шаблон, наприклад:
          "Вітаю, {name}! Ваш бал за модуль: {score}. {name}, так тримати!"
    - конструктор MessageTemplate(const string& text);
    - private static-метод void replaceAll(string& text, const string& from, const string& to) — замінює в text
      КОЖНЕ входження from на to:
          position = text.find(from);
          поки position != string::npos:
              text.replace(position, from.length(), to);
              position = text.find(from, position + to.length());     // шукати далі ПІСЛЯ вставленого значення
    - int countPlaceholders() const — кількість символів '{' у шаблоні (цикл і []);
    - string fill(const string& name, int score) const — повертає КОПІЮ шаблону, у якій {name} замінено на name,
      а {score} — на score (to_string). Сам шаблон не змінюється.

  main: string names[3] = { "Олена", "Марко", "Тарас" }; int scores[3] = { 11, 9, 12 };
  Виведіть кількість місць для підстановки і повідомлення для кожного студента.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Розбір рядка з даними (бонус)
  ---------------------------------------------------------------

  Дані часто зберігають рядками, де поля розділені символом ';' (так виглядають файли .csv).
  Функція void parseStudent(const string& line) для рядка
      "Олена Коваль;20;olena.koval@itstep.org"
  знаходить обидва ';' (find, другий — починаючи з позиції після першого), вирізає три частини через substr,
  перетворює вік на int через stoi і виводить:
      Ім'я: Олена Коваль
      Вік: 20, наступного року: 21
      Пошта: olena.koval@itstep.org

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Як би ви писали getExtension, якби name був char*? Перелічіть кроки. Що з цього string зробив за вас?
  2. Що повертає rfind для "README"? Як ваш getExtension це обробляє?
  3. У FileName немає деструктора і конструктора копіювання. Чому FileName copy = report; все одно працює правильно
     і пам'ять не витікає?
  4. Чому string text = "Вітаю, " + "Олена"; не компілюється, а string text = "Вітаю, " + name; (name — string)
     компілюється?
  5. (Рівень 2) Чому в replaceAll шукаємо далі з position + to.length(), а не з початку рядка? Що станеться,
     якщо студента звуть "{name}"?
  6. (Рівень 3) Навіщо перетворювати вік через stoi? Чому не можна просто додати 1 до рядка "20"?
*/

// ===== Ваш код =====

// ---------------- РІВЕНЬ 1: клас FileName ----------------

class FileName
{
private:
    string name;

    // позиція останньої крапки (або string::npos, якщо крапки немає)
    size_t findLastDot() const
    {
        return name.rfind('.');
    }

public:
    FileName(const string& name) : name(name) {}

    string getName() const
    {
        return name;
    }

    string getExtension() const
    {
        size_t dotPos = findLastDot();
        if (dotPos == string::npos)
        {
            return "";                       // "README" — розширення немає
        }
        return name.substr(dotPos + 1);      // усе після крапки до кінця
    }

    string getBaseName() const
    {
        size_t dotPos = findLastDot();
        if (dotPos == string::npos)
        {
            return name;
        }
        return name.substr(0, dotPos);
    }

    void changeExtension(const string& newExtension)
    {
        name = getBaseName() + "." + newExtension;
    }

    bool isImage() const
    {
        string ext = getExtension();
        return ext == "png" || ext == "jpg" || ext == "gif";
    }

    void print() const
    {
        cout << "  " << name << " | ім'я: " << getBaseName() << " | розширення: [" << getExtension() << "]";
        if (isImage())
        {
            cout << " | зображення";
        }
        cout << endl;
    }
};

// ---------------- РІВЕНЬ 2: розсилка за шаблоном ----------------

class MessageTemplate
{
private:
    string text;

    static void replaceAll(string& text, const string& from, const string& to)
    {
        size_t position = text.find(from);
        while (position != string::npos)
        {
            text.replace(position, from.length(), to);
            // шукаємо далі вже після того, що вставили
            position = text.find(from, position + to.length());
        }
    }

public:
    MessageTemplate(const string& text) : text(text) {}

    int countPlaceholders() const
    {
        int count = 0;
        for (int i = 0; i < (int)text.length(); i++)
        {
            if (text[i] == '{')
            {
                count++;
            }
        }
        return count;
    }

    string fill(const string& name, int score) const
    {
        string message = text;               // працюємо з копією, шаблон лишається як був
        replaceAll(message, "{name}", name);
        replaceAll(message, "{score}", to_string(score));
        return message;
    }
};

// ---------------- РІВЕНЬ 3: розбір рядка з даними ----------------

void parseStudent(const string& line)
{
    size_t firstSep = line.find(';');
    size_t secondSep = line.find(';', firstSep + 1);

    string fullName = line.substr(0, firstSep);
    string ageText = line.substr(firstSep + 1, secondSep - firstSep - 1);
    string email = line.substr(secondSep + 1);

    int age = stoi(ageText);

    cout << "  Ім'я: " << fullName << endl;
    cout << "  Вік: " << age << ", наступного року: " << age + 1 << endl;
    cout << "  Пошта: " << email << endl;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;

    cout << "1. Три файли" << endl;
    FileName report("report.final.docx");
    FileName photo("photo.png");
    FileName readme("README");
    report.print();
    photo.print();
    readme.print();

    cout << "2. Міняємо розширення" << endl;
    report.changeExtension("pdf");
    readme.changeExtension("md");
    report.print();
    readme.print();

    cout << "3. Копія" << endl;
    FileName copy = report;
    copy.changeExtension("txt");
    cout << "  оригінал: " << report.getName() << endl;
    cout << "  копія:    " << copy.getName() << endl;

    cout << "4. Порівняння оператором <" << endl;
    if (photo.getName() < report.getName())
    {
        cout << "  Раніше у списку: " << photo.getName() << ", потім " << report.getName() << endl;
    }
    else
    {
        cout << "  Раніше у списку: " << report.getName() << ", потім " << photo.getName() << endl;
    }

    // запитання 4: так не компілюється — це два const char*, а не string
    // string wrong = "Вітаю, " + "Олена";
    string student = "Олена";
    string greeting = "Вітаю, " + student;
    cout << "  (запитання 4) " << greeting << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "===== РІВЕНЬ 2 =====" << endl;

    MessageTemplate message("Вітаю, {name}! Ваш бал за модуль: {score}. {name}, так тримати!");
    cout << "Місць для підстановки: " << message.countPlaceholders() << endl;

    string names[3] = { "Олена", "Марко", "Тарас" };
    int scores[3] = { 11, 9, 12 };
    for (int i = 0; i < 3; i++)
    {
        cout << "  " << message.fill(names[i], scores[i]) << endl;
    }

    // запитання 5: студент з дивним іменем — програма не зациклюється
    cout << "  (запитання 5) " << message.fill("{name}", 10) << endl;

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3 =====" << endl;
    parseStudent("Олена Коваль;20;olena.koval@itstep.org");

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. З char* треба було б пройти рядок з кінця циклом і знайти останню '.', потім порахувати довжину хвоста
     через strlen, виділити new char[...] під розширення, скопіювати його strcpy_s, а потім не забути delete[].
     А ще окремо обробити випадок, коли крапки немає. string зробив майже все сам: rfind шукає крапку, substr
     сам виділяє пам'ять і копіює, а звільняє її деструктор string.

  2. Для "README" крапки немає, тому rfind повертає string::npos. У getExtension я це перевіряю першим ділом
     і повертаю порожній рядок "". Якби не перевіряти, то npos + 1 дало б 0 і substr повернув би все ім'я.

  3. Бо єдине поле name — це string, а в string вже є правильні конструктор копіювання, operator= і деструктор.
     Компілятор сам генерує копіювання FileName, яке просто викликає копіювання string, і копія отримує свій
     окремий буфер. Коли об'єкт знищується, деструктор string сам звільняє пам'ять. Це і є правило нуля.

  4. "Вітаю, " і "Олена" — це не string, а звичайні масиви char, тобто const char*. Додавати два покажчики
     C++ не вміє, тому помилка. А коли один з операндів string, викликається operator+ для string,
     і літерал просто перетворюється на string.

  5. Якщо після заміни шукати знову з початку, то можна знайти щось усередині щойно вставленого тексту.
     Якщо студента звуть "{name}", то після заміни в тексті на тому ж місці знову з'явиться "{name}", і пошук
     з початку знаходив би його знову і знову — нескінченний цикл. А з position + to.length() ми перестрибуємо
     вставлене і йдемо далі (у main я це перевірив).

  6. "20" — це рядок, тобто символи '2' і '0', а не число. Для рядків + означає склеювання, а не додавання,
     тому порахувати 20 + 1 так не вийде. Спочатку треба перетворити текст у int через stoi, і тоді
     вже 20 + 1 = 21.

*/
