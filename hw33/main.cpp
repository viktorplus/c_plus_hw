#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                    ДОМАШНЄ ЗАВДАННЯ 33 тема: поліморфізм — віртуальні методи, override і final, віртуальний деструктор.
==========================================================================================================================================

  Тема: алхімічна лабораторія. В котел кидають інгредієнти, і кожен змінює зілля по-своєму: трава лікує, гриб отруює,
  вода розбавляє, кристал підсилює все, що вже є в котлі. Котел не знає, що саме в нього кинули, — він просто
  просить кожен інгредієнт «подіяти». Важливий і порядок: кристал після трави і кристал до трави дають різне зілля.

  Завдання має 3 рівні. Рівень 1 обов'язковий для всіх, рівень 2 — бажаний, рівень 3 — бонус.
  Частина «ПОЯСНИ» обов'язкова для всіх.

  Можна користуватися ШІ, щоб розібратися в незрозумілому. Але на розборі вас можуть попросити пояснити
  будь-який рядок свого коду і відповісти на запитання «що буде, якщо цей рядок змінити».

  ---------------------------------------------------------------
  РІВЕНЬ 1. Зілля та інгредієнти — віртуальні методи (обов'язковий)
  ---------------------------------------------------------------

  Клас Potion — стан зілля в котлі:
    - поля (private): int healing (лікувальна сила); int toxicity (токсичність);
    - конструктор за замовчуванням — обидва поля 0;
    - void addHealing(int amount); void addToxicity(int amount);
    - void dilute() — розбавити: токсичність зменшується вдвічі;
    - void amplify() — підсилити: і лікувальна сила, і токсичність подвоюються;
    - string getResult() const — що вийшло. Правила перевіряються саме в такому порядку:
        healing + toxicity > 200     → "Котел вибухнув!"
        healing == 0 і toxicity == 0 → "Брудна вода"
        toxicity > healing           → "Отрута"
        інакше                       → "Зілля лікування";
    - void print() const — формат: Лікування: 60, токсичність: 40 → Зілля лікування

  Клас Ingredient — інгредієнт. Сам по собі це інертна домішка, яка нічого не змінює (пісок, попіл):
    - поля (protected): string name; int power (сила);
    - конструктор Ingredient(const string& name, int power);
    - віртуальний деструктор = default;
    - string getName() const — звичайний (НЕ віртуальний) метод;
    - virtual void addTo(Potion& potion) const — виводить «<name> розчиняється без сліду» і нічого не змінює.

  Нащадки Ingredient — перевизначте addTo з override:
    - Herb (трава): додає potion лікувальну силу power; виводить «<name>: +<power> до лікування»;
    - Mushroom (гриб): додає токсичність power; виводить «<name>: +<power> до токсичності»;
    - Water (вода): конструктор Water() — назва «Чиста вода», сила 0; розбавляє зілля (dilute);
      виводить «<name>: токсичність зменшено вдвічі»;
    - Crystal (кристал): конструктор Crystal(const string& name) — сила 0; підсилює зілля (amplify);
      виводить «<name>: усе в котлі посилено вдвічі».

  Функція void throwIn(Potion& potion, const Ingredient& ingredient) — виводить «Кидаємо: <назва>» і викликає
  ingredient.addTo(potion).

  У main (усі об'єкти — звичайні, без new):
    1. Створіть траву «М'ята» (сила 30), гриб «Мухомор» (40), кристал «Місячний кристал» і інгредієнт «Пісок» (0).
    2. Зваріть перше зілля: м'ята → кристал → мухомор. Виведіть результат.
    3. Зваріть друге зілля з тих самих інгредієнтів, але в порядку мухомор → кристал → м'ята. Виведіть результат.
    4. Зваріть третє зілля: мухомор → пісок → чиста вода. Виведіть результат.

  ---------------------------------------------------------------
  РІВЕНЬ 2. Котел — колекція інгредієнтів у динамічній пам'яті (бажаний)
  ---------------------------------------------------------------

  Клас Cauldron — котел. Котел ВОЛОДІЄ інгредієнтами: їх створюють через new і кидають у котел,
  а знищує їх котел (під час варіння інгредієнти згорають).
    - поля (private): Ingredient** ingredients — динамічний масив покажчиків; int count;
    - конструктор за замовчуванням — котел порожній: ingredients = nullptr, count = 0;
    - копіювання заборонене: конструктор копіювання і operator= — = delete;
    - bool add(Ingredient* ingredient) — false, якщо ingredient == nullptr. Інакше створює новий масив на один елемент
      більший, копіює в нього покажчики, додає інгредієнт, звільняє старий масив і повертає true
      (як addToTeam у Manager з попереднього ДЗ);
    - void clear() — видаляє КОЖЕН інгредієнт, потім масив покажчиків; ingredients = nullptr, count = 0;
    - деструктор — викликає clear();
    - void brew() — створює Potion, по черзі викликає addTo для кожного інгредієнта в порядку додавання,
      виводить зілля через print(), потім викликає clear() — котел знову порожній.

  У main:
    1. Створіть котел. Додайте: траву «Женьшень» (50), траву «М'ята» (30), кристал, гриб «Поганка» (20), чисту воду.
       Зваріть.
    2. Додайте другий рецепт: траву «Женьшень» (50), кристал, гриб «Мухомор» (40), ще один кристал. Зваріть.
    Котел сам видаляє інгредієнти — жодного delete в main.

  ---------------------------------------------------------------
  РІВЕНЬ 3. Філософський камінь (бонус)
  ---------------------------------------------------------------

    - До Potion додайте метод void transmute() — уся токсичність перетворюється на лікувальну силу:
      healing += toxicity; toxicity = 0.
    - Клас PhilosopherStone final : public Ingredient — філософський камінь (він єдиний у своєму роді,
      різновидів у нього не буває, тому final):
        - конструктор PhilosopherStone() — назва «Філософський камінь», сила 0;
        - addTo — викликає transmute() і виводить «<name>: отруту перетворено на ліки».
    - У main зваріть у котлі: гриб «Мухомор» (40) → гриб «Поганка» (20) → філософський камінь. Має вийти зілля лікування.
    - Зверніть увагу, скільки рядків довелося змінити в класі Cauldron, щоб він навчився працювати з каменем.

  ---------------------------------------------------------------
  ЧАСТИНА «ПОЯСНИ» (обов'язкова)
  ---------------------------------------------------------------

  Відповіді напишіть СВОЇМИ словами в коментарі в кінці файлу.

  1. throwIn (і котел у рівні 2) знає лише про Ingredient. Як він «дізнається», що мухомор треба додати до токсичності,
     а траву — до лікування?
  2. getName() не віртуальний, а addTo() — віртуальний. Чому getName() не потрібно робити віртуальним?
     Що станеться з зіллям, якщо прибрати virtual в addTo() (і override у нащадках)?
  3. Що буде, якщо в Mushroom написати void addTo(Potion& potion) без const: а) з override; б) без override?
  4. throwIn приймає const Ingredient&. Що станеться, якщо зробити параметр Ingredient (за значенням)?
  5. (Рівень 2) Чому деструктор Ingredient віртуальний, хоча він нічого не робить? Де саме в програмі інгредієнт
     видаляється через покажчик на базовий клас?
  6. (Рівень 3) Скільки рядків у Cauldron довелося змінити заради філософського каменя? Чому?
  7. Чи має сенс створювати об'єкт «просто Ingredient», як пісок у рівні 1? Як, на вашу думку, можна було б заборонити
     створювати такі об'єкти, щоб у котел потрапляли лише справжні інгредієнти?
*/

// ===== Ваш код =====

class Potion
{
private:
    int healing;
    int toxicity;

public:
    Potion() : healing(0), toxicity(0)
    {
    }

    void addHealing(int amount)
    {
        healing += amount;
    }

    void addToxicity(int amount)
    {
        toxicity += amount;
    }

    void dilute()
    {
        toxicity = toxicity / 2;
    }

    void amplify()
    {
        healing = healing * 2;
        toxicity = toxicity * 2;
    }

    // рівень 3
    void transmute()
    {
        healing += toxicity;
        toxicity = 0;
    }

    string getResult() const
    {
        if (healing + toxicity > 200)
        {
            return "Котел вибухнув!";
        }
        if (healing == 0 && toxicity == 0)
        {
            return "Брудна вода";
        }
        if (toxicity > healing)
        {
            return "Отрута";
        }
        return "Зілля лікування";
    }

    void print() const
    {
        cout << "Лікування: " << healing << ", токсичність: " << toxicity << " → " << getResult() << endl;
    }
};

class Ingredient
{
protected:
    string name;
    int power;

public:
    Ingredient(const string& name, int power) : name(name), power(power)
    {
    }

    virtual ~Ingredient() = default;

    string getName() const
    {
        return name;
    }

    // звичайна домішка нічого не робить
    virtual void addTo(Potion& potion) const
    {
        cout << "  " << name << " розчиняється без сліду" << endl;
    }
};

class Herb : public Ingredient
{
public:
    Herb(const string& name, int power) : Ingredient(name, power)
    {
    }

    void addTo(Potion& potion) const override
    {
        potion.addHealing(power);
        cout << "  " << name << ": +" << power << " до лікування" << endl;
    }
};

class Mushroom : public Ingredient
{
public:
    Mushroom(const string& name, int power) : Ingredient(name, power)
    {
    }

    void addTo(Potion& potion) const override
    {
        potion.addToxicity(power);
        cout << "  " << name << ": +" << power << " до токсичності" << endl;
    }
};

class Water : public Ingredient
{
public:
    Water() : Ingredient("Чиста вода", 0)
    {
    }

    void addTo(Potion& potion) const override
    {
        potion.dilute();
        cout << "  " << name << ": токсичність зменшено вдвічі" << endl;
    }
};

class Crystal : public Ingredient
{
public:
    Crystal(const string& name) : Ingredient(name, 0)
    {
    }

    void addTo(Potion& potion) const override
    {
        potion.amplify();
        cout << "  " << name << ": усе в котлі посилено вдвічі" << endl;
    }
};

// рівень 3: від каменя більше ніхто не успадковується
class PhilosopherStone final : public Ingredient
{
public:
    PhilosopherStone() : Ingredient("Філософський камінь", 0)
    {
    }

    void addTo(Potion& potion) const override
    {
        potion.transmute();
        cout << "  " << name << ": отруту перетворено на ліки" << endl;
    }
};

void throwIn(Potion& potion, const Ingredient& ingredient)
{
    cout << "Кидаємо: " << ingredient.getName() << endl;
    ingredient.addTo(potion);   // викличеться addTo справжнього інгредієнта
}

// котел володіє інгредієнтами, тому сам їх видаляє
class Cauldron
{
private:
    Ingredient** ingredients;
    int count;

public:
    Cauldron() : ingredients(nullptr), count(0)
    {
    }

    Cauldron(const Cauldron& other) = delete;
    Cauldron& operator=(const Cauldron& other) = delete;

    ~Cauldron()
    {
        clear();
    }

    bool add(Ingredient* ingredient)
    {
        if (ingredient == nullptr)
        {
            return false;
        }
        Ingredient** temp = new Ingredient*[count + 1];
        for (int i = 0; i < count; i++)
        {
            temp[i] = ingredients[i];
        }
        temp[count] = ingredient;
        delete[] ingredients;
        ingredients = temp;
        count++;
        return true;
    }

    void clear()
    {
        for (int i = 0; i < count; i++)
        {
            delete ingredients[i];   // через покажчик на Ingredient — тому деструктор віртуальний
        }
        delete[] ingredients;
        ingredients = nullptr;
        count = 0;
    }

    void brew()
    {
        cout << "Варимо зілля з " << count << " інгредієнтів:" << endl;
        Potion potion;
        for (int i = 0; i < count; i++)
        {
            ingredients[i]->addTo(potion);
        }
        potion.print();
        clear();   // інгредієнти згоріли
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "===== РІВЕНЬ 1 =====" << endl;
    Herb mint("М'ята", 30);
    Mushroom flyAgaric("Мухомор", 40);
    Crystal moonCrystal("Місячний кристал");
    Ingredient sand("Пісок", 0);
    Water water;

    cout << "--- Зілля 1: м'ята → кристал → мухомор ---" << endl;
    Potion potion1;
    throwIn(potion1, mint);
    throwIn(potion1, moonCrystal);
    throwIn(potion1, flyAgaric);
    potion1.print();

    cout << endl << "--- Зілля 2: мухомор → кристал → м'ята ---" << endl;
    Potion potion2;
    throwIn(potion2, flyAgaric);
    throwIn(potion2, moonCrystal);
    throwIn(potion2, mint);
    potion2.print();

    cout << endl << "--- Зілля 3: мухомор → пісок → чиста вода ---" << endl;
    Potion potion3;
    throwIn(potion3, flyAgaric);
    throwIn(potion3, sand);
    throwIn(potion3, water);
    potion3.print();

    // ---------------- РІВЕНЬ 2 ----------------
    cout << endl << "===== РІВЕНЬ 2 =====" << endl;
    Cauldron cauldron;

    cout << "--- Рецепт 1 ---" << endl;
    cauldron.add(new Herb("Женьшень", 50));
    cauldron.add(new Herb("М'ята", 30));
    cauldron.add(new Crystal("Місячний кристал"));
    cauldron.add(new Mushroom("Поганка", 20));
    cauldron.add(new Water());
    if (!cauldron.add(nullptr))
    {
        cout << "nullptr у котел не кинеш" << endl;
    }
    cauldron.brew();

    cout << endl << "--- Рецепт 2 ---" << endl;
    cauldron.add(new Herb("Женьшень", 50));
    cauldron.add(new Crystal("Місячний кристал"));
    cauldron.add(new Mushroom("Мухомор", 40));
    cauldron.add(new Crystal("Сонячний кристал"));
    cauldron.brew();

    // ---------------- РІВЕНЬ 3 ----------------
    cout << endl << "===== РІВЕНЬ 3 =====" << endl;
    cauldron.add(new Mushroom("Мухомор", 40));
    cauldron.add(new Mushroom("Поганка", 20));
    cauldron.add(new PhilosopherStone());
    cauldron.brew();
    // class GoldStone : public PhilosopherStone {};   // помилка: клас final, успадковуватися не можна

    return 0;
}

/*
  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Він нічого не «дізнається» сам — addTo віртуальний. Коли через посилання чи покажчик на Ingredient викликаємо
     addTo, програма під час виконання дивиться, який об'єкт там насправді лежить (Mushroom, Herb...), і викликає
     його версію методу. Тому гриб сам додає токсичність, а трава — лікування.

  2. getName() у всіх однаковий — він просто повертає поле name, ніхто його не перевизначає, тому віртуальність тут
     не потрібна. Якщо прибрати virtual в addTo(), то через const Ingredient& буде викликатися тільки версія
     Ingredient: усі інгредієнти «розчиняться без сліду», а зілля залишиться порожнім — «Брудна вода».

  3. а) З override буде помилка компіляції: метод без const має іншу сигнатуру і нічого не перевизначає, а override
     якраз це перевіряє. б) Без override програма скомпілюється, але це буде новий окремий метод, який лише ховає
     базовий. Через const Ingredient& викличеться версія Ingredient, і мухомор розчиниться без сліду — токсичність
     не додасться, і помилку буде важко помітити.

  4. Тоді при виклику робиться копія лише частини Ingredient (зрізання об'єкта), а частина Mushroom чи Herb губиться.
     У копії addTo буде вже від Ingredient, тому кожен інгредієнт просто «розчиниться без сліду», і зілля не зміниться.

  5. У котлі інгредієнти зберігаються як Ingredient*, а створені вони як Herb, Mushroom і т.д. Видаляються вони в
     Cauldron::clear() рядком delete ingredients[i]; — саме через покажчик на базовий клас. Без віртуального деструктора
     викликався б тільки ~Ingredient, а деструктор справжнього класу ні — це невизначена поведінка. Тепер у нащадків
     немає своїх деструкторів, але якщо колись з'являться (наприклад з new всередині), без virtual вони б не викликалися.

  6. Жодного. Котел працює тільки з Ingredient* і викликає віртуальний addTo, тому йому байдуже, який саме інгредієнт
     усередині. Новий клас просто перевизначає addTo, а котел сам викличе потрібну версію.

  7. Не дуже: пісок нічого не робить, це просто «порожній» інгредієнт. Щоб заборонити такі об'єкти, можна зробити
     конструктор Ingredient protected — тоді створювати його зможуть лише нащадки. Або зробити addTo чисто віртуальним
     (= 0), тоді Ingredient стане абстрактним класом і його об'єкт створити взагалі не вийде.

*/
