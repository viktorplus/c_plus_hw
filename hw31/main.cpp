#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                    ДОМАШНЄ ЗАВДАННЯ 31 тема: вкладені класи, композиція та агрегація.
==========================================================================================================================================

  Тема: музичний плеєр. Пісні створюються в програмі, а плейлисти лише посилаються на них,
  тому одна пісня може бути в кількох плейлистах одночасно.

  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  Можна користуватися ШІ, щоб розібратися в незрозумілому. Але на розборі вас можуть попросити пояснити
  будь-який рядок свого коду і відповісти на запитання «що буде, якщо цей рядок змінити».

  ---------------------------------------------------------------
  РІВЕНЬ 1. Пісня і її тривалість — вкладений клас і композиція (обов'язковий)
  ---------------------------------------------------------------

  Клас Song з public-вкладеним класом Duration (тривалість пісні).

  Клас Song::Duration:
    - поля (private): int minutes; int seconds;
    - конструктор Duration(int minutes, int seconds). Конструктора за замовчуванням НЕМАЄ.
      Від'ємні значення замінюються на 0. Якщо seconds >= 60, зайві секунди переносяться у хвилини:
      Duration(1, 130) → 3 хв 10 с;
    - int getTotalSeconds() const — загальна кількість секунд;
    - Duration operator+(const Duration& other) const — повертає нову тривалість, що дорівнює сумі двох
      (підказка: конструктор уже вміє переносити секунди у хвилини);
    - void print() const — формат 3:07 (секунди завжди двома цифрами).

  Клас Song:
    - поля (private): string title; string artist; Duration duration;
    - конструктор Song(const string& title, const string& artist, int minutes, int seconds) —
      поле duration ініціалізується у списку ініціалізації;
    - Duration getDuration() const;
    - void print() const — формат: Гурт Світанок — Ранкова кава (3:07).

  У main:
    1. Створіть дві пісні: «Ранкова кава» (Гурт Світанок, 3 хв 7 с) і «Нічне місто» (Ліна Вітер, 0 хв 250 с).
    2. Виведіть обидві пісні. Друга має вивестися з тривалістю 4:10.
    3. Створіть змінну Song::Duration total — суму тривалостей обох пісень через operator+, і виведіть її.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Плейлист — агрегація (бажаний)
  ---------------------------------------------------------------

  Клас Playlist (як Group з уроку):
    - поля (private): string name; Song** songs — динамічний масив покажчиків на пісні; int count;
    - конструктор Playlist(const string& name) — плейлист порожній: songs = nullptr, count = 0;
    - копіювання заборонене: конструктор копіювання і operator= — = delete;
    - деструктор — звільняє ЛИШЕ масив покажчиків, а не самі пісні;
    - bool add(Song* song) — повертає false, якщо song == nullptr або ЦЯ САМА пісня вже є в плейлисті
      (порівнюйте покажчики). Інакше створює новий масив на один елемент більший, копіює в нього покажчики,
      додає нову пісню, звільняє старий масив і повертає true;
    - Song::Duration totalDuration() const — загальна тривалість усіх пісень через operator+;
    - void show() const — назва плейлиста, пісні і загальна тривалість.

  У main:
    1. Створіть у main ще 3 пісні (назви, виконавців і тривалість вигадайте самі).
    2. Створіть два плейлисти: «Ранкова пробіжка» і «Вечірній чіл».
       Хоча б одна пісня має бути в обох плейлистах.
    3. Спробуйте додати в плейлист ту саму пісню вдруге і nullptr. Виведіть повідомлення, якщо add повернув false.
    4. Виведіть обидва плейлисти.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Видалення пісні з плейлиста (бонус)
  ---------------------------------------------------------------

    - Playlist: bool remove(Song* song) — знаходить пісню (порівнюйте покажчики), зсуває всі наступні
      покажчики на одну позицію вліво і зменшує count. Масив при цьому НЕ зменшується.
      Якщо пісні в плейлисті немає — повертає false.

  У main:
    1. Приберіть спільну пісню з одного плейлиста.
    2. Виведіть обидва плейлисти: в іншому плейлисті пісня має залишитися.

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Чому поле duration у Song треба ініціалізувати саме у списку ініціалізації? Що буде, якщо замість цього
     написати duration = Duration(minutes, seconds); у тілі конструктора?
  2. У якому порядку викликаються конструктори Song і Duration під час створення пісні? А деструктори під час знищення?
  3. Чому в main ви пишете Song::Duration, а не просто Duration? Що зміниться, якщо оголосити Duration
     у private-секції класу Song?
  4. Song і Duration — це композиція чи агрегація? А Playlist і Song? Поясніть, за якою ознакою ви це визначили.
  5. (Рівень 2) Чому деструктор Playlist робить delete[] songs, але НЕ робить delete для кожного songs[i]?
     Що станеться, якщо додати такий delete?
  6. (Рівень 2) Навіщо заборонене копіювання плейлиста? Що станеться в кінці програми, якщо його дозволити
     і написати Playlist copy = morning; ?
  7. (Рівень 3) remove прибирає пісню з плейлиста, але не видаляє її. Чому саме так?
*/

// ===== Ваш код =====

class Song
{
public:
    // вкладений клас — тривалість пісні
    class Duration
    {
    private:
        int minutes;
        int seconds;

    public:
        Duration(int minutes, int seconds)
        {
            if (minutes < 0)
            {
                minutes = 0;
            }
            if (seconds < 0)
            {
                seconds = 0;
            }
            // зайві секунди переносимо у хвилини
            this->minutes = minutes + seconds / 60;
            this->seconds = seconds % 60;
        }

        int getTotalSeconds() const
        {
            return minutes * 60 + seconds;
        }

        Duration operator+(const Duration& other) const
        {
            // конструктор сам перенесе секунди у хвилини
            Duration result(minutes + other.minutes, seconds + other.seconds);
            return result;
        }

        void print() const
        {
            cout << minutes << ":";
            if (seconds < 10)
            {
                cout << "0";
            }
            cout << seconds;
        }
    };

private:
    string title;
    string artist;
    Duration duration;   // композиція: тривалість живе всередині пісні

public:
    Song(const string& title, const string& artist, int minutes, int seconds)
        : title(title), artist(artist), duration(minutes, seconds)
    {
    }

    Duration getDuration() const
    {
        return duration;
    }

    void print() const
    {
        cout << artist << " — " << title << " (";
        duration.print();
        cout << ")";
    }
};

// агрегація: плейлист тільки зберігає адреси пісень, створює їх main
class Playlist
{
private:
    string name;
    Song** songs;
    int count;

public:
    Playlist(const string& name) : name(name), songs(nullptr), count(0)
    {
    }

    Playlist(const Playlist& other) = delete;
    Playlist& operator=(const Playlist& other) = delete;

    ~Playlist()
    {
        delete[] songs;   // тільки масив покажчиків, самі пісні не наші
    }

    bool add(Song* song)
    {
        if (song == nullptr)
        {
            return false;
        }
        for (int i = 0; i < count; i++)
        {
            if (songs[i] == song)
            {
                return false;   // ця сама пісня вже є
            }
        }

        Song** temp = new Song*[count + 1];
        for (int i = 0; i < count; i++)
        {
            temp[i] = songs[i];
        }
        temp[count] = song;
        delete[] songs;
        songs = temp;
        count++;
        return true;
    }

    // рівень 3
    bool remove(Song* song)
    {
        int index = -1;
        for (int i = 0; i < count; i++)
        {
            if (songs[i] == song)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            return false;
        }
        for (int i = index; i < count - 1; i++)
        {
            songs[i] = songs[i + 1];
        }
        count--;
        return true;
    }

    Song::Duration totalDuration() const
    {
        Song::Duration total(0, 0);
        for (int i = 0; i < count; i++)
        {
            total = total + songs[i]->getDuration();
        }
        return total;
    }

    void show() const
    {
        cout << "Плейлист: " << name << endl;
        if (count == 0)
        {
            cout << "  (порожній)" << endl;
        }
        for (int i = 0; i < count; i++)
        {
            cout << "  " << i + 1 << ". ";
            songs[i]->print();
            cout << endl;
        }
        cout << "  Загальна тривалість: ";
        totalDuration().print();
        cout << endl;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;
    Song coffee("Ранкова кава", "Гурт Світанок", 3, 7);
    Song nightCity("Нічне місто", "Ліна Вітер", 0, 250);

    coffee.print();
    cout << endl;
    nightCity.print();   // має бути 4:10
    cout << endl;

    Song::Duration total = coffee.getDuration() + nightCity.getDuration();
    cout << "Сума тривалостей: ";
    total.print();
    cout << " (" << total.getTotalSeconds() << " с)" << endl;

    Song::Duration test(1, 130);
    cout << "Duration(1, 130) = ";
    test.print();
    cout << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "===== РІВЕНЬ 2 =====" << endl;
    Song rhythm("Швидкий ритм", "DJ Каштан", 3, 30);
    Song sea("Море спить", "Ліна Вітер", 5, 2);
    Song road("Дорога додому", "Гурт Світанок", 2, 75);

    Playlist morning("Ранкова пробіжка");
    Playlist evening("Вечірній чіл");

    morning.add(&coffee);
    morning.add(&rhythm);
    morning.add(&road);

    evening.add(&nightCity);
    evening.add(&sea);
    evening.add(&road);   // ця пісня є в обох плейлистах

    if (!morning.add(&rhythm))
    {
        cout << "Не вдалося додати: «Швидкий ритм» вже є в плейлисті" << endl;
    }
    if (!evening.add(nullptr))
    {
        cout << "Не вдалося додати: nullptr — це не пісня" << endl;
    }
    // Playlist copy = morning;   // помилка: копіювання заборонене

    morning.show();
    evening.show();

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3 =====" << endl;
    if (morning.remove(&road))
    {
        cout << "«Дорога додому» прибрано з ранкового плейлиста" << endl;
    }
    if (!morning.remove(&sea))
    {
        cout << "«Море спить» у ранковому плейлисті немає — нічого не прибрано" << endl;
    }
    morning.show();
    evening.show();   // тут «Дорога додому» залишилась

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. У Duration немає конструктора за замовчуванням, а всі поля створюються ще до того, як почнеться тіло
     конструктора Song. Тому duration треба створити одразу з minutes і seconds у списку ініціалізації.
     Якщо написати duration = Duration(minutes, seconds); у тілі, компілятор спочатку спробує створити duration
     без параметрів, не знайде Duration() і видасть помилку.

  2. Спочатку створюються поля (title, artist, потім duration — тобто викликається конструктор Duration),
     а вже потім виконується тіло конструктора Song. Деструктори йдуть у зворотному порядку: спочатку
     деструктор Song, потім деструктори полів, серед них Duration.

  3. Duration оголошений всередині класу Song, тому його повне ім'я — Song::Duration, просто Duration
     поза класом компілятор не знає. Якщо перенести Duration у private, то в main не можна буде написати
     Song::Duration total — буде помилка доступу, і Playlist теж не зможе повертати Song::Duration.

  4. Song і Duration — композиція: тривалість є частиною пісні, створюється і знищується разом з нею, окремо
     не існує. Playlist і Song — агрегація: пісні створені в main, плейлист тільки зберігає їх адреси, і після
     знищення плейлиста пісні живуть далі. Ознака — хто створює і хто знищує об'єкт.

  5. Пісні створив не плейлист, а main (звичайні змінні на стеку), тому і видаляти їх не плейлисту. delete[] songs
     звільняє тільки масив покажчиків, який плейлист сам виділив у add. Якщо додати delete songs[i], буде delete
     для об'єкта, який не створювався через new, а спільна пісня ще й видалялася б двічі — програма впаде.

  6. Якщо дозволити копіювання, то copy отримає той самий покажчик songs, що й morning. У кінці програми обидва
     деструктори зроблять delete[] для одного масиву — подвійне звільнення пам'яті і падіння програми.

  7. Пісня не належить плейлисту: вона може бути в іншому плейлисті або використовуватися в main. Якщо remove
     її видалить, то в іншому плейлисті залишиться покажчик на вже знищений об'єкт. Тому remove лише прибирає
     покажчик з масиву, а сама пісня живе далі.

*/
