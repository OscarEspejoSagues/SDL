#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>  //Libreria que controla imagenes
#include <SDL_ttf.h>	//libreria de texto
#include <SDL_mixer.h>  //libreria de musica
#include <iostream>
#include <time.h>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define	FPS 60 //definimos los frame por segundo

//Scene Manatgement
/*
	controlar el delta time para que las clases no se chafen entre ellas
	tambien deberian conocer al render ya que este conoce al window y le permita mostrar en pantalla
	
	SCENE MANAGER
	enum class GameState{Play, Exit, Menu};

	//Game Loop
		SDL_event event;
		GameState gamestat = GameState::Menu;

		while(gamestat != GameState::Exit){
			while(SDL_PollEvent(&event)){
				switch(event.type){
					case SDL_QUIT:
						gamestate = GameState::Exit;
						break;
						
						case GameState::Play:
							//EventHandlePlay();
							//UpdatePlay();
							//DrawPlay();
							break;
						case GameState::Exit:
							break;
						case GameState::Menu:
							//EventHandleMenu();
							//UpdateMenu();
							//DrawMenu();
							break;
				}
			}
		}



*/


/*2 personajes animados con movimiento, sacos que recoger, no pasar del horizonte*/



int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";
	//-------BG
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";//image init
	//-------TEXT
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_TTF";//texto init
	//-------MUSICA
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags)& mixFlags)) throw "Eroor: SDL:MIXER INIT";

	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	//Bg
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	//Player
	/*SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect playerRect{ 0,0,350,189 };*/
	SDL_Rect playerTarget{ 0,0,100,100 };//player target para hacerlo mas fino el movimiento
	
	// --- Animated Sprite ---
	SDL_Texture *playerTexture1{ IMG_LoadTexture(renderer, "../../res/img/sp01.png") };//textura cargada
	SDL_Rect playerRect1, playerPosition1;//rectangulos
	int textWidth, textHeight, frameWidth, frameHeight;//tama�os
	SDL:SDL_QueryTexture(playerTexture1, NULL, NULL, &textWidth, &textHeight);//modifica la textWidth y la textHeight al tama�o del sprite del player y le asigna el valor del tama�o
	frameWidth = textWidth / 6;//el tama�o del frame de la animacion es el tama�o total / 6 ya que son 6 movimientos
	frameHeight = textHeight / 1;//exactamente lo mismo pero con el largo
	playerPosition1.x = playerPosition1.y = 0;
	playerRect1.x = playerRect1.y = 0;
	playerPosition1.h = playerRect1.h = frameHeight;
	playerPosition1.w = playerRect1.w = frameWidth;
	int frameTime = 0;


	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot initzializar";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first sdl Game", SDL_Color{155,0,255,255}) };
	SDL_Surface *tmpSurf1{ TTF_RenderText_Blended(font, "Play Music", SDL_Color{ 0,155,255,255 }) };
	SDL_Surface *tmpSurf2{ TTF_RenderText_Blended(font, "Stop Music", SDL_Color{ 0,155,255,255 }) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "Unable to creat the SDL SURFACE";
	if (tmpSurf1 == nullptr) TTF_CloseFont(font), throw "Unable to creat the SDL SURFACE";
	if (tmpSurf2 == nullptr) TTF_CloseFont(font), throw "Unable to creat the SDL SURFACE";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Texture *textTexture1{ SDL_CreateTextureFromSurface(renderer, tmpSurf1) };
	SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(renderer, tmpSurf2) };
	SDL_Rect textRect{ 100,50,tmpSurf->w, tmpSurf->h };//witdh and height 
	SDL_Rect textRect1{ 100,200,tmpSurf1->w, tmpSurf1->h };//witdh and height 
	SDL_Rect textRect2{ 100,300,tmpSurf2->w, tmpSurf2->h };//witdh and height 
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);

	// --- AUDIO ---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {// 1 = MONO 2=STEREO
		throw "Unable to initialize the muscic";
	}
	Mix_Music *sountrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!sountrack)throw "Unable to load";
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_PlayMusic(sountrack, -1);
	//FUNCIONES WORTH Mix_PlayingMusic(), Mix_PauseMusic(), Mix_PauseMusic(), Mix_HaltMusic()
	
	//-----TIME------
	clock_t lastTime = clock();
	float timeDown = 10;
	float deltaTime = 0;

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			case SDL_MOUSEMOTION:
				//playerTarget.x = event.motion.x - playerRect.w/2; //asignamos las coordenadas pero aplicamos el cambio en el update
				//playerTarget.y = event.motion.y - playerRect.h/2; break;//evento de movimiento del player
				default:;
			}
		}

		// UPDATE aqui va la logica del codigo
		/*playerRect.x += (playerTarget.x - playerRect.x) / 10;
		playerRect.y += (playerTarget.y - playerRect.y) / 10;*/

		deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;
		std::cout << timeDown << std::endl;

		frameTime++;
		if (FPS / frameTime <= 9) {
			frameTime = 0;
			playerRect1.x += frameWidth;
			if (playerRect1.x >= textWidth) {
				playerRect1.x = 0;
			}
		}
		

		// DRAW
			//Background
		SDL_RenderClear(renderer);//limpia pantalla SIEMPRE AL PRINCIPIO DEL DRAW
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect); // aqui dibujamos la textura usando un rectangulo - IMPORTANTE -
		//SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);//dibujamos la textura del nuvol quinton
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderCopy(renderer, textTexture1, nullptr, &textRect1);
		SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);

			//Animated Sprite
		SDL_RenderCopy(renderer, playerTexture1, &playerRect1, &playerPosition1);
		SDL_RenderPresent(renderer);//SOLO 1

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);//destruye el bg
	//SDL_DestroyTexture(playerTexture);//destuye al player
	SDL_DestroyTexture(textTexture);//destruyo el texto
	Mix_CloseAudio(); //cierra la musica no se destruye
	SDL_DestroyRenderer(renderer);//primero destruye al render por que esta referenciado por el window
	SDL_DestroyWindow(window);

	// --- QUIT ---
	IMG_Quit(); //Quit de las imagenes
	TTF_Quit(); //Quit del texto
	Mix_Quit(); //quit de la musica
	SDL_Quit();//SIEMPRE VA ULTIMO
	return 0;
}