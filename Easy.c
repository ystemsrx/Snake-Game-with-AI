#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <io.h>

#define WIDTH 50
#define HEIGHT 25
#define INIT_LENGTH 3
#define BUFFER_SIZE 1024 // Define the buffer size constant

char latest_message[256];
CRITICAL_SECTION message_mutex;

typedef struct {
    char *data;
    size_t size;
    size_t read_pos;
    CRITICAL_SECTION mutex;
    HANDLE event;
} Buffer;

Buffer buffer; // 声明全局缓冲区变量

void init_buffer(Buffer *buffer) {
    buffer->data = (char *)malloc(BUFFER_SIZE);
    buffer->size = 0;
    buffer->read_pos = 0;
    InitializeCriticalSection(&buffer->mutex);
    buffer->event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

DWORD WINAPI write_to_file_thread(LPVOID arg) {
    Buffer *buffer = (Buffer *)arg;
    while (1) {
        WaitForSingleObject(buffer->event, INFINITE); // 等待事件触发

        EnterCriticalSection(&buffer->mutex);
        if (buffer->size > 0) {
            FILE* file = fopen("Conversations.txt", "a");
            if (file) {
                fprintf(file, "%s\n", buffer->data);
                fclose(file);

                // 写入后等待0.2s重命名文件
                Sleep(200);
                rename("Conversations.txt", "Conversation.txt");
            }
            buffer->size = 0; // 清空缓冲区
            buffer->read_pos = 0;
        }
        LeaveCriticalSection(&buffer->mutex);
    }
    return 0;
}

void append_to_buffer(Buffer *buffer, const char *text) {
    EnterCriticalSection(&buffer->mutex);
    size_t text_len = strlen(text);
    // 覆盖旧的内容，保留最新的内容
    if (text_len >= BUFFER_SIZE) {
        // 如果新的内容比缓冲区还大，截取末尾部分
        memcpy(buffer->data, text + text_len - BUFFER_SIZE + 1, BUFFER_SIZE - 1);
        buffer->size = BUFFER_SIZE - 1;
    } else {
        buffer->size = 0; // 重置缓冲区大小
        memcpy(buffer->data, text, text_len);
        buffer->size = text_len;
    }
    buffer->data[buffer->size] = '\0';
    SetEvent(buffer->event);
    LeaveCriticalSection(&buffer->mutex);
}

void clear_buffer(Buffer *buffer) {
    EnterCriticalSection(&buffer->mutex);
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->read_pos = 0;
    LeaveCriticalSection(&buffer->mutex);
}

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[WIDTH * HEIGHT];
    int length;
    int dx, dy; // 用于记录蛇的当前方向
} Snake;

void initialize(Snake *snake) {
    snake->length = INIT_LENGTH;
    snake->dx = -1;
    snake->dy = 0;
    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = WIDTH / 2 + i;
        snake->body[i].y = HEIGHT / 2;
    }
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void drawMap() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        gotoxy(snake->body[i].x, snake->body[i].y);
        printf("*");
    }
}

void eraseSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        gotoxy(snake->body[i].x, snake->body[i].y);
        printf(" ");
    }
}

void generateFood(Point *food) {
    srand(time(NULL));
    food->x = rand() % (WIDTH - 2) + 1;
    food->y = rand() % (HEIGHT - 2) + 1;
}

void generateBonusFood(Point *bonusFood) {
    srand(time(NULL));
    bonusFood->x = rand() % (WIDTH - 3) + 1;
    bonusFood->y = rand() % (HEIGHT - 3) + 1;
}

void drawFood(Point *food) {
    gotoxy(food->x, food->y);
    printf("$");
}

void drawBonusFood(Point *bonusFood) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            gotoxy(bonusFood->x + i, bonusFood->y + j);
            printf("$");
        }
    }
}

void moveSnake(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0].x += snake->dx;
    snake->body[0].y += snake->dy;

    // 边界处理：从一端出去从另一端进来
    if (snake->body[0].x <= 0) snake->body[0].x = WIDTH - 2;
    if (snake->body[0].x >= WIDTH - 1) snake->body[0].x = 1;
    if (snake->body[0].y <= 0) snake->body[0].y = HEIGHT - 2;
    if (snake->body[0].y >= HEIGHT - 1) snake->body[0].y = 1;
}

int checkCollision(Snake *snake, Point *food) {
    return snake->body[0].x == food->x && snake->body[0].y == food->y;
}

int checkBonusCollision(Snake *snake, Point *bonusFood) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (snake->body[0].x == bonusFood->x + i && snake->body[0].y == bonusFood->y + j) {
                return 1;
            }
        }
    }
    return 0;
}

int checkSelfCollision(Snake *snake) {
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return i;
        }
    }
    return -1;
}

void growSnake(Snake *snake, int amount) {
    for (int i = 0; i < amount; i++) {
        snake->body[snake->length + i] = snake->body[snake->length - 1];
    }
    snake->length += amount;
}

void trimSnake(Snake *snake, int index) {
    for (int i = index; i < snake->length; i++) {
        gotoxy(snake->body[i].x, snake->body[i].y);
        printf(" ");
    }
    snake->length = index;
}

void writeToDocument(const char* message) {
    append_to_buffer(&buffer, message);
}


void create_or_reset_file() {
    if (_access("Conversation.txt", 0) != -1 && _access("Conversations.txt", 0) != -1) {
        _unlink("Conversation.txt");
    } else if (_access("Conversation.txt", 0) != -1) {
        rename("Conversation.txt", "Conversations.txt");
    }

    FILE *file = fopen("Conversations.txt", "w");
    if (file) {
        fclose(file);
    }
}

int main(int argc, char *argv[]) {
    InitializeCriticalSection(&message_mutex);
    init_buffer(&buffer); // 初始化缓冲区
    
    create_or_reset_file();
    writeToDocument("The player starts the game, gamemode: Easy.");

    system("start AI_Assistant.exe");

    // 启动写入线程
    HANDLE hThread = CreateThread(NULL, 0, write_to_file_thread, &buffer, 0, NULL);
    if (hThread == NULL) {
        perror("Failed to create write to file thread");
        exit(1);
    }
    if (argc < 2) {
        printf("Usage: Easy.exe [speed]\n");
        return 1;
    }
    int speed = atoi(argv[1]);

    Snake snake;

    if (system("start /min cmd /c Start_Python.bat") == -1) {
        perror("Failed to start batch script");
        exit(1);
    }

    Point food, bonusFood;
    int bonusActive = 0;
    int trimIndex = -1;
    clock_t lastBonusTime = clock();
    clock_t trimTime = 0;
    clock_t currentTime;

    hideCursor();
    initialize(&snake);
    generateFood(&food);

    drawMap();
    drawSnake(&snake);
    drawFood(&food);

    while (1) {
        // 显示当前长度和倒计时
        currentTime = clock();
        gotoxy(0, HEIGHT);
        printf("Current Score: %d", snake.length - 3);
        gotoxy(20, HEIGHT);
        
        if (bonusActive) {
            int remainingTime = 8 - (int)((currentTime - lastBonusTime) / CLOCKS_PER_SEC);
            gotoxy(20, HEIGHT);
            printf("Bonus food time: %d", remainingTime);

            // 如果倒计时为负值，移除Bonus Food并重置状态
            if (remainingTime <= 0) {
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        gotoxy(bonusFood.x + i, bonusFood.y + j);
                        printf(" ");
                    }
                }
                bonusActive = 0;
                lastBonusTime = currentTime;
            }
        } else {
            gotoxy(20, HEIGHT);
            printf("Next bonus food in: %d", 30 - (int)((currentTime - lastBonusTime) / CLOCKS_PER_SEC));
        }

        // 随机生成2x2的bonus food
        if ((currentTime - lastBonusTime) / CLOCKS_PER_SEC >= 30 && !bonusActive) {
            generateBonusFood(&bonusFood);
            drawBonusFood(&bonusFood);
            bonusActive = 1;
            lastBonusTime = currentTime;
        }

        // 移动蛇
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'w': if (snake.dy == 0) { snake.dx = 0; snake.dy = -1; } break;
                case 's': if (snake.dy == 0) { snake.dx = 0; snake.dy = 1; } break;
                case 'a': if (snake.dx == 0) { snake.dx = -1; snake.dy = 0; } break;
                case 'd': if (snake.dx == 0) { snake.dx = 1; snake.dy = 0; } break;
            }
        }

        eraseSnake(&snake);
        moveSnake(&snake);
        drawSnake(&snake);

        // 检查碰撞
        if (checkCollision(&snake, &food)) {
            generateFood(&food);
            drawFood(&food);
            growSnake(&snake, 1);
            char message[50];
            sprintf(message, "The player has ate a food and got 1 point (%d/30)", snake.length - 3);
            writeToDocument(message);
        }

    // 检查是否撞到自己
    trimIndex = checkSelfCollision(&snake);
    if (trimIndex != -1 && trimTime == 0) {
        trimTime = currentTime;
    }

    // 截断身体并更新分数
    if (trimIndex != -1 && (currentTime - trimTime) / CLOCKS_PER_SEC >= 3) {
        trimSnake(&snake, trimIndex);
        gotoxy(0, HEIGHT);
        printf("Current Score: %d", snake.length - 3);
        char message[50];
        sprintf(message, "The player has cut the snake's body and he now has %d/30", snake.length - 3);
        writeToDocument(message);
        trimIndex = -1;
        trimTime = 0;
    }

        if (bonusActive && checkBonusCollision(&snake, &bonusFood)) {
            growSnake(&snake, 5);
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    gotoxy(bonusFood.x + i, bonusFood.y + j);
                    printf(" ");
                }
            }
            char message[50];
            sprintf(message, "The player has ate a bonus food and got 5 point (%d/30)", snake.length - 3);
            writeToDocument(message);
            bonusActive = 0;
            lastBonusTime = currentTime;
        }

        if (trimIndex != -1 && (currentTime - trimTime) / CLOCKS_PER_SEC >= 3) {
            trimSnake(&snake, trimIndex);
            trimIndex = -1;
            trimTime = 0;
        }

        // 延时
        Sleep(speed);

        // 显示胜利信息
        if (snake.length >= 33) {
            gotoxy(WIDTH / 2 - 2, HEIGHT / 2);
            printf("Win!");
            writeToDocument("The player has won the game!");
            break;
        }
    }

    return 0;
}
