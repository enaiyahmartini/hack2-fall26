/* ***********

look to end of code for some citations

*********** */

#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
using namespace std;

class SongInformation {
    public:
        // setter functions
        void SetNumber(string songNumber);
        void SetTitle(string songTitle);
        void SetAlbum(string songAlbum);
        void SetGenre(string songGenre);
        void SetDuration(string songDuration);

        // getter functions
        string GetNumber();
        string GetTitle();
        string GetAlbum();
        string GetGenre();
        string GetDuration();
        string GetSelectedSong();

    private:
        string number;
        string title;
        string album;
        string genre;
        string duration;
        string returnSelctedSong;
};

// takes the number in the position of the vector
// and will eventually return it to another vector
void SongInformation::SetNumber(string songNumber) {
    number = songNumber;
}
string SongInformation::GetNumber() {
    return number;
}

// takes the title in the position of the vector
// and will eventually return it to another vector
void SongInformation::SetTitle(string songTitle) {
    title = songTitle;
}
string SongInformation::GetTitle() {
    return title;
}

// takes the song in the position of the vector
// and will eventually return it to another vector
void SongInformation::SetAlbum(string songAlbum) {
    album = songAlbum;
}
string SongInformation::GetAlbum() {
    return album;
}

// takes the genre in the position of the vector
// and will eventually return it to another vector
void SongInformation::SetGenre(string songGenre) {
    genre = songGenre;
}
string SongInformation::GetGenre() {
    return genre;
}

// takes the duration in the position of the vector
// and will eventually return it to another vector
void SongInformation::SetDuration(string songDuration) {
    duration = songDuration;
}
string SongInformation::GetDuration() {
    return duration;
}

string SongInformation::GetSelectedSong() {
    return returnSelctedSong;
}

int main() {

    // annoucement in terminal
    cout << endl;
    cout << "ATTENTION: Read these directions before proceeding to the port. Select 4 songs that will appear on the display. Be sure to click the name of the song you want selected!!! Once a song is selected is CANNOT be replaced. Upon selection of the song, you will be redirected to another screen. Please proceed to the port. Thank you! :)";
    cout << endl;

    // ******
    // ****** TO BE CHANGED: VECTOR OF SONG INFORMATION *******
    // ******
    vector<string> songNumbers = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    vector<string> songTitles = {"song1", "song2", "song3", "song4", "song5", "song6", "song7", "song8", "song9", "song10"};
    vector<string> songAlbums = {"album1", "album2", "album3", "album4", "album5", "album6", "album7", "album8", "album9", "album10"};
    vector<string> songGenre = {"genre1", "genre2", "genre3", "genre4", "genre5", "genre6", "genre7", "genre8", "genre9", "genre10"};
    vector<string> songDuration = {"duration1", "duration2", "duration3", "duration4", "duration5", "duration6", "duration7", "duration8", "duration9", "duration10"};

    // tracks which screen user is on
    // want one song per screen
    int currentScreen = 1;

    // creating 2 vectors. one for all the classifying details of the song
    // another for the 4 songs the user will pick
    vector<SongInformation> songInfo;
    vector<SongInformation> selectedSongs;

    // adding all the information from our single detail vectors
    // to a vector that has all the information, with the class SongInformation 
    for (int i = 0; i < songNumbers.size(); ++i) {
        SongInformation currentSong;

        currentSong.SetNumber(songNumbers.at(i));
        currentSong.SetTitle(songTitles.at(i));
        currentSong.SetAlbum(songAlbums.at(i));
        currentSong.SetGenre(songGenre.at(i));
        currentSong.SetDuration(songDuration.at(i));

        songInfo.push_back(currentSong);
    }

    //
    //
    //
    /* TO CONTINUE FOR NEXT TIME: 
    WITH THIS SELECTED SONGS VECTOR, TEAMMATE IS GOING TO
    CLASSIFY THE PERSON BASED ON THE SONGS THEY CHOOSE
    AFTER THAT:
    WILL MAKE A SCREEN TO DISPLAY WHAT THE CODE THINKS THAT PERSON IS LIKE */
    //
    //
    //

    
    InitWindow(1300, 800, "Spotify Playlist");
    SetTargetFPS(30);

    Texture2D photo = LoadTexture("spotifypicture.jpeg");
    if (photo.id == 0) {
        CloseWindow();
        return -1;
    }


    while (!WindowShouldClose()) {

        // adding the selected songs the user picked to a vector
        // link 2 (check end of code)
        for (int i = 0; i < songInfo.size(); ++i) {
            Rectangle songArea = {110, (340 + (i * 40)), 200, 30};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), songArea) && selectedSongs.size() < 10) {
                selectedSongs.push_back(songInfo.at(i));
                cout << "Song: " << songInfo.at(i).GetTitle() << endl;
                currentScreen++;
                break;
            }
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen <= 10) {
            Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
            Rectangle dest   = { 0, 0, 1300, 200 };

            DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);

            // text display strings
            string yourPlaylist = "YOUR PLAYLIST :)";
            string poundKey = "#";
            string titleKey = "Title";
            string albumKey = "Album";
            string genreKey = "Genre";
            string durationKey = "Duration";
            string message1 = "Please Select a Song";
            string message2 = "Screen " + to_string(currentScreen) + "/10";
            // when using static_cast for line 182, yielded in an error. 
            // the to_string code was written with the help of gemini 
            // when prompted "can static_cast be used to convert this integer variable to a string"
            // ascessed: Nov 23, 2026
       
            // coordinate system understanding from link 1 (check end of code)
            DrawLine(0, 200, 1300, 200, WHITE);

            // making play button
            DrawCircle(50, 240, 25, LIME);
            DrawTriangle(Vector2{42, 225}, Vector2{42, 255}, Vector2{65, 240}, BLACK);

            // on screen directions
            DrawText(message1.c_str(), 90, 230, 25, LIME);
            DrawText(message2.c_str(), 1000, 230, 25, LIME);
        
            // making settings button
            DrawCircle(1200, 240, 5, WHITE);
            DrawLine(1215, 240, 1275, 240, WHITE);
            DrawCircle(1200, 225, 5, WHITE);
            DrawLine(1215, 225, 1275, 225, WHITE);
            DrawCircle(1200, 255, 5, WHITE);
            DrawLine(1215, 255, 1275, 255, WHITE);
        
            // making the filters
            DrawLine(0, 320, 1300, 320, WHITE);
            DrawText(poundKey.c_str(), 40, 290, 20, WHITE);
            DrawText(titleKey.c_str(), 110, 290, 20, WHITE);
            DrawText(albumKey.c_str(), 400, 290, 20, WHITE);
            DrawText(genreKey.c_str(), 770, 290, 20, WHITE);
            DrawText(durationKey.c_str(), 1110, 290, 20, WHITE);

            // looping through the vector of songs. calling our functions to place 
            // the number, title, album, etc of the song in a list on the screen
            for (int i = 0; i < songInfo.size(); ++i) {
                DrawText(songInfo.at(i).GetNumber().c_str(), 40, 340 + (i * 40), 20, WHITE);
                DrawText(songInfo.at(i).GetTitle().c_str(), 110, 340 + (i * 40), 20, WHITE);
                DrawText(songInfo.at(i).GetAlbum().c_str(), 400, 340 + (i * 40), 20, WHITE);
                DrawText(songInfo.at(i).GetGenre().c_str(), 770, 340 + (i * 40), 20, WHITE);
                DrawText(songInfo.at(i).GetDuration().c_str(), 1110, 340 + (i * 40), 20, WHITE);
            }
        }
        else {
            DrawText("Check", 250, 250, 50, LIME);

            // **********
            // will replace; just here as placeholder until we get all code together
            // **********
            int averageScore = 1;

            if (averageScore == 1) {
                Texture2D photo = LoadTexture("pop.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 2) {
                Texture2D photo = LoadTexture("rap.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 3) {
                Texture2D photo = LoadTexture("randb.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 4) {
                Texture2D photo = LoadTexture("dance.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 5) {
                Texture2D photo = LoadTexture("rock.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 6) {
                Texture2D photo = LoadTexture("indie.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 7) {
                Texture2D photo = LoadTexture("country.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 8) {
                Texture2D photo = LoadTexture("jazz.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 9) {
                Texture2D photo = LoadTexture("afrobeats.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
            else if (averageScore == 10) {
                Texture2D photo = LoadTexture("classical.jpeg");
                if (photo.id == 0) {
                    CloseWindow();
                    return -1;
                }
                Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
                Rectangle dest   = { 0, 0, 1300, 200 };
                DrawTexturePro(photo, source, dest, {0, 0}, 0.0f, WHITE);
            }
        }

        
        EndDrawing();

    }

    UnloadTexture(photo);
    CloseWindow();

    return 0;
}



/* 

Link 1: 
This code was adpated from
https://youtu.be/RGzj-PF7D74?si=8OhCLHy0wwLSN3le
Accessed: Nov 17, 2026
Aided in overall understanding of Raylib
Used specifically in the understanding of how the coordinate system works


Link 2: 
This code was adapted from
https://youtu.be/klqbAOAFBXw?si=Srq6evwa5dmJy5hG
Accessed: Nov 20, 2026
Aided in creation adding selected songs to new vector
by using Raylib CheckCollision function

Link 3:
This code was adpated from
https://www.raylib.com/examples/core/loader.html?name=core_basic_screen_manager
Accessed: Nov 23, 2026
Aided in creation of a menu screen displaying game directions

Link 4:
This code was adapted from
https://www.raylib.com/examples/textures/loader.html?name=textures_sprite_button
Accessed: Nov 24, 2026
Aided in creation of highlight effect when hovering over a text

*/