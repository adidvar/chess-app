#ifndef FIGURES_H
#define FIGURES_H
#include <inttypes.h>

///  Тип данних для зберігання фігур
using Figures = uint8_t;
/// \brief Типи фігур
enum Figures_enum
{
    empty = 0, ///<Пуста клітинка
    pawn = 1, ///< Пішка
    knight = 2, ///< Кінь
    bishop = 3,///< Офіцер
    rook = 4, ///< Тура
    queen = 5, ///< Королева
    king = 6 ///< Король
};

/// \brief Тип данних для зберігання кольору фігури
using Color = uint8_t;
/// \brief Список кольорів
///
/// Описує кольори фігур
enum Color_enum
{
    white = 0,
    black = 1
};

#endif // FIGURES_H
