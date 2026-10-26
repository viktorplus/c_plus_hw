#include <iostream>
#include <cstring>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                        ДОМАШНЄ ЗАВДАННЯ 24 тема: конструктор копіювання, константні методи, explicit.
==========================================================================================================================================

  Тема: музичний плейлист (клас Playlist).
  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус. Частина «ПОЯСНИ» обов'язкова для всіх.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Клас Playlist (обов'язковий)
  ---------------------------------------------------------------

  Поля (private):
    - char* name         — назва плейлиста, динамічний рядок;
    - int* durations     — динамічний масив тривалостей пісень у секундах;
    - int count          — кількість пісень.

  Конструктори і деструктор:
    - Playlist(const char* name) — порожній плейлист: durations = nullptr, count = 0;
        * виводить повідомлення:  [+] Ранкова
    - конструктор копіювання Playlist(const Playlist& other) — ГЛИБОКЕ копіювання:
        * для назви і для масиву durations виділити НОВУ пам'ять і скопіювати в неї дані з other;
        * якщо в other немає пісень (count == 0), durations = nullptr;
        * дані беріть через гетери other: other.getName(), other.getCount(), other.getDuration(i);
        * виводить повідомлення:  [=] Ранкова
    - деструктор звільняє обидва масиви і виводить:  [-] Ранкова

  Модифікатори (НЕ константні):
    - void setName(const char* name) — як setName у Student;
    - void add(int seconds) — додає пісню в кінець масиву. Значення <= 0 ігноруються.
        * створіть новий масив на 1 елемент більший, скопіюйте старі значення, додайте нове, звільніть старий масив.

  Інспектори (КОНСТАНТНІ):
    - const char* getName() const;
    - int getCount() const;
    - int getDuration(int index) const — тривалість пісні за індексом; для некоректного індексу повертає 0;
    - int getTotalSeconds() const      — загальна тривалість;
    - int getLongest() const           — найдовша пісня (0, якщо пісень немає);
    - void print() const               — назва і список пісень у форматі хв:сс, наприклад 3:05.
    *   Тіло print винесіть за межі класу.

  Функція поза класом:
    - void printSummary(const Playlist& playlist) — виводить назву, кількість пісень, загальну тривалість і найдовшу пісню.

  main:
    1. Створіть плейлист, додайте 3 пісні і одне некоректне значення (наприклад, -5). Виведіть плейлист.
    2. Створіть КОНСТАНТНИЙ плейлист як копію першого:  const Playlist favorite = morning;
       Виведіть його і його загальну тривалість.
    3. Спробуйте викликати  favorite.add(100);  Прочитайте помилку компілятора, закоментуйте рядок
       і поруч напишіть коментар, чому він не компілюється.
    4. Викличте printSummary для обох плейлистів.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Копіювання, explicit і функції з const& (бажаний)
  ---------------------------------------------------------------

  1. Незалежність копій. Створіть дві копії плейлиста morning двома різними записами:
         Playlist workout = morning;
         Playlist backup(morning);
     Змініть workout: нова назва і ще одна пісня. Виведіть усі три плейлисти і переконайтеся,
     що morning і backup не змінилися.

  2. Передача за значенням. Напишіть функцію  void printCopy(Playlist playlist):
     - додає до playlist пісню на 60 секунд;
     - виводить playlist через print.
     Викличте printCopy(morning), потім виведіть morning. Подивіться на [=] і [-] у виводі:
     коли створюється і коли знищується копія? Чи з'явилася пісня на 60 секунд у morning?

  3. explicit. Поки БЕЗ explicit викличте  printSummary("Вечірня");  Запустіть і подивіться на [+] / [-] у виводі.
     Потім додайте explicit до конструктора Playlist(const char* name) і виправте виклик так, щоб він компілювався.

  4. Напишіть функцію  const Playlist& longer(const Playlist& first, const Playlist& second),
     яка повертає плейлист з більшою загальною тривалістю. Виведіть результат:
         printSummary(longer(morning, workout));

  ---------------------------------------------------------------
  РІВЕНЬ 3. Об'єднання плейлистів (бонус)
  ---------------------------------------------------------------

  Напишіть функцію  Playlist combine(const Playlist& first, const Playlist& second):
     - створіть result як КОПІЮ first;
     - дайте йому назву "Мікс";
     - додайте до result усі пісні second (через second.getCount() і second.getDuration(i));
     - поверніть result.
  У main:  Playlist mix = combine(morning, workout);  Виведіть mix і переконайтеся, що morning і workout не змінилися.

*/

class Playlist
{
private:
    char* name;         //— назва плейлиста, динамічний рядок;
    int* durations;    //— динамічний масив тривалостей пісень у секундах;
    int count;          //— кількість пісень.

public:
    explicit Playlist(const char* name) : name(nullptr), durations(nullptr), count(0) {
        this->name = new char[strlen(name) + 1];
        strcpy_s(this->name, strlen(name) + 1, name);
        cout << "[+] " << this->name << endl;
    }

    Playlist(const Playlist& other) : name(nullptr), durations(nullptr), count(other.count) {
        size_t nameLength = strlen(other.getName()) + 1;
        name = new char[nameLength];
        strcpy_s(name, nameLength, other.getName());
        if (count > 0) {
            durations = new int[count];
            for (int i = 0; i < count; i++) {
                durations[i] = other.getDuration(i);
            }
        }
        cout << "[=] " << name << endl;
    }

    ~Playlist() {
        cout << "[-] " << name << endl;
        delete[] name;
        delete[] durations;
    }

    void setName(const char* name) {
        size_t  nameLength = strlen(name) + 1;
        char* newName = new char[nameLength];
        strcpy_s(newName, nameLength, name);
        delete[] this->name;
        this->name = newName;
    }

    void add(int seconds) {
        if (seconds <= 0) {
            return;
        }
        int* newDurations = new int[count + 1];
        for (int i = 0; i < count; i++) {
            newDurations[i] = durations[i];
        }
        newDurations[count] = seconds;
        delete[] durations;
        durations = newDurations;
        count++;
    }

    const char* getName() const {
        if (name == nullptr) {
            return "";
        }
        return name;
    }

    const int getCount() const {
        return count;
    }

    const int getDuration(int index) const {
        if (durations == nullptr) {
            return 0;
        }
        return (index >= 0 && index < count) ? durations[index] : 0;
    }

    int getTotalSeconds() const {
        int total = 0;
        for (int i = 0; i < count; i++) {
            total += durations[i];
        }
        return total;
    }

    int getLongest() const {
        if (count == 0) {
            return 0;
        }
        int longest = durations[0];
        for (int i = 1; i < count; i++) {
            if (durations[i] > longest) {
                longest = durations[i];
            }
        }
        return longest;
    }

    void  print() const;

    
};

void Playlist::print() const {
    cout << "Плейлист: " << getName() << endl;
    for (int i = 0; i < count; i++) {
        int minutes = durations[i] / 60;
        int seconds = durations[i] % 60;
        cout << "  Пісня " << (i + 1) << ": " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << endl;
    }
}

void printSummary(const Playlist& playlist) {
	cout << "Плейлист: " << playlist.getName() << endl;
	cout << "Кількість пісень: " << playlist.getCount() << endl;
	cout << "Загальна тривалість: " << playlist.getTotalSeconds() << " секунд" << endl;
	cout << "Найдовша пісня: " << playlist.getLongest() << " секунд" << endl;
}
void printCopy(Playlist playlist) {
    playlist.add(60);
    playlist.print();
}

const Playlist& longer(const Playlist& first, const Playlist& second) {
    return (first.getTotalSeconds() >= second.getTotalSeconds()) ? first : second;
}

Playlist combine(const Playlist& first, const Playlist& second){
	Playlist result(first); // створюємо копію first
    result.setName("Мікс"); // даємо нову назву
    for (int i = 0; i < second.getCount(); i++) {
	    result.add(second.getDuration(i)); // додаємо всі пісні з second
    }
    return result; 
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

	Playlist morning("Ранкова");
	morning.add(180);
	morning.add(240);
	morning.add(150);
	const Playlist favorite = morning;
	morning.print();
	morning.getTotalSeconds();
	//favorite.add(100); // помилка компілятора: add не константний метод, а favorite константний об'єкт

	printSummary(morning);
	printSummary(favorite);

	cout << endl << "====================" << endl;
	cout << "Рівень 2. Копіювання, explicit і функції з const&" << endl;
    Playlist workout = morning;
    Playlist backup(morning);


	workout.setName("Вечірня");
	workout.add(200);

	workout.print();
    backup.print();
	morning.print();

	cout << "Виклик printCopy(morning):" << endl;
	printCopy(morning); // створюється копія, додається пісня на 60 секунд, виводиться копія, потім знищується копія

	cout << "Після виклику printCopy(morning):" << endl;
    //printSummary("Вечірня"); //explicit перетворення рядка на Playlist не відбулося
	printSummary(Playlist("Вечірня")); // створюється тимчасовий об'єкт, виводиться, потім знищується.

	cout << "Плейлист з більшою загальною тривалістю:" << endl;
    printSummary(longer(morning, workout));

    cout << endl << "=========Playlist mix=========" << endl;

    Playlist mix = combine(morning, workout);
	mix.print();

	morning.print();
    workout.print();

    return 0;
}

/*
    ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Чому getTotalSeconds можна зробити константним, а add — ні?

  тому що getTotalSeconds не змінює стан об'єкта, а add змінює стан об'єкта (додає нову пісню до плейлиста).

  2. Який тип має покажчик this всередині print? Що через це НЕ можна робити в print?

  тип покажчика this всередині print має тип const Playlist*. 
  Через це в print не можна викликати неконстантні методи класу Playlist, оскільки вони можуть змінювати стан об'єкта.

  3. Чому getName повертає const char*, а не char*? Що можна було б зробити з константним плейлистом, якби повертався char*?

  getName повертає const char*, щоб запобігти зміні рядка, на який він вказує. 
  Якщо б він повертав char*, то можна було б змінювати назву плейлиста через цей покажчик, що порушувало б інкапсуляцію і безпеку даних.

  4. Що станеться, якщо в конструкторі копіювання замість виділення нової пам'яті написати  durations = other.durations; ?
     Коли саме програма «зламається»?

   Буде створено два об'єкти, які вказують на один і той же масив durations. 
   Коли один з об'єктів буде знищено, його деструктор звільнить пам'ять, а другий об'єкт залишиться з вказівником на вже звільнену пам'ять. Це призведе до помилок доступу до пам'яті (dangling pointer) і потенційного краху програми при спробі використати або звільнити цю пам'ять вдруге.

  5. (Рівень 2) Що ви побачили у виводі при виклику printCopy(morning)? Чому пісня на 60 секунд не з'явилася в morning?
     Чому printSummary приймає const Playlist&, а не Playlist?

    Тому що передача за константним посиланням дозволяє уникнути копіювання об'єкта і забезпечує, 
    що функція не змінює переданий плейлист.
    
  6. (Рівень 2) Що відбувалося в п. 3 БЕЗ explicit? Які конструктори і деструктори викликалися і коли?

  БЕЗ explicit конструктор Playlist(const char* name) компілятор автоматично 
  створював тимчасовий об'єкт Playlist з рядка "Вечірня" при виклику printSummary("Вечірня"). 
  а потім цей тимчасовий об'єкт знищувався після завершення виклику функції.

  7. (Рівень 2) Чому конструктор копіювання не роблять explicit? Назвіть рядок вашого main, який тоді перестав би компілюватися.

  Конструктор копіювання не роблять explicit, щоб дозволити автоматичне копіювання об'єктів у виразах і при передачі аргументів за значенням. 
  Якщо б конструктор копіювання був explicit, рядок у main, який викликає combine(morning, workout), перестав би компілюватися, 
  оскільки компілятор не зміг би неявно створити копії об'єктів для передачі у функцію.

*/