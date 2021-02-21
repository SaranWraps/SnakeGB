#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h> 


void init();
void checkInput();
void updateSwitches();

typedef struct {
	INT8 x;
	INT8 y;
}Vector2I;

typedef struct{
	Vector2I movementVector;
} Player;

Player player;
void main() {

	player.movementVector.x = 0;
	player.movementVector.y = 0;

	printf("X value %d \n", player.movementVector.x);
	printf("Y value %d \n", player.movementVector.y);

	//printf("Position Y: %d \n", MainPlayer->directionVector->y);
	init();
	
	while(1) {
		checkInput();				// Check for user input (and act on it)
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory
	}
	
}

void init() {
	
	DISPLAY_ON;						// Turn on the display
	//set_bkg_data(0, 47, alpha);		// Load 47 tiles into background memory
	
	// Use the 'helloWorld' array to write background tiles starting at 0,6 (tile positions)
	//  and write for 10 tiles in width and 2 tiles in height
	//set_bkg_tiles(0,6,10,2,helloWorld);
	
	// Load the the 'sprites' tiles into sprite memory
	//set_sprite_data(0, 1, sprites);
	
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	//set_sprite_tile(0,0);

}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

    if (joypad() & J_B) {
	player.movementVector.x++;
	player.movementVector.y--;

	printf("X value %d \n", player.movementVector.x);
	printf("Y value %d \n", player.movementVector.y);
    }
	
	// UP
	if (joypad() & J_UP) {

	}

	// DOWN
	if (joypad() & J_DOWN) {
			
	}

	// LEFT
	if (joypad() & J_LEFT) {
		
		
	}	
	
	// RIGHT
	if (joypad() & J_RIGHT) {
		
	}
	
	// Move the sprite in the first movable sprite list (0)
	//  the the position of X (player[0]) and y (player[1])
	//move_sprite(0, player[0], player[1]);
}

