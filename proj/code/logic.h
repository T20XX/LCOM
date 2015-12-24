#ifndef __LOGIC_H
#define __LOGIC_H

#include "piece.h"
#include "game.h"

/**
 * @brief Verifies if a piece is able to move left in a board
 *
 * @param piece Actual_piece normally, to verify if it can change its position in x axys
 * @param board Board that contains borders and previous played pieces
 * @param dir Direction to check (0 to left, 1 to right)
 * @return 0 upon sucess and 1 upon failure
 */
int can_piece_move_x(Piece * piece, Board * board, unsigned int dir);
/**
 * @brief Verifies if a piece is able to move left in a board
 *
 * @param piece Actual_piece normally, to verify if it can change is position downwards
 * @param board Board that contains borders and previous played pieces
 * @return 0 upon sucess and 1 upon failure
 */

int can_piece_fall(Piece * piece, Board * board);

int can_piece_be_placed(Piece * piece, Board * board);

#endif /* __LOGIC_H */
