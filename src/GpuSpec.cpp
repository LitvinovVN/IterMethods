#ifndef _GpuSpec
#define _GpuSpec

#include <iostream>

/// @brief Спецификация видеокарты
class GpuSpec
{
    // Идентификатор видеоадаптера
    unsigned gpuId = 0;

    // Название
    std::string name;

    // Объём видеопамяти, Гб
    double vramSize;

    // Количество потоковых мультипроцессоров
    unsigned smNum;

public:
    GpuSpec(unsigned gpuId, std::string name, double vramSize, unsigned smNum)
    {
        //std::cout << "GpuSpec::GpuSpec(): " << name << std::endl;
        this->gpuId = gpuId;
        this->name = name;
        this->vramSize = vramSize;
        this->smNum = smNum;
    }
    ~GpuSpec()
    {
        //std::cout << "GpuSpec::~GpuSpec(): " << name << std::endl;
    }

    void Print()
    {
        std::cout << gpuId << ": " << name << "; " << vramSize << " Gb; " << smNum << " SMs" << std::endl;
    }
};

#endif