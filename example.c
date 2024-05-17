#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

struct Car {
    char* Brand;      // Brand
    char* Model;      // Model
    int Year;         // Year of manufacture
    float Mileage;    // Mileage
    float Price;      // Price
};
typedef struct Car Car_type;

Car_type AddCar();
void ShowCars(Car_type* Cars, int countCars);
Car_type* SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery, int* foundCount);
Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy);
void WriteToFile(Car_type* Cars, int countCars, char* filename);
int SizeFromFile(char* filename);
Car_type* ReadFromFile(char* filename, int* size);
void FreeCars(Car_type* Cars, int countCars);

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    system("chcp 65001 > nul");

    Car_type* Cars = NULL;
    int countCars = 0, menu, sortBy;
    char filename[25] = "cars.txt"; // Default file

    do {
        printf("\nCurrent file: %s (cars: %d)\n", filename, countCars);
        printf("\n1 - Add a car\n"
            "2 - Display added cars\n"
            "3 - Find a car\n"
            "4 - Sort cars\n"
            "5 - Write car data to file\n"
            "6 - Read car data from file\n"
            "7 - Change file\n"
            "8 - Clear terminal\n"
            "0 - Exit\n\n"
            "Select an action: ");

        scanf("%d", &menu);
        getchar(); 

        switch (menu) {
        case 1:
            Cars = (Car_type*)realloc(Cars, ++countCars * sizeof(Car_type));
            Cars[countCars - 1] = AddCar();
            break;
        case 2:
            if (countCars > 0) ShowCars(Cars, countCars);
            else printf("Car list is empty.\n");
            break;
        case 3: {
            char searchQuery[100];
            printf("Enter brand or model to search: ");
            fgets(searchQuery, 100, stdin);
            searchQuery[strcspn(searchQuery, "\n")] = 0;
            int foundCount = 0;
            Car_type* result = SearchCarByBrandOrModel(Cars, countCars, searchQuery, &foundCount);
            if (result != NULL) ShowCars(result, foundCount);
            else printf("Car not found.\n");
            free(result);
            break;
        }
        case 4:
            if (countCars > 0) {
                printf("Choose sorting parameter:\n1 - Year\n2 - Price\n");
                scanf("%d", &sortBy);
                Cars = SortCarsByYearAndPrice(Cars, countCars, sortBy);
                ShowCars(Cars, countCars);
            }
            else printf("Car list is empty.\n");
            break;
        case 5:
            if (countCars > 0) WriteToFile(Cars, countCars, filename);
            else printf("Car list is empty.\n");
            break;
        case 6:
            FreeCars(Cars, countCars);
            Cars = ReadFromFile(filename, &countCars);
            if (countCars > 0) ShowCars(Cars, countCars);
            else printf("File is empty or does not exist.\n");
            break;
        case 7:
            printf("Enter file name: ");
            fgets(filename, 25, stdin);
            filename[strcspn(filename, "\n")] = 0;
            break;
        case 8:
            system("cls||clear");
            break;
        case 0:
            FreeCars(Cars, countCars);
            free(Cars);
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (menu != 0);

    return 0;
}

Car_type AddCar() {
    Car_type newCar;
    newCar.Brand = (char*)malloc(50 * sizeof(char));
    newCar.Model = (char*)malloc(50 * sizeof(char));
    printf("Enter car brand: ");
    fgets(newCar.Brand, 50, stdin);
    newCar.Brand[strcspn(newCar.Brand, "\n")] = 0;
    printf("Enter car model: ");
    fgets(newCar.Model, 50, stdin);
    newCar.Model[strcspn(newCar.Model, "\n")] = 0;
    printf("Enter year of manufacture: ");
    scanf("%d", &newCar.Year);
    printf("Enter mileage: ");
    scanf("%f", &newCar.Mileage);
    printf("Enter price: ");
    scanf("%f", &newCar.Price);
    getchar();
    return newCar;
}

void ShowCars(Car_type* Cars, int countCars) {
    printf("\n--------------------------------------------------------------------------------\n");
    printf("|    Brand    |    Model    | Year     |  Mileage  |    Price   |\n");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < countCars; i++) {
        printf("|%12s|%12s|%10d|%10.2f|%12.2f|\n", Cars[i].Brand, Cars[i].Model, Cars[i].Year, Cars[i].Mileage, Cars[i].Price);
        printf("--------------------------------------------------------------------------------\n");
    }
}

Car_type* SearchCarByBrandOrModel(Car_type* Cars, int countCars, char* searchQuery, int* foundCount) {
    Car_type* foundCars = (Car_type*)malloc(countCars * sizeof(Car_type)); 
    *foundCount = 0;
    for (int i = 0; i < countCars; i++) {
        if (strstr(Cars[i].Brand, searchQuery) != NULL || strstr(Cars[i].Model, searchQuery) != NULL) {
            foundCars[(*foundCount)++] = Cars[i];
        }
    }
    if (*foundCount == 0) {
        free(foundCars);
        return NULL;
    }
    return foundCars;
}

Car_type* SortCarsByYearAndPrice(Car_type* Cars, int countCars, int sortBy) {
    for (int i = 0; i < countCars - 1; i++) {
        for (int j = 0; j < countCars - i - 1; j++) {
            int shouldSwap = 0;
            switch (sortBy) {
            case 1: // Year
                shouldSwap = Cars[j].Year > Cars[j + 1].Year;
                break;
            case 2: // Price
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
        printf("Failed to open file for writing.\n");
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
    while ((ch = fgetc(file)) != EOF) {
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
        printf("Failed to open file for reading.\n");
        *size = 0;
        return NULL;
    }

    int tempSize = SizeFromFile(filename);
    Car_type* Cars = (Car_type*)malloc(tempSize * sizeof(Car_type)); 

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        Cars[i].Brand = (char*)malloc(50 * sizeof(char));
        Cars[i].Model

            = (char*)malloc(50 * sizeof(char));
        if (sscanf(line, "%49s %49s %d %f %f", Cars[i].Brand, Cars[i].Model, &Cars[i].Year, &Cars[i].Mileage, &Cars[i].Price) == 5) {
            i++;
        }
        else {
            free(Cars[i].Brand);
            free(Cars[i].Model);
        }
    }
    fclose(file);
    *size = i;
    return Cars;
}

void FreeCars(Car_type* Cars, int countCars) {
    for (int i = 0; i < countCars; i++) {
        free(Cars[i].Brand);
        free(Cars[i].Model);
    }
}
