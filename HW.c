#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#define D_CRT_NONSTDC_NO_WARNINGS
#define N 50
#define B 30
#define MAX 100
#pragma warning(disable : 4996)

struct books { 
    unsigned long long int isbn;
    char author[N];
    char title[N];
    int whole;
    int av;
};
struct books book[MAX];

struct students {
    char num[8];
    char name[B];
    char sname[B];
    char otch[B];
    char fak[4];
    char napr[N];
};
struct students student[MAX];

struct users {
    char login[20];
    char pass[20];
    int stud; 
    int book; };
struct users user[MAX];

struct sb { 
     int isbn;
     char num[8];
     char date[12]; };
struct sb stud[MAX];

int BookFileOpen();
int StudentFileOpen();
int StudentBooksFileOpen();

int acesslevel(char* login, char* pass, int usercount);

void addstudent(int studcount);
void delstudent(int studcount, int studbook);
void addbook(int strcount);
void delbook(int strcount, int studbook);

void rebackup(int strcount);
void backup(int strcount);

char* settime(struct tm* u);
void searchstudent(int studcount);

void booksofstud(int studbook, int strcount, int studcount);
void studsofbook(int studbook, int strcount, int studcount);

void givebook(int studbook, int strcount, int studcount);
void takebook(int studbook, int strcount, int studcount);
int StrInt(char* tmp, char* newtmp);

FILE* file;


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    int choice;
    int status;
    char pass[20], login[20];
    int flag = 0;
    if ((file = fopen("users.csv", "r")) == NULL){
        printf("Невозможно открыть users.csv");
        exit(0);
    }
    int usernum;
    for (usernum = 0; !feof(file); usernum++) {
        fscanf(file, "%[^;]%*c", user[usernum].login);
        fscanf(file, "%[^;]%*c", user[usernum].pass);
        fscanf(file, "%d", &user[usernum].stud);
        getc(file);
        fscanf(file, "%d", &user[usernum].book);
        getc(file);
    }
    fclose(file);
    while (flag == 0) {
        printf("Введите логин: "); scanf("%s", login);
        printf("Введите пароль: "); scanf("%s", pass);
        status = acesslevel(login, pass, usernum);
        if (status != 0) {
            flag = 1;
        }
    }
    system("cls");
    flag = 0;
    if (status == 1)
    {
        printf("Что будем редактировать?:\n 1.Меню книг\n 2.Меню студентов\n 0.Выход\n");
        scanf("%d", &status);
        while (flag == 0) {
            if (status == 1) { 
                flag = 1; status = 3;
            }
            else if (status == 2) {
                flag = 1;
            }
            else if (status == 0) exit(0); 
            else printf("Введите другую цифру");
        }
        system("cls");
    }
    if (status == 2) {
        printf("Меню студентов:\n:");
        printf("1.Добавить студента\n");
        printf("2.Удалить студента\n");
        printf("3.Сделать бэкап студента\n");
        printf("4.Восстановить студента из бэкапа\n");
        printf("5.Поиск студента\n");
        printf("6.Информация о книгах у студента\n");
        printf("0.Выход \n");
    }
    if (status == 3) {
        printf("Меню книг:\n");
        printf("1.Добавить книгу\n");
        printf("2.Удалить книгу\n");
        printf("3.Выдать книгу\n");
        printf("4.Принять книгу\n");
        printf("5.Информация о книге у студентов\n");
        printf("0.Выход\n");
    }
    while(TRUE) {
        int strcoun = BookFileOpen();
        int studcoun = StudentFileOpen();
        int studbook = StudentBooksFileOpen();

        scanf("%d", &choice);

        if (status == 2)
        {
            if (choice == 1)
                addstudent(studcoun);
            else if (choice == 2)
                delstudent(studcoun, studbook);
            else if (choice == 3)
                backup(studcoun);
            else if (choice == 4)
                rebackup(studcoun);
            else if (choice == 5)
                searchstudent(studcoun);
            else if (choice == 6)
                booksofstud(studbook, strcoun, studcoun);
            else if (choice == 0)
                break;
            else { printf("wrong num\n"); }
        }
        if (status == 3)
        {
            if (choice == 1)
                addbook(strcoun);
            else if (choice == 2)
                delbook(strcoun, studbook);
            else if (choice == 3)
                givebook(studbook, strcoun, studcoun);
            else if (choice == 4)
                takebook(studbook, strcoun, studcoun);
            else if (choice == 5)
                studsofbook(studbook, strcoun, studcoun);
            else if (choice == 0)
                break;
            else { printf("wrong num\n"); }
        }
    }
    return 0;
}

int acesslevel(char* login, char* pass, int usercount) {
    int flag = 0;
    for (int i = 0; i < usercount; i++) {
        if ((strcmp(login, user[i].login) == 0) && (strcmp(pass, user[i].pass) == 0))
        {
            if ((user[i].stud == 1) && (user[i].book == 1))
                flag = 1;
            if ((user[i].stud == 1) && (user[i].book == 0))
                flag = 2;
            if ((user[i].stud == 0) && (user[i].book == 1))
                flag = 3;
            break;
        }
    }
    if (flag == 0)
        printf("Неверный логин или пароль\nПовторите попытку:\n");
    return flag;
}


int StudentBooksFileOpen() {
    if ((file = fopen("student_books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть student_books.csv");
        exit(0);
    }
    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%d", &stud[i].isbn);
        getc(file);
        fscanf(file, "%[^;]%*c", stud[i].num);
        fscanf(file, "%[^'\n']%*c", stud[i].date);
    }
    fclose(file);
    return i;
}

int BookFileOpen()
{
    if ((file = fopen("books.csv", "r")) == NULL){
        printf("Can't open books.csv");
        exit(0);
    }
    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%d", &book[i].isbn);
        getc(file);
        fscanf(file, "%[^;]%*c", book[i].author);
        fscanf(file, "%[^;]%*c", book[i].title);
        fscanf(file, "%d", &book[i].whole);
        getc(file);
        fscanf(file, "%d", &book[i].av);
        getc(file);
    }
    fclose(file);
    return i;
}

void addbook(int strcount)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    int isbn;
    char author[N], title[N];
    int all, ava, q = 0;
    if ((file = fopen("books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    fclose(file);

    file = fopen("books.csv", "a+");

    printf("Введите ISBN книги, которую хотите добавить: ");
    scanf("%lld", &isbn);

    for (int y = 0; y < strcount; y++)
    {
        if (isbn == book[y].isbn)
        {
            printf("Такая книга уже есть"); exit(2);
        }
    }

    printf("Автор: ");   
    scanf("%s", author);
    printf("Название: ");
    char c = getchar();
    while ((c = getchar()) != '\n')
    {
        title[q] = c; q++;
    }
    title[q] = '\0';

    printf("Всего книг: ");
    scanf("%d", &all);
    printf("Доступно: ");
    scanf("%d", &ava);

    fprintf(file, "\n%d;%s;%s;%d;%d", isbn, author, title, all, ava);
    fclose(file);
    printf("\nКнига добавлена\n");
}

void delbook(int strcount, int studbook)
{
    unsigned long long int delisbn;

    if ((file = fopen("books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    fclose(file);

    printf("Введите ISBN книги, которую хотите удалить: ");
    scanf("%d", &delisbn);

    int error = 0;
    for (int i = 0; i < studbook; i++)
    {
        if (stud[i].isbn == delisbn)
            error = 1;
    }

    if (error == 0)
    {
        file = fopen("books.csv", "r");
        char* a = (char*)malloc(strcount * (sizeof(struct books)));
        int w = 0;
        for (int i = 0; i < strcount; i++) {
            if (delisbn != book[i].isbn) {
                a[w] = getc(file);
                while ((a[w] != '\n') && (!feof(file))) {
                    w++;
                    a[w] = getc(file);
                }
                a[w] = '\n'; w++;
            }
            if (delisbn == book[i].isbn) {
                while ((getc(file) != '\n') && (!feof(file))) {
                    ;
                }
            }
        }
        a[w - 1] = '\0';
        fclose(file);
        file = fopen("books.csv", "w");
        for (int n = 0; a[n] != '\0'; n++)
            fprintf(file, "%c", a[n]);
        fclose(file);
        printf("\nКнига успешно удалена\n");
    }
    else
    {
        printf("\nНельзя удалять книги, находящиеся у студентов\n");
    }
}


int StudentFileOpen()
{
    if ((file = fopen("students.csv", "r")) == NULL)
    {
        printf("Невозможно открыть students.csv");
        exit(0);
    }
    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%[^;]%*c", student[i].num);
        fscanf(file, "%[^;]%*c", student[i].sname);
        fscanf(file, "%[^;]%*c", student[i].name);
        fscanf(file, "%[^;]%*c", student[i].otch);
        fscanf(file, "%[^;]%*c", student[i].fak);
        fscanf(file, "%[^'\n']%*c", student[i].napr);

    }
    fclose(file);
    return i;
}

void addstudent(int studcount)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    char num[8];
    char name[B];
    char sname[B];
    char otch[B];
    char fak[4];
    char napr[N];
    if ((file = fopen("students.csv", "r")) == NULL)
    {
        printf("Невозможно открыть students.csv");
        exit(0);
    }
    fclose(file);

    file = fopen("students.csv", "a+");

    printf("Введите номер зачетки, которую хотите добавить: ");
    scanf("%s", num);

    int y;
    for (y = 0; y < studcount; y++)
    {
        if (strcmp(num, student[y].num) == 0)
        {
            printf("Студент уже в базе");
            exit(2);
        }
    }

    printf("Фамилия: "); scanf("%s", sname);
    printf("Имя: ");  scanf("%s", name);
    printf("Отчество: "); scanf("%s", otch);
    printf("Факультет: "); scanf("%s", fak);
    printf("Направление: ");
    int q = 0;
    char c = getchar();
    while ((c = getchar()) != '\n')
    {
        napr[q] = c; q++;
    }
    napr[q] = '\0';

    fprintf(file, "\n%s;", num);
    fprintf(file, "%s;", sname);
    fprintf(file, "%s;", name);
    fprintf(file, "%s;", otch);
    fprintf(file, "%s;", fak);
    fprintf(file, "%s", napr);

    fclose(file);
    printf("\nСтудент добавлен\n");
}

void delstudent(int studcount, int studbook)
{
    char num[8];
    int w = 0;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    if ((file = fopen("students.csv", "r")) == NULL)
    {
        printf("Невозможно открыть students.csv");
        exit(0);
    }
    fclose(file);

    printf("Введите номер зачетки, которую хотите удалить: ");
    scanf("%s", num);

    int error = 0;
    for (int i = 0; i < studbook; i++)
    {
        if (strcmp(num, stud[i].num) == 0)
            error = 1;
    }
    if (error == 0)
    {
        file = fopen("students.csv", "r");
        char* a = (char*)malloc(studcount * (sizeof(struct students)));

        for (int y = 0; y < studcount; y++)
        {
            if (strcmp(num, student[y].num) == 0)
            {
                while ((getc(file) != '\n') && (!feof(file)))
                {
                }
            }
            if (strcmp(num, student[y].num) != 0)
            {
                a[w] = getc(file);
                while ((a[w] != '\n') && (!feof(file)))
                {
                    w++;
                    a[w] = getc(file);
                }
                a[w] = '\n'; w++;
            }
        }
        a[w - 1] = '\0';
        fclose(file);

        file = fopen("students.csv", "w");

        for (int n = 0; a[n] != '\0'; n++)
            fprintf(file, "%c", a[n]);

        fclose(file);
        printf("Студент успешно удален\n");
    }
    else
    {
        printf("\nНельзя удалять студентов, у которых есть книги\n");
    }
}

void searchstudent(int studcount)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    char surname[N];
    printf("Введите фамилию студента: ");
    scanf("%s", surname);

    for (int i = 0; i < studcount; i++)
    {
        if (strcmp(surname, student[i].sname) == 0)
        {
            printf("Зачетка %s \t ", student[i].num);
            printf("Фамилия: %s \t", student[i].sname);
            printf("Имя: %s \t", student[i].name);
            printf("Отчество: %s \t", student[i].otch);
            printf("Факультет: %s\t", student[i].fak);
            printf("Направление: %s\n", student[i].napr);
        }
    }
    printf("\n");
}

char* settime(struct tm* u)
{
    char s[20];
    char* tmp;
    for (int i = 0; i < 20; i++) s[i] = 0;
    int length = strftime(s, 20, "%d.%m.%Y %H.%M", u);
    tmp = (char*)malloc(sizeof(s));
    strcpy(tmp, s);
    return(tmp);
    free(tmp);
    free(s);
}


void backup(int strcount) {
    struct tm* u;
    char* stri;
    const time_t timer = time(NULL);
    u = localtime(&timer);
    stri = settime(u);

    char* a = 0;
    char name[31];

    if ((file = fopen("students.csv", "r")) == NULL)
    {
        printf("Невозможно открыть students.csv");
        exit(0);
    }
    a = (char*)malloc(strcount * (sizeof(struct students)));
    name[0] = 's'; name[1] = 't'; name[2] = 'u'; name[3] = 'd'; name[4] = 'e'; name[5] = 'n';
    name[6] = 't'; name[7] = 's'; name[8] = '_';

    for (int lon = 9; stri[lon - 9] != '\0'; lon++) {
        name[lon] = stri[lon - 9];
    }
    name[22] = '.'; name[25] = '.'; name[26] = 'c'; name[27] = 's'; name[28] = 'v'; name[29] = '\0';

    int i;
    for (i = 0; !feof(file); i++)
    {
        a[i] = getc(file);
    }
    a[i - 1] = '\0';

    file = fopen(name, "w");

    for (i = 0; (a[i] != '\0'); i++)
        fprintf(file, "%c", a[i]);

    fclose(file);

    if ((file = fopen(name, "r")) != NULL)
    {
        printf("Success\n");
        printf("Создан %s", name);
    }
    else printf("\nerror");
    printf("\n");
}

void rebackup(int strcount)
{
    int y;
    char name[MAX];
    int i = 0;
    printf("Введите название в форме students_DD.MM.YYYY HH.MM.csv: ");
    char c;
    while ((c = getchar()) != '\n') {
        name[i] = c;
        i++;
        c = getchar();
    }
    name[i] = '\0';
    fclose(file);
    file = fopen(name, "r");
    char* a = (char*)malloc((sizeof(char)));
    int n = 0;
    for (y = 0; !feof(file); y++) {
        a = (char*)realloc(a, (y + 1) * (sizeof(char)));
        a[y] = getc(file);
    }
    a[y - 1] = '\0';
    fclose(file);
    file = fopen("students.csv", "w");
    for (i = 0; (a[i] != '\0'); i++) {
        fprintf(file, "%c", a[i]);
    }
    fclose(file);
    printf("Файл восстановлен из %s", name);
    printf("\n");
}

void booksofstud(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    if ((file = fopen("books.csv", "r")) == NULL){
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    fclose(file);
    if ((file = fopen("student_books.csv", "r")) == NULL) {
        printf("Невозможно открыть student_books.csv");
        exit(0);
    }
    fclose(file);
    char num[8];
    int t = 0;
    int f = 0;
    printf("Номер зачетной книжки:");
    scanf("%s", num);
    for (int i = 0; i < studcount; i++) {
        if (strcmp(num, student[i].num) == 0)
            t = 1;
    }
    if (t == 1) {
        for (int i = 0; i < studbook; i++){
            if (strcmp(num, stud[i].num) == 0) {
                f = true;
                printf("Дата сдачи: %s \n ", stud[i].date);
                for (int j = 0; j < strcount; j++) {
                    if (stud[i].isbn == book[j].isbn){
                        printf("ISBN: %d \t ", book[j].isbn);
                        printf("Автор: %s \t", book[j].author);
                        printf("Название: %s \t", book[j].title);
                        printf("Всего книг: %d \t", book[j].whole);
                        printf("Доступно: %d\n", book[j].av);
                    }
                }
                printf("\n");
            }
        }
        if (f == false) {
            printf("\nУ этого студента нет книг\n");
        }
    }
    if (t == false) {
        printf("\nСтудента с таким номером зачетки не существует\n");
    }
}


void studsofbook(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    if ((file = fopen("books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    fclose(file);
    if ((file = fopen("student_books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть student_books.csv");
        exit(0);
    };
    fclose(file);
    int isbn;
    int t = 0;
    int f = 0;

    printf("Введите ISBN искомой книги:");
    scanf("%lld", &isbn);

    for (int i = 0; i < strcount; i++)
    {
        if (isbn == book[i].isbn)
            t = 1;
    }

    if (t == 1)
    {
        for (int j = 0; j < studbook; j++)
        {
            if (isbn == stud[j].isbn)
            {
                f = 1;
                printf("\nДата сдачи: %s\n", stud[j].date);

                for (int i = 0; i < studcount; i++)
                {
                    if (strcmp(stud[j].num, student[i].num) == 0)
                    {
                        printf("Зачетка: %s \t ", student[i].num);
                        printf("Фамилия: %s \t", student[i].sname);
                        printf("Имя: %s \t", student[i].name);
                        printf("Отчество: %s \t", student[i].otch);
                        printf("Факультет: %s\t", student[i].fak);
                        printf("Направление: %s\n", student[i].napr);
                    }
                }
                printf("\n");
            }
        }
        if (f == 0)
            printf("\nЭту книгу никто не брал\n");
    }
    if (t == 0)
        printf("\nКниги с таким ISBN не существует\n");
}

void givebook(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    int isbn;
    char num[8];
    int checkbook = 0;
    bool dat = false;
    int checkstudent = 0;
    int i;
    int checkdate;

    printf("ISBN книги для выдачи: ");
    scanf("%d", &isbn);
    if ((file = fopen("books.csv", "r")) == NULL)
    {
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    for (i = 0; i < strcount; i++) {
        if (isbn == book[i].isbn) {
            checkbook = 1;
            if (book[i].av > 0) {
                checkbook = 2;
                break;
            }
        }
    }
    fclose(file);
    if (checkbook == 0) {
        printf("Книги с таким ISBN не существует");
    }
    if (checkstudent == 0) {
        printf("Студента с таким номером зачетки не существует\n");
    }
    if (checkbook == 1) {
        printf("Нет доступных экземпляров нужной книги\n");
        char* date = 0;
        for (i = 0; i < studbook; i++) {
            if ((isbn == stud[i].isbn) && (dat == false)) {
                date = stud[i].date;
                dat = true;
                i++;
            }
            if ((isbn == stud[i].isbn) && (dat == true)) {
                checkdate = StrInt(stud[i].date, date);
                if (checkdate == 2)
                    date = stud[i].date;
            }
        }
        checkstudent = 1;
        printf("Экземпляр появится: %s\n", date);
    }
    if (checkbook == 2)
    {
        printf("Номер зачётки: ");
        scanf("%s", num);

        if ((file = fopen("students.csv", "r")) == NULL)
        {
            printf("Невозможно открыть students.csv");
            exit(0);
        }
        for (i = 0; i < studcount; i++)
        {
            if (strcmp(num, student[i].num) == 0)
            {
                checkstudent = 2;
            }
        }
        fclose(file);
    }
    
    if ((checkstudent == 2) && (checkbook == 2))
    {
        for (int i = 0; i < strcount; i++) {
            if (isbn == book[i].isbn)
                book[i].av--;
        }
        file = fopen("books.csv", "w");
        for (int i = 0; i < strcount; i++) {
            fprintf(file, "%d;", book[i].isbn);
            fprintf(file, "%s;", book[i].author);
            fprintf(file, "%s;", book[i].title);
            fprintf(file, "%d;", book[i].whole);
            if (i == (strcount - 1))
                fprintf(file, "%d", book[i].av);
            else
                fprintf(file, "%d\n", book[i].av);
        }
        fclose(file);

        file = fopen("student_books.csv", "a");

        struct tm* u;
        char* stri;
        time_t timer = time(NULL);
        timer += 604800;
        u = localtime(&timer);
        stri = settime(u);
        char givedate[11];
        for (i = 0; i < 10; i++)
        {
            givedate[i] = stri[i];
        }
        givedate[10] = '\0';
        fprintf(file, "\n%lld;", isbn);
        fprintf(file, "%s;", num);
        fprintf(file, "%s", givedate);
        fclose(file);
        printf("Книга выдана\n");
    }
}

int StrInt(char* newtmp, char* tmp)
{
    int day = 0, month = 0, year = 0, nday = 0, nmonth = 0, nyear = 0;
    day = (tmp[0] - '0') * 10;
    day += (tmp[1] - '0');
    month = (tmp[3] - '0') * 10;
    month += (tmp[4] - '0');

    year = (tmp[6] - '0') * 10;
    year += (tmp[7] - '0');
    year *= 10;
    year += (tmp[8] - '0');
    year *= 10;
    year += (tmp[9] - '0');

    nday = (newtmp[0] - '0') * 10;
    nday += (newtmp[1] - '0');

    nmonth = (newtmp[3] - '0') * 10;
    nmonth += (newtmp[4] - '0');

    nyear = (newtmp[6] - '0') * 10;
    nyear += (newtmp[7] - '0');
    nyear *= 10;
    nyear += (newtmp[8] - '0');
    nyear *= 10;
    nyear += (newtmp[9] - '0');

    if (nyear == year)
    {
        if (nmonth == month)
        {
            if (nday == day)
            {
                return 1;
            }
            if (nday < day)
            {
                return 2;
            }
            else return 3;
        }
        if (nmonth < month)
        {
            return 2;
        }
        else return 3;
    }
    if (nyear < year)
    {
        return 2;
    }
    else return 3;
}
void takebook(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    int isbn;
    char num[8];
    int checkbook = 0;
    bool dat = false;
    int checkstudent = 0;
    int i;
    int checkdate;
    int w = 0;

    printf("ISBN книги для приема: ");
    scanf("%d", &isbn);
    if ((file = fopen("books.csv", "r")) == NULL) {
        printf("Невозможно открыть books.csv");
        exit(0);
    }
    for (i = 0; i < strcount; i++) {
        if (isbn == book[i].isbn){
            checkbook = 1;
            if (book[i].av < book[i].whole)
            {
                checkbook = 2;
                break;
            }
        }
    }
    fclose(file);
    if (checkbook == 0) {
        printf("Такой книги нет в библиотеке");
    }
    if (checkbook == 1) {
        checkstudent = 1;
    }

    if (checkbook == 2) {
        printf("Номер зачётки: ");
        scanf("%s", num);
        if ((file = fopen("students.csv", "r")) == NULL){
            printf("Невозможно открыть students.csv");
            exit(0);
        }
        for (i = 0; i < studcount; i++){
            if (strcmp(num, student[i].num) == 0){
                checkstudent = 2;
            }
        }
        fclose(file);
    }
    if (checkstudent == 0) {
        printf("Нет такого студента или неверно введена зачетка\n");
    }
    if ((checkstudent == 2) && (checkbook == 2))
    {
        for (int i = 0; i < strcount; i++){
            if (isbn == book[i].isbn)
                book[i].av++;
        }
        if ((file = fopen("student_books.csv", "r")) == NULL){
            printf("Невозможно открыть student_books.csv");
            exit(0);
        }
        char* a = (char*)malloc(studbook * (sizeof(struct sb)));
        for (int y = 0; y < studbook; y++){
            if ((strcmp(num, stud[y].num) == 0) && (isbn == stud[y].isbn)){
                while ((getc(file) != '\n') && (!feof(file))) {
                    ;
                }
            }
            else {
                a[w] = getc(file);
                while ((a[w] != '\n') && (!feof(file))) {
                    w++;
                    a[w] = getc(file);
                }
                a[w] = '\n'; w++;
            }
        }
        a[w - 1] = '\0';
        fclose(file);
        file = fopen("student_books.csv", "w");
        for (int n = 0; a[n] != '\0'; n++) {
            fprintf(file, "%c", a[n]);
        }
        fclose(file);
        file = fopen("books.csv", "w");
        for (int i = 0; i < strcount; i++) {
            fprintf(file, "%d;", book[i].isbn);
            fprintf(file, "%s;", book[i].author);
            fprintf(file, "%s;", book[i].title);
            fprintf(file, "%d;", book[i].whole);
            if (i == (strcount - 1))
                fprintf(file, "%d", book[i].av);
            else
                fprintf(file, "%d\n", book[i].av);
        }
        fclose(file);
        printf("Книга принята\n");
    }
}

