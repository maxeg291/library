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

struct books { unsigned long long int isbn; char author[N]; char title[N]; int all; int ava; };
struct books book[MAX];

struct students { char num[8]; char name[B]; char sname[B]; char otch[B]; char fak[4];  char napr[N]; };
struct students student[MAX];

struct users { char login[20]; char pass[20]; int stud; int book; };
struct users user[MAX];

struct sb { unsigned long long int isbn; char num[8]; char date[12]; };
struct sb stud[MAX];

int BookFileOpen();
int StudentFileOpen();
int UserFileOpen();
int StudentBooksFileOpen();

int UserAccess(char* login, char* pass, int usercount);

void StudentAdd(int studcount);
void StudentDelete(int studcount, int studbook);
void BookAdd(int strcount);
void BookDelete(int strcount, int studbook);

void Recovery(int strcount);
void Backup(int strcount);

char* settime(struct tm* u);
void SearchBySurname(int studcount);

int BookDelCheck(int studbook, unsigned long long int isbn);
int StudentDelCheck(int studbook, char* num);
void CheckInfoStud(int studbook, int strcount, int studcount);
void CheckInfoBook(int studbook, int strcount, int studcount);

void Bookgive(int studbook, int strcount, int studcount);
void Booktake(int studbook, int strcount, int studcount);
int StrInt(char* tmp, char* newtmp);

FILE* file;

void Valid(int q) {
    if (q == 1)
    {
        if ((file = fopen("users.csv", "r")) == NULL)
        {
            printf("Can't open users.csv");
            exit(1);
        }
    }
    if (q == 2)
    {
        if ((file = fopen("books.csv", "r")) == NULL)
        {
            printf("Can't open books.csv");
            exit(1);
        }
    }
    if (q == 3)
    {
        if ((file = fopen("students.csv", "r")) == NULL)
        {
            printf("Can't open students.csv");
            exit(1);
        }
    }
    if (q == 4)
    {
        if ((file = fopen("student_books.csv", "r")) == NULL)
        {
            printf("Can't open student_books.csv");
            exit(1);
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    int num;
    int status;
    int usercoun = UserFileOpen();
    char pass[20], login[20];
    int tr = 0;

    while (tr == 0) {
        printf("Введите логин: "); scanf("%s", login);
        printf("Введите пароль: "); scanf("%s", pass);
        status = UserAccess(login, pass, usercoun);
        if (status != 0) {
            tr = 1;
        }
    }
    system("cls");
    tr = 0;
    if (status == 1)
    {
        printf("Что будем редактировать?:\n 1.Меню книг\n 2.Меню студентов\n 0.Выход\n");
        scanf("%d", &status);
        while (tr == 0) {
            if (status == 1) { tr = 1; status = 3; }
            else if (status == 2) tr = 1;
            else if (status == 0) exit(0); 
            else printf("Введите другую цифру");
        }
        system("cls");
    }
    if (status == 2)
        printf("Меню студентов:\n 1.Добавить студента\n 2.Удалить студента\n 3.Сделать бэкап студента\n 4.Восстановить студента из бэкапа\n 5.Поиск студента\n 6.Информация о книгах у студента\n 0.Выход \n");
    if (status == 3)
        printf("Меню книг:\n 1.Добавить книгу\n 2.Удалить книгу\n 3.Выдать книгу\n 4.Принять книгу\n 5.Информация о книге у студентов\n0.Выход\n");
    for (;;)
    {

        int strcoun = BookFileOpen();
        int studcoun = StudentFileOpen();
        int studbook = StudentBooksFileOpen();

        scanf("%d", &num);

        if (status == 2)
        {
            if (num == 1)
                StudentAdd(studcoun);
            else if (num == 2)
                StudentDelete(studcoun, studbook);
            else if (num == 3)
                Backup(studcoun);
            else if (num == 4)
                Recovery(studcoun);
            else if (num == 5)
                SearchBySurname(studcoun);
            else if (num == 6)
                CheckInfoStud(studbook, strcoun, studcoun);
            else if (num == 0)
                break;
            else { printf("wrong num\n"); }
        }
        if (status == 3)
        {
            if (num == 1)
                BookAdd(strcoun);
            else if (num == 2)
                BookDelete(strcoun, studbook);
            else if (num == 3)
                Bookgive(studbook, strcoun, studcoun);
            else if (num == 4)
                Booktake(studbook, strcoun, studcoun);
            else if (num == 5)
                CheckInfoBook(studbook, strcoun, studcoun);
            else if (num == 0)
                break;
            else { printf("wrong num\n"); }
        }
    }
    return 0;
}

int UserAccess(char* login, char* pass, int usercount) {
    int flag = 0;
    for (int i = 0; i < usercount; i++)
    {
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


int StudentBooksFileOpen()
{
    Valid(4);

    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%lld", &stud[i].isbn);
        getc(file);
        fscanf(file, "%[^;]%*c", stud[i].num);
        fscanf(file, "%[^'\n']%*c", stud[i].date);
    }
    fclose(file);
    return i;
}


int UserFileOpen()
{
    Valid(1);

    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%[^;]%*c", user[i].login);
        fscanf(file, "%[^;]%*c", user[i].pass);
        fscanf(file, "%d", &user[i].stud);
        getc(file);
        fscanf(file, "%d", &user[i].book);
        getc(file);
    }
    fclose(file);
    return i;
}

int BookFileOpen()
{
    Valid(2);
    int i;
    for (i = 0; !feof(file); i++)
    {
        fscanf(file, "%lld", &book[i].isbn);
        getc(file);
        fscanf(file, "%[^;]%*c", book[i].author);
        fscanf(file, "%[^;]%*c", book[i].title);
        fscanf(file, "%d", &book[i].all);
        getc(file);
        fscanf(file, "%d", &book[i].ava);
        getc(file);
    }
    fclose(file);
    return i;
}


/////////////////////
// ДОБАВЛЕНИЕ КНИГ //
/////////////////////


void BookAdd(int strcount)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    unsigned long long int isbn;
    char author[N], title[N];
    int all, ava;
    int q = 0;

    Valid(2);
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

    printf("Автор: ");   scanf("%s", author);
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

    fprintf(file, "\n%lld;%s;%s;%d;%d", isbn, author, title, all, ava);
    fclose(file);
    printf("\nКнига добавлена\n");
}


//////////////////
// УДАЛЕНИЕ КНИГ//
//////////////////

void BookDelete(int strcount, int studbook)
{
    unsigned long long int isbn;

    Valid(2);
    fclose(file);

    printf("Введите ISBN книги, которую хотите удалить: ");
    scanf("%lld", &isbn);

    int error = 0;
    for (int i = 0; i < studbook; i++)
    {
        if (stud[i].isbn == isbn)
            error = 1;
    }

    if (error == 0)
    {
        file = fopen("books.csv", "r");
        char* a = (char*)malloc(strcount * (sizeof(struct books)));
        int w = 0;

        for (int y = 0; y < strcount; y++)
        {
            if (isbn != book[y].isbn)
            {
                a[w] = getc(file);
                while ((a[w] != '\n') && (!feof(file)))
                {
                    w++;
                    a[w] = getc(file);
                }
                a[w] = '\n'; w++;
            }
            if (isbn == book[y].isbn)
            {
                while ((getc(file) != '\n') && (!feof(file)))
                {
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

////////////////////////////////////////////////////////////////////////
////////////////// STUDENTS ///////////////////////////////////
////////////////////////////////////////////////////////////////////


int StudentFileOpen()
{
    Valid(3);
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


////////////////
// ДОБАВЛЕНИЕ //
////////////////


void StudentAdd(int studcount)
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
    //Locale();
    Valid(3);
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


////////////////
// УДАЛЕНИЕ //
////////////////

void StudentDelete(int studcount, int studbook)
{
    char num[8];
    int w = 0;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    Valid(3);
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
                    ;
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


/////////////////////
// ПОИСК ПО ФАМИЛИИ //
/////////////////////


void SearchBySurname(int studcount)
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


//////////////////////////////
// ВОССТАНОВЛЕНИЕ ИЗ БЭКАПА //
//////////////////////////////

void Recovery(int strcount)
{
    int y;
    char name[MAX];
    int i = 0;

    printf("Введите название в форме <<students_DD.MM.YYYY HH.MM.csv>>: ");

    char c;
    while ((c = getchar()) != '\n')
    {
        name[i] = c;
        i++;
        c = getchar();
    }
    name[i] = '\0';

    fclose(file);

    file = fopen(name, "r");

    char* a = (char*)malloc((sizeof(char)));
    int n = 0;

    for (y = 0; !feof(file); y++)
    {
        a = (char*)realloc(a, (y + 1) * (sizeof(char)));
        a[y] = getc(file);
    }
    a[y - 1] = '\0';

    fclose(file);

    file = fopen("students.csv", "w");

    for (i = 0; (a[i] != '\0'); i++)
        fprintf(file, "%c", a[i]);

    fclose(file);

    printf("Файл восстановлен из %s", name);
    printf("\n");
}


///////////
// БЭКАП //
///////////


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


void Backup(int strcount) {
    struct tm* u;
    char* stri;
    const time_t timer = time(NULL);
    u = localtime(&timer);
    stri = settime(u);

    char* a = 0;
    char name[31];
    
    Valid(3);
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


///////////////////////////////////
// ИНФОРМАЦИЯ ПО КНИГАМ СТУДЕНТА //
///////////////////////////////////


void CheckInfoStud(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    Valid(2);
    fclose(file);
    Valid(4);
    fclose(file);
    char zachnum[8];
    bool t = false;
    bool f = false;

    printf("Номер зачетной книжки:");
    scanf("%s", zachnum);

    for (int i = 0; i < studcount; i++)
    {
        if (strcmp(zachnum, student[i].num) == 0)
        {
            t = true;
        }
    }

    if (t == true)
    {
        for (int i = 0; i < studbook; i++)
        {
            if (strcmp(zachnum, stud[i].num) == 0)
            {
                f = true;
                printf("Дата сдачи: %s \n ", stud[i].date);
                for (int j = 0; j < strcount; j++)
                {
                    if (stud[i].isbn == book[j].isbn)
                    {
                        printf("ISBN: %lld \t ", book[j].isbn);
                        printf("Автор: %s \t", book[j].author);
                        printf("Название: %s \t", book[j].title);
                        printf("Всего книг: %d \t", book[j].all);
                        printf("Доступно: %d\n", book[j].ava);
                    }
                }
                printf("\n");
            }
        }
        if (f == false)
            printf("\nУ этого студента нет книг\n");
    }
    if (t == false)
        printf("\nСтудента с таким номером зачетки не существует\n");
}


void CheckInfoBook(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    Valid(2);
    fclose(file);
    Valid(4);
    fclose(file);
    unsigned long long int isbn;
    bool t = false;
    bool f = false;

    printf("ISBN книги для поиска:");
    scanf("%lld", &isbn);

    for (int i = 0; i < strcount; i++)
    {
        if (isbn == book[i].isbn)
        {
            t = true;
        }
    }

    if (t == true)
    {
        for (int j = 0; j < studbook; j++)
        {
            if (isbn == stud[j].isbn)
            {
                f = true;
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
        if (f == false)
            printf("\nЭту книгу никто не брал\n");
    }
    if (t == false)
        printf("\nКниги с таким ISBN не существует\n");
}


////////////////////////////
// ВЫДАЧА И СДАЧА КНИГ /////
////////////////////////////


void Bookgive(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    unsigned long long int isbn;
    char num[8];
    int checkbook = 0;
    bool dat = false;
    int checkstudent = 0;
    int i;
    int checkdate;

    printf("ISBN книги для выдачи: ");
    scanf("%lld", &isbn);
    Valid(2);
    for (i = 0; i < strcount; i++)
    {
        if (isbn == book[i].isbn)
        {
            checkbook = 1;
            if (book[i].ava > 0)
            {
                checkbook = 2;
                break;
            }
        }
    }

    fclose(file);

    if (checkbook == 0) printf("Книги с таким ISBN не существует");

    if (checkbook == 1)
    {
        printf("Нет доступных экземпляров нужной книги\n");
        char* date = 0;
        for (i = 0; i < studbook; i++)
        {
            if ((isbn == stud[i].isbn) && (dat == false))
            {
                date = stud[i].date;
                dat = true;
                i++;
            }
            if ((isbn == stud[i].isbn) && (dat == true))
            {
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

        Valid(3);
        for (i = 0; i < studcount; i++)
        {
            if (strcmp(num, student[i].num) == 0)
            {
                checkstudent = 2;
            }
        }
        fclose(file);
    }

    if (checkstudent == 0)
        printf("Студента с таким номером зачетки не существует\n");

    if ((checkstudent == 2) && (checkbook == 2))
    {
        for (int i = 0; i < strcount; i++)
        {
            if (isbn == book[i].isbn)
                book[i].ava--;
        }
        file = fopen("books.csv", "w");
        for (int i = 0; i < strcount; i++)
        {
            fprintf(file, "%lld;", book[i].isbn);
            fprintf(file, "%s;", book[i].author);
            fprintf(file, "%s;", book[i].title);
            fprintf(file, "%d;", book[i].all);
            if (i == (strcount - 1))
                fprintf(file, "%d", book[i].ava);
            else
                fprintf(file, "%d\n", book[i].ava);
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
    int day = 0;
    int month = 0;
    int year = 0;
    int nday = 0;
    int nmonth = 0;
    int nyear = 0;

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


void Booktake(int studbook, int strcount, int studcount) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    unsigned long long int isbn;
    char num[8];
    int checkbook = 0;
    bool dat = false;
    int checkstudent = 0;
    int i;
    int checkdate;
    int w = 0;

    printf("ISBN книги для приема: ");
    scanf("%lld", &isbn);
    Valid(2);
    for (i = 0; i < strcount; i++)
    {
        if (isbn == book[i].isbn)
        {
            checkbook = 1;
            if (book[i].ava < book[i].all)
            {
                checkbook = 2;
                break;
            }
        }
    }

    fclose(file);

    if (checkbook == 0) printf("Такой книги нет в библиотеке");

    if (checkbook == 1)
    {
        printf("Книги все в библиотеке\n");
        checkstudent = 1;
    }

    if (checkbook == 2)
    {
        printf("Номер зачётки: ");
        scanf("%s", num);

        Valid(3);
        for (i = 0; i < studcount; i++)
        {
            if (strcmp(num, student[i].num) == 0)
            {
                checkstudent = 2;
            }
        }
        fclose(file);
    }

    if (checkstudent == 0)
        printf("Нет такого студента или неверно введена зачетка\n");

    if ((checkstudent == 2) && (checkbook == 2))
    {
        for (int i = 0; i < strcount; i++)
        {
            if (isbn == book[i].isbn)
                book[i].ava++;
        }

        Valid(4);
        char* a = (char*)malloc(studbook * (sizeof(struct sb)));

        for (int y = 0; y < studbook; y++)
        {
            if ((strcmp(num, stud[y].num) == 0) && (isbn == stud[y].isbn))
            {
                while ((getc(file) != '\n') && (!feof(file)))
                {
                    ;
                }
            }
            else
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

        file = fopen("student_books.csv", "w");
        for (int n = 0; a[n] != '\0'; n++)
            fprintf(file, "%c", a[n]);

        fclose(file);

        file = fopen("books.csv", "w");
        for (int i = 0; i < strcount; i++)
        {
            fprintf(file, "%lld;", book[i].isbn);
            fprintf(file, "%s;", book[i].author);
            fprintf(file, "%s;", book[i].title);
            fprintf(file, "%d;", book[i].all);
            if (i == (strcount - 1))
                fprintf(file, "%d", book[i].ava);
            else
                fprintf(file, "%d\n", book[i].ava);
        }
        fclose(file);
        printf("Книга принята\n");
    }
}
