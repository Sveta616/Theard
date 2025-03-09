#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

// Размер массивов фиксированный
const int n = 1000000;

// Избегание переполнения стека
int arr1[n];
int arr2[n];
int result[n];

// Функция для умножения двух массивов без использования потоков
void WithOut() {
    for (int i = 0; i < n; ++i) {
        result[i] = arr1[i] * arr2[i];
    }
}

// Функция для умножения части массива с потоком
void With(int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = arr1[i] * arr2[i];
    }
}


void RezToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        for (int i = 0; i < n; ++i) {
            outFile << result[i] << " ";
        }
        outFile.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
    }
}


void PrintToConsole(const std::string& name, int arr[], int size) {
    std::cout << name << ": ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");


    for (int i = 0; i < n; ++i) {
        arr1[i] = rand() % 100;
        arr2[i] = rand() % 100;
    }


    std::cout << "Первые 10 элементов массивов:" << std::endl;
    PrintToConsole("arr1 (первые 10 элементов)", arr1, 10);
   PrintToConsole("arr2 (первые 10 элементов)", arr2, 10);
   std::cout << "--------------------------------------"<< std::endl;

    // Умножение без потоков
    auto start = std::chrono::high_resolution_clock::now();
     WithOut();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Время выполнения без потоков: " << elapsed.count() << " секунд" << std::endl;


    std::cout << "Результат умножения (первые 10 элементов):" << std::endl;
    PrintToConsole("result (первые 10 элементов)", result, 10);


    RezToFile("without.txt");
    std::cout << "--------------------------------------" << std::endl;

    // Умножение с использованием потоков
    const int numThreads = 4; // Количество потоков
    std::thread threads[numThreads];
    int partSize = n / numThreads; // Размер части массива для каждого потока

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numThreads; ++i) {
        int startIndex = i * partSize;
        int endIndex;

        // Опредтление конца диапазона для текущего потока
        if (i == numThreads - 1) {
            endIndex = n; // Последний поток обрабатывает оставшиеся элементы
        }
        else {
            endIndex = startIndex + partSize;
        }

        // Создание потока для обработки части массива
        threads[i] = std::thread(With, startIndex, endIndex);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Время выполнения с потоками: " << elapsed.count() << " секунд" << std::endl;

 
    std::cout << "Результат умножения с потоками (первые 10 элементов):" << std::endl;
    PrintToConsole("result (первые 10 элементов)", result, 10);


    RezToFile("with.txt");

    return 0;
}