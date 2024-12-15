#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY_CELL '_'
#define HUMAN_PLAYER 'X'
#define CPU_PLAYER 'O'

// Board is 3x3
char game_board[3][3];
FILE* game_log;

void init_game_board() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            game_board[i][j] = EMPTY_CELL;
        }
    }
}

void log_message(const char* message) {
    fprintf(game_log, "%s\n", message);
}

void display_board() {
    printf("\n");
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            printf(" %c ", game_board[i][j]);
            if(j < 2) printf("|");
        }
        printf("\n");
        if(i < 2) printf("-----------\n");
    }
    printf("\n");
}

void log_board_state() {
    fprintf(game_log, "\nCurrent board state:\n");
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            fprintf(game_log, " %c ", game_board[i][j]);
            if(j < 2) fprintf(game_log, "|");
        }
        fprintf(game_log, "\n");
        if(i < 2) fprintf(game_log, "-----------\n");
    }
    fprintf(game_log, "\n");
}

int check_winner() {
    // Check rows
    for(int i = 0; i < 3; i++) {
        if(game_board[i][0] != EMPTY_CELL && 
           game_board[i][0] == game_board[i][1] && 
           game_board[i][1] == game_board[i][2]) {
            return 1;
        }
    }
    
    // Check columns
    for(int i = 0; i < 3; i++) {
        if(game_board[0][i] != EMPTY_CELL && 
           game_board[0][i] == game_board[1][i] && 
           game_board[1][i] == game_board[2][i]) {
            return 1;
        }
    }
    
    // Check diagonals
    if(game_board[0][0] != EMPTY_CELL && 
       game_board[0][0] == game_board[1][1] && 
       game_board[1][1] == game_board[2][2]) {
        return 1;
    }
    
    if(game_board[0][2] != EMPTY_CELL && 
       game_board[0][2] == game_board[1][1] && 
       game_board[1][1] == game_board[2][0]) {
        return 1;
    }
    
    return 0;
}

int is_board_full() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(game_board[i][j] == EMPTY_CELL) {
                return 0;
            }
        }
    }
    return 1;
}

void make_cpu_move() {
    // Simple CPU strategy: choose random empty cell
    int row, col;
    do {
        row = rand() % 3;
        col = rand() % 3;
    } while(game_board[row][col] != EMPTY_CELL);
    
    game_board[row][col] = CPU_PLAYER;
    fprintf(game_log, "CPU placed O at position (%d,%d)\n", row + 1, col + 1);
}

int main() {
    // Initialize random seed
    srand(time(NULL));
    
    // Open log file
    game_log = fopen("tictactoe.log", "w");
    if(game_log == NULL) {
        printf("Error opening log file!\n");
        return 1;
    }
    
    log_message("Game started");
    
    init_game_board();
    display_board();
    log_board_state();
    
    while(1) {
        int row, col;
        
        // Human turn
        while(1) {
            printf("Enter row (1-3) and column (1-3) separated by space: ");
            scanf("%d %d", &row, &col);
            row--; col--; // Convert to 0-based indexing
            
            if(row >= 0 && row < 3 && col >= 0 && col < 3 && 
               game_board[row][col] == EMPTY_CELL) {
                break;
            }
            printf("Invalid move! Try again.\n");
        }
        
        game_board[row][col] = HUMAN_PLAYER;
        fprintf(game_log, "Player placed X at position (%d,%d)\n", row + 1, col + 1);
        
        display_board();
        log_board_state();
        
        if(check_winner()) {
            printf("You win!\n");
            log_message("Player wins!");
            break;
        }
        
        if(is_board_full()) {
            printf("It's a tie!\n");
            log_message("Game ended in a tie!");
            break;
        }
        
        // CPU turn
        make_cpu_move();
        display_board();
        log_board_state();
        
        if(check_winner()) {
            printf("CPU wins!\n");
            log_message("CPU wins!");
            break;
        }
        
        if(is_board_full()) {
            printf("It's a tie!\n");
            log_message("Game ended in a tie!");
            break;
        }
    }
    
    fclose(game_log);
    return 0;
}