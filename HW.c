#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>

char state;
char status;
char c[2];
int m;
int kaunt = 0;

int MainMenustd();
int MainMenu();
char* writestr(char str[]);

typedef struct studenttable { 
	char num[10]; 
	char surname[25]; 
	char name[15]; 
	char middlename[25];
	char fac[5];
	char dep[30];
} studenttable;


studentbook* loadstb(char* stb, int* l) {
	FILE* filestb = fopen(stb, "r");

	if (filestb == 0) {
		printf("Не удалось открыть файл '%s'\n", stb);
		*l = 0;
		return NULL;
	}

	studentbook* stbs = (studentbook*)malloc(sizeof(studentbook));
	*l = 0;

	while (feof(filestb) == 0) {
		fscanf(filestb, "%[^;]%*c", stbs[*l].isbn);
		fscanf(filestb, "%[^;]%*c;", stbs[*l].number);
		fscanf(filestb, "%d%*c", &stbs[*l].day);
		fscanf(filestb, "%d%*c", &stbs[*l].mon);
		fscanf(filestb, "%d%*c", &stbs[*l].year);
		*l = *l + 1;
		stbs = (studentbook*)realloc(stbs, (*l + 1) * sizeof(studentbook));
	}

	fclose(filestb);
	return stbs;
}

studenttable read_student() {
	studenttable students; // введение нового студента
	printf("Введите номер зачетной книжки: "); scanf("%[^\n]%*c", students.num);
	printf("Введите фамилию студента: "); scanf("%[^\n]%*c", students.surname);
	printf("Введите имя студента: "); scanf("%[^\n]%*c", students.name);
	printf("Введите отчество студента: "); scanf("%[^\n]%*c", students.middlename);
	printf("Введите факультет студента: "); scanf("%[^\n]%*c", students.fac);
	printf("Введите специальность студента: "); scanf("%[^\n]%*c", students.dep);
	return students;
}

studenttable* addstudent(studenttable* students, int* m) {
	printf("Введите информацию о новом студенте\n");
	studenttable astudent = read_student();

	for (int i = 0; i < *m; i++) { 
		if (!strcmp(students[i].num, astudent.num)) {
			printf("Студент с номером '%s' уже имеется\n", astudent.num);
			return students;
		}
	}

	students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable));
	students[*m] = astudent;
	(*m)++;

	return students;
}

studenttable* loadbackupst(studenttable* students, int* m) {
	char backupname[20];
	printf("Введите название файла бэкапа, который вы хотите загрузить: ");
	scanf("%[^\n]%*c", backupname);

	FILE* filest = fopen(backupname, "r");

	if (filest == 0) {
		printf("Не удалось открыть файл '%s'\n", backupname);
		*m = 0;
		return NULL;
	}

	studenttable* students = (studenttable*)malloc(sizeof(studenttable));
	*m = 0;

	while (feof(filest) == 0) {
		fscanf(filest, "%[^;]%*c", students[*m].num);
		fscanf(filest, "%[^;]%*c", students[*m].surname);
		fscanf(filest, "%[^;]%*c;", students[*m].name);
		fscanf(filest, "%[^;]%*c", students[*m].middlename);
		fscanf(filest, "%[^;]%*c", students[*m].fac);
		fscanf(filest, "%[^\n]%*c;", students[*m].dep);
		*m = *m + 1; // увеличиваем размер
		students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable));
	}

	fclose(filest);
	printf("Файл открыт\n");
	return students; 
}

studenttable* loadtablest(const char* tablest, int* m) {
	FILE* filest = fopen(tablest, "r");

	if (filest == 0) {
		printf("Не удалось открыть файл '%s'\n", tablest);
		*m = 0;
		return NULL;
	}

	studenttable* students = (studenttable*)malloc(sizeof(studenttable));
	*m = 0;

	while (feof(filest) == 0) {
		fscanf(filest, "%[^;]%*c", students[*m].num);
		fscanf(filest, "%[^;]%*c", students[*m].surname);
		fscanf(filest, "%[^;]%*c", students[*m].name);
		fscanf(filest, "%[^;]%*c", students[*m].middlename);
		fscanf(filest, "%[^;]%*c", students[*m].fac);
		fscanf(filest, "%[^\n]%*c", students[*m].dep);
		*m = *m + 1;
		students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable));
	}

	fclose(filest);
	return students;
}

void printstudent(studenttable students) {
	printf("%s ", students.num,")");
	printf("%s ", students.surname);
	printf("%s ", students.name);
	printf("%s ", students.middlename,", ");
	printf("факультет %s", students.fac,", ");
	printf("кафедра %s", students.dep);
	printf("|\n");
}

void delstudent(studenttable* students, int* m, studentbook* stbs, int* l);
void createbackupst(struct studenttable* backup, int m);
void findstudent(studenttable* students, int m);
studenttable* loadbackupst(char* tablest, int* m);
void logging(char* log, char* func);

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
	int lenst = 0;
	studenttable* students = loadtablest("students.csv", &lenst);
	printf("Введите логин(log): ");
	writestr(login);
	while (fgets(data, 100, users)) {
		char cpy[100];
		char* tmp;
		strcpy(cpy, data);
		tmp = strtok(cpy, ";");
		if (strcmp(login, tmp) == 0) {
			printf("Введите пароль(pas): ");
			arr++;
			tmp = strtok(NULL, ";");
			writestr(passwd);
			if (strcmp(passwd, tmp) != 0) {
				printf("Пароль неверный, попробуйте еще раз(inpas): ");
				return 0;
			}
			else {
				tmp = strtok(NULL, ";");
				strcpy(book1, tmp);
				tmp = strtok(NULL, ";");
				strcpy(studs, tmp);
				break;
			}


		}
	}if (arr == 0) {
		printf("Данного пользователя нет\n\nПопробуйте еще раз!(inlog)\n\n");
		return 0;
	}


	while (status != '0') {
		system("cls");
		if (kaunt != 0) {
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
					m = MainMenu();
					break;
				}
				else if (status == '2') {
					system("cls");
					m = MainMenustd();
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
		if (kaunt != 0) {
			kaunt = 0;
			state = NULL;
		}

		while (state != '0') {
			if (m == 1) {
				if (kaunt != 0 || (book1[0] != '1' || studs[0] != '1')) {
					MainMenu();
					state = getc(stdin);
				}
			}
			if (m == 2) {
				if (kaunt != 0 || (book1[0] != '1' || studs[0] != '1')) {
					MainMenustd();
					state = getc(stdin);
				}
				if (state == 1) {
					students = addstudent(students, &lenst);
					logging(login, "addstudent");
				}
				if (state == 2) {
					delstudent(students, &lenst);
					logging(login, "delstudent");
				}
				if (state == 3) {
					createbackupst(students, lenst);
					logging(login, "createbackupst");
				}
				if (state == 4) {
					loadbackupst(students, &lenst);
					logging(login, "loadbackupst");
				}
				if (state == 5) {
					findstudent(students, lenst);
					logging(login, "findstudent");
				}
				if (state == 0) {
					savetablest("students.csv", students, lenst);
					//savestb("student_books.csv", stbs, l);
					logging(login, "savetablest");
					free(users);
					free(students);
					//free(stbs);
					return 0;
				}
			}
		}
	}
}

void createbackupst(struct studenttable* backup, int m) {
	char backupname[50] = { 0 };
	struct tm* t;
	time_t timer = time(NULL);
	t = localtime(&timer);
	strftime(backupname, 50, "students_%d.%m.%Y; %H:%M:%S.csv", t);
	FILE* filest = fopen(backupname, "w");
	for (int i = 0; i < m; i++) {
		fprintf(filest, "%s;", backup[i].num);
		fprintf(filest, "%s;", backup[i].surname);
		fprintf(filest, "%s;", backup[i].name);
		fprintf(filest, "%s;", backup[i].middlename);
		fprintf(filest, "%s;", backup[i].fac);
		fprintf(filest, "%s", backup[i].dep);

		if (i < m - 1)
			fprintf(filest, "\n");
	}
	printf("Бэкап создан\n");
	fclose(filest);
}



void delstudent(studenttable* students, int* m){
	if (m == 0) {
		printf("Ни одного студента еще не добавлено\n");
		return;
	}

	char number[10];
	printf("Введите номер зачетной книжки студента, которого хотите удалить: ");
	scanf("%[^\n]%*c", &number);

	int flagst = 0; 
	int j = 0;

	for (int i = 0; i < *m; i++) {
		if (strcmp(students[i].num, number) != 0) {
			students[j++] = students[i];
		}
		else {
			flagst = 1;
		}
	}

	if (flagst == 0) {
		printf("Студента с номером '%s' не существует\n", number);
	}

	*m = j;
}

void findstudent(studenttable* students, int m) {
	char surname[25];
	int flagst = 0;
	printf("Введите фамилию, когорого хотите найти: ");
	scanf("%[^\n]%*c", surname);

	for (int i = 0; i < m; i++) {
		if (strcmp(students[i].surname, surname) == 0) {
			printstudent(students[i]);
			flagst++;
		}
	}
	if (flagst == 0) {
		printf("Студента с фамилией '%s' не существует\n", surname);
	}
}

void savestb(char* stb, studentbook* stbs, int l) {
	FILE* filestb = fopen(stb, "w");

	for (int i = 0; i < l; i++) {
		fprintf(filestb, "%s;", stbs[i].isbn);
		fprintf(filestb, "%s;", stbs[i].number);
		fprintf(filestb, "%d.", stbs[i].day);
		fprintf(filestb, "%d.", stbs[i].mon);
		fprintf(filestb, "%d", stbs[i].year);

		if (i < l - 1)
			fprintf(filestb, "\n");
	}

	fclose(filestb);
}

void savetablest(const char* tablest, studenttable* students, int m) {
	if (m != 0) {
		FILE* filest = fopen(tablest, "w");

		for (int i = 0; i < m; i++) {
			fprintf(filest, "%s;", students[i].num);
			fprintf(filest, "%s;", students[i].surname);
			fprintf(filest, "%s;", students[i].name);
			fprintf(filest, "%s;", students[i].middlename);
			fprintf(filest, "%s;", students[i].fac);
			fprintf(filest, "%s", students[i].dep);

			if (i < m - 1)
				fprintf(filest, "\n");
		}

		fclose(filest);
	}
	else {
		printf("Ни одного студента еще не добавлено\n");
	}
}

void logging(char* log,const char* func) {
	FILE* logging = fopen("library.log", "a");
	time_t s_time;
	struct tm* m_time;
	char timer[100];
	s_time = time(NULL);
	m_time = localtime(&s_time);
	strftime(timer, 128, "%d.%m.%y %H:%M:%S", m_time);
	fprintf(logging, "%s;%s;%s", timer, log, func);
	fprintf(logging, "\n");
	fclose(logging);
}

int MainMenu() {
	printf("-------Книжное меню-------\n\n");
	printf("(1) Добавить новую книгу\n");
	printf("(2) Удалить книгу\n");
	printf("(3) Редактировать информацию по книге\n");
	printf("(4) Изменить количество книг по номеру ISBN\n");
	printf("(5) Просмотреть информацию студентов по номеру ISBN\n");
	printf("(6) Выдача книг\n");
	printf("(7) Сдача книги\n");
	printf("(0) Выйти\n");
	printf(" Ваш выбор: ");
	return 1;
}

int MainMenustd() {
	printf("\n-------Студентечское меню-------\n\n");
	printf("(1) Добавить нового студента\n");
	printf("(2) Удалить студента\n");
	printf("(3) Создать бэкап\n");
	printf("(4) Восстановить базу данных из файла бэкапа\n");
	printf("(5) Поиск по фамилии\n");
	printf("(0) Выйти\n");
	printf(" Ваш выбор: ");
	return 2;
}

char* writestr(char str[]) {
	int i = 0;
	int c = getchar();
	while (c != EOF && c != '\n') {
		str[i++] = c;
		c = getchar();
	}str[i] = '\0';
	return str;
}
