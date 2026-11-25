/* ***********

Note: Takes some time to load if white screen


Put into terminal to install API:
sudo apt update && sudo apt install libmusicbrainz5-2 libmusicbrainz5-dev


look to end of code for some other citations


The following file allows you to get the metadata from the API
It then puts the metadata into readable format into the song object
It does this 10 times to add to the 10 song vector


I looked at the following code file with documentation as an example to help me:
https://github.com/metabrainz/libmusicbrainz/blob/master/examples/cdlookup.cc


I also used gemini on 11/23 at 7 am to help me fix bugs with how I accessed mymetadata

*********** */


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "raylib.h"
#include "musicbrainz5/Query.h"
#include "musicbrainz5/Medium.h"
#include "musicbrainz5/MediumList.h"
#include "musicbrainz5/ReleaseGroup.h"
#include "musicbrainz5/Track.h"
#include "musicbrainz5/TrackList.h"
#include "musicbrainz5/Recording.h"
#include "musicbrainz5/Disc.h"
#include "musicbrainz5/HTTPFetch.h"
#include "musicbrainz5/Release.h"
#include "musicbrainz5/Collection.h"
#include "musicbrainz5/CollectionList.h"
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/NameCreditList.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Artist.h>
#include <musicbrainz5/Tag.h>
#include <unistd.h> //time library for mac/os
#include "song.cpp"

using namespace std;
using namespace MusicBrainz5;

// The following function gets the songs from the API
vector<Song> getSongs(int limit, int offset) {
   vector<Song> songs;
   MusicBrainz5::CQuery Query("MyMusicApp/1.0 (contact@example.com)"); //To start the query you have to link it to an email, I just used the one in the documentation

   // This code (the try catch strucuture specifically) came from the github documentation above
   // The try catch allows the program to catch exceptions without crashing thr program
   try{
    // These parameters allow you to be selective above what kinds of songs you want to access
       MusicBrainz5::CQuery::tParamMap Params;
       Params["query"] = "country:US AND tag:*"; // This sets the query to songs from the United States that have genres (the tags)
       Params["limit"] = to_string(limit); // This generaes the limit amount of songs per page
       Params["offset"] = to_string(offset); // This offsets the songs by 50 each time so that songs do not repeat


       MusicBrainz5::CMetadata Metadata = Query.Query("recording", "", "", Params); // This runs the query based on the parameters we made above


       if (Metadata.RecordingList()) {
           MusicBrainz5::CRecordingList *RecordingList = Metadata.RecordingList();
           cout << "Found " << RecordingList->NumItems() << " recordings(s)\n" << endl;

           // The following returns the attributes of the songs, to be put into the song class and songs vector
           for (int i = 0; i < RecordingList->NumItems(); i++) {
               MusicBrainz5::CRecording *Recording = RecordingList->Item(i);

               string id = Recording->ID(); // Gets song ID
               string title = Recording->Title(); // Gets song name

               // 11/23/26 11:45 Gemini generated the following artist accessor because
               // it kept returning errors that it couldn't find the following.
               // This returns the first listed artist if the artist credit list isn't empty.
               string artist = "Unknown";
               if (Recording->ArtistCredit() &&
                   Recording->ArtistCredit()->NameCreditList() &&
                   Recording->ArtistCredit()->NameCreditList()->NumItems() > 0) {
                  
                   auto *NC = Recording->ArtistCredit()->NameCreditList()->Item(0);
                   if (!NC->Name().empty()) {
                       artist = NC->Name();
                   } else if (NC->Artist()) {
                       artist = NC->Artist()->Name();
                   }
               }

               string album = "Unknown"; // Set to unknown in case the information at the endpoint doesn't exist
               if (Recording->ReleaseList() && Recording->ReleaseList()->NumItems() > 0) {
                   album = Recording->ReleaseList()->Item(0)->Title();
               }

               // the genre is inside the taglist in this database
               string genre = "Unknown"; // Set to unknown in case the information at the endpoint doesn't exist
               if (Recording->TagList() && Recording->TagList()->NumItems() > 0) {
                   genre = Recording->TagList()->Item(0)->Name();
               }

               int durationS = Recording->Length()/1000; // Gives duration in seconds
               int durationM = durationS / 60; // Gets the amount of time in minutes
               durationS %= 60; // Gets remainder of time in seconds
               string duration = "";
               if (durationS < 10){ // Converts duration to a string
                   duration = to_string(durationM) + ":0" + to_string(durationS); //handles single seconds
               } else {
                   duration = to_string(durationM) + ":" + to_string(durationS);
               }

               // Creates a song object that will be put into the vector
               Song s = Song(id, title, artist, album, genre, duration);
               s.print();
               songs.push_back(s);
           }
           return songs;
       }
   }
   catch (MusicBrainz5::CConnectionError& Error)
   {
       cout << "Connection Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   catch (MusicBrainz5::CTimeoutError& Error)
   {
       cout << "Timeout Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   catch (MusicBrainz5::CAuthenticationError& Error)
   {
       cout << "Authentication Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   catch (MusicBrainz5::CFetchError& Error) {
       cout << "Fetch Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   catch (MusicBrainz5::CRequestError& Error) {
       cout << "Request Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   catch (MusicBrainz5::CResourceNotFoundError& Error) {
       cout << "ResourceNotFound Exception: '" << Error.what() << "'" << endl;
       cout << "LastResult: " << Query.LastResult() << endl;
       cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
       cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
   }
   return songs;
}

map<string,int> scoreGenre = {
 {"pop", 1},
 {"kpop", 1},
 {"anime", 1},

 {"hip-hop", 2},

 {"R&B", 3},
 {"funk & disco", 3},

 {"dance", 4},
 {"electronic", 4},
 {"electro", 4},

 {"rock", 5},
 {"punk", 5},
 {"metal", 5},
 {"heavy metal", 5},

 {"indie", 6},

 {"country", 7},
 {"folk & acoustic", 7},

 {"jazz", 8},
 {"blues", 8},

 {"afrobeats", 9},
 {"caribbean", 9},
 {"reggae", 9},

 {"classical", 10},
 {"christian & gospel", 10}};
// this map shows the similarity between some of the music genre group and assign them to different score from 1 - 10.
/* the categories generated by Chat GPT to show similarity and the codes 11/24/26 2:32AM*/

int CalculatingScore(vector<Song> selectedSongs){ //getAverage
   int sum = 0;
   int average = 0;
   int count = 0;
   for (int i = 0; i < selectedSongs.size(); i++) {
       if (scoreGenre.find(selectedSongs[i].getGenre()) != scoreGenre.end()) {
          sum += scoreGenre[selectedSongs[i].getGenre()]; //if the genre is found in class object, then it will add that score assigned in scoreGenre.
          count++; // the number of genre
       }
   } 
   // 11/24/26 2 AM research on map find and end functions (I used at, and it doesn't seem to work with more values
   // (overflow, out of range))https://www.geeksforgeeks.org/cpp/map-find-function-in-c-stl/

   if (count == 0) {
      return -1;
   }

   average = static_cast<int>(sum/count);
   return average;
}

// Gives the classification of the general personality; note that this function is not directly used because our user interface uses images
void getResult(int averageScore){
   cout << "Get to know your playlist and your personality..." << endl;
   if (averageScore == 1){
       cout << "You are Pop lover. Extroverted, honest, and conventional. 🎤✨🪩💖 "<< endl ;
       cout << "Researchers suggest that they are less creative and more uneasy than those enamored by other musical styles."<< endl;
       cout << endl;
   } else if (averageScore == 2){
       cout << "You are Rap/Hip-Hop lover. 🎙️🎧🔊👟 " << endl;
       cout << "The rap fans tended to have high self-esteem and were generally more outgoing than fans of other styles." << endl;
       cout << endl;
   } else if (averageScore == 3){
       cout << "You are R&B lover. 🎹💜🕯️🎙️ " << endl;
       cout << "Elevated agreeableness and emotional intelligence." << endl;
       cout << "Also have high self-esteem and social confidence." << endl;
       cout << endl;
   } else if (averageScore == 4){
       cout << "You are Dance/Electronic lover. 🎆🔊👟🎛️ Those who preferred dance music were typically outgoing, assertive, and open to experience but ranked lower than others in gentleness." << endl;
       cout << endl;
   } else if (averageScore == 5){
       cout << "You are Rock/Heavy Metal lover. 🤘💀⛓️🔥 Rock and heavy metal often project images of anger, bravado, and aggression." << endl;
       cout << "However, this study found such fans to be gentle, creative, and introverted." << endl;
       cout << "They also tended to have low self-esteem." << endl;
       cout << endl;
   } else if (averageScore == 6){
       cout << "You are Indie lover. 📻🌿🎞️🎸 Fans of the indie genre registered as introverted, intellectual, and creative, but less hardworking and gentle than fans of other styles. " << endl;
       cout << "Passivity, anxiousness, and low self-esteem were other notable personality characteristics." << endl;
       cout << endl;
   } else if (averageScore == 7){
       cout << "You are Country lover. 🤠🪕🎸🌾 You are identified as hardworking, conventional, outgoing, and conservative." << endl;
       cout << "Although country music frequently centers on heartbreak, people who prefer it tended to be emotionally stable. They also ranked lower than others in openness to experience." << endl;
       cout << endl;
   } else if (averageScore == 8){
       cout << "You are Jazz/Blues lover. 🎷🎺🎹🥃 Extroverted with high self-esteem. They also tend to be very creative, intelligent, and at ease." << endl;
       cout << endl;
   } else if (averageScore == 9){
       cout << "You are Afrobeats/Caribbean lover. 🥁🌍💃🔥 " << endl;
       cout << endl;
   } else if (averageScore == 10){
       cout << "You are Classical lover. 🎻🎼🏛️🕯️ The study shows that classical music lovers were generally somewhat introverted but at ease with themselves." << endl;
       cout << "Creativity and healthy self-esteem were common among them." << endl;
       cout << endl;
   }
}

// link 3 (check end of code)
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING} GameScreen;

int main() {
   // annoucement in terminal
   cout << endl;
   cout << "ATTENTION: Read these directions before proceeding to the port. Select 4 songs that will appear on the display. Be sure to click the name of the song you want selected!!! Once a song is selected is CANNOT be replaced. Upon selection of the song, you will be redirected to another screen. Please proceed to the port. Thank you! :)";
   cout << endl;

   // tracks which screen user is on
   // want one song per screen
   int currentScreen = 0;

   // creating 2 vectors. one for all the classifying details of the song
   // another for the songs the user will pick
   int limit = 10;
   vector<Song> songInfo;
   vector<Song> selectedSongs;

    for (int offset = 0; offset < 100; offset += limit) {
        vector<Song> page = getSongs(limit, offset);
        songInfo.insert(songInfo.end(), page.begin(), page.end());

        // wait one second to match MusicBrainz's 1 request/second rate limit
        sleep(1);
    }
  
   InitWindow(1300, 800, "Spotify Playlist");
   SetTargetFPS(30);

   // header spotify picture
   Texture2D photo = LoadTexture("spotifypicture.jpeg");
    if (photo.id == 0) {
        CloseWindow();
        return -1;
    }

   // will be updated later in code
   Texture2D finalScreen = {};
   bool finalScreenDisplay = false;

   // home screen start button
   Rectangle startButton = {450, 600, 400, 80};


    while (!WindowShouldClose()) {
        // adding the selected songs the user picked to a vector
        // link 2 (check end of code)
        if (currentScreen >= 1 && currentScreen <= 10) {
           for (int i = 0; i < 10; ++i) {
                // creating "invisible rectangles" for the song to be added to a new vector when the user clicks the song name
                Rectangle songArea = {110, (340 + (i * 40.0f)), 200, 30};


                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), songArea) && selectedSongs.size() < 10) {
                    selectedSongs.push_back(songInfo.at(i));
                    cout << "Song: " << songInfo.at(i).getName() << endl;
                    
                    // Accessed 11/24/26 10:40 pm https://www.geeksforgeeks.org/cpp/vector-erase-in-cpp-stl/
                    songInfo.erase(songInfo.begin(), songInfo.begin() + 10);
                    
                    currentScreen++;
                    break;
                }
            }   
        }
      
  
       BeginDrawing();
       ClearBackground(BLACK);

        // menu screen
        if (currentScreen == 0) {
           string startMessage1 = "Hello! Welcome to our code! :)";
           string directions = "Directions:";
           string direction1 = "1) You will be directed to a Spotify Screen. Please select your favorite song.";
           string direction2 = "2) There will be 10 screens in total to choose one song from each.";
           string direction3 = "3) Then, you will be sent to a personality analysis based on your choices";
           string direction4 = "4) Click ''Start Game'' to continue.";

           DrawText(startMessage1.c_str(), 70, 100, 70, LIME);
           DrawText(directions.c_str(), 70, 230, 50, LIME);
           DrawText(direction1.c_str(), 70, 320, 30, WHITE);
           DrawText(direction2.c_str(), 70, 380, 30, WHITE);
           DrawText(direction3.c_str(), 70, 440, 30, WHITE);
           DrawText(direction4.c_str(), 70, 500, 30, WHITE);
      
           string sb = "Start Game";
           DrawRectangleRec(startButton, LIME);
           DrawText(sb.c_str(), 510, 625, 40, BLACK);

           // once clicked increments current screen, which is the spotify screen
           if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), startButton)) {
               currentScreen++;
           }
       }
       //spotify screens
       else if (currentScreen >= 1 && currentScreen <= 10) {
           Rectangle source = { 0, 0, (float)photo.width, (float)photo.height };
           Rectangle dest   = { 0, 0, 1300, 200 };

           // header picture
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
           for (int i = 0; i < 10; ++i) {
               DrawText(to_string(i + 1).c_str(), 40, 340 + (i * 40), 20, WHITE);
               DrawText(songInfo.at(i).getName().c_str(), 110, 340 + (i * 40), 20, WHITE);
               DrawText(songInfo.at(i).getAlbum().c_str(), 400, 340 + (i * 40), 20, WHITE);
               DrawText(songInfo.at(i).getGenre().c_str(), 770, 340 + (i * 40), 20, WHITE);
               DrawText(songInfo.at(i).getDuration().c_str(), 1110, 340 + (i * 40), 20, WHITE);
           }
       }
       // personality analysis screen
       else {
        // makes sure loads personality analysis image once  
            if (finalScreenDisplay == false) {
               int averageScore = CalculatingScore(selectedSongs);
               getResult(averageScore);

               // based on calculation assigns personality
               if (averageScore == 1) {
                   finalScreen = LoadTexture("pop.jpg");
               }
               else if (averageScore == 2) {
                   finalScreen = LoadTexture("rap.jpg");
               }
               else if (averageScore == 3) {
                   finalScreen = LoadTexture("randb.jpg");
               }
               else if (averageScore == 4) {
                   finalScreen = LoadTexture("dance.jpg");
               }
               else if (averageScore == 5) {
                   finalScreen = LoadTexture("rock.jpg");
               }
               else if (averageScore == 6) {
                   finalScreen = LoadTexture("indie.jpg");
               }
               else if (averageScore == 7) {
                   finalScreen = LoadTexture("country.jpg");
               }
               else if (averageScore == 8) {
                   finalScreen = LoadTexture("jazz.jpg");
               }
               else if (averageScore == 9) {
                   finalScreen = LoadTexture("afrobeats.jpg");
               }
               else if (averageScore == 10) {
                   finalScreen = LoadTexture("classical.jpg");
               }
               else {
                   finalScreen = LoadTexture("none.jpg");
               }


               finalScreenDisplay = true;
            }
        
            // based on score, gives produces associated final screen
           Rectangle source = { 0, 0, (float)finalScreen.width, (float)finalScreen.height };
           Rectangle dest   = { 0, 0, 1300, 800 };
           DrawTexturePro(finalScreen, source, dest, {0, 0}, 0.0f, WHITE);
        }
      
       EndDrawing();
    }

   UnloadTexture(photo);
   UnloadTexture(finalScreen);

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

*/