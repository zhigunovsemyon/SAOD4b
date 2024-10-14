#include <stdbool.h> /*bool, true, false*/
#include <stddef.h>
#include <stdio.h>  /*printf()*/
#include <stdlib.h> /*EXIT_SUCCESS, malloc(), free()*/
#include <string.h> /*memcpy()*/
#include <time.h> /*time()*/

// Тип данных, используемый в алгоритме
#define DATATYPE long
// Строка для вывода одного элемента в printf()
#define PRINT_EL(iter) printf("%ld ", iter)

// Однобайтовый тип данных
typedef unsigned char byte;

void PrintArray(DATATYPE const *const Arr, size_t const len) {
    for (size_t i = 0; i < len; i++)
        PRINT_EL(Arr[i]);
        
    putchar('\n');
}

// Функция замены местами двух элементов размера element_size
void GenSwap(void *const a, void *const b, size_t const element_size) {
    for (size_t i = 0; i < element_size; ++i) {
        byte const tmp = ((byte *)a)[i];
        ((byte *)a)[i] = ((byte *)b)[i];
        ((byte *)b)[i] = tmp;
    }
}

/* Функция реализует сортировку алгоритмом quicksort. В процессе работы
 * применяет рекурсию 2-го порядка */
void My_qsort2(void *const source, // Область памяти, которую надо сортировать
               size_t const element_count, // Число элементов
               size_t const element_size,  // Размер одного элемента
               int (*compar)(const void *const,
                             const void *const) // Функция сравнения
) {
    // Пограничное условие
    if (element_count < 2)
        return;

    // Указатель на ключевой элемент массива
    byte *pivot_ptr = (byte *)source + (element_count - 1) * element_size;
    // Указатель на текущий элемент массива
    byte *cur = (byte *)source;

    // Цикл перебирает каждый элемент массива до ключевого
    do {
        /*Если функция сортировки вернула признак переноса
        (выбрана сорт-ка по возр., cur был больше pivot_ptr), то:*/
        if (compar(cur, pivot_ptr) > 0) {
            // Ключевой и идущий перед ним элемент переставляются
            GenSwap(pivot_ptr - element_size, pivot_ptr, element_size);
            pivot_ptr -= element_size;
            /*Если ключ и текущий элемент не были соседними, и, соответственно,
             * уже поменянными на прошлом шагу, то текущий элемент, и элемент,
             * оказавшийся по правое плечо от ключа, меняются местами*/
            if (pivot_ptr != cur) {
                GenSwap(cur, pivot_ptr + element_size, element_size);
            }
        } else {
            /*Если функция сорт-ки не вернула признак перестановки,
             * указатель на текущий элемент переставляется на следующий*/
            cur += element_size;
        }

        /*Если указатель на текущий элемент оказался в области правого плеча,
         * сравнивать с ключём нет смысла */
    } while (pivot_ptr > cur);

    // Размер левого плеча
    size_t const leftlen = (size_t)(pivot_ptr - (byte *)source) / element_size;
    // Размер правого плеча
    size_t const rightlen = element_count - leftlen - 1;

    // Повторный запуск сортировок для каждого из плеч
    My_qsort2(source, leftlen, element_size, compar);
    My_qsort2(pivot_ptr + element_size, rightlen, element_size, compar);
}

//Счётчик числа сравнений
size_t count = 0;

// Функция сортировки int чисел по убыванию
int DescIntSort(const void *const a, const void *const b) {
    // Локальные копии объектов
    const DATATYPE x = *(DATATYPE *)a, y = *(DATATYPE *)b;
    /*  Если первое число больше второго, возврат -1,
        Если второе число больше первого, возврат +1
        Если два числа равны, оба неравенства ложны => 0 - 0 == 0*/
    count++;
    return (x < y) - (x > y);
}

// Функция сортировки int чисел по возрастанию
int AscIntSort(const void *const a, const void *const b) {
    // Локальные копии объектов
    const DATATYPE x = *(DATATYPE *)a, y = *(DATATYPE *)b;
    /*  Если первое число больше второго, возврат +1,
        Если второе число больше первого, возврат -1
        Если два числа равны, оба неравенства ложны => 0 - 0 == 0*/
    count++;
    return (x > y) - (x < y);
}

void FillWithRandom(void *const ptr, size_t memlen){
    while(memlen--)
        ((byte *)ptr)[memlen] = (byte)rand();
}

int main(int const argc, char const *const *const argv) {
    size_t const A_len = (argc == 1) ? 2048lu : (size_t)atoi(argv[1]);
    DATATYPE *A = (DATATYPE *)malloc(sizeof(DATATYPE) * A_len);
    if (A == nullptr)
        return EXIT_FAILURE;
    srand((unsigned int)time(NULL));
    FillWithRandom(A, A_len * sizeof(DATATYPE));

    // Размер массива
    printf("Размер массива: %lu\n", A_len);

    printf("Массив до изменения:\t");
    PrintArray(A, A_len);

    My_qsort2((void *)A, A_len, sizeof(DATATYPE), AscIntSort);
    printf("Массив после сорт-ки по возрастанию: ");
    PrintArray(A, A_len);
    printf("Шагов для сортировки по возр. %lu\n", count);
    count = 0;

    My_qsort2((void *)A, A_len, sizeof(DATATYPE), DescIntSort);
    printf("Шагов для сортировки по убыв. %lu\n", count);
    // printf("Массив после сорт-ки по убыванию   : ");
    // PrintArray(A, A_len);

    free(A);
    return EXIT_SUCCESS;
}
