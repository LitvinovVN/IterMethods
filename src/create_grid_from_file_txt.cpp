// g++ create_grid_from_file_txt.cpp -o create_grid_from_file_txt
// ./create_grid_from_file_txt

/* Программа считывает данные из текстового файла, имеющего следующий формат:
 * 2D       Размерность пространства
 * 10 6     Количество строк (6) и столбцов (10) в текстовом файле
 *          (подобластей расчетной области по Ox и Oy) 
 * 50 70    Размер одной подобласти (символа) по Ox и Oy
 * 0000000010 0 - не принадлежит расчетной области, 1 - принадлежит РО
 * 0000011110
 * 0000011110
 * 0111111111
 * 0111111111
 * 0000000000
*/

#include <iostream>
#include <vector>
#include "Geometry2d.cpp"
#include "Cluster.cpp"
#include "Geometry2dClusterDec.cpp"

using namespace std;

int main()
{    
    //Geometry2d g2d(3,5,10,20);
    //g2d.Print();
    std::string data_in_path = "data_in/";

    Geometry2d g2df(data_in_path, "rectangle.txt");
    g2df.Print();

    Cluster cluster(data_in_path, "cluster.txt");
    cluster.Print();

    Geometry2dClusterDec geom2dClusterDec(data_in_path, "rectangle_cluster_dec_conf.txt");
    geom2dClusterDec.Print();
    // Массив для хранения строк, описывающих расчетную область
    /*vector<string> lines;
    string line;
    while (getline(in, line))
    {
        lines.push_back(line);
    }*/
    
    

    /*for(auto line : lines)
    {
        cout << line << endl;
    }*/

    return 0;
}