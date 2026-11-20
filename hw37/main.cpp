#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                  ДОМАШНЄ ЗАВДАННЯ 37 тема: файлові потоки. Текстові та двійкові файли.
==========================================================================================================================================

  Тема: збереження гри. Гра, яка після закриття все забуває, нікому не потрібна: герой, журнал подій і пройдені рівні
  мають зберігатися у файлах і завантажуватися під час наступного запуску.

  Класи Hero, HeroStorage, LevelProgress і структура LevelResult уже частково написані нижче.
  Вам треба дописати методи і функцію там, де стоять коментарі НАПИШІТЬ, і main.
  Частина «ПОЯСНИ» обов'язкова: це питання, які часто ставлять на співбесідах та іспитах.

  Запустіть готову програму 2–3 рази поспіль: рівень героя має зростати, журнал — доповнюватися, а рекорди рівнів — зберігатися.
  Щоб почати гру спочатку, видаліть файли hero.txt, log.txt і progress.dat з папки проєкту.

  Можна користуватися ШІ, щоб розібратися в незрозумілому. Але на розборі вас можуть попросити пояснити
  будь-який рядок свого коду і відповісти на запитання «що буде, якщо цей рядок змінити».

  ---------------------------------------------------------------
  ЧАСТИНА 1. Текстові файли: герой і журнал подій
  ---------------------------------------------------------------

  Hero — клас даних, про файли він нічого не знає. Збереженням героя займається клас HeroStorage.

  1. Напишіть метод  void HeroStorage::save(const Hero& hero) const  — записує героя у файл одним рядком
     (старий вміст замінюється). Формат: рівень, золото, ім'я (ім'я може містити пробіл):   3 300 Сер Галахад
     Якщо файл не відкрився — кидає runtime_error.

  2. Напишіть метод  bool HeroStorage::load(Hero& hero) const  — читає героя з файлу.
     Якщо файлу немає — повертає false (збереження ще не було, і це не помилка). Якщо прочитати вдалося —
     записує в hero прочитаного героя (hero = Hero(ім'я, рівень, золото)) і повертає true.

  3. Напишіть функцію  void writeLog(const string& message)  — дописує message окремим рядком у кінець файлу "log.txt".
     Якщо файл не відкрився — кидає runtime_error.

  У main:
    1. Створіть HeroStorage для файлу "hero.txt". Якщо load повернув true — виведіть «Завантажено збереження».
       Інакше створіть героя «Сер Галахад» (рівень 1, золото 0) і виведіть «Нова гра».
    2. Викличте hero.levelUp() і hero.addGold(100), виведіть героя через print().
    3. Збережіть героя через save.
    4. Викличте writeLog з повідомленням на зразок «Сер Галахад досяг рівня 2».
    5. Прочитайте "log.txt" через getline і виведіть увесь журнал.

  ---------------------------------------------------------------
  ЧАСТИНА 2. Двійковий файл: рекорди рівнів
  ---------------------------------------------------------------

  У грі 5 рівнів. Для кожного у файлі зберігається LevelResult: кількість зірок (0–3) і найкращий час.
  Клас LevelProgress відповідає за цей файл. Метод createIfMissing уже написаний: якщо файлу немає,
  він створює його з порожніми результатами.

  1. Напишіть метод  int getLevelCount() const  — кількість рівнів у файлі (seekg, tellg).

  2. Напишіть метод  LevelResult readLevel(int index) const  — читає результат рівня з номером index (рахуючи з 0)
     одразу з потрібного місця файлу, не читаючи попередні.
     Якщо index менший за 0 або не менший за getLevelCount() — кидає out_of_range("Немає такого рівня").

  3. Напишіть метод  void saveResult(int index, const LevelResult& result) const  — через fstream:
       - читає старий результат рівня;
       - якщо в нового більше зірок — записує його на місце старого і виводить «Рівень 2: новий рекорд!»;
       - інакше нічого не записує і виводить «Рівень 2: рекорд не побито».
     (номер рівня для людей — index + 1)
  Якщо файл не відкрився, методи кидають runtime_error.

  У main:
    1. Створіть LevelProgress для файлу "progress.dat" і викличте createIfMissing(5).
    2. Збережіть результати: рівень 1 — 3 зірки за 45 с, рівень 2 — 1 зірка за 120 с,
       рівень 2 — 2 зірки за 95 с, рівень 1 — 2 зірки за 30 с.
    3. Виведіть усі рівні через readLevel і printLevel та загальну кількість зірок: «Зірок: 5 з 15».

  Увесь код main помістіть у try, а в catch (const exception& e) виведіть «Помилка: » і e.what().

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу — так, як відповідали б на співбесіді.

  1. Що таке потік? Чому з файлом працюють тими самими <<, >> і getline, що й з консоллю?
  2. Що сталося б із журналом, якби writeLog відкривала файл без ios::app? Хто і коли закриває файл,
     якщо не викликати close()?
  3. Навіщо окремий клас HeroStorage, якщо збереження можна написати прямо в Hero?
     Чому load повертає false, коли файлу немає, а save у такій ситуації кидає виняток?
  4. Чим двійковий файл відрізняється від текстового? Чому в progress.dat можна одразу перейти до рівня 4,
     а в log.txt одразу перейти до 4-го запису не вийде? Чому героя не можна записати у файл через write і reinterpret_cast,
     як LevelResult?
*/

// ==================== ГОТОВИЙ КОД ====================

// Клас даних: про файли нічого не знає
class Hero
{
private:
    string name;
    int level;
    int gold;

public:
    Hero(const string& name, int level, int gold) : name(name), level(level), gold(gold)
    {}

    Hero() : Hero("", 1, 0)
    {}

    string getName() const { return name; }
    int getLevel() const { return level; }
    int getGold() const { return gold; }

    void levelUp()
    {
        level++;
    }

    void addGold(int amount)
    {
        gold += amount;
    }

    void print() const
    {
        cout << name << ": рівень " << level << ", золото " << gold << endl;
    }
};

// Клас збереження героя в текстовий файл
class HeroStorage
{
private:
    string fileName;

public:
    explicit HeroStorage(const string& fileName) : fileName(fileName)
    {}

    // Записує героя одним рядком: рівень золото ім'я (старий вміст файлу стирається)
    void save(const Hero& hero) const
    {
        ofstream out(fileName);
        if (!out.is_open())
        {
            throw runtime_error("Не вдалося відкрити файл " + fileName + " для запису");
        }
        out << hero.getLevel() << " " << hero.getGold() << " " << hero.getName() << endl;
    }

    // Читає героя з файлу. Немає файлу — немає збереження, повертаємо false
    bool load(Hero& hero) const
    {
        ifstream in(fileName);
        if (!in.is_open())
        {
            return false;
        }

        int level = 0;
        int gold = 0;
        string name;
        in >> level >> gold;
        in.ignore();                          // пропускаємо пробіл після золота
        getline(in, name);                    // ім'я може бути з пробілом, тому getline

        if (in.fail() || name.empty())        // файл порожній або зіпсований
        {
            return false;
        }
        hero = Hero(name, level, gold);
        return true;
    }
};

// Результат одного рівня: лише int, тому його можна записувати у двійковий файл байтами як є
struct LevelResult
{
    int stars;                                // 0–3
    int bestTime;                             // секунди
};

void printLevel(const LevelResult& result, int index)
{
    cout << "Рівень " << index + 1 << ": зірок " << result.stars << ", час " << result.bestTime << " с" << endl;
}

// Клас збереження рекордів рівнів у двійковий файл
class LevelProgress
{
private:
    string fileName;

public:
    explicit LevelProgress(const string& fileName) : fileName(fileName)
    {}

    // Якщо файлу ще немає — створює його з порожніми результатами для count рівнів
    void createIfMissing(int count) const
    {
        ifstream check(fileName, ios::binary);
        if (check.is_open())
        {
            return;                           // файл уже є — нічого не робимо
        }
        ofstream file(fileName, ios::binary);
        if (!file.is_open())
        {
            throw runtime_error("Не вдалося створити " + fileName);
        }
        LevelResult empty = { 0, 0 };
        for (int i = 0; i < count; i++)
        {
            file.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        }
    }

    // Кількість рівнів = розмір файлу / розмір одного запису
    int getLevelCount() const
    {
        ifstream in(fileName, ios::binary);
        if (!in.is_open())
        {
            throw runtime_error("Не вдалося відкрити файл " + fileName);
        }
        in.seekg(0, ios::end);                         // стаємо в кінець файлу
        int bytes = (int)in.tellg();                   // номер байта в кінці = розмір файлу
        return bytes / (int)sizeof(LevelResult);
    }

    // Читає один рівень одразу з потрібного місця, попередні не читаємо
    LevelResult readLevel(int index) const
    {
        if (index < 0 || index >= getLevelCount())
        {
            throw out_of_range("Немає такого рівня");
        }
        ifstream in(fileName, ios::binary);
        if (!in.is_open())
        {
            throw runtime_error("Не вдалося відкрити файл " + fileName);
        }
        LevelResult result = { 0, 0 };
        in.seekg(index * sizeof(LevelResult), ios::beg);
        in.read(reinterpret_cast<char*>(&result), sizeof(result));
        return result;
    }

    // Записує новий результат, тільки якщо в ньому більше зірок, ніж у старому
    void saveResult(int index, const LevelResult& result) const
    {
        if (index < 0 || index >= getLevelCount())
        {
            throw out_of_range("Немає такого рівня");
        }
        fstream file(fileName, ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            throw runtime_error("Не вдалося відкрити файл " + fileName);
        }

        int position = index * (int)sizeof(LevelResult);
        LevelResult old = { 0, 0 };
        file.seekg(position, ios::beg);
        file.read(reinterpret_cast<char*>(&old), sizeof(old));

        if (result.stars > old.stars)
        {
            file.seekp(position, ios::beg);            // після read позиція зсунулась, повертаємося назад
            file.write(reinterpret_cast<const char*>(&result), sizeof(result));
            cout << "Рівень " << index + 1 << ": новий рекорд!" << endl;
        }
        else
        {
            cout << "Рівень " << index + 1 << ": рекорд не побито" << endl;
        }
    }
};

// ===== Ваш код =====

// Дописує повідомлення в кінець журналу
void writeLog(const string& message)
{
    ofstream out("log.txt", ios::app);
    if (!out.is_open())
    {
        throw runtime_error("Не вдалося відкрити log.txt");
    }
    out << message << endl;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    try
    {
        // ---------------- ЧАСТИНА 1 ----------------
        cout << "---------------- ЧАСТИНА 1: герой і журнал ----------------" << endl;

        // 1. Завантаження або нова гра
        HeroStorage storage("hero.txt");
        Hero hero;
        if (storage.load(hero))
        {
            cout << "Завантажено збереження" << endl;
        }
        else
        {
            hero = Hero("Сер Галахад", 1, 0);
            cout << "Нова гра" << endl;
        }

        // 2. Герой росте
        hero.levelUp();
        hero.addGold(100);
        hero.print();

        // 3. Зберігаємо
        storage.save(hero);
        cout << "Героя збережено в hero.txt" << endl;

        // 4. Запис у журнал
        writeLog(hero.getName() + " досяг рівня " + to_string(hero.getLevel()));

        // 5. Виводимо весь журнал
        ifstream logFile("log.txt");
        if (!logFile.is_open())
        {
            throw runtime_error("Не вдалося відкрити log.txt");
        }
        cout << endl << "Журнал подій:" << endl;
        string line;
        while (getline(logFile, line))
        {
            cout << "  " << line << endl;
        }
        logFile.close();

        // ---------------- ЧАСТИНА 2 ----------------
        cout << endl << "---------------- ЧАСТИНА 2: рекорди рівнів ----------------" << endl;

        // 1. Файл з рекордами
        LevelProgress progress("progress.dat");
        progress.createIfMissing(5);
        cout << "Рівнів у файлі: " << progress.getLevelCount() << endl;

        // 2. Результати проходження
        progress.saveResult(0, { 3, 45 });
        progress.saveResult(1, { 1, 120 });
        progress.saveResult(1, { 2, 95 });
        progress.saveResult(0, { 2, 30 });

        // 3. Усі рівні і загальна кількість зірок
        cout << endl;
        int count = progress.getLevelCount();
        int stars = 0;
        for (int i = 0; i < count; i++)
        {
            LevelResult result = progress.readLevel(i);
            printLevel(result, i);
            stars += result.stars;
        }
        cout << "Зірок: " << stars << " з " << count * 3 << endl;

        // Перевірка винятку: рівня 6 немає
        cout << endl << "Пробуємо прочитати рівень 6:" << endl;
        progress.readLevel(5);
    }
    catch (const exception& e)
    {
        cout << "Помилка: " << e.what() << endl;
    }

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Потік — це послідовність байтів, яка йде від джерела до приймача: з клавіатури в програму, з програми на екран,
     з файлу в програму і навпаки. ifstream і ofstream — нащадки istream і ostream, так само як cin і cout.
     Тому оператори <<, >> і getline, написані для istream/ostream, працюють і з файлом: їм не важливо, куди саме йдуть дані.

  2. Без ios::app ofstream відкриває файл у режимі «з нуля» — старий вміст стирається. Тоді в журналі завжди був би
     тільки один, останній запис. Якщо не викликати close(), файл закриє деструктор потоку, коли об'єкт вийде
     з області видимості (наприклад, у кінці функції writeLog), і заодно скине буфер у файл.

  3. Hero відповідає лише за дані героя, а HeroStorage — лише за те, як і куди їх зберігати (принцип однієї відповідальності).
     Якщо захочемо зберігати в інший формат чи базу даних, Hero не доведеться чіпати. Відсутність файлу при load — нормальна
     ситуація (перший запуск, збереження ще немає), тому це просто false. А якщо save не зміг відкрити файл, то прогрес
     гравця пропаде — це справжня помилка, і про неї треба повідомити винятком.

  4. У текстовому файлі числа записані символами ("300" — це три символи, а 5 — один), тому записи мають різну довжину.
     У двійковому файлі значення записані байтами так, як вони лежать у пам'яті, і кожен LevelResult займає рівно sizeof(LevelResult)
     байтів. Тому позицію рівня 4 можна порахувати: 3 * sizeof(LevelResult) і перейти туди через seekg. У log.txt рядки різної
     довжини, і щоб знайти 4-й, треба прочитати три попередні. Героя через write записати не можна, бо в ньому є string:
     у пам'яті об'єкта лежить не сам текст, а покажчик на нього в купі. У файл потрапила б адреса, яка при наступному
     запуску вже нічого не означає.

*/
