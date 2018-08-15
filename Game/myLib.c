/**
* @file MyLib.c
* @author Chanhee Lee
* @date 07/08/2018
* @brief 
*/

#include "myLib.h"

//BUFFER0 
volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;

void setPixel(int row, int col, Pixel pixel){
        videoBuffer[row*240 + col] = pixel; 
}

void waitForVBlank(){
	while(*SCANLINECOUNTER > 160); 
	while(*SCANLINECOUNTER < 160); 
}

void drawImage3(int r, int c, int width, int height, const u16* image){
    for (int i = 0; i < height; i++) {
        DMA[3].src = &image[(i * width)];
        DMA[3].dst = &videoBuffer[((r + i) * 240) + c];
        DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void removeImage(const u16* background, OBJ oldimage){ 
	// Draw background in oldimage's position
	for (unsigned int i = 0; i < oldimage.size.y; i++){
		DMA[3].src = &background[(oldimage.top_left.y+i)*240 + oldimage.top_left.x];
		DMA[3].dst = &videoBuffer[((oldimage.top_left.y + i) * 240) + oldimage.top_left.x];
		DMA[3].cnt = oldimage.size.x | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

int collision(OBJ ob1, OBJ ob2){
	if((ob1.top_left.x >= ob2.top_left.x && ob1.top_left.x <= ob2.top_left.x + ob2.size.x-1 
	&& ob1.top_left.y >= ob2.top_left.y && ob1.top_left.y <= ob2.top_left.y + ob2.size.y-1 ) || // top left of ob1
	(ob1.top_left.x + ob1.size.x-1 >= ob2.top_left.x && ob1.top_left.x + ob1.size.x-1 <= ob2.top_left.x + ob2.size.x-1 
	&& ob1.top_left.y >= ob2.top_left.y && ob1.top_left.y <= ob2.top_left.y + ob2.size.y-1 ) || // top right of ob1
	(ob1.top_left.x >= ob2.top_left.x && ob1.top_left.x <= ob2.top_left.x + ob2.size.x-1 && ob1.top_left.y + 
	ob1.size.y-1>= ob2.top_left.y && ob1.top_left.y + ob1.size.y-1 <= ob2.top_left.y + ob2.size.y-1 ) || // bottom left of ob1
	(ob1.top_left.x + ob1.size.x-1  >= ob2.top_left.x && ob1.top_left.x + ob1.size.x-1  <= ob2.top_left.x + ob2.size.x-1 
	&& ob1.top_left.y + ob1.size.y-1>= ob2.top_left.y && ob1.top_left.y + ob1.size.y-1 <= ob2.top_left.y + 
	ob2.size.y-1 ))
		return 1; 
	else
		return 0;
}
