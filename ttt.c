#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EMPTY_CELL '_'
#define HUMAN_PLAYER 'X'
#define CPU_PLAYER 'O'
#define BOARD_SIZE 3

// Game state structure (will be useful for networking)
typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    char current_player;
    int game_active;
} GameState;

// Move structure (will be useful for networking)
typedef struct {
    int row;
    int col;
    char player;
} Move;

// Function prototypes
void init_game_state(GameState* state);
int process_move(GameState* state, Move move);
int check_winner(const GameState* state);
int is_board_full(const GameState* state);
Move get_cpu_move(const GameState* state);
Move get_player_move(const GameState* state);
void log_final_state(const GameState* state, const char* result);
void display_board(const GameState* state);
void log_move(const Move* move);

// Game state functions
void init_game_state(GameState* state) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            state->board[i][j] = EMPTY_CELL;
        }
    }
    state->current_player = HUMAN_PLAYER;
    state->game_active = 1;
}

int process_move(GameState* state, Move move) {
    if (move.row < 0 || move.row >= BOARD_SIZE || 
        move.col < 0 || move.col >= BOARD_SIZE ||
        state->board[move.row][move.col] != EMPTY_CELL) {
        return 0;
    }
    
    state->board[move.row][move.col] = move.player;
    return 1;
}

int check_winner(const GameState* state) {
    // Check rows
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(state->board[i][0] != EMPTY_CELL && 
           state->board[i][0] == state->board[i][1] && 
           state->board[i][1] == state->board[i][2]) {
            return 1;
        }
    }
    
    // Check columns
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(state->board[0][i] != EMPTY_CELL && 
           state->board[0][i] == state->board[1][i] && 
           state->board[1][i] == state->board[2][i]) {
            return 1;
        }
    }
    
    // Check diagonals
    if(state->board[0][0] != EMPTY_CELL && 
       state->board[0][0] == state->board[1][1] && 
       state->board[1][1] == state->board[2][2]) {
        return 1;
    }
    
    if(state->board[0][2] != EMPTY_CELL && 
       state->board[0][2] == state->board[1][1] && 
       state->board[1][1] == state->board[2][0]) {
        return 1;
    }
    
    return 0;
}

int is_board_full(const GameState* state) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(state->board[i][j] == EMPTY_CELL) {
                return 0;
            }
        }
    }
    return 1;
}

// I/O functions
Move get_cpu_move(const GameState* state) {
    Move move;
    move.player = CPU_PLAYER;
    
    do {
        move.row = rand() % BOARD_SIZE;
        move.col = rand() % BOARD_SIZE;
    } while(state->board[move.row][move.col] != EMPTY_CELL);
    
    return move;
}

Move get_player_move(const GameState* state) {
    Move move;
    move.player = HUMAN_PLAYER;
    
    while(1) {
        printf("Enter row (1-3) and column (1-3) separated by space: ");
        scanf("%d %d", &move.row, &move.col);
        move.row--; move.col--; // Convert to 0-based indexing
        
        if(move.row >= 0 && move.row < BOARD_SIZE && 
           move.col >= 0 && move.col < BOARD_SIZE && 
           state->board[move.row][move.col] == EMPTY_CELL) {
            break;
        }
        printf("Invalid move! Try again.\n");
    }
    
    return move;
}

void log_final_state(const GameState* state, const char* result) {
    FILE* game_log = fopen("tictactoe.log", "a");
    if(game_log == NULL) return;
    
    // Get timestamp
    time_t now = time(NULL);
    char timestamp[26];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(game_log, "\n=== Game Ended: %s ===\n", timestamp);
    fprintf(game_log, "Result: %s\n", result);
    fprintf(game_log, "Final board state:\n");
    
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            fprintf(game_log, " %c ", state->board[i][j]);
            if(j < BOARD_SIZE-1) fprintf(game_log, "|");
        }
        fprintf(game_log, "\n");
        if(i < BOARD_SIZE-1) fprintf(game_log, "-----------\n");
    }
    fprintf(game_log, "\n");
    
    fclose(game_log);
}

void log_move(const Move* move) {
    FILE* game_log = fopen("tictactoe.log", "a");
    if(game_log == NULL) return;
    
    fprintf(game_log, "%c placed at position (%d,%d)\n", 
            move->player, move->row + 1, move->col + 1);
    
    fclose(game_log);
}

void display_board(const GameState* state) {
    printf("\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", state->board[i][j]);
            if(j < BOARD_SIZE-1) printf("|");
        }
        printf("\n");
        if(i < BOARD_SIZE-1) printf("-----------\n");
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    GameState game_state;
    Move current_move;
    
    init_game_state(&game_state);
    display_board(&game_state);
    
    while(game_state.game_active) {
        // Human turn
        current_move = get_player_move(&game_state);
        process_move(&game_state, current_move);
        log_move(&current_move);
        display_board(&game_state);
        
        if(check_winner(&game_state)) {
            printf("You win!\n");
            log_final_state(&game_state, "Player wins!");
            break;
        }
        
        if(is_board_full(&game_state)) {
            printf("It's a tie!\n");
            log_final_state(&game_state, "Game ended in a tie!");
            break;
        }
        
        // CPU turn
        current_move = get_cpu_move(&game_state);
        process_move(&game_state, current_move);
        log_move(&current_move);
        display_board(&game_state);
        
        if(check_winner(&game_state)) {
            printf("CPU wins!\n");
            log_final_state(&game_state, "CPU wins!");
            break;
        }
        
        if(is_board_full(&game_state)) {
            printf("It's a tie!\n");
            log_final_state(&game_state, "Game ended in a tie!");
            break;
        }
    }
    
    return 0;
}