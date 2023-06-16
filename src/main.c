#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

int HEIGHT = 600;
int WIDTH = 800;

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL;

int	initialize_window(void){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr,"error initializing SDL\n");
		return false;
	}
	
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	WIDTH = display_mode.w;
	HEIGHT = display_mode.h;

	//create SDL window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_BORDERLESS
		);
	if (!window){
		fprintf(stderr, "not able to init window\n");
		return false;
	}
	//create an SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer){
		fprintf(stderr, "error creating renderer\n");
		return false;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	return true;
}

void setup(void){
	//allocare i byte richiesti in memoria per il color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
	//set color buffer pixel to something
	//color_buffer[(WIDTH * 10)+20] = 0xFFFF0000;
	if (!color_buffer){
		printf("allocation not successful");
		exit(1);
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH,
		HEIGHT
		);
};

void process_input(void){

	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type){
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
				break;
	}

};

void update(void){

};

void	draw_grid(){

	//TODO:render a background grid thatshows a line every row or column 
	//of pixel that is multiple of 10//
};


void	render_color_buffer(void){
		SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(WIDTH * sizeof(uint32_t))
		);

		SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
		);

};

void	clear_color_buffer(uint32_t color){
	for (int y = 0; y < HEIGHT; y++){
		for(int x = 0; x < WIDTH; x++){
		color_buffer[(WIDTH * y) + x] = color;
		}
	}
}


void render(void){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
	SDL_RenderClear(renderer);
	render_color_buffer();
	clear_color_buffer(0xFFFFFF00);
	SDL_RenderPresent(renderer);
};

void	destroy_window(void){
		free(color_buffer);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
}

int main(){
	//create an SDL window //
	is_running = initialize_window();
	if (!is_running)
		return 1;
	setup();
	//color_buffer[(WIDTH * 10)+ 20] = 0x00000000;
	while(is_running){
		process_input();
		update();
		render();
	}
	destroy_window();
	return 0;
}
