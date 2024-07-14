// g++ Heat2D.cpp -o Heat2D
// ./Heat2D
#include <iostream>

void printArray2D(size_t Nx_nodes, size_t Ny_nodes,
double* T,
size_t indStart_x, size_t length_x,
size_t indStart_y, size_t length_y)
{
    std::cout << "printArray2D: "   << indStart_x << " " << length_x << "; "
                                    << indStart_y << " " << length_y << std::endl;
    for(auto j = indStart_y; j < indStart_y + length_y; j++)
    {
        std::cout << "j = " << j << ":\t";
        for(auto i = indStart_x; i < indStart_x + length_x; i++)
        {
            std::cout << T[i + j * Nx_nodes] << "\t";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "--- Heat2D ---" << std::endl;

    // Исходные данные
    // Количество отрезков по Ox и Oy
    //   1   2   3
    // .---.---.---. <- Nx = 3; Nx_nodes = 4
    // 0   1   2   3
    size_t Nx = 10;
    size_t Ny = 10;
    size_t Nx_nodes = Nx+1;
    size_t Ny_nodes = Ny+1;
    // Количество узлов расчетной сетки
    size_t N_nodes_grid2D = Nx_nodes * Ny_nodes;

    // Количество отрезков по времени
    size_t Nt = 100;
    // Количество узлов сетки по времени
    size_t Nt_nodes = Nt + 1;

    // Поле температур 2D как одномерный массив
    double* T = new double[N_nodes_grid2D];

    // Время моделирования
    double t_end = 60.0;

    // Длина и ширина пластины
    double L = 0.5;
    double H = 0.5;
    // Коэффициент теплопроводности
    double lambda = 384;
    // Плотность материала
    double rho = 8800;
    // Теплоёмкость материала
    double c = 381;
    // Температуры на границах
    double Th = 80;
    double Tc = 30;
    // Начальная температура
    double T0 = 5;

    // 1. Определяем расчетные шаги сетки по пространственным координатам
    double hx = L/Nx;
    double hy = L/Ny;
    std::cout << "hx = " << hx << " m." << std::endl;
    std::cout << "hy = " << hy << " m." << std::endl;

    // 2. Определяем коэффициент температуропроводности
    double a = lambda / (rho * c);
    std::cout << "a = lambda / (rho * c) = " << a << std::endl;

    // 3. Определяем расчетный шаг сетки по времени
    double tau = t_end / Nt;    
    std::cout << "tau = " << tau << " sec." << std::endl;

    // 4. Инициализируем поле температур
    for(auto j{0ull}; j < Ny_nodes; j++)
    {
        for(auto i{0ull}; i < Nx_nodes; i++)
        {
            T[i + j*Nx_nodes] = T0;
            //T[i + j*Nx_nodes] = i + j*Nx_nodes;
        }
    }

    // 5. Задаём граничные условия первого рода слева и справа
    for(auto j{0ull}; j < Ny_nodes; j++)
    {
        T[0 + j * Ny_nodes] = Th;
        T[Nx + j * Ny_nodes] = Tc;
    }

    printArray2D(Nx_nodes, Ny_nodes, T, 0, Nx_nodes, 0, Ny_nodes);
    std::cout << "\n\n";
    //printArray2D(Nx_nodes, Ny_nodes, T, 0, 5, 0, 5);

    // Проводим интегрирование нестационарного уравнения теплопроводности
    double time = 0;

    while(time < t_end)
    {
        // Увеличиваем переменную времени на шаг tau
        time += tau;
        std::cout << "----- time = " << time << "-----" << std::endl;
        
        // Решаем СЛАУ в направлении оси Ox для определения поля
        // температуры на промежуточном временном слое
        // Выделяем память для хранения прогоночных коэффициентов
        double* alfa = new double[Nx_nodes];
        double* beta = new double[Nx_nodes];

        for(auto j{0ull}; j <= Ny; j++)
        {
            // Начало прогонки по Ox
            // Определяем начальные прогоночные коэффициенты
            // на основе левого граничного условия
            alfa[0] = 0;
            //beta[0] = Th;
            beta[0] = T[0+j*Nx_nodes];
                        
            for(size_t i = 1; i < Nx; i++)
            {
                double ai = lambda / (hx*hx);
                double bi = - 2.0 * lambda / (hx*hx) - rho * c / tau;
                double ci = lambda / (hx*hx);
                double fi = - rho * c * T[i+j*Nx_nodes] / tau;

                double delta = -bi - ai * alfa[i-1];
                alfa[i] = ci/delta;
                beta[i] = (ai*beta[i-1] - fi) / delta;
            }

            // Обратный ход
            for(long long i = Nx-1; i > -1; i--)
            {
                T[i+j*Nx_nodes] = alfa[i] * T[i+1+j*Nx_nodes] + beta[i];                
            }

            // Конец прогонки по Ox
            printArray2D(Nx_nodes, Ny_nodes, T, 0, Nx_nodes, 0, Ny_nodes);
            std::cout << "\n\n";
        }
        delete[] alfa;
        delete[] beta;
        ///////////////////////////


    }

    
}