#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>  //Libreria que controla imagenes

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";
	//-------
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		// --- Animated Sprite ---

	// --- TEXT ---

	// --- AUDIO ---

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			default:;
			}
		}

		// UPDATE

		// DRAW
			//Background
		SDL_RenderClear(renderer);//limpia pantalla SIEMPRE AL PRINCIPIO DEL DRAW
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect); // aqui dibujamos la textura usando un rectangulo - IMPORTANTE -
		SDL_RenderPresent(renderer);
			//Animated Sprite
		SDL_RenderPresent(renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);//destruye el bg
	SDL_DestroyRenderer(renderer);//primero destruye al render por que esta referenciado por el window
	SDL_DestroyWindow(window);

	// --- QUIT ---
	SDL_Quit();
	IMG_Quit(); //Quit de las imagenes
	return 0;
}