// g++ create_grid_from_file_txt.cpp -o create_grid_from_file_txt
// ./create_grid_from_file_txt

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    cout << "Opening txt file..." << endl;    
    std::ifstream in("rectangle.txt"); // окрываем файл для чтения
    if (!in.is_open())
    {
        cout << "ERROR! File not opened!" << endl;
        return -1;
    }

    vector<string> lines;
    string line;
    while (getline(in, line))
    {
        lines.push_back(line);
    }
    
    in.close();     // закрываем файл

    for(auto line : lines)
    {
        cout << line << endl;
    }

    return 0;
}