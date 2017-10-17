#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>  //Libreria que controla imagenes
#include <SDL_ttf.h>	//libreria de texto
#include <SDL_mixer.h>  //libreria de musica

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";
	//-------
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";//image init
	//-------
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_TTF";//texto init
	//-------
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
	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect playerRect{ 0,0,350,189 };
	SDL_Rect playerTarget{ 0,0,100,100 };//player target para hacerlo mas fino el movimiento
		// --- Animated Sprite ---

	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot initzializar";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first sdl Game", SDL_Color{155,0,255,255}) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "Unable to creat the SDL SURFACE";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 100,50,tmpSurf->w, tmpSurf->h };//witdh and height 
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);

	// --- AUDIO ---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		throw "Unable to initialize the muscic";
	}
	Mix_Music *sountrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!sountrack)throw "Unable to load";
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_PlayMusic(sountrack, -1);


	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			case SDL_MOUSEMOTION:playerRect.x = event.motion.x-50; playerRect.y = event.motion.y-50; break;//evento de movimiento del player
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x) / 10;
		playerRect.y += (playerTarget.y - playerRect.y) / 10;

		// DRAW
			//Background
		SDL_RenderClear(renderer);//limpia pantalla SIEMPRE AL PRINCIPIO DEL DRAW
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect); // aqui dibujamos la textura usando un rectangulo - IMPORTANTE -
		SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);//dibujamos la textura del nuvol quinton
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderPresent(renderer);//solo 1
			//Animated Sprite
		SDL_RenderPresent(renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);//destruye el bg
	SDL_DestroyTexture(playerTexture);//destuye al player
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