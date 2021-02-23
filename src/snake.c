#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>
#include <rand.h>

#define TRUE 1
#define FALSE 0
#define TAIL_MAX_LENGTH 25
#define START_TAIL_SPRITE_INDEX 2
#define PLAYER_SPRITE_INDEX 0
#define TARGET_SPRITE_INDEX 1

const int SCREEN_X_MIN = 0;
const int SCREEN_X_MAX = 168;
const int SCREEN_Y_MIN = 0;
const int SCREEN_Y_MAX = 160;

typedef struct {
	INT8 x;
	INT8 y;
}Vector2I;

typedef struct {
	UINT8 x;
	UINT8 y;
}UVector2I;

typedef struct {
	UVector2I lastPosition;
	UVector2I position;
	UVector2I size;
}Object;

typedef struct{
	Vector2I movementVector;
	INT8 movementSpeed;
	Object object;
} Player;

typedef struct{
	Object object;
} TailObject;

unsigned char PlayerSprite[] =
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

unsigned char TargetSprite[] =
{
  0x3C,0x3C,0x7E,0x42,0xC3,0x81,0xC3,0x81,
  0xC3,0x81,0xC3,0x81,0x7E,0x42,0x3C,0x3C
};

void init();
int checkInput();
void updateSwitches();
void ApplyPlayerPosition();
void SetTargetPositionRandom();
void MoveSnake();
void SetObjectPosition(Object *object, UINT8 posX, UINT8 posY);
UINT8 CollisionCheck(Object* object1, Object* object2);

INT16 randSeed = 0;

Player player;
TailObject tail[TAIL_MAX_LENGTH];

Object target;

INT8 tailLength;

void main() {
	
	//Initialise Player members
	player.movementSpeed = 1;

	player.object.position.x = SCREEN_X_MAX / 2;
	player.object.position.y = SCREEN_Y_MAX / 2;
	player.object.size.x = 8;
	player.object.size.y = 8;

	//Initialise target members
	target.size.x = 8;
	target.size.y = 8;

	tailLength = 0;


	init();

	//printf("test: %d \n", tail[0].lastPosition.x );

	while (checkInput() == FALSE)
	{
		randSeed++; //we increment our seed for our number generator

		ApplyPlayerPosition();
		move_sprite(0, player.object.position.x, player.object.position.y);
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();
	}

	initrand(randSeed);

	SetTargetPositionRandom();
	
	while(TRUE) {
		checkInput();				// Check for user input (and act on it)
		ApplyPlayerPosition();
		MoveSnake();
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory

		if(CollisionCheck(&player.object, &target) == TRUE){
			SetTargetPositionRandom();
			tailLength++;
		}
	}
	
}

void init() {
	
	DISPLAY_ON;						// Turn on the display

	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(PLAYER_SPRITE_INDEX, 8, PlayerSprite);
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	set_sprite_tile(PLAYER_SPRITE_INDEX,0);

	set_sprite_data(TARGET_SPRITE_INDEX, 8, TargetSprite);

	set_sprite_tile(TARGET_SPRITE_INDEX,1);

	for (int i = 0; i < TAIL_MAX_LENGTH - 1; ++i){
		set_sprite_tile(i + START_TAIL_SPRITE_INDEX,0);
	}
}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

int checkInput() {

	// UP
	if ((joypad() & J_UP) && player.movementVector.y != 1) {
		player.movementVector.x = 0;
		player.movementVector.y = -1;

		return TRUE;
	}

	// DOWN
	else if ((joypad() & J_DOWN) && player.movementVector.y != -1) {
		player.movementVector.x = 0;
		player.movementVector.y = 1;

		return TRUE;
	}

	// LEFT
	else if ((joypad() & J_LEFT) && player.movementVector.x != 1) {
		player.movementVector.x = -1;
		player.movementVector.y = 0;

		return TRUE;
	}	
	
	// RIGHT
	else if ((joypad() & J_RIGHT) && player.movementVector.x != -1) {
		player.movementVector.x = 1;
		player.movementVector.y = 0;

		return TRUE;
	}

	return FALSE;
}


void ApplyPlayerPosition(){

	player.object.lastPosition.x = player.object.position.x;
	player.object.lastPosition.y = player.object.position.y;

	player.object.position.x += player.movementVector.x;
	player.object.position.y += player.movementVector.y;

	if(player.object.position.x <= SCREEN_X_MIN){
		player.object.position.x = SCREEN_X_MAX;
	}
	else if(player.object.position.x > SCREEN_X_MAX){
		player.object.position.x = SCREEN_X_MIN;
	}
	else if(player.object.position.y <= SCREEN_Y_MIN){
		player.object.position.y = SCREEN_Y_MAX;
	}
	else if(player.object.position.y > SCREEN_Y_MAX){
		player.object.position.y = SCREEN_Y_MIN;
	}
}

void SetObjectPosition(Object *object, UINT8 posX, UINT8 posY){
	object->lastPosition.x = object->position.x;
	object->lastPosition.y = object->position.y;

	object->position.x = posX;
	object->position.y = posY;
}

void SetTargetPositionRandom(){

	target.position.x = rand();

	while (target.position.x > SCREEN_X_MAX - 8)
	{
		target.position.x /= 2;
	}
	
	target.position.y = rand();

	while (target.position.y > SCREEN_Y_MAX - 8)
	{
		target.position.y /= 2;
	}

	initrand(target.position.y * target.position.x);

	move_sprite(1, target.position.x, target.position.y);
}

UINT8 CollisionCheck(Object* object1, Object* object2) {

	if ((object1->position.x < (object2->position.x + object1->size.x)) && 
		((object1->position.x + object1->size.x) > object2->position.x) && 
		(object1->position.y < (object2->size.y + object2->position.y)) && 
		((object1->position.y + object1->size.y) > object2->position.y)) {
		
		return TRUE;
	
	} else {
		
		return FALSE;
	
	}

}

void MoveSnake(){
	move_sprite(0, player.object.position.x, player.object.position.y);

	if(tailLength > 0){
		SetObjectPosition(&tail[0].object, player.object.lastPosition.x + (8 * -player.movementVector.x), player.object.lastPosition.y + (8 * -player.movementVector.y));
		move_sprite(START_TAIL_SPRITE_INDEX, tail[0].object.position.x, tail[0].object.position.y);
	}
	//for(int i )
}



