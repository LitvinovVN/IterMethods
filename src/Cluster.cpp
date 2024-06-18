#ifndef _Cluster
#define _Cluster

#include <iostream>
#include <fstream>
#include <vector>
#include "string_helpers.cpp"
#include "ClusterNode.cpp"

class Cluster
{
    unsigned nodeNumber = 0;
    std::string name;
    std::vector<ClusterNode> clusterNodes;

public:
    Cluster(std::string dirName, std::string fileName)
    {
        std::cout << "Cluster::Cluster(): " << dirName << fileName << std::endl;

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
        if(lines[0] != "cluster")
        {
            std::cout << "ERROR! File format is not cluster!" << std::endl;
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
            if (vecOfSubStr[0] == "node")
            {
                std::cout << vecOfSubStr[0] << ": ";

                int nodeId = atoi(vecOfSubStr[1].c_str());
                std::cout << "nodeId = " << nodeId << "; ";

                std::string nodeName = vecOfSubStr[2];
                std::cout << "nodeName = " << nodeName << "; ";

                std::string fileName = vecOfSubStr[3];
                std::cout << "fileName = " << fileName << std::endl;

                ClusterNode clusterNode(nodeId, nodeName, dirName, fileName);
                //clusterNode.Print();
                clusterNodes.push_back(clusterNode);
            }

            // Print the vector of substrings 
            for (auto& it : vecOfSubStr)
            {                               
                 
            } 
        }
    }

    ~Cluster()
    {
        //std::cout << "Cluster::~Cluster()" << std::endl;
    }

    void Print()
    {
        std::cout << "Cluster::Print()" << std::endl;
        std::cout << "  | nodeNumber = " << nodeNumber << std::endl;
        std::cout << "  | name = " << name << std::endl;
        for(auto clusterNode : clusterNodes)
        {
            clusterNode.Print();
        }
    }
};

#endif