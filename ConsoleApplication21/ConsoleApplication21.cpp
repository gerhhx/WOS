#include <iostream>
#include <random>
#include <cmath>

#define M_PI 3.14159265358979323846

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    double norm() const { return std::sqrt(x * x + y * y + z * z); }
};

double uniform(double a, double b) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(a, b);
    return dis(gen);
}

// Граничное условие: u = 1 + cos(2θ) на сфере r = 1
// После проекции на сферу cos theta = z, поэтому u = 2*z*z
double boundary_condition(double x, double y, double z) {
    // Точка уже лежит на единичной сфере
    return 2.0 * z * z;
}

// Вероятностное решение задачи Дирихле в 3D с проекцией на границу
double solve_dirichlet_monte_carlo(const Point& x, int N, double epsilon) {
    double u_sum = 0.0;

    for (int i = 0; i < N; ++i) {
        Point current = x;

        // Блуждание по сферам до выхода на границу
        while (true) {
            double r = 1.0 - current.norm();
            if (r < epsilon) {
                // Проекция текущей точки на границу сферы
                double norm = current.norm();
                if (norm > 0) {
                    current.x /= norm;
                    current.y /= norm;
                    current.z /= norm;
                }
                break;
            }

            // Генерация равномерного направления на сфере
            double theta = std::acos(2.0 * uniform(0, 1) - 1.0); // равномерно по cos theta
            double phi = uniform(0, 2.0 * M_PI);

            double dx = r * std::sin(theta) * std::cos(phi);
            double dy = r * std::sin(theta) * std::sin(phi);
            double dz = r * std::cos(theta);

            current.x += dx;
            current.y += dy;
            current.z += dz;
        }

        // Вычисляем граничное значение в точке на сфере
        double g_value = boundary_condition(current.x, current.y, current.z);
        u_sum += g_value;
    }

    return u_sum / N;
}

// Аналитическое решение для проверки: u = 2/3 + (4/3)z^2 - (2/3)(x^2+y^2)
double analytical_solution(const Point& x) {
    return 2.0 / 3.0 + (4.0 / 3.0) * x.z * x.z - (2.0 / 3.0) * (x.x * x.x + x.y * x.y);
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Тестовые точки в трёхмерном пространстве
    Point points[] = {
        Point(0.5, 0, 0),   
        Point(0, 0.5, 0),   
        Point(0, 0, 0.5),    
        Point(0.7, 0.3, 0.2),
        Point(0.9, 0, 0)     
    };

    int N = 100000;       // количество траекторий
    double epsilon = 1e-4; // точность выхода на границу

    std::cout << "Сравнение методов:" << std::endl;
    std::cout << "Точка (x,y,z)\t\tМонте-Карло\tАналитическое\tПогрешность" << std::endl;

    for (const auto& p : points) {
        double mc_result = solve_dirichlet_monte_carlo(p, N, epsilon);
        double analytical = analytical_solution(p);
        double error = std::abs(mc_result - analytical);

        std::cout << "(" << p.x << "," << p.y << "," << p.z << ")\t"
            << mc_result << "\t\t" << analytical << "\t\t" << error << std::endl;
    }

    return 0;
}
