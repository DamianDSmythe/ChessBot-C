
#ifndef CHESSBOT_BOARD_H
#define CHESSBOT_BOARD_H

#define START_POSITION(board) malloc(sizeof (board_t));\
board->white_pawns   = 0b0000000000000000000000000000000000000000000000001111111100000000;\
board->white_knights = 0b0000000000000000000000000000000000000000000000000000000001000010;\
board->white_bishops = 0b0000000000000000000000000000000000000000000000000000000000100100;\
board->white_rooks   = 0b0000000000000000000000000000000000000000000000000000000010000001;\
board->white_queen   = 0b0000000000000000000000000000000000000000000000000000000000001000;\
board->white_king    = 0b0000000000000000000000000000000000000000000000000000000000010000;\
board->black_pawns   = 0b0000000011111111000000000000000000000000000000000000000000000000;\
board->black_knights = 0b0100001000000000000000000000000000000000000000000000000000000000;\
board->black_bishops = 0b0010010000000000000000000000000000000000000000000000000000000000;\
board->black_rooks   = 0b1000000100000000000000000000000000000000000000000000000000000000;\
board->black_queen   = 0b0000100000000000000000000000000000000000000000000000000000000000;\
board->black_king    = 0b0001000000000000000000000000000000000000000000000000000000000000

#define location unsigned long
#define SQUARES 64
#define BOARD_LENGTH 8
#define MAX_MOVES 4096
#define RANK2_LIMIT 1 << 16

#define A_FILE 0b0000000100000001000000010000000100000001000000010000000100000001
#define B_FILE 0b0000001000000010000000100000001000000010000000100000001000000010
#define G_FILE 0b0100000001000000010000000100000001000000010000000100000001000000
#define H_FILE 0b1000000010000000100000001000000010000000100000001000000010000000

#define FIRST_RANK   0b0000000000000000000000000000000000000000000000000000000011111111
#define SECOND_RANK  0b0000000000000000000000000000000000000000000000001111111100000000
#define SEVENTH_RANK 0b0000000011111111000000000000000000000000000000000000000000000000
#define EIGHTH_RANK  0b1111111100000000000000000000000000000000000000000000000000000000

#define BISHOP 0
#define ROOK 1
#define QUEEN 2

int KNIGHT_MOVES[8] = {17, 15, 10, 6, 6, 10, 15, 17};
unsigned long KNIGHT_BOUNDARIES[8] = {FIRST_RANK | SECOND_RANK | A_FILE,
                                             FIRST_RANK | SECOND_RANK | H_FILE,
                                             FIRST_RANK | A_FILE | B_FILE,
                                             FIRST_RANK | G_FILE | H_FILE,
                                             EIGHTH_RANK | A_FILE | B_FILE,
                                             EIGHTH_RANK | G_FILE | H_FILE,
                                             SEVENTH_RANK | EIGHTH_RANK | A_FILE,
                                             SEVENTH_RANK | EIGHTH_RANK | H_FILE};

int BISHOP_MOVES[4] = {9, 7, 7, 9};
unsigned long BISHOP_BOUNDARIES[4] = {FIRST_RANK | A_FILE,
                                             FIRST_RANK | H_FILE,
                                             EIGHTH_RANK | A_FILE,
                                             EIGHTH_RANK | H_FILE};

int ROOK_MOVES[4] = {1, 8, 8, 1};
unsigned long ROOK_BOUNDARIES[4] = {A_FILE,
                                           FIRST_RANK,
                                           EIGHTH_RANK,
                                           H_FILE};

int ROYAL_MOVES[8] = {9, 7, 1, 8, 8, 1, 7, 9};
unsigned long ROYAL_BOUNDARIES[8] = {FIRST_RANK | A_FILE,
                                            FIRST_RANK | H_FILE,
                                            A_FILE,
                                            FIRST_RANK,
                                            EIGHTH_RANK,
                                            H_FILE,
                                            EIGHTH_RANK | A_FILE,
                                            EIGHTH_RANK | H_FILE};

typedef struct board_s {
    unsigned long white_pawns;
    unsigned long white_knights;
    unsigned long white_bishops;
    unsigned long white_rooks;
    unsigned long white_queen;
    unsigned long white_king;
    unsigned long black_pawns;
    unsigned long black_knights;
    unsigned long black_bishops;
    unsigned long black_rooks;
    unsigned long black_queen;
    unsigned long black_king;
    unsigned int state;
} board_t;

void decode_board(board_t* board);

#endif //CHESSBOT_BOARD_H
