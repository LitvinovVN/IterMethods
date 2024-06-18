#ifndef _Geometry2dClusterDec
#define _Geometry2dClusterDec

#include <iostream>
#include <fstream>
#include <vector>
#include "string_helpers.cpp"

/// @brief Класс для описания декомпозиции  двумерной расчетной области с закреплением за вычислительными узлами и вычислительными устройствами
class Geometry2dClusterDec
{
    /// @brief Количество подобластей расчетной области по Ox
    unsigned nx = 0;
    // Количество подобластей расчетной области по Oy
    unsigned ny = 0;

    /// @brief Длина подобласти расчетной области по Ox
    double Lx = 0.0;
    // Длина подобласти расчетной области по Oy
    double Ly = 0.0;

    // Двумерный массив с индексами узлов, обрабатывающих фрагменты расчетной области
    unsigned** dataNodes;
    // Вектор пар (индекс узла, двумерный массив сопоставления индекса вычислительного устройства фрагменту расчетной области)
    std::vector<std::pair<int, unsigned**>> dataDevicesVector;

public:
    
    /// @brief Создаёт объект из файла
    /// @param fileName Путь к файлу с описанием декомпозиции
    Geometry2dClusterDec(std::string dirName, std::string fileName)
    {
        std::cout << "Geometry2dClusterDec(std::string dirName, std::string fileName): Opening txt file..." << std::endl;    
        
        // Считываем конфигурационный файл, содержащий имена с файлами с распределениями
        /*            
            rectangle_cluster_dec_conf
            nodes rectangle_nodes.txt
            node 1 rectangle_node_1.txt
            node 2 rectangle_node_2.txt
        */
        std::ifstream in(dirName + fileName); // окрываем файл для чтения
        if (!in.is_open())
        {
            std::cout << "ERROR! File not opened!" << std::endl;
            exit(-1);
        }

        // Массив для хранения строк конфигурационного файла
        std::vector<std::string> lines;
        std::string line;
        while (getline(in, line))
        {
            if(line[0] == '#') continue;
            lines.push_back(line);
        }
        in.close();

        for(auto line : lines)
        {
            std::cout << line << std::endl;
        }


        
        std::cout << "File format checking: " << lines[0] << std::endl;
        if(lines[0] != "cluster_decomposition_conf")
        {
            std::cout << "ERROR! File format is not cluster_decomposition_conf!" << std::endl;
            exit(-1);
        }
        

        for(int i{1}; i<lines.size();i++)
        {
            std::vector<std::string> vecOfSubStr 
                = splitString(lines[i], ' ');
            if(vecOfSubStr.size() == 0) continue;
            
            if (vecOfSubStr[0] == "nodes")
            {
                // Считываем файл с распределением по узлам кластера
                std::ifstream in(dirName + vecOfSubStr[1]); // окрываем файл для чтения
                if (!in.is_open())
                {
                    std::cout << "ERROR! File " << (dirName + vecOfSubStr[1]) << " not opened!" << std::endl;
                    exit(-1);
                }

                std::string fileFormat;
                in >> fileFormat;
                std::cout << "File format checking: " << fileFormat << std::endl;
                if(fileFormat != "2D_nodes")
                {
                    std::cout << "ERROR! File format is not 2D_nodes!" << std::endl;
                    exit(-1);
                }

                in >> nx >> ny >> Lx >> Ly;
                std::cout << "nx = " << nx << "; ";
                std::cout << "ny = " << ny << "; ";
                std::cout << "Lx = " << Lx << "; ";
                std::cout << "Ly = " << Ly << std::endl;

                // Выделяем память под хранение данных
                dataNodes = new unsigned*[ny]{};
                for (unsigned j{}; j < ny; j++)
                {
                    dataNodes[j] = new unsigned[nx]{};
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
                        dataNodes[j][i] = num;
                    }
                }

                in.close();     // закрываем файл
            }
            else if (vecOfSubStr[0] == "node")
            {
                int nodeId = atoi(vecOfSubStr[1].c_str());
                // Считываем файл с распределением по устройствам узла кластера
                std::ifstream in(dirName + vecOfSubStr[2]); // окрываем файл для чтения
                if (!in.is_open())
                {
                    std::cout << "ERROR! File " << (dirName + vecOfSubStr[2]) << " not opened!" << std::endl;
                    exit(-1);
                }

                std::string fileFormat;
                in >> fileFormat;
                std::cout << "File format checking: " << fileFormat << std::endl;
                if(fileFormat != "2D_node_devices")
                {
                    std::cout << "ERROR! File format is not 2D_node_devices!" << std::endl;
                    exit(-1);
                }

                in >> nx >> ny >> Lx >> Ly;
                std::cout << "nx = " << nx << "; ";
                std::cout << "ny = " << ny << "; ";
                std::cout << "Lx = " << Lx << "; ";
                std::cout << "Ly = " << Ly << std::endl;

                // Выделяем память под хранение данных
                auto dataDevices = new unsigned*[ny]{};
                for (unsigned j{}; j < ny; j++)
                {
                    dataDevices[j] = new unsigned[nx]{};
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
                        dataDevices[j][i] = num;
                    }
                }

                in.close();     // закрываем файл

                dataDevicesVector.push_back(std::pair<int, unsigned**>(nodeId, dataDevices));
            }
            
        }
        
    }

    ~Geometry2dClusterDec()
    {
        // std::cout << "~Geometry2dClusterDec()" << std::endl;
        for (unsigned j{}; j < ny; j++)
        {
            delete[] dataNodes[j];
        }
        delete[] dataNodes;
    }

    void Print()
    {
        if(nx == 0 || ny == 0)
        {
            std::cout << "Geometry2dClusterDec::Print(): Object not initialized!" << std::endl;
            return;
        }

        std::cout << "Geometry - Nodes:" << std::endl;
        for (unsigned j{}; j < ny; j++)
        {
            // выводим данные столбцов j-й строки
            for (unsigned i{}; i < nx; i++)
            {
                std::cout << dataNodes[j][i] << "\t";
            }
            std::cout << std::endl;
        }

        std::cout << "Geometry - Devices: " << dataDevicesVector.size() << std::endl;
        for(auto& node : dataDevicesVector)
        {
            std::cout << "Node " << node.first << std::endl;
            for (unsigned j{}; j < ny; j++)
            {
                // выводим данные столбцов j-й строки
                for (unsigned i{}; i < nx; i++)
                {
                    std::cout << node.second[j][i] << "\t";
                }
                std::cout << std::endl;
            }
        }
    }
};

#endif