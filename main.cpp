#include "raylib.h"
using namespace std;

int main() {

    InitWindow(1000, 800, "Spotify Playlist");
    SetTargetFPS(30);

    Texture2D spotifyScreen = LoadTexture("//include image here//");
    if (spotifyScreen.id == 0) {
        CloseWindow();
        return -1;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        
    }

    return 0;
}