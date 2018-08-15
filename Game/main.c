/**
* @file main.c
* @author Chanhee Lee
* @date 07/08/2018
* @brief 
*/

#include <stdio.h>
#include <stdbool.h>
#include "myLib.h"
#include "bg.h"
#include "start.h"
#include "gameover.h"
#include "rwreck.h"
#include "ob.h"
#include "text.h"

enum GameState{
	START_STATE1, START_STATE2, SETUP_STATE, GAME_STATE, GAMEOVER_STATE1, GAMEOVER_STATE2
};

int main() {

	// Set Up Mode and Start State 
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	enum GameState state = START_STATE1;
	
	// Set Up Variables 
	bool start_down = FALSE; 
	unsigned int vBlanks = 0;
	unsigned int score = 0; 
	unsigned int newscore = 0;

	// Set Up Structs (player and obstacle)
	OBJ wreck = {(Vector) {5,60}, (Vector) {12,12}, 0}; 
	OBJ curwreck = {(Vector) {5,60}, (Vector) {12,12}, 0}; 
	OBJ obs1 = {(Vector) {230,5}, (Vector) {10,10}, 0};
	OBJ obs2 = {(Vector) {230,65}, (Vector) {10,10}, 0};
	OBJ obs3 = {(Vector) {230,125}, (Vector) {10,10}, 0};
	OBJ obs4 = {(Vector) {230,35}, (Vector) {10,10}, 0};
	OBJ obs5 = {(Vector) {230,95}, (Vector) {10,10}, 0};
	OBJ curobs1 = {(Vector) {230,5}, (Vector) {10,10}, 1};
	OBJ curobs2 = {(Vector) {230,65}, (Vector) {10,10}, 1};
	OBJ curobs3 = {(Vector) {230,125}, (Vector) {10,10}, 1};
	OBJ curobs4 = {(Vector) {230,35}, (Vector) {10,10}, 0};
	OBJ curobs5 = {(Vector) {230,95}, (Vector) {10,10}, 0};

	// Game Start 
	while(TRUE){
		switch(state){
			// Draw Start Screen 
			case START_STATE1:
				drawImage3(0,0, START_WIDTH, START_HEIGHT, start);
				state = START_STATE2; 
				break; 

			// Wait For Start Button to Be Pressed
			case START_STATE2: 

				// Reset Variables  
				score = 0; 
				newscore = 0; 
				vBlanks = 0; 
				wreck = (OBJ) {(Vector) {5,60}, (Vector) {12,12}, 0}; 
				curwreck = (OBJ) {(Vector) {5,60}, (Vector) {12,12}, 0}; 
				obs1 = (OBJ) {(Vector) {230,5}, (Vector) {10,10}, 0};
				obs2 = (OBJ) {(Vector) {230,65}, (Vector) {10,10}, 0};
				obs3 = (OBJ) {(Vector) {230,125}, (Vector) {10,10}, 0};
				obs4 = (OBJ) {(Vector) {230,35}, (Vector) {10,10}, 0};
				obs5 = (OBJ) {(Vector) {230,95}, (Vector) {10,10}, 0};
				curobs1 = (OBJ) {(Vector) {230,5}, (Vector) {10,10}, 1};
				curobs2 = (OBJ) {(Vector) {230,65}, (Vector) {10,10}, 1};
				curobs3 = (OBJ) {(Vector) {230,125}, (Vector) {10,10}, 1};
				curobs4 = (OBJ) {(Vector) {230,35}, (Vector) {10,10}, 0};
				curobs5 = (OBJ) {(Vector) {230,95}, (Vector) {10,10}, 0};			

				// Move onto next state if start button is pressed and released
				if(!start_down && KEY_DOWN_NOW(BUTTON_START)){
					start_down = KEY_DOWN_NOW(BUTTON_START); 
					state = SETUP_STATE; 
					break;  
				}
				else {
					start_down = KEY_DOWN_NOW(BUTTON_START); 
					break; 
				}

			// Draw Background
			case SETUP_STATE: 
				drawImage3(0, 0, BACKGROUND_WIDTH,BACKGROUND_HEIGHT, background);

				// Set up player 
				drawImage3(curwreck.top_left.y, curwreck.top_left.x, RAMBLINWRECK_WIDTH, RAMBLINWRECK_HEIGHT, ramblinwreck);

				state = GAME_STATE; 
				break; 

			// Code During Actual Game Play 
			case GAME_STATE: 

				// if select is pressed, reset to start screen 
				if (KEY_DOWN_NOW(BUTTON_SELECT)){
					state = START_STATE1; 
					break;					
				}

				// check for collision 
				if ((collision(curobs1, curwreck) == 1) || (collision(curobs2, curwreck) == 1)  || (collision(curobs3, curwreck) == 1) 
				|| (collision(curobs4, curwreck) ==  1) || (collision(curobs5, curwreck) ==  1) ) {	
					state = GAMEOVER_STATE1; 
				}

				// movement of obstacles 
				//(if objects reach the left side of the screen, reset them to the right)
				//(else move them to the left based on their speed) 
				if( (&curobs1)->top_left.x <= 4 )
					(&curobs1)->top_left.x = 239;
				else
					(&curobs1)->top_left.x -= curobs1.speed;
				
				if( (&curobs2)->top_left.x <= 4 )
					(&curobs2)->top_left.x = 239;
				else
					(&curobs2)->top_left.x -= curobs2.speed;

				if( (&curobs3)->top_left.x <= 4 )
					(&curobs3)->top_left.x = 239;
				else
					(&curobs3)->top_left.x -= curobs3.speed;
				
				if( (&curobs4)->top_left.x <= 4 )
					(&curobs4)->top_left.x = 239;
				else
					(&curobs4)->top_left.x -= curobs4.speed;
				
				if( (&curobs5)->top_left.x <= 4 )
					(&curobs5)->top_left.x = 239;
				else
					(&curobs5)->top_left.x -= curobs5.speed;

				// update score 			
				if(vBlanks % 60 == 0) {
					newscore++;
				}

				// Level Increase Over Time (based on score)
				// level 1 -> level 2
				if (score > 3 && curobs1.top_left.x == 239){
					(&curobs1)->speed = 2; 
				}
				if (score > 3 && curobs2.top_left.x == 239){
					(&curobs2)->speed = 3; 
				}
				if (score > 3 && curobs3.top_left.x == 239){
					(&curobs3)->speed = 1; 
				}				
			
				// level 2 -> level 3
				if (score > 10 && curobs1.top_left.x == 239){
					(&curobs1)->speed = 3; 
				}
				if (score > 10 && curobs2.top_left.x == 239){
					(&curobs2)->speed = 2; 
				}
				if (score > 10 && curobs3.top_left.x == 239){
					(&curobs3)->speed = 2; 
				}
				if (score > 10) {
					(&curobs5)->speed = 4; 
					(&curobs4)->speed = 1; 
				}

				// level 3 -> level 4
				if (score > 20) {
					(&curobs1)->speed = 3; 
					(&curobs2)->speed = 3; 
					(&curobs3)->speed = 3; 
					(&curobs4)->speed = 3; 
					(&curobs5)->speed = 3; 
				}

				// level 4 -> 5 
				if (score > 25) {
					(&curobs1)->speed = 3; 
					(&curobs2)->speed = 4; 
					(&curobs3)->speed = 3; 
					(&curobs4)->speed = 4; 
					(&curobs5)->speed = 2; 
				}

				// level 5 -> 6
				// obstacle 1,3,5 start moving down 
				if (score > 30){
					if ( ((&curobs1)->top_left.x == 239) ){
						if ((&curobs1)->top_left.y > 149)
							(&curobs1)->top_left.y = 0; 
						else
							(&curobs1)->top_left.y += 5; 
					}
					if ( ((&curobs3)->top_left.x == 239) ){
						if ((&curobs3)->top_left.y > 149)
							(&curobs3)->top_left.y = 0; 
						else
							(&curobs3)->top_left.y += 5; 
					}
					if ( ((&curobs5)->top_left.x == 239) ){
						if ((&curobs5)->top_left.y > 149)
							(&curobs5)->top_left.y = 0; 
						else
							(&curobs5)->top_left.y += 5; 
					}
				}
				

				// movement of RamblinWreck  
				if(KEY_DOWN_NOW(BUTTON_RIGHT) && (&curwreck)->top_left.x + (&curwreck)->size.x-1 < 237)
					(&curwreck)->top_left.x += 3; 
				if(KEY_DOWN_NOW(BUTTON_LEFT) && (&curwreck)->top_left.x > 3)
					(&curwreck)->top_left.x -= 3; 
				if(KEY_DOWN_NOW(BUTTON_UP) && (&curwreck)->top_left.y > 3)
					(&curwreck)->top_left.y -= 3; 
				if(KEY_DOWN_NOW(BUTTON_DOWN) && (&curwreck)->top_left.y + (&curwreck)->size.y-1 < 156)
					(&curwreck)->top_left.y += 3;

				// VBlank (update images after this point)
				waitForVBlank(); 
				vBlanks++; 

				// Clear Previous Images and Scores  
				removeImage(background, wreck); 
 				removeImage(background, obs1);
				removeImage(background, obs2);
				removeImage(background, obs3);
				if (score > 10){
					removeImage(background, obs4);
					removeImage(background, obs5);
				}
				removeImage(background, (OBJ) {(Vector){180,140}, (Vector){60,20}, 0});
				
 
				// Redraw New Images and Scores 
				drawImage3(curwreck.top_left.y, curwreck.top_left.x, RAMBLINWRECK_WIDTH, RAMBLINWRECK_HEIGHT, ramblinwreck); 
				drawImage3(curobs1.top_left.y, curobs1.top_left.x, OB_WIDTH, OB_HEIGHT, ob);
				drawImage3(curobs2.top_left.y, curobs2.top_left.x, OB_WIDTH, OB_HEIGHT, ob);
				drawImage3(curobs3.top_left.y, curobs3.top_left.x, OB_WIDTH, OB_HEIGHT, ob);
				if (score  > 10){
					drawImage3(curobs4.top_left.y, curobs4.top_left.x, OB_WIDTH, OB_HEIGHT, ob);
					drawImage3(curobs5.top_left.y, curobs5.top_left.x, OB_WIDTH, OB_HEIGHT, ob);
				}
				// Draw screen 
				char scrBuffer[100];
				sprintf(scrBuffer, "Score: %d", newscore); 
				drawString(140, 180, scrBuffer, WHITE);
				
	
				// Update old values (often used to pass into removeImage)
				(&wreck)->top_left.y = (&curwreck)->top_left.y;
				(&wreck)->top_left.x = (&curwreck)->top_left.x;
				(&obs1)->top_left.y = (&curobs1)->top_left.y;
				(&obs1)->top_left.x = (&curobs1)->top_left.x; 
				(&obs1)->speed = (&curobs1)->speed; 
				(&obs2)->top_left.y = (&curobs2)->top_left.y;
				(&obs2)->top_left.x = (&curobs2)->top_left.x;
				(&obs2)->speed = (&curobs2)->speed;
				(&obs3)->top_left.y = (&curobs3)->top_left.y;
				(&obs3)->top_left.x = (&curobs3)->top_left.x;
				(&obs3)->speed = (&curobs3)->speed;
				(&obs4)->top_left.y = (&curobs4)->top_left.y;
				(&obs4)->top_left.x = (&curobs4)->top_left.x;
				(&obs4)->speed = (&curobs4)->speed;
				(&obs5)->top_left.y = (&curobs5)->top_left.y;
				(&obs5)->top_left.x = (&curobs5)->top_left.x;
				(&obs5)->speed = (&curobs5)->speed;				
				score = newscore; 
	
				break;

			// Draw the game over screen 
			case GAMEOVER_STATE1:
				drawImage3(0,0,GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover); 
				state = GAMEOVER_STATE2;
				break;

			// Wait For Select Button to Restart
			case GAMEOVER_STATE2: 
				if (KEY_DOWN_NOW(BUTTON_SELECT)){
					state = START_STATE1; 
					break;					
				}
				state = GAMEOVER_STATE2; 
				break;
				
		}
	}


}
