#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
//This muste be here :(
#include "board.h"

static inline void add_if_legal_advance(board_t board, location pawn, location pawns, location all_pieces, board_t* move_list, int* idx) {
    if (!(pawn & all_pieces)) {
        board.white_pawns = pawns;
        move_list[(*idx)++] = board;
    }
}

static inline void white_pawn_moves(board_t* board, location all_pieces, location enemy_pieces, board_t* move_list, int* idx) {
    location pawns = board->white_pawns;
    location remaining = pawns;
    while (remaining) {
        location next_pawns = remaining & (remaining - 1);
        location pawn = next_pawns ^ remaining;
        location advance1 = pawn << BOARD_LENGTH;
        add_if_legal_advance(*board, advance1, pawns ^ pawn ^ advance1, all_pieces, move_list, idx);
        if (pawn < RANK2_LIMIT && !(all_pieces & advance1)) {
            location advance2 = pawn << (BOARD_LENGTH << 1);
            add_if_legal_advance(*board, advance2, pawns ^ pawn ^ advance2, all_pieces, move_list, idx);
        }
        remaining = next_pawns;
    }
}

static inline void add_white_move(board_t* board, location move, board_t* move_list, int* idx) {
    board->black_pawns ^= move;
    board->black_knights ^= move;
    board->black_bishops ^= move;
    board->black_rooks ^= move;
    board->black_queen ^= move;
    move_list[(*idx)++] = *board;
}

static inline void add_white_knight_move(board_t board, location move, location knights, board_t* move_list, int* idx) {
    board.white_knights = knights | move;
    board.black_pawns ^= move;
    board.black_knights ^= move;
    board.black_bishops ^= move;
    board.black_rooks ^= move;
    board.black_queen ^= move;
    move_list[(*idx)++] = board;
}

static inline void white_knight_moves(board_t* board, location white_pieces, board_t* move_list, int* idx) {
    location knights = board->white_knights;
    location remaining = knights;
    while (remaining) {
        location next_knights = remaining & (remaining - 1);
        location knight = next_knights ^ remaining;
        for (int i = 0; i < 8; i++) {
            if (!(KNIGHT_BOUNDARIES[i] & knight)) {
                location move = i < 4 ? knight >> KNIGHT_MOVES[i] : knight << KNIGHT_MOVES[i];
                if (!(move & white_pieces)) {
                    add_white_knight_move(*board, move, knights ^ knight, move_list, idx);
                }
            }
        }
        remaining = next_knights;
    }
}

static inline void add_white_bishop_move(board_t board, location move, location pieces, board_t* move_list, int* idx) {
    board.white_bishops = move | pieces;
    add_white_move(&board, move, move_list, idx);
}

static inline void add_white_rook_move(board_t board, location move, location pieces, board_t* move_list, int* idx) {
    board.white_rooks = move | pieces;
    add_white_move(&board, move, move_list, idx);
}

static inline void add_white_queen_move(board_t board, location move, location pieces, board_t* move_list, int* idx) {
    board.white_queen = move | pieces;
    add_white_move(&board, move, move_list, idx);
}

static inline void add_white_king_move(board_t board, location move, board_t* move_list, int* idx) {
    board.white_king = move;
    add_white_move(&board, move, move_list, idx);
}

static inline void line_moves(board_t* board, location pieces, location player_pieces, location opponent_pieces,
                int directions, const int move_deltas[], const location boundaries[], board_t* move_list, int* idx,
                void add_move(board_t, location, location, board_t*, int*)) {
    location remaining_pieces = pieces;
    while (remaining_pieces) {
        location next_pieces = remaining_pieces & (remaining_pieces - 1);
        location curr_piece = next_pieces ^ remaining_pieces;
        for (int i = 0; i < directions; i++) {
            int delta = move_deltas[i];
            location move = curr_piece;
            location other_pieces = move ^ pieces;
            while (!(move & boundaries[i]) && !(move & opponent_pieces)) {
                move = i < (directions >> 1) ? move >> delta : move << delta;
                if (move & player_pieces) break;
                add_move(*board, move, other_pieces, move_list, idx);
            }
        }
        remaining_pieces = next_pieces;
    }
}

static inline void white_bishop_moves(board_t* board, location white_pieces, location black_pieces, board_t* move_list, int* idx) {
    line_moves(board, board->white_bishops, white_pieces, black_pieces,
               4, BISHOP_MOVES, BISHOP_BOUNDARIES, move_list, idx, &add_white_bishop_move);
}

static inline void white_rook_moves(board_t* board, location white_pieces, location black_pieces, board_t* move_list, int* idx) {
    line_moves(board, board->white_rooks, white_pieces, black_pieces,
               4, ROOK_MOVES, ROOK_BOUNDARIES, move_list, idx, &add_white_rook_move);
}

static inline void white_queen_moves(board_t* board, location white_pieces, location black_pieces, board_t* move_list, int* idx) {
    line_moves(board, board->white_queen, white_pieces, black_pieces,
               8, ROYAL_MOVES, ROYAL_BOUNDARIES, move_list, idx, &add_white_queen_move);
}

static inline void white_king_moves(board_t* board, location white_pieces, location black_pieces, board_t* move_list, int* idx) {
    location king = board->white_king;
    for (int i = 0; i < 8; i++) {
        if (!(ROYAL_BOUNDARIES[i] & king)) {
            location move = i < 4 ? king >> ROYAL_MOVES[i] : king << ROYAL_MOVES[i];
            if (!(move & white_pieces)) {
                add_white_king_move(*board, move, move_list, idx);
            }
        }
    }

}

static void legal_white_moves(board_t* board) {
    board_t moves_list[MAX_MOVES];
    int idx = 0;
    int* idx_ptr = &idx;
    location white_pieces = board->white_pawns ^ board->white_knights ^ board->white_bishops
            ^ board->white_rooks ^ board->white_queen ^ board->white_king;
    location black_pieces = board->black_pawns ^ board->black_knights ^ board->black_bishops
            ^ board->black_rooks ^ board->black_queen ^ board->black_king;
    white_pawn_moves(board, white_pieces ^ black_pieces, black_pieces, moves_list, idx_ptr);
    white_knight_moves(board, white_pieces, moves_list, idx_ptr);
    white_bishop_moves(board, white_pieces, black_pieces, moves_list, idx_ptr);
    white_rook_moves(board, white_pieces, black_pieces, moves_list, idx_ptr);
    white_queen_moves(board, white_pieces, black_pieces, moves_list, idx_ptr);
    white_king_moves(board, white_pieces, black_pieces, moves_list, idx_ptr);
//    for (int i = 0; i < idx; i++) {
//        puts("");
//        decode_board(&moves_list[i]);
//    }
//    printf("%d", idx);
}

board_t* fen_to_bitboard(const char* fen) {
    board_t* board = malloc(sizeof (board_t));
    board->white_pawns   = 0;
    board->white_knights = 0;
    board->white_bishops = 0;
    board->white_rooks   = 0;
    board->white_queen   = 0;
    board->white_king    = 0;
    board->black_pawns   = 0;
    board->black_knights = 0;
    board->black_bishops = 0;
    board->black_rooks   = 0;
    board->black_queen   = 0;
    board->black_king    = 0;
    int square = 56;
    int i = 0;
    unsigned long length = strlen(fen);
    while (i < length) {
        location curr = 1lu << square;
        switch (fen[i]) {
            case 'p': board->black_pawns |= curr;
                break;
            case 'n': board->black_knights |= curr;
                break;
            case 'b': board->black_bishops |= curr;
                break;
            case 'r': board->black_rooks |= curr;
                break;
            case 'q': board->black_queen |= curr;
                break;
            case 'k': board->black_king |= curr;
                break;
            case 'P': board->white_pawns |= curr;
                break;
            case 'N': board->white_knights |= curr;
                break;
            case 'B': board->white_bishops |= curr;
                break;
            case 'R': board->white_rooks |= curr;
                break;
            case 'Q': board->white_queen |= curr;
                break;
            case 'K': board->white_king |= curr;
                break;
            case '/': square -= 17;
                break;
            default: square += fen[i] - 49;
        }
        square++;
        i++;
    }
    return board;
}

void decode_board(board_t* board) {
    for (int i = 7; i > -1; i--) {
        for (int j = 0; j < 8; j++) {
            location curr = 1lu << ((i << 3) + j);
            if (board->white_pawns & curr) printf("♟ ");
            else if (board->white_knights & curr) printf("♞ ");
            else if (board->white_bishops & curr) printf("♝ ");
            else if (board->white_rooks & curr) printf("♜ ");
            else if (board->white_queen & curr) printf("♛ ");
            else if (board->white_king & curr) printf("♚ ");
            else if (board->black_pawns & curr) printf("♙ ");
            else if (board->black_knights & curr) printf("♘ ");
            else if (board->black_bishops & curr) printf("♗ ");
            else if (board->black_rooks & curr) printf("♖ ");
            else if (board->black_queen & curr) printf("♕ ");
            else if (board->black_king & curr) printf("♔ ");
            else printf("- ");
        }
        puts("");
    }
}

int main() {
    board_t* start = START_POSITION(start);
    board_t* board = fen_to_bitboard("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R");
    board_t* board1 = fen_to_bitboard("rnbq1bnr/pp1ppppp/8/2p1N3/4P2k/3P4/PPP2PPP/RNBQKB1R");
    board_t* board2 = fen_to_bitboard("8/8/8/2R2Q2/7k/8/5N2/4K3");
    clock_t begin = clock();
    for (int i = 0; i < 1000000000; i++) {
        legal_white_moves(board);
        legal_white_moves(start);
        legal_white_moves(board1);
        legal_white_moves(board2);
    }
    clock_t end = clock();
    printf("%f", (float) (end - begin) / CLOCKS_PER_SEC);
    return 0;
}
