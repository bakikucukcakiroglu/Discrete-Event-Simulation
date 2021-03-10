#include "HoodieDesk.h"

HoodieDesk::HoodieDesk(int deskID, float service_time){

	this->deskID=deskID;
	this->service_time=service_time;
	this->isFull= false; 
}