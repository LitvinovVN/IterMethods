#pragma once

#include <iostream>

/// @brief Класс для описания геометрии расчетной области в двумерном формате
class Geometry2d
{
    /// @brief Количество подобластей расчетной области по Ox
    unsigned nx;
    // Количество подобластей расчетной области по Oy
    unsigned ny;

    /// @brief Длина подобласти расчетной области по Ox
    unsigned Lx;
    // Длина подобласти расчетной области по Oy
    unsigned Ly;

    // Двумерный массив с данными, описывающими расчетную область
    unsigned** data;

public:
    Geometry2d(unsigned nx, unsigned ny, unsigned Lx, unsigned Ly)
    {
        this->nx = nx;
        this->ny = ny;
        this->Lx = Lx;
        this->Ly = Ly;

        data = new unsigned*[nx]{};
        for (unsigned i{}; i < nx; i++)
        {
            data[i] = new unsigned[ny]{};
        }
    }

    ~Geometry2d()
    {
        // std::cout << "~Geometry2d()" << std::endl;
        for (unsigned i{}; i < nx; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    void Print()
    {
        for (unsigned i{}; i < nx; i++)
        {
            // выводим данные столбцов i-й строки
            for (unsigned j{}; j < ny; j++)
            {
                std::cout << data[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }
};