#ifndef POSITIONS_H
#define POSITIONS_H
#include <inttypes.h>
#include <string>
#include <assert.h>

/**
 * Тип данних по якому здійснюється доступ до дошки
 */
using Pos = uint8_t;

/**
 * @brief Помилкова позиція
 */
const Pos error_pos = 64;

/**
 * @brief Повертає позицію за двома координатами
 * @param x Координата x
 * @param y Координата y
 * @return позиція
 */
constexpr Pos Position(Pos x , Pos y)
{
    if( x > 7 || y > 7)
        return error_pos;

    return x*8 + y;
}

constexpr bool PositionNormal(Pos test)
{
    return test < error_pos;
}

constexpr Pos Position_x(Pos p)
{
    return p/8;
}
constexpr Pos Position_y(Pos p)
{
    return p%8;
}

inline Pos to_default(std::string pos)
{
  //*
    return error_pos;
}

inline std::string to_format(Pos pos)
{
    std::string str = "00";
    str[0] = static_cast<char>('a'+Position_y(pos));
    str[1] = static_cast<char>('8'-Position_x(pos));
    return str;
}

#endif // POSITIONS_H
