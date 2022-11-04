#ifndef DOUBLE_H
#define DOUBLE_H
#include <QTypeInfo>
#include <QtMath>

constexpr double eps = 1e-9;
constexpr double shift = 1e9;

inline constexpr bool eq(double a, double b)
{
    return qAbs(a - b) < eps;
}
inline constexpr bool ne(double a, double b)
{
    return !eq(a, b);
}
inline constexpr bool lt(double a, double b)
{
    return a - b <= -eps;
}
inline constexpr bool le(double a, double b)
{
    return a - b < eps;
}
inline constexpr bool gt(double a, double b)
{
    return a - b >= eps;
}
inline constexpr bool ge(double a, double b)
{
    return a - b > -eps;
}
inline constexpr double hash(double a, double b)
{
    return a * shift + b;
}

#endif // DOUBLE_H
