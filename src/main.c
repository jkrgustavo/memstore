
#include "hash.h"
#define UTIL_STD
#include "utils.h"
#include "../res/raylib/raylib.h"

#define WIDTH 100
#define HEIGHT 100
int8_t pixels[WIDTH * HEIGHT] = { 0 };

void render() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (i < hash_size()) {
            pixels[i] = hash_status(i);
        } else {
            pixels[i] = 2;
        }
    }
}

void draw() {
    Color status = GREEN;
    
    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            switch (pixels[(i*WIDTH) + j]) {
                case 0: {
                    status = GREEN;
                } break;
                case 1: {
                    status = RED;
                } break;
                case -1: {
                    status = YELLOW;
                } break;
                default: {
                    status = GRAY;
                } break;
            }
            DrawRectangleV((Vector2) {.x = 10 * j, .y = 10 * i}, (Vector2) {.x = 9, .y = 9}, status);
        }
    }
    DrawFPS(500, 900);
    EndDrawing();
}

int main() {
    
    char *file_name = "wordlSorted.txt";
    char *buffer = read_file(file_name);
    char **words = process_words(buffer);

/*------------------------Drawing--------------------------*/

    int frame_count = 0;
    char buff[6] = { 0 };

    InitWindow(1000, 1000, "Cum");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (frame_count < 5195) {
            sprintf(buff, "%d", frame_count);
            hash_insert(words[frame_count], buff);
        }
        render();
        draw();
        frame_count++;
    }
    CloseWindow();

/*------------------------Drawing--------------------------*/

    free(words);
    free(buffer);
    hash_destroy();
    return 0;
}
