#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Выбор метода
#ifdef BISECTION
int method = 2;
#else
int method = 1;
#endif

//Ключи запуска
int key_help = 0;
int key_test = 0;
int key_test_root = 0;
int key_test_int = 0;
int key_iter = 0;
int key_points = 0;
int key_int = 0;

int sign(double x)
{
    if (x >= 0.0)
        return 1;
    return -1;
}

void swap(double *a, double *b)
{
    double c = *a;
    *a = *b;
    *b = c;
}

double point_f1_f2, point_f2_f3, point_f1_f3; //Точки пересечений
double border_a = -1.9, border_b = 1.9; //Границы поиска точек пересечений
double eps1 = 0.00001, eps2 = 0.00001; //Погрешности вычислений


double (*g1)(double), (*g2)(double); //Указатели на пары функций f1, f2, f3
double f(double x) //Функция для решения уравнения g1(x) = g2(x)
{
    return g1(x) - g2(x);
}


extern double f1(double x); //Функция 1
extern double f2(double x); //Функция 2
extern double f3(double x); //Функция 3
double d1(double x) //Тестовая функция 1
{
    return x;
}
double d2(double x) //Тестовая функция 2
{
    return 1/x;
}
double d3(double x) //Тестовая функция 3
{
    return 2*x*x*x*x;
}


double method1(double a, double b, double eps1, double(*func1)(double), double(*func2)(double)) //Метод хорд
{
    g1 = func1;
    g2 = func2;
    int iter = 0;
    while (fabs(b - a) >= eps1 / 2.0 || fabs(f(b) - f(a)) >= eps1 / 2.0) {
        iter++;
        a = b - (b - a) * f(b) / (f(b) - f(a));
        b = a - (a - b) * f(a) / (f(a) - f(b));
    }
    if (key_iter)
        printf("%d iterations\n", iter);
    if (key_points)
        printf("%lf is abscissa\n", b);
    return b;
}

double method2(double a, double b, double eps1, double(*func1)(double), double(*func2)(double)) //Метод бисекции (деления отрезка пополам)
{
    g1 = func1;
    g2 = func2;
    double c = 0.0;
    int iter = 0;
    while (fabs(b - a) >= eps1 / 2.0 || fabs(f(b) - f(a)) >= eps1 / 2.0) {
        iter++;
        c = (a + b) / 2;
        if (sign(f(a)) == sign(f(c)))
            a = c;
        else
            b = c;
    }
    if (key_iter)
        printf("%d iterations\n", iter);
    if (key_points)
        printf("%lf is abscissa\n", c);
    return c;
}

double root(double a, double b, double eps1, double(*func1)(double), double(*func2)(double)) //Буферная функция для вычисления корня
{
    if (method == 1) {
        printf("Using secant method!\n");
        return method1(a, b, eps1, func1, func2);
    }
    if (method == 2) {
        printf("Using bisection method!\n");
        return method2(a, b, eps1, func1, func2);
    }
}

double formula(double a, double b, double(*g)(double)) //Формула Симпсона для нахождения приближенного значения интеграла от a до b функции g
{
    return (b - a) / 6.0 * (g(a) + g(b) + 4.0 * g((a + b) / 2.0));
}

double integral(double a, double b, double eps2, double(*g)(double)) //Функция вычисления интеграла
{
    if (b < a)
        swap(&a, &b);
    int iter = 4;
    double ans_prev = 999999999999.0;
    double ans = 0.0;
    while (fabs(ans - ans_prev) >= eps2) {
        ans_prev = ans;
        ans = 0;
        double step = (b - a) / iter;
        for (int i = 0;  i < iter;  i++) {
            ans += formula(a + i * step, a + (i + 1) * step, g);
        }
        iter *= 2;
    }
    if (key_iter)
        printf("%d iterations for integral\n", iter);
    if (key_int)
        printf("%lf is answer for this integral\n", ans);
    return ans;
}


void print_help() //Вывод всех возможных ключей в случае запуска с ключом -help
{
    printf("\t-help\t\tshows all valid keys\n");
    printf("\t-testroot\tis to test root function\n");
    printf("\t-testint\tis to test integral function\n");
    printf("\t-iterations\tis to print the amount of iterations\n");
    printf("\t-points\t\tis to print the abscissa coordinates\n");
    printf("\t-int\t\tis to print each integral\n");
}

int main(int argc, char *argv[])
{
    for (int i = 0;  i < argc;  i++) {
        char *key = argv[i];
        if (!strcmp(key, "-help"))
            key_help = 1;
        else if (!strcmp(key, "-testroot"))
            key_test_root = 1;
        else if (!strcmp(key, "-testint"))
            key_test_int = 1;
        else if (!strcmp(key, "-iterations"))
            key_iter = 1;
        else if (!strcmp(key, "-points"))
            key_points = 1;
        else if (!strcmp(key, "-int"))
            key_int = 1;
    }
    if (key_test_int || key_test_root)
        key_test = 1;

    if (key_help) {
        print_help();
        return 0;
    }
    if (key_test) {
        key_iter = 1;
        key_points = 1;
        if (key_test_root) {
            double a1, a2, a3;
            int a4, a5;
            printf("To find the root of fx and fy on [a; b] with eps precision, input\na b eps x y:\n");
            scanf("%lf %lf %lf %d %d", &a1, &a2, &a3, &a4, &a5);
            double (*h1)(double), (*h2)(double);
            if (a4 == 1)
                h1 = f1;
            else if (a4 == 2)
                h1 = f2;
            else if (a4 == 3)
                h1 = f3;
            else if (a4 == 4)
                h1 = d1;
            else if (a4 == 5)
                h1 = d2;
            else if (a4 == 6)
                h1 = d3;
            if (a5 == 1)
                h2 = f1;
            else if (a5 == 2)
                h2 = f2;
            else if (a5 == 3)
                h2 = f3;
            else if (a5 == 4)
                h2 = d1;
            else if (a5 == 5)
                h2 = d2;
            else if (a5 == 6)
                h2 = d3;
            root(a1, a2, a3, h1, h2);
        }
        if (key_test_int) {
            key_int = 1;
            double a1, a2, a3;
            int a4;
            printf("To calculate the integral of fx on [a; b] with eps precision, input\na b eps x:\n");
            scanf("%lf %lf %lf %d", &a1, &a2, &a3, &a4);
            double (*h1)(double);
            if (a4 == 1)
                h1 = f1;
            else if (a4 == 2)
                h1 = f2;
            else if (a4 == 3)
                h1 = d3;
            else if (a4 == 4)
                h1 = d1;
            else if (a4 == 5)
                h1 = d2;
            else if (a4 == 6)
                h1 = d3;
            integral(a1, a2, a3, h1);
        }
        return 0;
    }

    printf("Calculating root for f1 and f2\n");
    point_f1_f2 = root(-1, 1, eps1, f1, f2);
    printf("\n");
    printf("Calculating root for f2 and f3\n");
    point_f2_f3 = root(-1, 1, eps1, f2, f3);
    printf("\n");
    printf("Calculating root for f1 and f3\n");
    point_f1_f3 = root(-1.9, -1.5, eps1, f1, f3);
    printf("\n");

    g1 = f1;
    g2 = f2;
    double s1 = integral(point_f1_f2, point_f2_f3, eps2, f);
    g1 = f1;
    g2 = f3;
    double s2 = integral(point_f2_f3, point_f1_f3, eps2, f);

    printf("%lf\n", fabs(s1 + s2));

    return 0;
}
