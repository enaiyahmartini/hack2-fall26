#include "raylib.h"
using namespace std;

int main() {

    InitWindow(1000, 800, "Spotify Playlist");
    SetTargetFPS(30);

   Texture2D photo = LoadTexture("//include image here//");
    if (photo.id == 0) {
        CloseWindow();
        return -1;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        Rectangle source = {0, 0, (float)photo.width, (float)photo.height};
        Rectangle dest = {0, 0, 1000, 800};

        

    }

    return 0;
}