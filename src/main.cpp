#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring> 
#include <SFML/Graphics.hpp>

#include "config.h"
using namespace std;

//Should probably go in header
void redrawMap(int start, int wave);

const sf::Vector2f WINDOW_SIZE(500.0f, 500.0f); //Predetermined window size

sf::Texture backgroundTexture; //Variable to hold background texture
sf::Sprite backgroundSprite;
sf::RenderWindow gameWindow; //The main game window

sf::Texture tzKihTexture;
sf::Texture tzKekTexture;
sf::Texture tokXilTexture;
sf::Texture YtMejKotTexture;
sf::Texture ketZekTexture;
sf::Texture tzTokJadTexture;

sf::Sprite tzKihSprite;
sf::Sprite tzKekSprite;
sf::Sprite tokXilSprite;
sf::Sprite YtMejKotSprite;
sf::Sprite ketZekSprite;
sf::Sprite tzTokJadSprite;
 
static bool sfmlSetup(int start, int wave) {
  
    //Load in required resources
    if (!backgroundTexture.loadFromFile("../images/Fight_Caves_Map.png")) {
            std::cerr << "Could not load background texture" << std::endl;
            return false;
    } else if (!tzKihTexture.loadFromFile("../images/Tz-Kih.png")) {
            std::cerr << "Could not load tzKihTexture texture" << std::endl;
            return false;
    } else if (!tzKekTexture.loadFromFile("../images/Tz-Kek.png")) {
            std::cerr << "Could not load Tz-Kek texture" << std::endl;
            return false;
    } else if (!tokXilTexture.loadFromFile("../images/Tok-Xil.png")) {
            std::cerr << "Could not load Tok-Xil texture" << std::endl;
            return false;
    } else if (!YtMejKotTexture.loadFromFile("../images/Yt-MejKot.png")) {
            std::cerr << "Could not load Yt-MejKot texture" << std::endl;
            return false;
    } else if (!ketZekTexture.loadFromFile("../images/Ket-Zek.png")) {
            std::cerr << "Could not load ketZekTexture texture" << std::endl;
            return false;
    } else if (!tzTokJadTexture.loadFromFile("../images/TzTok-Jad.png")) {
            std::cerr << "Could not load tzTokJadTexture texture" << std::endl;
            return false;
    }
    
    //Load Monster sprites here too
    
    backgroundSprite.setTexture(backgroundTexture);
    tzKihSprite.setTexture(tzKihTexture);
    tzKekSprite.setTexture(tzKekTexture);
    tokXilSprite.setTexture(tokXilTexture);
    YtMejKotSprite.setTexture(YtMejKotTexture);
    ketZekSprite.setTexture(ketZekTexture);
    tzTokJadSprite.setTexture(tzTokJadTexture);
    
    backgroundSprite.setScale( //Scale the background sprite to fit the current window size
            WINDOW_SIZE.x / backgroundSprite.getLocalBounds().width,
            WINDOW_SIZE.y / backgroundSprite.getLocalBounds().height);

    gameWindow.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Fight Caves Spawn Map", sf::Style::Titlebar | sf::Style::Close); //Create SFML Window
    
    redrawMap(start, wave);
    return gameWindow.isOpen(); //Only returns true if the game window was successfully created
}

int printspawn (int wave) {
  if (wave == 63) { printf("JAD "); return 0; }
  if (wave >= 31) { printf("360 "); return wave-31; }
  if (wave >= 15) { printf("180 "); return wave-15; }
  if (wave >= 7) { printf("90 "); return wave-7; }
  if (wave >= 3) { printf("45 "); return wave-3; }
  printf("22 ");
  return wave-1;
}
 
void getspawn (int loc) {
  int spawnarr[15] = {3, 5, 2, 1, 5, 3, 4, 1, 2, 3, 5, 4, 1, 2, 4};
  int sval = spawnarr[loc];
  if (sval == 1) printf("NW; ");
  if (sval == 2) printf("C; ");
  if (sval == 3) printf("SE; ");
  if (sval == 4) printf("S; ");
  if (sval == 5) printf("SW; ");
  return;
}
 
void printwave (int start, int wave) {
  int i = 0;
  int curwave = wave;
  printf("Wave %d: ", wave);
  while (curwave > 0) {
    curwave = printspawn(curwave);
    getspawn((start+i)%15);
    i++;
  }
  printf("\n");
  return;
}
 
int spawn2int(char spn[3]) {
  if (strcmp(spn,"NW")==0) return 1;
  if (strcmp(spn,"C")==0) return 2;
  if (strcmp(spn,"SE")==0) return 3;
  if (strcmp(spn,"S")==0) return 4;
  if (strcmp(spn,"SW")==0) return 5;
  printf("ERROR");
  exit(-1);
  return 0;
}
 
int spawnarr2start (int spawnarr[4]) {
  int spawns[15] = {3, 5, 2, 1, 5, 3, 4, 1, 2, 3, 5, 4, 1, 2, 4};
  int i = 0, j, k;
  while (i < 15) {
    j = 0, k = 0;
    while ((j < 4) && (k == 0)) {
      if (spawns[(i+j)%15] != spawnarr[j]) k=1;
      j++;
    }
    if (k == 0) return i;
    i++;
  }
  printf("Spawns aren't accurate!");
  exit(-1);
}

void redrawMap(int start, int wave) {
    printwave(start,wave);
}

int main() {
    int wave = 1, start = 0;
    int startspawns[4];
    char s1[3], s2[3];

    printf("Enter spawns as NW, C, SE, S, or SW. \nFirst 22 spawn: ");
    scanf("%s", s1);
    startspawns[0] = spawn2int(s1);
    printf("Second two 22 spawns, separated by a space: ");
    scanf("%s %s", s1, s2);
    startspawns[1] = spawn2int(s1);
    startspawns[2] = spawn2int(s2);
    printf("First 45 spawn: ");
    scanf("%s", s1);
    start = spawn2int(s1);
    
    if (start != startspawns[2]) {
      startspawns[1] = startspawns[2];
      startspawns[2] = start;
    }
    
    printf("Second 45 spawn: ");
    scanf("%s", s1);
    startspawns[3] = spawn2int(s1);
    start = spawnarr2start(startspawns);
    printf("\n");

    if (sfmlSetup(start, wave)) {
        while (gameWindow.isOpen()) {
            sf::Event event;
            
            
            while (gameWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                            gameWindow.close();
                    
                    if (event.type == sf::Event::KeyReleased)
                    {
                        if (event.key.code == sf::Keyboard::Right)
                        {
                          if(wave<63){
                            wave++;
                            start++;
                            redrawMap(start, wave);
                          }
                        }
                        if (event.key.code == sf::Keyboard::Left)
                        {
                          if(wave>1){
                            wave--;
                            start--;
                            redrawMap(start, wave);
                          }
                        }
                    }
            }

            gameWindow.clear(); //Clear and redraw the screen
            gameWindow.draw(backgroundSprite);
            //gameWindow.draw(text);
            gameWindow.display();
        }
    }
    
    return 0;
}

/*
First 22 spawn: SE
Second two 22 spawns, separated by a space: S NW
First 45 spawn: NW
Second 45 spawn: C
 */
