#ifndef _Geometry2d
#define _Geometry2d

#include <iostream>
#include <fstream>

/// @brief Класс для описания геометрии расчетной области в двумерном формате
class Geometry2d
{
    /// @brief Количество подобластей расчетной области по Ox
    unsigned nx = 0;
    // Количество подобластей расчетной области по Oy
    unsigned ny = 0;

    /// @brief Длина подобласти расчетной области по Ox
    double Lx = 0.0;
    // Длина подобласти расчетной области по Oy
    double Ly = 0.0;

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

    /// @brief Создаёт объект из файла
    /// @param fileName Путь к файлу с описанием геометрии
    Geometry2d(std::string fileName)
    {
        std::cout << "Geometry2d(std::string fileName): Opening txt file..." << std::endl;    
        std::ifstream in(fileName); // окрываем файл для чтения
        if (!in.is_open())
        {
            std::cout << "ERROR! File not opened!" << std::endl;
            exit(-1);
        }

        std::string fileFormat;
        in >> fileFormat;
        std::cout << "File format checking: " << fileFormat << std::endl;
        if(fileFormat != "2D")
        {
            std::cout << "ERROR! File format is not 2D!" << std::endl;
            exit(-1);
        }

        in >> nx >> ny >> Lx >> Ly;
        std::cout << "nx = " << nx << "; ";
        std::cout << "ny = " << ny << "; ";
        std::cout << "Lx = " << Lx << "; ";
        std::cout << "Ly = " << Ly << std::endl;

        // Выделяем память под хранение данных
        data = new unsigned*[ny]{};
        for (unsigned j{}; j < ny; j++)
        {
            data[j] = new unsigned[nx]{};
        }

        // Считываем данные из файла
        for(unsigned j{}; j < ny; j++)
        {
            std::string line;
            getline(in, line);
            std::cout << "line = " << line << "; ";
            std::cout << "line size = " << line.size() << std::endl;
            if(line.size() < nx)
            {
                j--;
                continue;
            }

            for(unsigned i{}; i < nx; i++)
            {                
                char sym[1];
                sym[0] = line[i];
                //std::cout << "sym = " << (int)sym[0] << "; ";
                unsigned num = atoi(sym);
                //std::cout << "num = " << num << std::endl;
                data[j][i] = num;
            }
        }

        in.close();     // закрываем файл
    }

    ~Geometry2d()
    {
        // std::cout << "~Geometry2d()" << std::endl;
        for (unsigned j{}; j < ny; j++)
        {
            delete[] data[j];
        }
        delete[] data;
    }

    void Print()
    {
        if(nx == 0 || ny == 0)
        {
            std::cout << "Geometry2d::Print(): Object not initialized!" << std::endl;
            return;
        }

        for (unsigned j{}; j < ny; j++)
        {
            // выводим данные столбцов j-й строки
            for (unsigned i{}; i < nx; i++)
            {
                std::cout << data[j][i] << "\t";
            }
            std::cout << std::endl;
        }
    }
};

#endif