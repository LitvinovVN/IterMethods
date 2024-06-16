#ifndef _ClusterNode
#define _ClusterNode

#include <iostream>
#include <fstream>
#include <vector>
#include "string_helpers.cpp"
#include "GpuSpec.cpp"

class ClusterNode
{
    std::string nodeName;
    unsigned nodeId = 0;
    double ramSize;
    int deviceIdCpu;
    unsigned cpuThreads;

    std::vector<GpuSpec> gpus;

public:
    ClusterNode(std::string nodeName, std::string fileName)
    {
        this->nodeName = nodeName;

        std::cout << "ClusterNode::ClusterNode(): " << fileName << std::endl;

        std::ifstream in(fileName); // окрываем файл для чтения
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

        /*for(auto line : lines)
        {
            std::cout << line << std::endl;
        }*/
        
        std::cout << "File format checking: " << lines[0] << std::endl;
        if(lines[0] != "cluster_node")
        {
            std::cout << "ERROR! File format is not cluster_node!" << std::endl;
            exit(-1);
        }

        for(int i{1}; i<lines.size();i++)
        {
            std::vector<std::string> vecOfSubStr 
                = splitString(lines[i], ' ');
            if(vecOfSubStr.size() == 0) continue;

            /*
            Формат описания вычислительных узлов
            Тип(node) id(int) name(string) file-name
            node 0 n0cpu4gpu12-8 cluster_node_1.txt
            */
            //std::cout << vecOfSubStr[0] << ": ";
            if (vecOfSubStr[0] == "RAM")
            {                
                ramSize = atoi(vecOfSubStr[1].c_str());
                //std::cout << "ramSize = " << ramSize << "; ";
                //std::cout << std::endl;
            }
            else if (vecOfSubStr[0] == "CPU")
            {                
                deviceIdCpu = atoi(vecOfSubStr[1].c_str());
                //std::cout << "deviceIdCpu = " << deviceIdCpu << "; ";
                //std::cout << std::endl;

                cpuThreads = atoi(vecOfSubStr[2].c_str());
                //std::cout << "cpuThreads = " << cpuThreads << "; ";
                //std::cout << std::endl;
            }
            else if (vecOfSubStr[0] == "GPU")
            {                
                int deviceIdGpu = atoi(vecOfSubStr[1].c_str());
                //std::cout << "deviceIdGpu = " << deviceIdGpu << "; ";
                //std::cout << std::endl;

                std::string gpuName = vecOfSubStr[2];
                //std::cout << "gpuName = " << gpuName << "; ";
                //std::cout << std::endl;

                int vramSize = atoi(vecOfSubStr[3].c_str());
                //std::cout << "vramSize = " << vramSize << "; ";
                //std::cout << std::endl;

                int smNum = atoi(vecOfSubStr[4].c_str());
                //std::cout << "smNum = " << smNum << "; ";
                //std::cout << std::endl;

                GpuSpec gpu(deviceIdGpu, gpuName, vramSize, smNum);
                gpus.push_back(gpu);
            }

            
        }
    }

    ~ClusterNode()
    {
        //std::cout << "ClusterNode::~ClusterNode()" << std::endl;
    }

    void Print()
    {
        std::cout << "ClusterNode::Print()" << std::endl;
        std::cout << "  | nodeName = " << nodeName << std::endl;
        std::cout << "  | nodeId = " << nodeId << std::endl;
        std::cout << "  | ramSize = " << ramSize << " Gb" << std::endl;
        std::cout << "  | deviceIdCpu = " << deviceIdCpu << std::endl;
        std::cout << "  | cpuThreads = " << cpuThreads << std::endl;
        std::cout << "  | gpus: " << gpus.size() << std::endl;
        for(auto gpu : gpus)
        {
            std::cout << "    | ";
            gpu.Print();
        }
    }
};

#endif