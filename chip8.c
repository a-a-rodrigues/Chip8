#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "SDL.h"

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} sdl_t;

typedef struct {
	uint32_t window_width; 	// SDL window width
	uint32_t window_height;	// SDL window height
	uint32_t fg_color;	// Foreground color RGBA8888
	uint32_t bg_color;	// Background color RGBA8888
	uint32_t scale_factor; 	// Amount to scale a CHIP8 pixel by (e.g., 20x will be a 20x larger window)
} config_t;

// Initialize SDL
bool init_sdl(sdl_t *sdl, const config_t config) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		SDL_Log("Could not initialize SDL subsystems!  %s\n", SDL_GetError());
		return false;
	}

	sdl->window = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					 config.window_width * config.scale_factor,
					 config.window_height * config.scale_factor, 0);

	if (!sdl->window) {
		SDL_Log("Could not create SDL window. %s\n", SDL_GetError());
		return false;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl->renderer) {
		SDL_Log("Could not create SDL renderer. %s\n", SDL_GetError());
		return false;
	}

	return true; // Success
}

// Set up initial emulator configuration from passed-in arguments
bool set_config_from_args(config_t *config, int argc, char **argv) {
	
	// Set defaults
	*config = (config_t) {
		.window_width = 64,	// Chip8 original X res
		.window_height = 32,	// Chip8 original Y res
		.fg_color = 0xFFFF00FF,	// YELLOW
		.bg_color = 0xFFFF00FF,	// YELLOW
		.scale_factor = 20,	// Default resolution is 1280x640
	};

	// Override defaults from passed arguments
	for (int i = 1; i < argc; i++)
		(void)argv[i];

	return true;
}

// Cleanup
void final_cleanup(const sdl_t sdl) {
	SDL_DestroyRenderer(sdl.renderer);
	SDL_DestroyWindow(sdl.window);
	SDL_Quit(); //Shuts down SDL subsystems
}

// Clear screen / SDL window to background color
void clear_screen(const sdl_t sdl, const config_t config) {
	const uint32_t r = (config.bg_color >> 24) & 0xFF;
	const uint32_t g = (config.bg_color >> 16) & 0xFF;
	const uint32_t b = (config.bg_color >> 8) & 0xFF;
	const uint32_t a = (config.bg_color >> 0) & 0xFF;

	SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
	SDL_RenderClear(sdl.renderer);
}

// Update window with any changes
void update_screen(const sdl_t sdl) {
	SDL_RenderPresent(sdl.renderer);
}

void handle_input(void) {
	
}

int main(int argc, char **argv) {

	// Initialize emulator configuration/options
	config_t config = {0};
	if (!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);

	// Initialize SDL
	sdl_t sdl = {0};
	if (!init_sdl(&sdl, config)) exit(EXIT_FAILURE);

	// Initial screen clear to background color
	clear_screen(sdl, config);

	// Main emulator loop
	while (true) {
		// Handle user input
		handle_input();

		// Get time()
		// Emulate CHIP8 instructions
		// Get_time() elapsed since last get_time()

		// Delay for approximately 60hz/60fps (16.67 ms)
		// SDL_Delay(16 - actual time elapsed);
		SDL_Delay(16);

		// Update window with changes
		update_screen(sdl);
	}

	// Final cleanup
	final_cleanup(sdl);

	exit(EXIT_SUCCESS);
}

