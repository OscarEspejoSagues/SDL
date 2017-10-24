#pragma once
#include<SDL.h>

/*
* Header que contiene las funciones para generar las colisiones
* entre un objeto y otro
*/

namespace Colisiones{
	bool Colision(SDL_Rect origen, SDL_Rect target) {
		if (origen.h == target.h || origen.w == target.w || origen.x == target.x || origen.y == target.y){
			return true;
		}
		else {
			return false;
		}
	}

	bool ColisionWalls() {
	
	}

}



