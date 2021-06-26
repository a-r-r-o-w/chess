#ifndef chess_move_generation_h
#define chess_move_generation_h

#include "types.h"
#include "chess.h"
#include "board.h"
#include "piece.h"
#include "vector.h"

vector(position)

void __generate_sliding_moves (struct chess*, struct piece*, vector_position*);
bool __is_valid_square (position);

// Should be called only for piece->type == *_queen, *_rook, *_bishop
void __generate_sliding_moves (struct chess* chess, struct piece* piece, vector_position* valid_moves) {
    int direction[8] = {
        -1, // left
        +1, // right
        -8, // up
        +8, // down
        -9, // up-left
        -7, // up-right
        +7, // down-left
        +9  // down-right
    };

    if (piece->position == -1)
        return;
    
    int begin = 0, end = 8;
    
    if (piece->type == white_rook || piece->type == black_rook)
        end = 4;
    
    if (piece->type == white_bishop || piece->type == black_bishop)
        begin = 4;

    for (int i = begin; i < end; ++i) {
        for (int times = 1; times <= 8; ++times) {
            position new_position = piece->position + times * direction[i];
            char file = new_position % 8 + 'a';
            char rank = 8 - new_position / 8 - 1 + '1';

            if (__is_valid_square(new_position)) {
                if (chess->board.is_occupied[new_position] != 0) {
                    if ((__is_white_occupied(chess->board.is_occupied[new_position]) && __is_black_piece(piece)) ||
                        (__is_black_occupied(chess->board.is_occupied[new_position]) && __is_white_piece(piece)))
                        vector_position_pushback(valid_moves, new_position);
                    break;
                }
                else
                    vector_position_pushback(valid_moves, new_position);
            }

            if ((direction[i]  > +1 && (rank == '1' || file == 'a' || file == 'h')) ||
                (direction[i]  < -1 && (rank == '8' || file == 'a' || file == 'h')) ||
                (direction[i] == +1 && file == 'h') ||
                (direction[i] == -1 && file == 'a'))
                break;
        }
    }

    // TODO: filter(valid_moves); - to check if making a move puts king into check
}

bool __is_valid_square (position position) {
    return 0 <= position && position < 64;
}

#endif // chess_move_generation_h
