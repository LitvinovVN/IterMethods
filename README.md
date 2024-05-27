# IterMethods
Репозиторий для изучения итерационных методов решения сеточных уравнений

## [https://litvinovvn.github.io/IterMethods/](https://litvinovvn.github.io/IterMethods/)

## Формирование документации с помощью doxygen

1. Cоздаем шаблонный файл Doxyfile командой:

doxygen -g

2. Для генерации документации

doxygen

3. [Документация](doxygen/html/index.html)

## Описание библиотеки функций
### vector1d.c
1. Структура **vector1d**.
Описывает вектор в пространстве $R^n$.
Поле **length** содержит количество элементов массива, доступного по указателю  **data**.
```c++
typedef struct
{
    int length;
    double *data;
} vector1d;
```

2. Функция **vector1d_create** - создаёт в динамической памяти структуру **vector1d**, задаёт значение поля **length** равным параметру **size** и создаёт динамический массив элементов типа **double**. Возвращает указатель **vector1d\*** на созданную структуру.   

```c++
vector1d* vector1d_create(int size)
{
    vector1d *vec1d = (vector1d*)malloc(sizeof(vector1d));
    vec1d->length = size;    
    vec1d->data = (double*)malloc(vec1d->length * sizeof(double));

    return vec1d;
}
```

2.реализовать метод зейделя