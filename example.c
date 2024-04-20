#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

struct Car {
    char* Brand;      // Марка
    char* Model;      // Модель
    int Year;         // Год выпуска
    float Mileage;    // Пробег
    float Price;      // Стоимость
};
typedef struct Car Car_type;

Car_type AddCar();
void ShowCars(Car_type* Cars, int countCars);
Car_type SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery);
Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy);
void WriteToFile(Car_type* Cars, int countCars, char* filename);
int SizeFromFile(char* filename);
Car_type* ReadFromFile(char* filename, int* size);
void FreeCars(Car_type* Cars, int countCars);

int main() {
    setlocale(LC_ALL, "RUS");
    system("chcp 1251 > nul");

    Car_type* Cars = NULL;
    int countCars = 0, menu, sortBy;
    char filename[25] = "cars.txt"; // Файл по умолчанию

    do {
        printf("\nТекущий файл: %s (автомобилей: %d)\n", filename, countCars);
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
        getchar(); // Для захвата '\n' после ввода числа

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
            fgets(searchQuery, 100, stdin);
            searchQuery[strcspn(searchQuery, "\n")] = 0; // Удаление новой строки
            Car_type result = SearchCarByBrandOrModel(Cars, countCars, searchQuery);
            if (strlen(result.Brand) > 0) ShowCars(&result, 1);
            else printf("Автомобиль не найден.\n");
            break;
        }
        case 4:
            if (countCars > 0) {
                printf("Выберите параметр сортировки:\n1 - Год\n2 - Цена\n");
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
            free(Cars); // Освобождаем старый массив
            Cars = ReadFromFile(filename, &countCars);
            if (countCars > 0) ShowCars(Cars, countCars);
            else printf("Файл пуст или не существует.\n");
            break;
        case 7:
            printf("Введите название файла: ");
            fgets(filename, 25, stdin);
            filename[strcspn(filename, "\n")] = 0; // Удаление новой строки
            break;
        case 8:
            system("cls||clear");
            break;
        case 0:
            FreeCars(Cars, countCars);
            free(Cars);
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (menu != 0);

    return 0;
}

Car_type AddCar() {
    Car_type newCar;
    newCar.Brand = (char*)malloc(50 * sizeof(char));
    newCar.Model = (char*)malloc(50 * sizeof(char));
    printf("Введите марку автомобиля: ");
    fgets(newCar.Brand, 50, stdin);
    newCar.Brand[strcspn(newCar.Brand, "\n")] = 0; // Удаление новой строки
    printf("Введите модель автомобиля: ");
    fgets(newCar.Model, 50, stdin);
    newCar.Model[strcspn(newCar.Model, "\n")] = 0; // Удаление новой строки
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
    printf("\n--------------------------------------------------------------------------------\n");
    printf("|    Марка    |    Модель   | Год вып. |  Пробег  |  Стоимость  |\n");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < countCars; i++) {
        printf("|%12s|%12s|%10d|%9.2f|%12.2f|\n", Cars[i].Brand, Cars[i].Model, Cars[i].Year, Cars[i].Mileage, Cars[i].Price);
        printf("--------------------------------------------------------------------------------\n");
    }
}

Car_type SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery) {
    Car_type notFound = { NULL, NULL, 0, 0.0f, 0.0f }; // Инициализация пустого автомобиля для возвращения в случае отсутствия результата
    for (int i = 0; i < countCars; i++) {
        if (strstr(Cars[i].Brand, searchQuery) != NULL || strstr(Cars[i].Model, searchQuery) != NULL) {
            return Cars[i];
        }
    }
    return notFound;
}

Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy) {
    for (int i = 0; i < countCars - 1; i++) {
        for (int j = 0; j < countCars - i - 1; j++) {
            int shouldSwap = 0;
            switch (sortBy) {
            case 1: // Год
                shouldSwap = Cars[j].Year > Cars[j + 1].Year;
                break;
            case 2: // Цена
                shouldSwap = Cars[j].Price > Cars[j + 1].Price;
                break;
            }
            if (shouldSwap) {
                Car_type temp = Cars[j];
                Cars[j] = Cars[j + 1];
                Cars[j + 1] = temp;
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
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            count++;
        }
    }
    fclose(file);
    return count;
}

Car_type* ReadFromFile(char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл для чтения.\n");
        *size = 0;
        return NULL;
    }

    int tempSize = SizeFromFile(filename);
    Car_type* Cars = (Car_type*)malloc(tempSize * sizeof(Car_type));

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        Cars[i].Brand = (char*)malloc(50 * sizeof(char));
        Cars[i].Model = (char*)malloc(50 * sizeof(char));
        if (sscanf(line, "%49s %49s %d %f %f", Cars[i].Brand, Cars[i].Model, &Cars[i].Year, &Cars[i].Mileage, &Cars[i].Price) == 5) {
            i++;
        }
        else {
            // Обработка ошибки, если строка не соответствует ожидаемому формату
            free(Cars[i].Brand);
            free(Cars[i].Model);
        }
    }
    fclose(file);
    *size = i; // Обновляем фактический размер массива, основанный на количестве успешно прочитанных записей
    return Cars;
}


void FreeCars(Car_type* Cars, int countCars) {
    for (int i = 0; i < countCars; i++) {
        free(Cars[i].Brand);
        free(Cars[i].Model);
    }
}
