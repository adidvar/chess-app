#ifndef TURNGENERATOR_H
#define TURNGENERATOR_H
#include "board.h"
#include "turn.h"
#include <vector>

/**
 * @brief Генерує тільки можливі ходи
 * @param board Доска
 * @param color Стартовий колір
 * @return Сховище з можливими ходами
 */
std::vector<Turn> TurnGenerate(Board board , Color color);

#endif // TURNGENERATOR_H
