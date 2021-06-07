#ifndef chess_board_h
#define chess_board_h

#include "exceptions.h"
#include "pieces.h"

typedef struct {
    piece white_king   [1];
    piece white_queen  [1];
    piece white_bishop [2];
    piece white_knight [2];
    piece white_rook   [2];
    piece white_pawn   [8];

    piece black_king   [1];
    piece black_queen  [1];
    piece black_bishop [2];
    piece black_knight [2];
    piece black_rook   [2];
    piece black_pawn   [8];
    
} board;

void process_piece_placement (board* board, const char* FEN) {
    char rank = '8';
    char file = 'a';

    int index[2][6];
    /* index[x][y]: current index of piece y of color x
        x = 0 -> white
        x = 1 -> black

        y = 0 -> king
        y = 1 -> queen
        y = 2 -> rook
        y = 3 -> bishop
        y = 4 -> knight
        y = 5 -> pawn
    */

    for (int i = 0; i < 2; ++i) for (int j = 0; j < 6; ++j) index[i][j] = 0;

    for (int i = 0; FEN[i] != '\0'; ++i) {
        char c = FEN[i];
        
        if (c == '/')
            continue;
        else if ('A' <= c && c <= 'Z') {
            switch(c) {
                case 'K': set_piece(&board->white_king   [index[0][0]], (char[2]){file, rank}, c); ++index[0][0]; break;
                case 'Q': set_piece(&board->white_queen  [index[0][1]], (char[2]){file, rank}, c); ++index[0][1]; break;
                case 'R': set_piece(&board->white_rook   [index[0][2]], (char[2]){file, rank}, c); ++index[0][2]; break;
                case 'B': set_piece(&board->white_bishop [index[0][3]], (char[2]){file, rank}, c); ++index[0][3]; break;
                case 'N': set_piece(&board->white_knight [index[0][4]], (char[2]){file, rank}, c); ++index[0][4]; break;
                case 'P': set_piece(&board->white_pawn   [index[0][5]], (char[2]){file, rank}, c); ++index[0][5]; break;

                default: exception("invalid FEN string", __FILE__, __LINE__);
            }
            ++file;
        }
        else if ('a' <= c && c <= 'z') {
            switch(c) {
                case 'k': set_piece(&board->black_king   [index[1][0]], (char[2]){file, rank}, c); ++index[1][0]; break;
                case 'q': set_piece(&board->black_queen  [index[1][1]], (char[2]){file, rank}, c); ++index[1][1]; break;
                case 'r': set_piece(&board->black_rook   [index[1][2]], (char[2]){file, rank}, c); ++index[1][2]; break;
                case 'b': set_piece(&board->black_bishop [index[1][3]], (char[2]){file, rank}, c); ++index[1][3]; break;
                case 'n': set_piece(&board->black_knight [index[1][4]], (char[2]){file, rank}, c); ++index[1][4]; break;
                case 'p': set_piece(&board->black_pawn   [index[1][5]], (char[2]){file, rank}, c); ++index[1][5]; break;

                default: exception("invalid FEN string", __FILE__, __LINE__);
            }
            ++file;
        }
        else if ('1' <= c && c <= '8') {
            file += c - '0';
            if (file > 'h' + 1)
                exception("invalid FEN string", __FILE__, __LINE__);
        }
        else
            exception("invalid FEN string", __FILE__, __LINE__);
        
        if (file == 'i') {
            file = 'a';
            --rank;
        }
    }

    if (file != 'a' && rank != '0')
        exception("invalid FEN string", __FILE__, __LINE__);
}

void process_FEN (board* board, const char* FEN) {
    char piece_placement[100];
    // char active_color;
    // char castling[4];
    // char en_passant[2];
    // char halfmoves[4];
    // char fullmoves[4];

    int index = 0, offset = 0;
    while (FEN[index] != '\0') {
        if (FEN[index] == ' ')
            break;
        piece_placement[index - offset] = FEN[index];
        ++index;
    }
    piece_placement[index - offset] = '\0';

    process_piece_placement(board, piece_placement);
}

void board_constructor (board* board) {
    const char* startposition_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    process_FEN(board, startposition_FEN);
}

void __display_board_set (char pretty_board[8][8], piece* piece) {
    char* coordinate = get_position(piece);
    if (('a' <= coordinate[0]) && (coordinate[0] <= 'h') && ('1' <= coordinate[1]) && (coordinate[1] <= '8')) {
        int file = coordinate[0] - 'a';
        int rank = coordinate[1] - '1';
        pretty_board[rank][file] = piece->symbol;
    }
    else if (coordinate[0] == 'a' && coordinate[1] == '9') {
        // piece is no longer present on the board
    }
    else
        exception("invalid coordinate", __FILE__, __LINE__);
}

void display_board (board* board) {
    char pretty_board[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            pretty_board[i][j] = '-';
    
    for (int i = 0; i < 1; ++i) {
        __display_board_set(pretty_board, &board->white_king[i]);
        __display_board_set(pretty_board, &board->black_king[i]);
    }

    for (int i = 0; i < 1; ++i) {
        __display_board_set(pretty_board, &board->white_queen[i]);
        __display_board_set(pretty_board, &board->black_queen[i]);
    }

    for (int i = 0; i < 2; ++i) {
        __display_board_set(pretty_board, &board->white_rook[i]);
        __display_board_set(pretty_board, &board->black_rook[i]);
    }

    for (int i = 0; i < 2; ++i) {
        __display_board_set(pretty_board, &board->white_bishop[i]);
        __display_board_set(pretty_board, &board->black_bishop[i]);
    }

    for (int i = 0; i < 2; ++i) {
        __display_board_set(pretty_board, &board->white_knight[i]);
        __display_board_set(pretty_board, &board->black_knight[i]);
    }

    for (int i = 0; i < 8; ++i) {
        __display_board_set(pretty_board, &board->white_pawn[i]);
        __display_board_set(pretty_board, &board->black_pawn[i]);
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            printf("%c ", pretty_board[i][j]);
        printf("\n");
    }
}

#endif
