#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h> 

typedef struct {
	INT8 x;
	INT8 y;
}Vector2I;

typedef struct {
	UINT8 x;
	UINT8 y;
}UVector2I;

typedef struct {
	UVector2I position;
}Object;

typedef struct{
	Vector2I movementVector;
	INT8 movementSpeed;
	Object object;
} Player;

unsigned char sprites[] =
{
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x81,0x81,0xBD,0xBD,0x81,0x81,0x7E,0x7E,
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x99,0x99,0xA5,0xA5,0x81,0x81,0x7E,0x7E
};

void init();
void checkInput();
void updateSwitches();
void ApplyPlayerPosition();

Player player;

void main() {

	player.movementVector.x = 0;
	player.movementVector.y = 0;
	player.object.position.x = 80;
	player.object.position.y = 72;
	player.movementSpeed = 1;

	init();
	
	while(1) {
		checkInput();				// Check for user input (and act on it)
		ApplyPlayerPosition();
		move_sprite(0, player.object.position.x, player.object.position.y);
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory
	}
	
}

void init() {
	
	DISPLAY_ON;						// Turn on the display

	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);
	
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	set_sprite_tile(0,0);
}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

	if(joypad() & J_A){
		player.movementSpeed--;
	}
	else if(joypad() & J_B){
		player.movementSpeed++;
	}

	// UP
	if (joypad() & J_UP) {
		player.movementVector.x = 0 * player.movementSpeed;
		player.movementVector.y = -1 * player.movementSpeed;
	}

	// DOWN
	else if (joypad() & J_DOWN) {
		player.movementVector.x = 0 * player.movementSpeed;
		player.movementVector.y = 1 * player.movementSpeed;
	}

	// LEFT
	else if (joypad() & J_LEFT) {
		player.movementVector.x = -1 * player.movementSpeed;
		player.movementVector.y = 0 * player.movementSpeed;
	}	
	
	// RIGHT
	else if (joypad() & J_RIGHT) {
		player.movementVector.x = 1 * player.movementSpeed;
		player.movementVector.y = 0 * player.movementSpeed;
	}
}


void ApplyPlayerPosition(){
	player.object.position.x += player.movementVector.x;
	player.object.position.y += player.movementVector.y;
}



