#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                    ДОМАШНЄ ЗАВДАННЯ 30 тема: динамічні структури даних — стек, черга, однозв'язний список.
==========================================================================================================================================

  Тема: карткова гра на виживання. Гравці по черзі тягнуть карти з колоди, і кожна карта завдає їм шкоди. Хто втратив усе здоров'я — 
  вибуває. Перемагає останній, хто залишився у грі.

  Ви самі побудуєте структури даних для гри: колода — це стек, черга ходів — це черга, журнал гри — список.

  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус. Частина «ПОЯСНИ» обов'язкова для всіх.

  ---------------------------------------------------------------
  РІВЕНЬ 1. Колода карт Deck<T> (обов'язковий)
  ---------------------------------------------------------------

  Шаблонний клас Node<T> — вузол (як у прикладах з уроку):
    - поля (public): T data; Node* next;
    - конструктор Node(const T& value) — записує value в data, next = nullptr.

  Шаблонний клас Deck<T> — колода карт, працює як стек.
  Поля (private):
    - Node<T>* top — верхня карта колоди;
    - int count    — кількість карт у колоді.

  Методи:
    - Deck() — порожня колода;
    - копіювання заборонене: конструктор копіювання і operator= — = delete;
    - ~Deck() — видаляє всі вузли;
    - bool isEmpty() const;
    - int size() const — повертає count (без проходу по вузлах);
    - void putOnTop(const T& card) — кладе карту на верх колоди;
    - bool draw(T& card) — знімає верхню карту: записує її дані в card, видаляє вузол і повертає true.
      Якщо колода порожня — нічого не робить і повертає false;
    - void show() const — виводить карти від верхньої до нижньої в один рядок через пробіл.

  У main:
    1. Створіть Deck<int> deck. Карта — це число, скільки шкоди вона завдає.
    2. Покладіть на верх колоди по черзі карти з масиву: int cards[] = { 4, 2, 6, 3, 5, 1, 7, 2, 4, 3 };
    3. Виведіть колоду і її розмір.
    4. Візьміть 2 карти через draw, виведіть кожну взяту карту і новий розмір колоди.
    5. Перевірте draw на порожній колоді: створіть Deck<int> empty і виведіть повідомлення, якщо draw повернув false.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Черга ходів TurnQueue<T> і сама гра (бажаний)
  ---------------------------------------------------------------

  Клас Player:
    - поля (private): string name; int hp;
    - конструктор Player(const string& name, int hp);
    - string getName() const; int getHp() const;
    - void takeDamage(int damage) — зменшує hp на damage, але hp не може стати меншим за 0;
    - bool isAlive() const — true, якщо hp > 0;
    - перевантажений оператор << виводить гравця у форматі: Олена (HP: 10).

  Шаблонний клас TurnQueue<T> — черга ходів (використовує той самий Node<T>).
  Поля (private): Node<T>* head; Node<T>* tail; int count.

  Методи:
    - TurnQueue(); копіювання заборонене (= delete); ~TurnQueue() видаляє всі вузли;
    - bool isEmpty() const; int size() const;
    - void enqueue(const T& value) — додає в кінець черги;
    - T& current() — повертає того, чий зараз хід (перший у черзі). Викликати лише для непорожньої черги;
    - void nextTurn() — передає хід: перший вузол переходить у кінець черги.
      Нові вузли НЕ створюються і НЕ видаляються, змінюються тільки покажчики.
      Якщо в черзі менше 2 елементів — нічого не робить;
    - bool removeCurrent() — видаляє першого в черзі (гравець вибув). Якщо черга порожня — повертає false;
    - void show() const — виводить усіх по порядку, кожного з нового рядка.

  У main (продовжуйте після рівня 1, колода вже без двох карт):
    1. Створіть TurnQueue<Player> players і додайте гравців: Олена, Марко, Тарас — у кожного 10 HP.
    2. Поки в грі більше одного гравця і колода не порожня:
         - гравець current() тягне карту з колоди і отримує шкоду, що дорівнює значенню карти;
         - виведіть, наприклад: Марко тягне карту 7 -> Марко (HP: 3);
         - якщо гравець вибув — виведіть повідомлення і видаліть його через removeCurrent();
         - інакше передайте хід через nextTurn().
    3. Після гри: якщо залишився один гравець — виведіть переможця. Інакше виведіть, що колода скінчилася,
       і покажіть усіх, хто залишився, через show().

  ---------------------------------------------------------------
  РІВЕНЬ 3. Журнал гри GameLog<T> (бонус)
  ---------------------------------------------------------------

  Шаблонний клас GameLog<T> — однозв'язний список подій гри (використовує той самий Node<T>).
  Поля (private): Node<T>* head; Node<T>* tail.

  Методи:
    - GameLog(); копіювання заборонене (= delete); ~GameLog() видаляє всі вузли;
    - void addToTail(const T& value) — додає в кінець, використовуючи tail (без проходу по списку);
    - void show() const — виводить кожен запис з нового рядка;
    - void reverse() — розвертає список «на місці»: останній вузол стає першим, перший — останнім.
      Нові вузли НЕ створюються і НЕ видаляються, змінюються тільки покажчики next. Не забудьте про head і tail.

  У main:
    1. Створіть GameLog<string> gameLog.
    2. Під час гри з рівня 2 додавайте в журнал кожну подію, наприклад: "Марко: -7 HP"
       (число в рядок перетворюйте через to_string).
    3. Після гри викличте reverse() і виведіть журнал: «Останні події гри».

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Ви поклали карти в порядку 4, 2, 6, 3, 5 ... Яку карту draw повернув першою і чому?
  2. Чому draw приймає T& card і повертає bool, а не просто повертає T? Що мав би повернути такий метод,
     якщо колода порожня?
  3. Що станеться, якщо в Deck прибрати = delete і написати Deck<int> copy = deck; ? Що буде в кінці програми?
  4. size() повертає поле count, а не рахує вузли. Що це дає? У яких методах count обов'язково треба змінювати?
  5. (Рівень 2) Опишіть, які покажчики змінює nextTurn() і чому для цього не потрібні new і delete.
  6. (Рівень 2) Що має статися з tail, коли removeCurrent() видаляє останнього гравця в черзі?
     Що зламається, якщо про це забути?
  7. (Рівень 3) Навіщо в reverse() потрібен окремий покажчик на наступний вузол, перш ніж змінювати current->next?
*/

// ===== Ваш код =====

// ---------------- РІВЕНЬ 1: вузол і колода (стек) ----------------

template <typename T>
class Node
{
public:
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class Deck
{
private:
    Node<T>* top;
    int count;

public:
    Deck() : top(nullptr), count(0) {}

    Deck(const Deck&) = delete;
    Deck& operator=(const Deck&) = delete;

    ~Deck()
    {
        while (top != nullptr)
        {
            Node<T>* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const
    {
        return top == nullptr;
    }

    int size() const
    {
        return count;
    }

    void putOnTop(const T& card)
    {
        Node<T>* node = new Node<T>(card);
        node->next = top;
        top = node;
        count++;
    }

    bool draw(T& card)
    {
        if (isEmpty())
        {
            return false;
        }
        card = top->data;           // спочатку забираємо дані
        Node<T>* temp = top;
        top = top->next;            // потім зсуваємо вершину
        delete temp;                // і тільки тепер видаляємо вузол
        count--;
        return true;
    }

    void show() const
    {
        for (Node<T>* current = top; current != nullptr; current = current->next)
        {
            cout << current->data << " ";
        }
        cout << endl;
    }
};

// ---------------- РІВЕНЬ 2: гравець і черга ходів ----------------

class Player
{
private:
    string name;
    int hp;

public:
    Player(const string& name, int hp) : name(name), hp(hp) {}

    string getName() const
    {
        return name;
    }

    int getHp() const
    {
        return hp;
    }

    void takeDamage(int damage)
    {
        hp -= damage;
        if (hp < 0)
        {
            hp = 0;
        }
    }

    bool isAlive() const
    {
        return hp > 0;
    }

    friend ostream& operator<<(ostream& out, const Player& player)
    {
        out << player.name << " (HP: " << player.hp << ")";
        return out;
    }
};

template <typename T>
class TurnQueue
{
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    TurnQueue() : head(nullptr), tail(nullptr), count(0) {}

    TurnQueue(const TurnQueue&) = delete;
    TurnQueue& operator=(const TurnQueue&) = delete;

    ~TurnQueue()
    {
        while (removeCurrent())
        {
        }
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    int size() const
    {
        return count;
    }

    void enqueue(const T& value)
    {
        Node<T>* node = new Node<T>(value);
        if (isEmpty())
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
        count++;
    }

    // викликати тільки для непорожньої черги!
    T& current()
    {
        return head->data;
    }

    // перший вузол переставляємо в кінець, нічого не створюємо і не видаляємо
    void nextTurn()
    {
        if (count < 2)
        {
            return;
        }
        Node<T>* first = head;
        head = head->next;      // ходить наступний
        first->next = nullptr;  // перший тепер буде останнім
        tail->next = first;     // чіпляємо його за хвіст
        tail = first;
    }

    bool removeCurrent()
    {
        if (isEmpty())
        {
            return false;
        }
        Node<T>* temp = head;
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;     // черга спорожніла
        }
        delete temp;
        count--;
        return true;
    }

    void show() const
    {
        for (Node<T>* current = head; current != nullptr; current = current->next)
        {
            cout << "  " << current->data << endl;
        }
    }
};

// ---------------- РІВЕНЬ 3: журнал гри (список) ----------------

template <typename T>
class GameLog
{
private:
    Node<T>* head;
    Node<T>* tail;

public:
    GameLog() : head(nullptr), tail(nullptr) {}

    GameLog(const GameLog&) = delete;
    GameLog& operator=(const GameLog&) = delete;

    ~GameLog()
    {
        while (head != nullptr)
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    void addToTail(const T& value)
    {
        Node<T>* node = new Node<T>(value);
        if (head == nullptr)
        {
            head = node;
        }
        else
        {
            tail->next = node;
        }
        tail = node;
    }

    void show() const
    {
        for (Node<T>* current = head; current != nullptr; current = current->next)
        {
            cout << "  " << current->data << endl;
        }
    }

    void reverse()
    {
        Node<T>* previous = nullptr;
        Node<T>* current = head;
        tail = head;                        // старий перший стане останнім
        while (current != nullptr)
        {
            Node<T>* nextNode = current->next;  // запам'ятали, куди йти далі
            current->next = previous;           // розвернули стрілку назад
            previous = current;
            current = nextNode;
        }
        head = previous;                    // старий останній тепер перший
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;

    Deck<int> deck;
    int cards[] = { 4, 2, 6, 3, 5, 1, 7, 2, 4, 3 };
    int cardsCount = sizeof(cards) / sizeof(cards[0]);
    for (int i = 0; i < cardsCount; i++)
    {
        deck.putOnTop(cards[i]);
    }

    cout << "Колода (зверху вниз): ";
    deck.show();
    cout << "Розмір колоди: " << deck.size() << endl;

    for (int i = 0; i < 2; i++)
    {
        int card;
        if (deck.draw(card))
        {
            cout << "Взяли карту " << card << ", у колоді лишилось " << deck.size() << endl;
        }
    }

    Deck<int> empty;
    int card;
    if (!empty.draw(card))
    {
        cout << "Порожня колода: тягнути нічого" << endl;
    }

    // ---------------- РІВЕНЬ 2 + РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 2: гра =====" << endl;
    cout << "Колода перед грою: ";
    deck.show();

    TurnQueue<Player> players;
    players.enqueue(Player("Олена", 10));
    players.enqueue(Player("Марко", 10));
    players.enqueue(Player("Тарас", 10));
    cout << "Гравці:" << endl;
    players.show();
    cout << endl;

    GameLog<string> gameLog;

    while (players.size() > 1 && !deck.isEmpty())
    {
        Player& player = players.current();
        int damage;
        deck.draw(damage);
        player.takeDamage(damage);
        cout << player.getName() << " тягне карту " << damage << " -> " << player << endl;
        gameLog.addToTail(player.getName() + ": -" + to_string(damage) + " HP");

        if (!player.isAlive())
        {
            cout << "  " << player.getName() << " вибуває з гри!" << endl;
            gameLog.addToTail(player.getName() + " вибуває");
            players.removeCurrent();
        }
        else
        {
            players.nextTurn();
        }
    }

    cout << endl;
    if (players.size() == 1)
    {
        cout << "Переможець: " << players.current() << endl;
        gameLog.addToTail("Переможець: " + players.current().getName());
    }
    else
    {
        cout << "Колода скінчилася! У грі залишились:" << endl;
        players.show();
    }

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3: журнал =====" << endl;
    gameLog.reverse();
    cout << "Останні події гри:" << endl;
    gameLog.show();

    // кінець списку після reverse: додаємо в tail, щоб перевірити, що він правильний
    gameLog.addToTail("(кінець журналу)");
    cout << "Після addToTail в розвернутий журнал:" << endl;
    gameLog.show();

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Першою draw повернув 3 — останню карту з масиву. Колода працює як стек: кожна нова карта кладеться
     на верх, а draw бере саме з верху. Тому карти виходять у зворотному порядку (LIFO), а перша покладена
     карта 4 лежить на самому дні.

  2. Бо колода може бути порожньою, і тоді повертати просто нічого. Якби draw повертав T, то для порожньої
     колоди треба було б повернути якесь «фейкове» значення, наприклад 0 або -1, але 0 теж може бути
     нормальною картою, і не зрозуміло, чи це помилка. А так результат пишеться в card, а bool чесно каже,
     вдалося чи ні.

  3. Тоді компілятор зробить звичайне копіювання полів: copy.top буде вказувати на ті самі вузли, що й deck.top.
     Дві колоди ділять одні вузли. В кінці програми спочатку деструктор copy видалить усі вузли, а потім
     деструктор deck спробує видалити їх ще раз — подвійний delete, програма може впасти. А якщо одна колода
     зробить draw, друга буде вказувати на вже видалений вузол.

  4. Розмір дізнаємось одразу, а не проходимо по всіх вузлах циклом, що для великої колоди довго. Але тоді
     count треба вручну тримати правильним: збільшувати в putOnTop (і enqueue в черзі) і зменшувати в draw
     (і removeCurrent). Якщо десь забути, size() почне брехати.

  5. nextTurn запам'ятовує перший вузол, переносить head на другий, у першого обнуляє next, потім старий tail
     показує на цей вузол, і він стає новим tail. Тобто змінюються тільки head, tail і два next. Вузол
     просто переставляється з початку в кінець, гравець той самий, тому нічого створювати чи видаляти не треба.

  6. Якщо видалили останнього, то head стає nullptr, і tail теж треба зробити nullptr. Якщо забути, tail
     вказуватиме на видалений вузол, і наступний enqueue зробить tail->next = node у вже звільнену пам'ять,
     а head залишиться nullptr — новий елемент «загубиться», і можливий краш.

  7. Бо як тільки ми пишемо current->next = previous, стара стрілка на наступний вузол зникає, і решту списку
     вже не знайти. Тому спочатку зберігаємо nextNode = current->next, розвертаємо стрілку, і тоді переходимо
     на nextNode. Без цього після першого кроку список би обірвався.

*/
