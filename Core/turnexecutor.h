#ifndef TURNEXECUTOR_H
#define TURNEXECUTOR_H
#include "turn.h"
#include "board.h"
/**
 * @brief Виконує хід на дошці
 * @param board дошка
 * @param turn хід
 * @return чи виконаний був хід
 */
bool ExecuteTurn(Board &board , Turn turn);
/**
 * @brief Перевіряє хід на правильність
 * @param board дошка
 * @param turn хід
 * @return flag правильності
 */
bool TurnTest(const Board& board, const Turn turn);

#endif // TURNEXECUTOR_H
