#include "Hackers.h"
#include <iostream>

using namespace std;

Hackers::Hackers(int id, float arrival_time)
{

	this->id=id;
	this->arrival_time=arrival_time;
	this->gift_count=0;
	this->total_attempts=0;
	this->invalid_attempts=0;
	this->total_invalid_by_gift=0;
	this->commit_count=0;
 	this->valid_commit_count=0;
 	this->total_changed_lines=0;
 	
	this->total_sticker_queue_time=0;
	this->total_hoodie_queue_time=0;
	this->total_tour_time=0;

	this->sticker_queue_enterance_time=0;
 	this->hoodie_queue_enterance_time=0;
 	this->sticker_queue_exit_time=0;
 	this->hoodie_queue_exit_time=0;

 	this->current_sticker_desk_index=0;
 	this->current_hoodie_desk_index=0;

}

void Hackers::commit(int commited_line)
{

	this->commit_count++;
	this->total_changed_lines+= commited_line;

	if(commited_line>=20)
	{

		this->valid_commit_count++;
	}
}

