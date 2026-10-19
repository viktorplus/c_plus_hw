
#include <iostream>
#include <cstring>

using namespace std;

class BankAccount
{
private:
    char owner[50]{};
    double balance = 0;

public:
    void init(const char* newOwner);

    const char* getOwner() const { return owner; }
    double getBalance() const { return balance; }

    bool deposit(double amount);
    bool withdraw(double amount);
    void print() const;
};

void BankAccount::init(const char* newOwner)
{
    strncpy(owner, newOwner, sizeof(owner) - 1);
    owner[sizeof(owner) - 1] = '\0';
    balance = 0;
}

bool BankAccount::deposit(double amount)
{
    if (amount > 0)
    {
        balance += amount;
        return true;
    }

    return false;
}

bool BankAccount::withdraw(double amount)
{
    if (amount > 0 && amount <= balance)
    {
        balance -= amount;
        return true;
    }

    return false;
}

void BankAccount::print() const
{
    cout << "Власник: " << owner
        << ", баланс: " << balance << " грн" << endl;
}

class Book
{
private:
    char title[100]{};
    char author[50]{};
    int pages = 1;
    int year = 1450;

public:
    void setTitle(const char* newTitle);
    void setAuthor(const char* newAuthor);
    bool setPages(int newPages);
    bool setYear(int newYear);

    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }
    int getPages() const { return pages; }
    int getYear() const { return year; }

    bool isThick() const;
    void print() const;
};

void Book::setTitle(const char* newTitle)
{
    strncpy(title, newTitle, sizeof(title) - 1);
    title[sizeof(title) - 1] = '\0';
}

void Book::setAuthor(const char* newAuthor)
{
    strncpy(author, newAuthor, sizeof(author) - 1);
    author[sizeof(author) - 1] = '\0';
}

bool Book::setPages(int newPages)
{
    if (newPages >= 1 && newPages <= 5000)
    {
        pages = newPages;
        return true;
    }

    return false;
}

bool Book::setYear(int newYear)
{
    if (newYear >= 1450 && newYear <= 2026)
    {
        year = newYear;
        return true;
    }

    return false;
}

inline bool Book::isThick() const
{
    return pages > 500;
}

void Book::print() const
{
    cout << "Назва: " << title
        << ", автор: " << author
        << ", сторінок: " << pages
        << ", рік: " << year;

    if (isThick())
    {
        cout << ", товста";
    }

    cout << endl;
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    BankAccount account;
    account.init("Олена");

    account.deposit(1000);
    account.withdraw(300);

    if (!account.withdraw(5000))
    {
        cout << "Помилка: недостатньо коштів" << endl;
    }

    account.print();

    cout << endl;

    Book book1;
    book1.setTitle("Лісова пісня");
    book1.setAuthor("Леся Українка");
    book1.setPages(200);
    book1.setYear(1911);

    Book book2;
    book2.setTitle("Кобзар");
    book2.setAuthor("Тарас Шевченко");
    book2.setPages(600);
    book2.setYear(1840);

    if (!book2.setYear(3000))
    {
        cout << "Помилка: рік має бути від 1450 до 2026" << endl;
    }

    book1.print();
    book2.print();

    return 0;
}