#ifndef HACKERS_H
#define HACKERS_H

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
class Hackers
{

 	public: 
 		int id;
        float arrival_time;
 		int gift_count;
 		int total_attempts;
 		int invalid_attempts;
 		int total_invalid_by_gift;
 		int commit_count;
 		int valid_commit_count;
 		int total_changed_lines;

 		float sticker_queue_enterance_time;
 		float hoodie_queue_enterance_time;
 		float sticker_queue_exit_time;
 		float hoodie_queue_exit_time;
 		
        float total_sticker_queue_time;
 		float total_hoodie_queue_time;
        float total_tour_time;
 		
        int current_sticker_desk_index;
 		int current_hoodie_desk_index;

		Hackers(int id, float arrival_time);
 		void commit(int commited_line);



};

struct CompareHackers
{ 
    bool operator()(Hackers const& h1, Hackers const& h2) 
    { 
    
        
    	if(h1.valid_commit_count<h2.valid_commit_count)
    	{

    		return true;
    	}
    	else if(h1.valid_commit_count==h2.valid_commit_count)
    	{

    		if(h1.hoodie_queue_enterance_time-h2.hoodie_queue_enterance_time>0.00001)
    		{
    			
    			return true;
    		
    		}
    		else if(abs(h1.hoodie_queue_enterance_time-h2.hoodie_queue_enterance_time)<0.00001)
    		{
    			if(h1.id>h2.id)
    			{

    				return true;
    			}
    			else
    			{

    				return false;
    			}
    		}
    		else
    		{
    			return false;

    		}
    	}
    	else
    	{

    		return false;
    	}
    } 
}; 




#endif