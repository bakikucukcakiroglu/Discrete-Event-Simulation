#include "StickerDesk.h"

StickerDesk::StickerDesk(int deskID, float service_time){

	this->deskID=deskID;
	this->service_time=service_time;
	this->isFull= false; 
}