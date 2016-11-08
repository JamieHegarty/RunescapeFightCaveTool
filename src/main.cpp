#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring> 

#include <SFML/Graphics.hpp>

/* TODO 
	- Tidy up functions (maybe seperate out into a header file)
	- Make input more robust
	- Stop monsters stacking if they have the same spawn
	- Determine potential player safespots
	- Add nicer fields for user input
	- Add more comments
*/


//#include "config.h"
using namespace std;

//Should probably go in header
//Maybe move all these variables to a header also
void redrawMap(int start, int wave);

//Make these width and height into variables to elimintate magic numbers
const sf::Vector2f WINDOW_SIZE(500.0f, 500.0f); //Predetermined window size

sf::Texture backgroundTexture; //Variable to hold background texture
sf::Sprite backgroundSprite;
sf::Text waveText;
sf::Font mainFont;

sf::RenderWindow gameWindow; //The main game window
vector<sf::Sprite> gameSprites;

sf::Texture tzKihTexture;
sf::Texture tzKekTexture;
sf::Texture tokXilTexture;
sf::Texture YtMejKotTexture;
sf::Texture ketZekTexture;
sf::Texture tzTokJadTexture;

int wave = 1, start = 0; //Can this be moved or tidied up?

static bool loadTextures() {    
    if (!backgroundTexture.loadFromFile("../../images/Fight_Caves_Map.png")) {
            std::cerr << "Could not load background texture" << std::endl;
            return false;
    } else if (!tzKihTexture.loadFromFile("../../images/Tz-Kih.png")) {
            std::cerr << "Could not load tzKihTexture texture" << std::endl;
            return false;
    } else if (!tzKekTexture.loadFromFile("../../images/Tz-Kek.png")) {
            std::cerr << "Could not load Tz-Kek texture" << std::endl;
            return false;
    } else if (!tokXilTexture.loadFromFile("../../images/Tok-Xil.png")) {
            std::cerr << "Could not load Tok-Xil texture" << std::endl;
            return false;
    } else if (!YtMejKotTexture.loadFromFile("../../images/Yt-MejKot.png")) {
            std::cerr << "Could not load Yt-MejKot texture" << std::endl;
            return false;
    } else if (!ketZekTexture.loadFromFile("../../images/Ket-Zek.png")) {
            std::cerr << "Could not load ketZekTexture texture" << std::endl;
            return false;
    } else if (!tzTokJadTexture.loadFromFile("../../images/TzTok-Jad.png")) {
            std::cerr << "Could not load tzTokJadTexture texture" << std::endl;
            return false;
    }else if (!mainFont.loadFromFile("../../fonts/Share-TechMono.ttf")) {
		std::cerr << "Could not load main font" << std::endl;
		return false;
	}

    return true;
}

static bool sfmlSetup() {
 
    loadTextures();

    backgroundSprite.setTexture(backgroundTexture);

    waveText.setFont(mainFont);
    waveText.setCharacterSize(24); //Magic number? Move to header?
    waveText.setColor(sf::Color::Cyan);
    waveText.setPosition(225, 465); //Same as above, magic number?
    
    backgroundSprite.setScale( //Scale the background sprite to fit the current window size
              WINDOW_SIZE.x / backgroundSprite.getLocalBounds().width,
              WINDOW_SIZE.y / backgroundSprite.getLocalBounds().height);

    //Change this raw string to a variable in header maybe?
    gameWindow.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Fight Caves Spawn Map", sf::Style::Titlebar | sf::Style::Close); //Create SFML Window
    
    redrawMap(start, wave); //Does redraw map need to be in here?
    return gameWindow.isOpen(); //Only returns true if the game window was successfully created
}

//There might be a better way to do this, worth brain storming
int printspawn (int wave) {
  if (wave == 63) { 
	  printf("JAD ");
	  sf::Sprite tzTokJadSprite(tzTokJadTexture);
	  gameSprites.push_back(tzTokJadSprite);

	  return 0; 
  }
  if (wave >= 31) { 
	  printf("360 "); 
	  sf::Sprite ketZekSprite(ketZekTexture);
	  gameSprites.push_back(ketZekSprite);

	  return wave-31; 
  }
  if (wave >= 15) { 
	  printf("180 ");
	  sf::Sprite YtMejKotSprite(YtMejKotTexture);
	  gameSprites.push_back(YtMejKotSprite);

	  return wave - 15;
  }
  if (wave >= 7) {
	  printf("90 "); 
	  sf::Sprite tokXilSprite(tokXilTexture);
	  gameSprites.push_back(tokXilSprite);

	  return wave-7; 
  }
  if (wave >= 3) {
	  printf("45 ");
	  sf::Sprite tzKekSprite(tzKekTexture);
	  gameSprites.push_back(tzKekSprite);

	  return wave-3; 
  }

  printf("22 ");
  sf::Sprite tzKihSprite(tzKihTexture);
  gameSprites.push_back(tzKihSprite);

  return wave-1;
}
 
/* 
   Maybe make a struct or similar for each position? Could prob combine it 
   with the other postional enum, this one basically translate an int value
   to an object that has the text e.g. "NW" and the co-ordinates to draw the 
   monster 
*/

void getSpawn (int loc) {
  int spawnarr[15] = {3, 5, 2, 1, 5, 3, 4, 1, 2, 3, 5, 4, 1, 2, 4};
  int sval = spawnarr[loc];

  if (sval == 1) {
	  gameSprites.back().setPosition(85, 85);
	  printf("NW; ");
  }
  if (sval == 2) {
	  gameSprites.back().setPosition(220, 210);
	  printf("C; ");
  }
  if (sval == 3) {
	  gameSprites.back().setPosition(390, 270);
	  printf("SE; ");
  }
  if (sval == 4) {
	  gameSprites.back().setPosition(260, 350);
	  printf("S; ");
  }
  if (sval == 5) {
	  gameSprites.back().setPosition(85, 345);
	  printf("SW; ");
  }
  return;
}
 
void printwave (int start, int wave) {
  int i = 0;
  int currentWave = wave;
  printf("Wave %d: ", wave);
  while (currentWave > 0) {
    currentWave = printspawn(currentWave);
    getSpawn((start+i)%15);
    i++;
  }
  printf("\n");
  return;
}

//Can prob make these enums and rid this function completely
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
	gameSprites.clear();
	std::string curWaveText("Wave:" + std::to_string(wave));
	waveText.setString(curWaveText);

    printwave(start,wave);

	gameWindow.clear(); //Clear and redraw the screen
	gameWindow.draw(backgroundSprite);

	for (sf::Sprite sprite : gameSprites) {
		gameWindow.draw(sprite);
	}

	gameWindow.draw(waveText);
	gameWindow.display();
}

/* Function Work in Progress */
void getUserInput() {
    int startspawns[4];
    char spawnInput_1[3], spawnInput_2[3]; //Give these meaningful names

    printf("Enter spawns as NW, C, SE, S, or SW. \nFirst 22 spawn: ");
    scanf("%s", spawnInput_1); //Change this from scanf to something more C++ / Robust
    startspawns[0] = spawn2int(spawnInput_1);
    printf("Second two 22 spawns, separated by a space: ");
    scanf("%s %s", spawnInput_1, spawnInput_2);
    startspawns[1] = spawn2int(spawnInput_1);
    startspawns[2] = spawn2int(spawnInput_2);
    printf("First 45 spawn: ");
    scanf("%s", spawnInput_1);
    start = spawn2int(spawnInput_1);
    
    if (start != startspawns[2]) {
      startspawns[1] = startspawns[2];
      startspawns[2] = start;
    }
    
    printf("Second 45 spawn: ");
    scanf("%s", s1);
    startspawns[3] = spawn2int(s1);
    start = spawnarr2start(startspawns);
    printf("\n");
}

int main() {

    getUserInput();
    
    if (sfmlSetup()) {
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
