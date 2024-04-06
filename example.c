#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

struct Car {
    char Brand[50];      // Марка
    char Model[50];      // Модель
    int Year;            // Год выпуска
    float Mileage;       // Пробег
    float Price;         // Стоимость
};
typedef struct Car Car_type;

Car_type AddCar();
void ShowCars(Car_type* Cars, int countCars);
Car_type SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery);
Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy);
void WriteToFile(Car_type* Cars, int countCars, char* filename);
int SizeFromFile(char* filename);
Car_type* ReadFromFile(char* filename);


int main() {
    setlocale(LC_ALL, "RUS");
    system("chcp 1251 > nul");

    Car_type* Cars = NULL;
    int countCars = 0, menu, sortBy;
    char filename[25] = "cars.txt"; // Файл по умолчанию
    struct __stat64 st;
    time_t t = time(NULL); // Текущая дата и время

    do {
        printf("\nТекущий файл: %s (автомобилей: %d)", filename, countCars);
        if (__stat64(filename, &st) == 0) {
            printf("\nПоследнее редактирование: %s", ctime(&st.st_mtime));
        }
        printf("\n1 - Добавить автомобиль\n"
            "2 - Вывод добавленных автомобилей\n"
            "3 - Найти автомобиль\n"
            "4 - Отсортировать автомобили\n"
            "5 - Запись данных об автомобилях в файл\n"
            "6 - Чтение данных об автомобилях из файла\n"
            "7 - Сменить файл\n"
            "8 - Очистка терминала\n"
            "0 - Выход\n\n"
            "Выберите действие: ");

        scanf("%d", &menu);
        getchar();

        switch (menu) {
        case 1:
            Cars = realloc(Cars, ++countCars * sizeof(Car_type));
            Cars[countCars - 1] = AddCar();
            break;
        case 2:
            if (countCars > 0) ShowCars(Cars, countCars);
            else printf("Список автомобилей пуст.\n");
            break;
        case 3: {
            char searchQuery[100];
            printf("Введите марку или модель для поиска: ");
            gets(searchQuery);
            Car_type result = SearchCarByBrandOrModel(Cars, countCars, searchQuery);
            if (strcmp(result.Brand, "") != 0) ShowCars(&result, 1);
            else printf("Автомобиль не найден.\n");
            break;
        }
        case 4:
            if (countCars > 0) {
                printf("Выберите параметр сортировки:\n");
                printf("1 - Год\n");
                printf("2 - Цена\n");
                printf("3 - Пробег\n");
                printf("4 - Алфавит марки\n");
                scanf("%d", &sortBy);
                Cars = SortCarsByYearAndPrice(Cars, countCars, sortBy);
                ShowCars(Cars, countCars);
            }
            else printf("Список автомобилей пуст.\n");
            break;
        case 5:
            if (countCars > 0) WriteToFile(Cars, countCars, filename);
            else printf("Список автомобилей пуст.\n");
            break;
        case 6:
            countCars = SizeFromFile(filename);
            Cars = realloc(Cars, sizeof(Car_type) * countCars);
            Cars = ReadFromFile(filename);
            if (countCars > 0) ShowCars(Cars, countCars);
            else printf("Файл пуст или не существует.\n");
            break;
        case 7:
            printf("Введите название файла: ");
            gets(filename);
            break;
        case 8:
            system("cls");
            break;
        case 0:
            free(Cars);
            return 0;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (menu != 0);

    free(Cars);
    return 0;
}

Car_type AddCar() {
    Car_type newCar;
    printf("Введите марку автомобиля: ");
    gets(newCar.Brand);
    printf("Введите модель автомобиля: ");
    gets(newCar.Model);
    printf("Введите год выпуска: ");
    scanf("%d", &newCar.Year);
    printf("Введите пробег: ");
    scanf("%f", &newCar.Mileage);
    printf("Введите стоимость: ");
    scanf("%f", &newCar.Price);
    getchar();
    return newCar;
}

void ShowCars(Car_type* Cars, int countCars) {
    printf("\n---------------------------------------------------------------------------------\n");
    printf("|    Марка    |    Модель   | Год вып. |  Пробег  |  Стоимость  |\n");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < countCars; i++) {
        printf("|%12s|%12s|%10d|%9.2f|%12.2f|\n", Cars[i].Brand, Cars[i].Model, Cars[i].Year, Cars[i].Mileage, Cars[i].Price);
        printf("---------------------------------------------------------------------------------\n");
    }
}

Car_type SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery) {
    Car_type notFound = { "", "", 0, 0.0f, 0.0f };
    for (int i = 0; i < countCars; i++) {
        if (strstr(Cars[i].Brand, searchQuery) != NULL || strstr(Cars[i].Model, searchQuery) != NULL) {
            return Cars[i];
        }
    }
    return notFound;
}

Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy) {
    Car_type temp;
    for (int i = 0; i < countCars - 1; i++) {
        for (int j = 0; j < countCars - i - 1; j++) {
            switch (sortBy) {
            case 1: // Год
                if (Cars[j].Year > Cars[j + 1].Year || (Cars[j].Year == Cars[j + 1].Year && Cars[j].Price > Cars[j + 1].Price)) {
                    temp = Cars[j];
                    Cars[j] = Cars[j + 1];
                    Cars[j + 1] = temp;
                }
                break;
            case 2: // Цена
                if (Cars[j].Price > Cars[j + 1].Price || (Cars[j].Price == Cars[j + 1].Price && Cars[j].Year > Cars[j + 1].Year)) {
                    temp = Cars[j];
                    Cars[j] = Cars[j + 1];
                    Cars[j + 1] = temp;
                }
                break;
            case 3: // Пробег
                if (Cars[j].Mileage > Cars[j + 1].Mileage || (Cars[j].Mileage == Cars[j + 1].Mileage && Cars[j].Year > Cars[j + 1].Year)) {
                    temp = Cars[j];
                    Cars[j] = Cars[j + 1];
                    Cars[j + 1] = temp;
                }
                break;
            case 4: // Алфавит марки
                if (strcmp(Cars[j].Brand, Cars[j + 1].Brand) > 0 || (strcmp(Cars[j].Brand, Cars[j + 1].Brand) == 0 && Cars[j].Year > Cars[j + 1].Year)) {
                    temp = Cars[j];
                    Cars[j] = Cars[j + 1];
                    Cars[j + 1] = temp;
                }
                break;
            }
        }
    }
    return Cars;
}

void WriteToFile(Car_type* Cars, int countCars, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Не удалось открыть файл для записи.\n");
        return;
    }
    for (int i = 0; i < countCars; i++) {
        fprintf(file, "%s %s %d %.2f %.2f\n", Cars[i].Brand, Cars[i].Model, Cars[i].Year, Cars[i].Mileage, Cars[i].Price);
    }
    fclose(file); 
}

int SizeFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    int count = 0;
    while (!feof(file)) {
        if (fgetc(file) == '\n') count++;
    }
    fclose(file);
    return count;
}

Car_type* ReadFromFile(char* filename) {
    int size = SizeFromFile(filename);
    Car_type* Cars = malloc(size * sizeof(Car_type));
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл для чтения.\n");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        fscanf(file, "%s %s %d %f %f\n", Cars[i].Brand, Cars[i].Model, &Cars[i].Year, &Cars[i].Mileage, &Cars[i].Price);
    }
    fclose(file);
    return Cars;
}
