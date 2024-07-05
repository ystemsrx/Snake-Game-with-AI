#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义速度
#define VERY_SLOW 300
#define SLOW 200
#define MEDIUM 120
#define FAST 100
#define ULTRA_FAST 80

// 函数声明
void startGame(const char *gameMode, int speed);
void compileAndRun(const char *gameMode, int speed);

int main() {
    int choice;

    printf("Select Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Normal\n");
    printf("3. Expert\n");
    printf("4. Master\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 4) {
        int speed;
        switch(choice) {
            case 1:
                speed = SLOW;
                startGame("Easy", speed);
                break;
            case 2:
                speed = MEDIUM;
                startGame("Normal", speed);
                break;
            case 3:
                speed = FAST;
                startGame("Expert", speed);
                break;
            case 4:
                speed = ULTRA_FAST;
                startGame("Master", speed);
                break;
        }
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}

void startGame(const char *gameMode, int speed) {
    printf("Starting game %s at speed %d...\n", gameMode, speed);
    compileAndRun(gameMode, speed);
}

void compileAndRun(const char *gameMode, int speed) {
    char compileCommand[256];
    snprintf(compileCommand, sizeof(compileCommand), "gcc -o %s.exe %s.c", gameMode, gameMode);
    system(compileCommand);

    char command[256];
    snprintf(command, sizeof(command), "%s.exe %d", gameMode, speed);
    system(command);
}

