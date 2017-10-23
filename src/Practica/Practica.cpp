#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>


//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

bool movL, movR, movTop, movBot = false;
int posX, posY;
int posX1, posY1;
int posX2, posY2;
int posX3, posY3;
int posX4, posY4;
int posX5, posY5;

int puntuasao = 0;

int main(int, char*[]) {
	srand(time(NULL));

	posX = 1 + rand() % (768 - 1);
	posY = 147 + rand() % (563 - 144);

	posX1 = 1 + rand() % (768 - 1);
	posY1 = 147 + rand() % (563 - 144);

	posX2 = 1 + rand() % (768 - 1);
	posY2 = 147 + rand() % (563 - 144);

	posX3 = 1 + rand() % (768 - 1);
	posY3 = 147 + rand() % (563 - 144);

	posX4 = 1 + rand() % (768 - 1);
	posY4 = 147 + rand() % (563 - 144);

	posX5 = 1 + rand() % (768 - 1);
	posY5 = 147 + rand() % (563 - 144);


	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";
	//---BG
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";//inicializo al bg

																		// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("Castle Stealers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";
	//Text
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_TTF";//texto init
																// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	//BackGround
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/img/bgCastle.jpg") };
	if (bgTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	//Monedillas
	SDL_Texture *moneda{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda{ posX,posY,50,50 };

	SDL_Texture *moneda1{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda1 == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda1{ posX1,posY1,50,50 };

	SDL_Texture *moneda2{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda2 == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda2{ posX2,posY2,50,50 };

	SDL_Texture *moneda3{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda3 == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda3{ posX3,posY3,50,50 };

	SDL_Texture *moneda4{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda4 == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda4{ posX4,posY4,50,50 };

	SDL_Texture *moneda5{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	if (moneda5 == nullptr) throw "No s'han pogut crear les monedes";
	SDL_Rect rectMoneda5{ posX5,posY5,50,50 };

	// --- Animated Sprite ---
	//Player
	SDL_Texture *playerTexture1{ IMG_LoadTexture(renderer, "../../res/img/spCastle.png") };//textura cargada
	SDL_Rect playerRect1, playerPosition1;//rectangulos
	int textWidth, textHeight, frameWidth, frameHeight;//tamaños
SDL:SDL_QueryTexture(playerTexture1, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 12;
	frameHeight = textHeight / 8;
	playerPosition1.x = playerPosition1.y = SCREEN_WIDTH / 2;
	playerRect1.x = playerRect1.y = 0;
	playerPosition1.h = playerRect1.h = frameHeight;
	playerPosition1.w = playerRect1.w = frameWidth;
	int frameTime = 0;

	// --- TEXT ---
	//Puntos
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 40) };
	if (font == nullptr) throw "No es pot initzializar";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My Points : ", SDL_Color{ 100,0,255,255 }) };
	//SDL_Surface *tmpSurf1{ TTF_RenderText_Blended(font, ToString(puntuasao), SDL_Color{ 100,0,255,255 }) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "Unable to creat the SDL SURFACE";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 0,0,tmpSurf->w, tmpSurf->h };//witdh and height 
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);
	// --- AUDIO ---


	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false; break;
				}
				if (event.key.keysym.sym == SDLK_RIGHT && playerPosition1.x <= 770) {
					playerPosition1.x += 4;
					std::cout << "Posicion " << playerPosition1.x << " : " << playerPosition1.y << std::endl;
					movR = true;
					movL = false;
					movBot = false;
					movTop = false;
				}
				if (event.key.keysym.sym == SDLK_LEFT && playerPosition1.x >= 0) {
					playerPosition1.x -= 4;
					std::cout << "Posicion " << playerPosition1.x << " : " << playerPosition1.y << std::endl;
					movR = false;
					movL = true;
					movBot = false;
					movTop = false;

				}
				if (event.key.keysym.sym == SDLK_UP  && playerPosition1.y >= 148) {
					playerPosition1.y -= 4;
					std::cout << "Posicion " << playerPosition1.x << " : " << playerPosition1.y << std::endl;
					movBot = false;
					movL = false;
					movR = false;
					movTop = true;
				}
				if (event.key.keysym.sym == SDLK_DOWN && playerPosition1.y <= 568) {
					playerPosition1.y += 4;
					std::cout << "Posicion " << playerPosition1.x << " : " << playerPosition1.y << std::endl;
					movBot = true;
					movL = false;
					movR = false;
					movTop = false;

				}
			default:;
			}
		}
		// UPDATE
		if (movR) {
			frameTime++;
			if (FPS / frameTime <= 10) {
				frameTime = 0;
				playerRect1.x += frameWidth;
				if (playerRect1.x >= textWidth / 4) {
					playerRect1.y = frameHeight * 2;
					playerRect1.x = 0;
				}
			}
		}
		if (movL) {
			frameTime++;
			if (FPS / frameTime <= 10) {
				frameTime = 0;
				playerRect1.x += frameWidth;
				if (playerRect1.x >= textWidth / 4) {
					playerRect1.y = frameHeight * 1;
					playerRect1.x = 0;
				}
			}
		}
		if (movBot) {
			frameTime++;
			playerRect1.y = frameHeight % 2;
			if (FPS / frameTime <= 10) {
				frameTime = 0;
				playerRect1.x += frameWidth;
				if (playerRect1.x >= textWidth / 4) {
					playerRect1.x = 0;
				}
			}
		}
		if (movTop) {
			frameTime++;
			if (FPS / frameTime <= 10) {
				frameTime = 0;
				playerRect1.x += frameWidth;
				if (playerRect1.x >= textWidth / 4) {
					playerRect1.y = frameHeight * 3;
					playerRect1.x = 0;
				}
			}
		}

		if (playerPosition1.x >= rectMoneda.x && playerPosition1.x <= rectMoneda.x + 20 && playerPosition1.y >= rectMoneda.y && playerPosition1.y <= rectMoneda.y + 20) {
			SDL_DestroyTexture(moneda);
		}
		if (playerPosition1.x >= rectMoneda1.x && playerPosition1.x <= rectMoneda1.x + 20 && playerPosition1.y >= rectMoneda1.y && playerPosition1.y <= rectMoneda1.y + 20) {
			SDL_DestroyTexture(moneda1);
		}
		if (playerPosition1.x >= rectMoneda2.x && playerPosition1.x <= rectMoneda2.x + 20 && playerPosition1.y >= rectMoneda2.y && playerPosition1.y <= rectMoneda2.y + 20) {
			SDL_DestroyTexture(moneda2);
		}
		if (playerPosition1.x >= rectMoneda3.x && playerPosition1.x <= rectMoneda3.x + 20 && playerPosition1.y >= rectMoneda3.y && playerPosition1.y <= rectMoneda3.y + 20) {
			SDL_DestroyTexture(moneda3);
		}
		if (playerPosition1.x >= rectMoneda4.x && playerPosition1.x <= rectMoneda4.x + 20 && playerPosition1.y >= rectMoneda4.y && playerPosition1.y <= rectMoneda4.y + 20) {
			SDL_DestroyTexture(moneda4);
		}
		if (playerPosition1.x >= rectMoneda5.x && playerPosition1.x <= rectMoneda5.x + 20 && playerPosition1.y >= rectMoneda5.y && playerPosition1.y <= rectMoneda5.y + 20) {
			SDL_DestroyTexture(moneda5);
		}

		// DRAW
		//Background
		SDL_RenderClear(renderer);//limpia pantalla SIEMPRE AL PRINCIPIO DEL DRAW
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect); // aqui dibujamos la textura usando un rectangulo - IMPORTANTE -
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderCopy(renderer, moneda, nullptr, &rectMoneda);
		SDL_RenderCopy(renderer, moneda1, nullptr, &rectMoneda1);
		SDL_RenderCopy(renderer, moneda2, nullptr, &rectMoneda2);
		SDL_RenderCopy(renderer, moneda3, nullptr, &rectMoneda3);
		SDL_RenderCopy(renderer, moneda4, nullptr, &rectMoneda4);
		SDL_RenderCopy(renderer, moneda5, nullptr, &rectMoneda5);
		//Animated Sprite
		SDL_RenderCopy(renderer, playerTexture1, &playerRect1, &playerPosition1);
		SDL_RenderPresent(renderer);//SOLO 1

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);//destruye el bg
	SDL_DestroyTexture(textTexture);//destruyo el texto
	SDL_DestroyTexture(moneda);
	SDL_DestroyTexture(moneda1);
	SDL_DestroyTexture(moneda2);
	SDL_DestroyTexture(moneda3);
	SDL_DestroyTexture(moneda4);
	SDL_DestroyTexture(moneda5);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// --- QUIT ---
	IMG_Quit(); //Quit de las imagenes
	TTF_Quit(); //Quit del texto
	SDL_Quit();
	return 0;
}

