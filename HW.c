#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <ctime>

typedef struct books {
    char isbn[20];
    char avt[60];
    char bname[60];
    int q;
    int qs;
} bookstruct;
bookstruct books[1000];
typedef struct students {
    char num[20];
    char surname[30];
    char name[30];
    char name2[40];
    char fac[5];
    char depart[40];
} studstruct;
studstruct students[1000];
struct stbooks {
    char time[20];
};
stbooks stdbooks[100];

char* writestr(char str[]); //запись строки в массив
int readbooks(const char* file); //количество книг в файле
int readstud(const char* file); //количество студентов в файле
int comparebooks(char isbn[20]); //возвращает ISBN одинаковых книг
int comparestud(char num[15]); //возвращает номера зачеток одинаковых студентов
int have(char isbn[20]); //проверка наличия книг у студента
void bookfunc(FILE* book); //все операции с книгами
void delbook(const char* file); //удаление книги
void studfunc(FILE* stdents); //все операции со студентами
void findst(const char* filename); //поиск студента по фамилии
void delstudent(const char* file); //удаление студента

void findst(const char* filename) {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FILE* stdents;
    char surname[40];
    char file[5000];
    printf("Введите фамилию студента:");
    scanf_s("%s", surname);
    stdents = fopen(filename, "r+");
    rewind(stdents);
    int t = -1;
    for (int i = 0; i < readstud("students.csv"); i++)
        if (strcmp(surname, students[i].surname) == 0)
            t = i;
    int getfile = t;
    printf("%s;%s;%s;%s;%s;%s\n", students[getfile].num, students[getfile].surname, students[getfile].name, students[getfile].name2, students[getfile].fac, students[getfile].depart);
    fclose(stdents);
}

char* writestr(char str[]) {
    int a, i = 0;
    while ((a = getchar()) != EOF && a != '\n') {
        str[i++] = a;
    }
    str[i] = '\0';
    return str;
}

int StudMenu();
int BookMenu();
int BookMenu() {
    printf("********Редактирование книг********\n");
    printf("(1) Добавить новую книгу\n");
    printf("(2) Удалить книгу\n");
    printf("(3) Редактировать информацию по книге\n");
    printf("(4) Просмотреть информацию студентов по номеру ISBN\n");
    printf("(5) Выдача/сдача книг\n");
    printf("(0) Выйти\n");
    FILE* book;
    book = fopen("books.csv", "r");
    bookfunc(book);
    fclose(book);
    return 1;
}

int StudMenu() {
    printf("********Редактирование студентов********\n");
    printf("(1) Добавить нового студента\n");
    printf("(2) Удалить студента\n");
    printf("(3) Редактировать информацию по студенту\n");
    printf("(4) Поиск студента\n");
    printf("(5) Выдача книг/сдача\n");
    printf("(0) Выйти\n");
    FILE* stdents;
    stdents = fopen("students.csv", "r");
    studfunc(stdents);
    fclose(stdents);
    return 2;

}

int readbooks(const char* file) {
    FILE* book = fopen(file, "r+");
    rewind(book);
    char str[1000];
    int i = 0;
    while (fgets(str, 1000, book))
    {
        fscanf_s(book, "%[^;];%[^;];%[^;];%d;%d", books[i].isbn, books[i].avt, books[i].bname, &books[i].q, &books[i].qs);
        i++;
    }
    return i;
}

int readstud(const char* file) {
    FILE* stdents = fopen(file, "r+");
    rewind(stdents);
    char str[1000];
    int i = 0;
    while (fgets(str, 1000, stdents)) {
        fscanf_s(stdents, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]", students[i].num, students[i].surname, students[i].name, students[i].name2, students[i].fac, students[i].depart);
        i++;
    }
    return i;
}

int comparebooks(char isbn[20]) {
    int t = -1;
    for (int i = 0; i < readbooks("books.csv"); i++)
        if (strcmp(isbn, books[i].isbn) == 0)
            t = i;
    return t;
}

int comparestud(char num[15]) {
    int t = -1;
    for (int i = 0; i < readstud("students.csv"); i++)
        if (strcmp(num, students[i].num) == 0)
            t = i;
    return t;
}

int have(char isbn[20]) {
    int i, j;
    int t = -1;
    for (i = 0; i < readbooks("student_books.csv"); i++)
        if (strcmp(isbn, books[i].isbn) == 0)
            t = i;
    t = i;
    return t;
}

void delbook(const char* file) {
    FILE* book;
    char delisbn[20];
    printf("ISBN:");
    fflush(stdin);
    scanf_s("%s", delisbn);
    book = fopen(file, "r");
    rewind(book);
    int getfile = comparebooks(delisbn);
    int getfile1 = readbooks(file);
    fclose(book);
    if (getfile != -1)
    {
        book = fopen("books.csv", "w");
        int i;
        for (i = 0; i < getfile1; i++)
        {
            if (i != getfile)
                fprintf(book, "%s;%s;%s;%d;%d\n", books[i].isbn, books[i].avt, books[i].bname, books[i].q, books[i].qs);
        }
        fclose(book);
        printf("\nКнига удалена!\n");
    }
    else
        printf("\nКниги с таким ISBN не существует\n");
}

void bookfunc(FILE* book) {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FILE* studbooks;
    struct books elem;
    book = fopen("books.csv", "r");
    char file[5000];
    while (fgets(file, 5000, book) != NULL) {
        printf("%s", file);
    }
    fclose(book);
    int add;
    printf("Чего надо, хозяин?:");
    scanf_s("%d", &add);
    if (add == 1) {
        book = fopen("books.csv", "a+");
        printf("ISBN:");
        fflush(stdin);
        scanf_s("%s", elem.isbn);
        if (comparebooks(elem.isbn) != -1)
        {
            printf("Книга с таким ISBN уже добалена!\n");
            fclose(book);
        }
        else {
            printf("Автора книги:");
            scanf_s("%s", elem.avt);
            printf("Название книги:");
            scanf_s("%s", elem.bname);
            printf("Введите количество:");
            scanf_s("%d", &elem.q);
            printf("Введите количество доступных книг:");
            scanf_s("%d", &elem.qs);
            book = fopen("books.csv", "a+");
            studbooks = fopen("student_books.csv", "a+");
            if (elem.qs != elem.q) {
                fprintf(book, "%s;%s;%s;%d;%d\n", elem.isbn, elem.avt, elem.bname, elem.q, elem.qs);
                fprintf(studbooks, "%s\n", elem.isbn);
            }
            else fprintf(book, "%s;%s;%s;%d;%d\n", elem.isbn, elem.avt, elem.bname, elem.q, elem.qs);
            fclose(book);
            fclose(studbooks);
        }
    }
    if (add == 2) {
        FILE* stubooks;
        stubooks = fopen("student_books.csv", "r");
        char str[5000];
        while (fgets(str, 5000, stubooks) != NULL)
            printf("%s", str);
        printf("ISBN:");
        scanf_s("%s", elem.isbn);
        int l = have(elem.isbn);
        if (l != 1) {
            printf("Нельзя удалять книги, находящие у студентов!\n");
        }
        else
            delbook("books.csv");
    }
    if (add == 3) {
        printf("\nВведите ISBN:");
        scanf_s("%s", elem.isbn);
        book = fopen("books.csv", "r");
        rewind(book);
        int getfile = comparebooks(elem.isbn);
        int getfile1 = readbooks("books.csv");
        fclose(book);
        if (getfile != -1)
        {
            printf("Автор книги:");
            scanf_s("%s", elem.avt);
            printf("Название книги:");
            scanf_s("%s", elem.bname);
            printf("Всего книг:");
            scanf_s("%d", &elem.q);
            printf("Количество доступных книг:");
            scanf_s("%d", &elem.qs);
            book = fopen("books.csv", "w");
            int i;
            for (i = 0; i < getfile1; i++)
            {
                if (i != getfile)
                    fprintf(book, "%s;%s;%s;%d;%d\n", books[i].isbn, books[i].avt, books[i].bname, books[i].q, books[i].qs);
                fprintf(book, "%s;%s;%s;%d;%d\n", elem.isbn, elem.avt, elem.bname, elem.q, elem.qs);
            }
            fclose(book);
        }
        else
            printf("\nНет такого ISBN!\n ");
    }
    if (add == 4) {

    }
    if (add == 5) {
        struct students elem1;
        struct stbooks elem2;
        FILE* book, * studbook, * stdents;
        printf("\nISBN:");
        scanf_s("%s", elem.isbn);
        printf("Номер зачетки:");
        scanf_s("%s", elem1.num);
        book = fopen("books.csv", "r");
        stdents = fopen("students.csv", "r");
        rewind(book);
        rewind(stdents);
        int getfile = comparebooks(elem.isbn);
        int getfile2 = comparestud(elem1.num);
        int getfile1 = readbooks("books.csv");
        int getfile12 = readstud("students.csv");
        fclose(book);
        fclose(stdents);
        if (getfile != 1 && getfile2 != 1)
        {
            printf("Введите автора:");
            scanf_s("%s", elem.avt);
            printf("Введите название:");
            scanf_s("%s", elem.bname);
            printf("Количество книг:");
            scanf_s("%d", &elem.q);
            printf("Количество доступных книг:");
            scanf_s("%d", &elem.qs);
            printf("Введите дату сдачи:");
            scanf_s("%s", elem2.time);
            book = fopen("books.csv", "w");
            stdents = fopen("students.csv", "r");
            studbook = fopen("student_books.csv", "a+");
            int i, j;
            for (i = 0; i < getfile1; i++)
            {
                for (j = 0; j < getfile12; j++)
                    if (i != getfile && j != getfile2)
                        fprintf(book, "%s;%s;%s;%d;%d\n", books[i].isbn, books[i].avt, books[i].bname, books[i].q, books[i].qs);
                fprintf(studbook, "%s;%s;%s\n", books[i].isbn, students[j].num, elem2.time);
            }
            int n;
            printf("1-уменьшить; 2-увеличить");
            scanf_s("%d", &n);
            if (n == 2) {
                if (elem.qs < elem.q) {
                    fprintf(book, "%s;%s;%s;%d;%d\n", elem.isbn, elem.avt, elem.bname, elem.q, elem.qs + 1);
                    fprintf(studbook, "%s;%s;%s\n", elem.isbn, elem1.num, elem2.time);
                }
                else printf("Нечегозабирать!");
            }
            else if (n == 1) {
                if (elem.qs > 1) {
                    fprintf(book, "%s;%s;%s;%d;%d\n", elem.isbn, elem.avt, elem.bname, elem.q, elem.qs - 1);
                    fprintf(studbook, "%s;%s;%s\n", elem.isbn, elem1.num, elem2.time);
                }
                else {
                    printf("Недостаточно книг\n");
                    printf("Книга будет сдана: %s", elem2.time);
                }
                fclose(book);
            }
            else
                    printf("\nНет таких ISBN или зачетки!\n ");
        }
    }
    if (add == 0) {
        exit(0);
    }
}

void delstudent(const char* file) {
    FILE* studentsf;
    char dbooknum[15];
    printf("\nВведите номер зачетки:");
    fflush(stdin);
    scanf_s("%s", dbooknum);
    studentsf = fopen(file, "r");
    rewind(studentsf);
    int getfile = comparestud(dbooknum);
    int getfile1 = readstud(file);
    fclose(studentsf);
    if (getfile != -1)
    {
        studentsf = fopen("students.csv", "w");
        int i;
        for (i = 0; i < getfile1; i++)
        {
            if (i != getfile)
                fprintf(studentsf, "%s;%s;%s;%s;%s;%s\n", students[i].num, students[i].surname, students[i].name, students[i].name2, students[i].fac, students[i].depart);
        }
        fclose(studentsf);
        printf("\nСтудент удален!\n");
    }
    else
        printf("\nНет такой зачетки\n");
}

void studfunc(FILE* stdents) {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    struct students elem;
    FILE* stubooks;
    char file[5000];
    stdents = fopen("students.csv", "r");
    while ((fgets(file, 5000, stdents)) != NULL) {
        printf("\n%s", file);
    }
    fclose(stdents);
    printf("\nЧего надо, хозяин?:");
    int add;
    scanf_s("%d", &add);
    if (add == 1) {
        printf("\nВведите номер зачетки:");
        scanf_s("%s", elem.num);
        if (comparestud(elem.num) != -1)
        {
            printf("Такая зачетка уже есть!\n");
            fclose(stdents);
        }
        else {
            printf("Введите фамилию:");
            scanf_s("%s", elem.surname);
            printf("Введите имя:");
            scanf_s("%s", elem.name);
            printf("Введите отчество:");
            scanf_s("%s", elem.name2);
            printf("Введите факультет:");
            scanf_s("%s", elem.fac);
            printf("Введите кафедру:");
            scanf_s("%s", elem.depart);
            stdents = fopen("students.csv", "a+");
            fprintf(stdents, "%s;%s;%s;%s;%s;%s\n", elem.num, elem.surname, elem.name, elem.name2, elem.fac, elem.depart);
        }
    }
    if (add == 2) {
        stubooks = fopen("student_books.csv", "r");
        char str[5000];
        while (fgets(str, 5000, stubooks) != NULL)
            printf("%s", str);
        printf("Введите номер зачетки:");
        scanf_s("%s", elem.num);
        int l = have(elem.num);
        if (l != -1) {
            printf("У этого студента остались книги, удалить невозможно!\n");
        }
        else
            delstudent("students.csv");
    }
    if (add == 3) {
        printf("\nВведите номер зачетки:");
        scanf_s("%s", elem.num);
        stdents = fopen("students.csv", "r");
        rewind(stdents);
        int getfile = comparestud(elem.num);
        int getfile1 = readstud("students.csv");
        fclose(stdents);
        if (getfile != -1)
        {
            printf("Введите фамилию:");
            scanf_s("%s", elem.surname);
            printf("Введите имя:");
            scanf_s("%s", elem.name);
            printf("Введите отчество:");
            scanf_s("%s", &elem.name2);
            printf("Введите факультет:");
            scanf_s("%s", &elem.fac);
            printf("Введите кафедру:");
            scanf_s("%s", elem.depart);
            stdents = fopen("students.csv", "w");
            int i;
            for (i = 0; i < getfile1; i++)
            {
                if (i != getfile)
                    fprintf(stdents, "%s;%s;%s;%s;%s;%s\n", students[i].num, students[i].surname, students[i].name, students[i].name2, students[i].fac, students[i].depart);
                fprintf(stdents, "%s;%s;%s;%s;%s;%s\n", elem.num, elem.surname, elem.name, elem.name2, elem.fac, elem.depart);
            }
            fclose(stdents);
        }
        else
            printf("\nНет такой зачетки!\n ");
    }
    if (add == 4) {
        findst("students.csv");
    }
    if (add == 5) {
        struct books elem1;
        struct stbooks elem2;
        FILE* book, * studbook;
        printf("\nВведите ISBN:");
        scanf_s("%s", elem1.isbn);
        printf("Введите номер зачетки:");
        scanf_s("%s", elem.num);
        book = fopen("books.csv", "r");
        stdents = fopen("students.csv", "r");
        rewind(book);
        rewind(stdents);
        int getfile = comparebooks(elem1.isbn);
        int getfile2 = comparestud(elem.num);
        int getfile1 = readbooks("books.csv");
        int getfile12 = readstud("students.csv");
        fclose(book);
        fclose(stdents);
        if (getfile == 0 && getfile2 == 0)
        {
            printf("Введите автора:");
            scanf_s("%s", elem1.avt);
            printf("Введите название:");
            scanf_s("%s", elem1.bname);
            printf("Количество книг:");
            scanf_s("%d", &elem1.q);
            printf("Количество доступных книг:");
            scanf_s("%d", &elem1.qs);
            printf("Введите дату сдачи:");
            scanf_s("%s", elem2.time);
            book = fopen("books.csv", "w");
            stdents = fopen("students.csv", "r");
            studbook = fopen("student_books.csv", "a+");
            int i, j;
            for (i = 0; i < getfile1; i++) {
                for (j = 0; j < getfile12; j++)
                    if (i != getfile && j != getfile2)
                        fprintf(book, "%s;%s;%s;%d;%d\n", books[i].isbn, books[i].avt, books[i].bname, books[i].q, books[i].qs);
                        fprintf(studbook, "%s;%s;%s\n", books[i].isbn, students[j].num, elem2.time);
            }
            int m;
            printf("1-выдать; 2-забрать:");
            scanf_s("%d", &m);
            if (m == 2) {
                if (elem1.qs <= elem1.q) {
                    fprintf(book, "%s;%s;%s;%d;%d\n", elem1.isbn, elem1.avt, elem1.bname, elem1.q, elem1.qs + 1);
                    fprintf(studbook, "%s;%s;\n", books[i].isbn, students[j].num);
                }

                else printf("Нечего забирать!");
            }
            else if (m == 1) {
                if (elem1.qs >= 1) {
                    fprintf(book, "%s;%s;%s;%d;%d\n", books[i].isbn, books[i].avt, books[i].bname, books[i].q, books[i].qs);
                    fprintf(book, "%s;%s;%s;%d;%d\n", elem1.isbn, elem1.avt, elem1.bname, elem1.q, elem1.qs - 1);
                    fprintf(studbook, "%s;%s;%s\n", elem1.isbn, elem.num, elem2.time);
                }
                else printf("Недостаточно книг\n");
                printf("Книга будет сдана: %s", elem2.time);
            }
            fclose(book);
        }
        else
            printf("\nНет таких ISBN или зачетки!\n ");
    }
    if (add == 0) {
        exit(0);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FILE* users;
    users = fopen("users.csv", "r+");
    char data[100];
    char book1[5];
    char studs[5];
    char login[100];
    char passwd[100];
    int arr = 0;
    char state;
    char status = '\0';
    char c[2];
    int m;
    int count = 0;
    printf("Введите логин: ");
    writestr(login);
    while (fgets(data, 100, users)) {
        char cpy[100];
        char* temp;
        strcpy(cpy, data);
        temp = strtok(cpy, ";");
        if (strcmp(login, temp) == 0) {
            printf("Введите пароль: ");
            arr++;
            temp = strtok(NULL, ";");
            writestr(passwd);
            if (strcmp(passwd, temp) != 0) {
                printf("Пароль неверный, попробуйте еще раз: ");
                return 0;
            }
            else {
                temp = strtok(NULL, ";");
                strcpy(book1, temp);
                temp = strtok(NULL, ";");
                strcpy(studs, temp);
                break;
            }


        }
    }
    if (arr == 0) {
        printf("Данного пользователя нет\n\nПопробуйте еще раз!(inlog)\n\n");
        return 0;
    }
    while (status != '0') {
        system("cls");
        if (count != 0) {
            writestr(c);
        }
        if (book1[0] == '1' && studs[0] == '1') {
            printf("----------Главное меню-------------\n");
            printf("(1) Меню книг\n");
            printf("(2) Меню студентов\n");
            printf("(0) Выйти\n");
            printf(" Ваш Выбор: ");
            status = getc(stdin);
            while (true) {
                if (status == '1') {
                    system("cls");
                    m = BookMenu();
                    break;
                }
                else if (status == '2') {
                    system("cls");
                    m = StudMenu();
                    break;
                }
                else if (status == '0') {
                    printf("\n----------До связи----------");
                    return 0;

                }
                else {
                    printf("\nВведите другую цифру: ");
                    status = getc(stdin);
                }
            }
        }
        else if (book1[0] == '1' && studs[0] != '1') {
            m = 1;
        }
        else if (book1[0] != '1' && studs[0] == '1') {
            m = 2;
        }
        if (count != 0) {
            count = 0;
            state = NULL;
        }

        while (state != '0') {
            if (m == 1) {
                if (count != 0 || (book1[0] != '1' || studs[0] != '1')) {
                    BookMenu();
                    state = getc(stdin);
                }
            }
            if (m == 2) {
                if (count != 0 || (book1[0] != '1' || studs[0] != '1')) {
                    StudMenu();
                    state = getc(stdin);
                }
            }
        }
    }
}

int have(char isbn[20]) {
    int i, j;
    int t = -1;
    for (i = 0; i < readbooks("student_books.csv"); i++)
        if (strcmp(isbn, books[i].isbn) == 0)
            t = i;
    t = i;
    return t;
}
