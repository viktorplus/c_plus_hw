#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                        ДОМАШНЄ ЗАВДАННЯ 34 тема: абстрактний клас, чисто віртуальні методи, інтерфейси.
==========================================================================================================================================

  Тема: підземелля. Герой ходить кімнатами і взаємодіє з усім, що трапляється: дверима, скринями, фонтаном, торговцем.
  Це зовсім різні речі, без спільного «є». Але в усіх є спільне вміння — з ними можна взаємодіяти.
  Тому код героя працює з усіма через один інтерфейс і не знає, хто перед ним.

  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  Можна користуватися ШІ, щоб розібратися в незрозумілому. Але на розборі вас можуть попросити пояснити
  будь-який рядок свого коду і відповісти на запитання «що буде, якщо цей рядок змінити».

  ---------------------------------------------------------------
  РІВЕНЬ 1. Інтерфейс Interactable (обов'язковий)
  ---------------------------------------------------------------

  Клас Hero — герой (звичайний клас, не базовий):
    - поля (private): string name; int hp; int maxHp; int gold; int keys;
    - конструктор Hero(const string& name, int hp, int maxHp) — gold = 0, keys = 0;
    - string getName() const;
    - void addGold(int amount); bool spendGold(int amount) — false, якщо золота не вистачає, інакше зменшує gold;
    - void addKey(); bool useKey() — false, якщо ключів немає, інакше зменшує keys на 1;
    - void heal(int amount) — збільшує hp, але не вище maxHp;
    - void print() const — формат: Конан: hp 60/100, золото 0, ключів 0

  Інтерфейс Interactable — «з цим можна взаємодіяти»:
    - virtual string describe() const = 0 — короткий опис об'єкта;
    - virtual void interact(Hero& hero) = 0 — що відбувається, коли герой взаємодіє;
    - віртуальний деструктор = default.
    Полів в інтерфейсі немає.

  Класи, що реалізують Interactable (усі методи — з override):
    - Door (двері): поля string name; bool locked; конструктор Door(const string& name, bool locked);
        describe() — «<name> (зачинені)» або «<name> (відчинені)»;
        interact — якщо двері відчинені: «Ви проходите далі»; якщо зачинені і герой має ключ (useKey) —
        відчиняє їх: «Ключ підійшов! <name> відчинено»; інакше — «Зачинено. Потрібен ключ».
    - Chest (скриня): поля int gold; bool hasKeyInside; bool opened; конструктор Chest(int gold, bool hasKeyInside);
        describe() — «Скриня» або «Скриня (порожня)», якщо вже відкрита;
        interact — перший раз віддає герою золото (addGold) і ключ, якщо він є (addKey), і виводить, що знайдено;
        далі — «Скриня порожня».
    - Fountain (фонтан): поле int healAmount; конструктор Fountain(int healAmount);
        describe() — «Фонтан зцілення»; interact — лікує героя на healAmount і виводить «Ви п'єте з фонтану: +<healAmount> hp».

  Функція void use(Hero& hero, Interactable& object) — виводить «> <опис об'єкта>», викликає interact,
  потім виводить героя через print().

  У main (усі об'єкти — звичайні, без new):
    1. Створіть героя «Конан» (hp 60 з 100), зачинені двері «Дубові двері», скриню з 30 золота і ключем, фонтан на 25 hp.
    2. Через use по черзі: двері → скриня → двері → скриня → фонтан.
       Двері мають відчинитися лише з другої спроби, а скриня вдруге — бути порожньою.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Абстрактний клас Npc і кімната підземелля (бажаний)
  ---------------------------------------------------------------

  Абстрактний клас Npc : public Interactable — неігровий персонаж:
    - поля (protected): string name; string phrase (що він каже при зустрічі);
    - конструктор Npc(const string& name, const string& phrase);
    - describe() — «<name>» (спільний для всіх NPC);
    - void greet() const — звичайний метод, виводить «<name>: "<phrase>"»;
    - interact НЕ перевизначайте — він залишається чисто віртуальним, тому Npc абстрактний.

  Нащадки Npc:
    - Merchant (торговець): поля int potionPrice; int potionHeal;
        interact — вітається (greet), потім продає зілля: якщо герой може заплатити (spendGold) — лікує його на potionHeal
        і виводить «Ви купили зілля: +<potionHeal> hp»; інакше — «Не вистачає золота».
    - Healer (цілителька): поле bool helped;
        interact — вітається, потім перший раз лікує героя на 50 hp («Цілителька лікує вас: +50 hp»),
        далі — «Я вже допомогла тобі сьогодні».

  У main:
    1. Переконайтеся, що рядок  Npc npc("Хтось", "Привіт");  не компілюється, і закоментуйте його.
    2. Створіть нового героя «Ліра» (hp 30 з 100) і кімнату — динамічний масив Interactable** на 5 об'єктів,
       усі через new: скриня з 20 золота без ключа, торговець «Гном Борін» (зілля за 15 золота на 40 hp),
       цілителька «Сестра Мирна», ще раз торговець «Гном Борін» (окремий об'єкт), фонтан на 10 hp.
    3. У циклі пройдіть кімнату: use для кожного об'єкта.
    4. Звільніть пам'ять: спочатку кожен об'єкт, потім масив.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Другий інтерфейс Damageable — двері можна виламати (бонус)
  ---------------------------------------------------------------

    - Інтерфейс Damageable — «це можна пошкодити»: virtual void takeDamage(int damage) = 0;
      virtual bool isDestroyed() const = 0; віртуальний деструктор = default.
    - Змініть Door: class Door : public Interactable, public Damageable.
        Додайте поле int durability (міцність) і третій параметр конструктора.
        takeDamage — зменшує міцність (не нижче 0); коли вона стає 0 — двері відчиняються (locked = false)
        і виводиться «<name> виламано!»; isDestroyed() — durability == 0.
        Двері з рівня 1 створіть з міцністю 50.
    - Клас Barrel : public Damageable (бочка): поле int durability; takeDamage — як у Door, при знищенні «Бочку розбито».
      Бочка НЕ Interactable: взаємодіяти з нею не можна, лише розбити.
    - Додайте в Hero поле int power (сила удару) — четвертий параметр конструктора: Hero(name, hp, maxHp, power);
      героям з рівнів 1 і 2 дайте силу 10. Додайте метод void attack(Damageable& target) — виводить «<name> б'є: <power> шкоди» і викликає target.takeDamage(power).
      Підказка: Hero тепер використовує Damageable, тому інтерфейс Damageable оголосіть вище за Hero.
  У main:
    1. Створіть «Залізні двері» (зачинені, міцність 30) і бочку (міцність 15). Ключа в героя немає.
    2. Спробуйте відчинити двері через use — зачинено. Бийте їх attack, поки не isDestroyed(), потім знову use.
    3. Розбийте бочку.
    4. Покажіть, що на ті самі двері можна дивитися двома способами:
       Interactable* asObject = &ironDoor;  Damageable* asTarget = &ironDoor;

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. Функція use приймає Interactable&. Чому вона працює і з дверима, і зі скринею, і з фонтаном, хоча ці класи
     між собою ніяк не пов'язані?
  2. Чому Door, Chest і Fountain не мають спільного базового класу з полями (наприклад, DungeonObject з name і hp)?
     Чим інтерфейс тут кращий?
  3. (Рівень 2) Чим абстрактний клас Npc відрізняється від інтерфейсу Interactable? Чому в Npc є поля і готові методи?
  4. (Рівень 2) Чому не можна створити об'єкт Npc? Що станеться, якщо в Healer забути перевизначити interact?
  5. (Рівень 2) Скільки рядків у функції use довелося змінити, коли з'явились торговець і цілителька? Чому?
  6. (Рівень 2) Навіщо інтерфейсу Interactable віртуальний деструктор, якщо в нього немає жодного поля?
     Де в програмі об'єкт видаляється через покажчик на інтерфейс?
  7. (Рівень 3) Door реалізує два інтерфейси. Які методи доступні через asObject, а які — через asTarget?
     Чому множинне успадкування від інтерфейсів не має проблеми ромба, яку ми бачили в темі про успадкування?
*/

// ===== Ваш код =====

// ---------- Рівень 3: інтерфейс Damageable (стоїть вище за Hero, бо Hero::attack його використовує) ----------
class Damageable
{
public:
    virtual void takeDamage(int damage) = 0;
    virtual bool isDestroyed() const = 0;
    virtual ~Damageable() = default;
};

// ---------- Герой ----------
class Hero
{
private:
    string name;
    int hp;
    int maxHp;
    int gold;
    int keys;
    int power;      // сила удару (рівень 3)

public:
    Hero(const string& name, int hp, int maxHp, int power)
        : name(name), hp(hp), maxHp(maxHp), gold(0), keys(0), power(power)
    {}

    string getName() const
    {
        return name;
    }

    void addGold(int amount)
    {
        gold += amount;
    }

    bool spendGold(int amount)
    {
        if (gold < amount)
        {
            return false;
        }
        gold -= amount;
        return true;
    }

    void addKey()
    {
        keys++;
    }

    bool useKey()
    {
        if (keys <= 0)
        {
            return false;
        }
        keys--;
        return true;
    }

    void heal(int amount)
    {
        hp += amount;
        if (hp > maxHp)
        {
            hp = maxHp;
        }
    }

    void print() const
    {
        cout << "  " << name << ": hp " << hp << "/" << maxHp << ", золото " << gold << ", ключів " << keys << endl;
    }

    void attack(Damageable& target) const
    {
        cout << name << " б'є: " << power << " шкоди" << endl;
        target.takeDamage(power);
    }
};

// ---------- Рівень 1: інтерфейс Interactable ----------
class Interactable
{
public:
    virtual string describe() const = 0;
    virtual void interact(Hero& hero) = 0;
    virtual ~Interactable() = default;
};

// Двері: з ними можна взаємодіяти, а ще їх можна виламати (рівень 3)
class Door : public Interactable, public Damageable
{
private:
    string name;
    bool locked;
    int durability;

public:
    Door(const string& name, bool locked, int durability) : name(name), locked(locked), durability(durability)
    {}

    string describe() const override
    {
        if (locked)
        {
            return name + " (зачинені)";
        }
        return name + " (відчинені)";
    }

    void interact(Hero& hero) override
    {
        if (!locked)
        {
            cout << "Ви проходите далі" << endl;
        }
        else if (hero.useKey())
        {
            locked = false;
            cout << "Ключ підійшов! " << name << " відчинено" << endl;
        }
        else
        {
            cout << "Зачинено. Потрібен ключ" << endl;
        }
    }

    void takeDamage(int damage) override
    {
        if (durability == 0)          // вже виламані
        {
            return;
        }
        durability -= damage;
        if (durability <= 0)
        {
            durability = 0;
            locked = false;
            cout << name << " виламано!" << endl;
        }
    }

    bool isDestroyed() const override
    {
        return durability == 0;
    }
};

class Chest : public Interactable
{
private:
    int gold;
    bool hasKeyInside;
    bool opened;

public:
    Chest(int gold, bool hasKeyInside) : gold(gold), hasKeyInside(hasKeyInside), opened(false)
    {}

    string describe() const override
    {
        if (opened)
        {
            return "Скриня (порожня)";
        }
        return "Скриня";
    }

    void interact(Hero& hero) override
    {
        if (opened)
        {
            cout << "Скриня порожня" << endl;
            return;
        }
        opened = true;
        hero.addGold(gold);
        cout << "У скрині знайдено: " << gold << " золота";
        if (hasKeyInside)
        {
            hero.addKey();
            cout << " і ключ";
        }
        cout << endl;
    }
};

class Fountain : public Interactable
{
private:
    int healAmount;

public:
    Fountain(int healAmount) : healAmount(healAmount)
    {}

    string describe() const override
    {
        return "Фонтан зцілення";
    }

    void interact(Hero& hero) override
    {
        hero.heal(healAmount);
        cout << "Ви п'єте з фонтану: +" << healAmount << " hp" << endl;
    }
};

// Працює з будь-яким об'єктом, який реалізує Interactable
void use(Hero& hero, Interactable& object)
{
    cout << "> " << object.describe() << endl;
    object.interact(hero);
    hero.print();
}

// ---------- Рівень 2: абстрактний клас Npc ----------
class Npc : public Interactable
{
protected:
    string name;
    string phrase;

public:
    Npc(const string& name, const string& phrase) : name(name), phrase(phrase)
    {}

    string describe() const override
    {
        return name;
    }

    void greet() const
    {
        cout << name << ": \"" << phrase << "\"" << endl;
    }

    // interact тут не пишемо — він залишається чисто віртуальним, тому Npc абстрактний
};

class Merchant : public Npc
{
private:
    int potionPrice;
    int potionHeal;

public:
    Merchant(const string& name, int potionPrice, int potionHeal)
        : Npc(name, "Зілля на будь-який смак, підходь!"), potionPrice(potionPrice), potionHeal(potionHeal)
    {}

    void interact(Hero& hero) override
    {
        greet();
        if (hero.spendGold(potionPrice))
        {
            hero.heal(potionHeal);
            cout << "Ви купили зілля: +" << potionHeal << " hp" << endl;
        }
        else
        {
            cout << "Не вистачає золота" << endl;
        }
    }
};

class Healer : public Npc
{
private:
    bool helped;

public:
    Healer(const string& name) : Npc(name, "Відпочинь, мандрівнику"), helped(false)
    {}

    void interact(Hero& hero) override
    {
        greet();
        if (helped)
        {
            cout << "Я вже допомогла тобі сьогодні" << endl;
        }
        else
        {
            helped = true;
            hero.heal(50);
            cout << "Цілителька лікує вас: +50 hp" << endl;
        }
    }
};

// ---------- Рівень 3: бочка — її можна лише розбити ----------
class Barrel : public Damageable
{
private:
    int durability;

public:
    Barrel(int durability) : durability(durability)
    {}

    void takeDamage(int damage) override
    {
        if (durability == 0)
        {
            return;
        }
        durability -= damage;
        if (durability <= 0)
        {
            durability = 0;
            cout << "Бочку розбито" << endl;
        }
    }

    bool isDestroyed() const override
    {
        return durability == 0;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "========== РІВЕНЬ 1 ==========" << endl;
    Hero conan("Конан", 60, 100, 10);
    Door oakDoor("Дубові двері", true, 50);
    Chest chest(30, true);
    Fountain fountain(25);

    use(conan, oakDoor);      // ключа ще немає
    use(conan, chest);        // 30 золота і ключ
    use(conan, oakDoor);      // тепер ключ є — відчиняються
    use(conan, chest);        // вже порожня
    use(conan, fountain);     // 60 + 25 = 85

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "========== РІВЕНЬ 2 ==========" << endl;
    // Npc npc("Хтось", "Привіт");   // C2259: не можна створити об'єкт абстрактного класу (interact не перевизначений)

    Hero lira("Ліра", 30, 100, 10);
    const int roomSize = 5;
    Interactable** room = new Interactable*[roomSize];
    room[0] = new Chest(20, false);
    room[1] = new Merchant("Гном Борін", 15, 40);
    room[2] = new Healer("Сестра Мирна");
    room[3] = new Merchant("Гном Борін", 15, 40);
    room[4] = new Fountain(10);

    for (int i = 0; i < roomSize; i++)
    {
        use(lira, *room[i]);
    }

    for (int i = 0; i < roomSize; i++)
    {
        delete room[i];       // видаляємо через Interactable* — тому потрібен віртуальний деструктор
    }
    delete[] room;

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "========== РІВЕНЬ 3 ==========" << endl;
    Door ironDoor("Залізні двері", true, 30);
    Barrel barrel(15);

    use(conan, ironDoor);     // ключ Конан уже витратив на дубові двері
    while (!ironDoor.isDestroyed())
    {
        conan.attack(ironDoor);
    }
    use(conan, ironDoor);

    while (!barrel.isDestroyed())
    {
        conan.attack(barrel);
    }
    // use(conan, barrel);    // не компілюється: Barrel не Interactable

    Interactable* asObject = &ironDoor;
    Damageable* asTarget = &ironDoor;
    cout << "Як Interactable: " << asObject->describe() << endl;
    cout << "Як Damageable: " << (asTarget->isDestroyed() ? "виламані" : "цілі") << endl;
    // asObject->takeDamage(10);   // помилка: в Interactable немає takeDamage
    // asTarget->describe();       // помилка: в Damageable немає describe

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Бо всі три класи успадковують Interactable і перевизначають describe та interact. Посилання на базовий клас
     може дивитися на об'єкт нащадка, а методи віртуальні, тому викликається версія саме того об'єкта, який передали.
     Функції use не важливо, що це — двері чи фонтан, їй достатньо знати, що з об'єктом можна взаємодіяти.

  2. Бо у них немає нічого спільного в даних: у фонтана немає hp, у скрині немає імені, у дверей немає золота.
     Якби ми зробили DungeonObject з полями, частина полів у більшості класів просто висіла б без діла,
     і це було б штучне «є». Інтерфейс описує лише вміння (з цим можна взаємодіяти), не нав'язуючи полів,
     і його може реалізувати будь-який клас, навіть зовсім не схожий на інші.

  3. Інтерфейс Interactable — це тільки чисто віртуальні методи і віртуальний деструктор, без полів, тобто тільки «що вміє».
     Npc теж абстрактний, але в нього є поля name і phrase та готові методи describe і greet, бо всі NPC мають ім'я
     і вітаються однаково — немає сенсу писати це в кожному нащадку заново. Абстрактним його робить лише interact,
     який кожен NPC реалізує по-своєму.

  4. Бо в Npc залишився чисто віртуальний метод interact без реалізації, а об'єкт з «дірою» замість методу створити не можна —
     компілятор видасть помилку. Якщо в Healer забути перевизначити interact, Healer теж стане абстрактним,
     і помилка буде на рядку new Healer("Сестра Мирна") — створити цілительку вже не вийде.

  5. Жодного. use працює через інтерфейс Interactable і викликає тільки describe та interact, а торговець і цілителька
     теж реалізують цей інтерфейс (через Npc). Нові класи просто додались, а старий код не довелося чіпати.

  6. Поля тут ні до чого — важливо, ЯКИЙ деструктор викличеться. У рівні 2 ми робимо delete room[i], а room[i] має тип
     Interactable*. Без віртуального деструктора викликався б тільки деструктор Interactable, а деструктор Merchant
     чи Chest — ні, і, наприклад, рядки name і phrase не звільнились би (це невизначена поведінка).
     З віртуальним деструктором спочатку викликається деструктор справжнього класу, а потім базових.

  7. Через asObject доступні тільки describe і interact (методи Interactable), а через asTarget — тільки takeDamage
     і isDestroyed (методи Damageable). Об'єкт один і той самий, але кожен покажчик «бачить» лише свою частину.
     Проблеми ромба немає, бо в інтерфейсах немає полів — дублюватися нічому. Крім того, Interactable і Damageable
     не мають спільного базового класу, тож двох копій одного предка всередині Door не виникає.
*/
