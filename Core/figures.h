#ifndef FIGURES_H
#define FIGURES_H
#include <inttypes.h>

///  Тип данних для зберігання фігур
/// \brief Типи фігур
enum class Figure:uint8_t
{
    kEmpty = 0, ///<Пуста клітинка
    kPawn = 1, ///< Пішка
    kKnight = 2, ///< Кінь
    kBishop = 3,///< Офіцер
    kRook = 4, ///< Тура
    kQueen = 5, ///< Королева
    kKing = 6 ///< Король
};

/// \brief Тип данних для зберігання кольору фігури
/// Описує кольори фігур
enum class Color:uint8_t
{
    kWhite = 0,
    kBlack = 1
};

#endif // FIGURES_H
