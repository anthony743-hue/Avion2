#pragma once
#include <cmath>

struct Vecteur
{
    float x = 0.f, y = 0.f;

    Vecteur() = default;
    Vecteur(float x, float y) : x(x), y(y) {}

    Vecteur operator*(float s) const
    {
        return {x * s, y * s};
    }

    Vecteur operator+(const Vecteur &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vecteur operator-(const Vecteur &other) const
    {
        return {x - other.x, y - other.y};
    }

    Vecteur &operator+=(const Vecteur &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    float norme() const
    {
        return std::sqrt(x * x + y * y);
    }
};
