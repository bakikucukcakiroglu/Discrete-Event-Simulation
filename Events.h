
#ifndef EVENTS_H
#define EVENTS_H
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
class Events{

public:

	int hackerID;
	int eventID; 
	float event_time;
	int commit_length;

	Events(int hackerID, int eventID, float event_time, int commit_length);


};

struct CompareEvents 
{ 
    bool operator()(Events const& e1, Events const& e2) 
    { 
    
        
    	if(e2.event_time-e1.event_time>0.00001)
    	{

    		return false;
    	}
    	else if(abs(e1.event_time-e2.event_time)< 0.00001)
    	{

    		if(e1.hackerID<e2.hackerID)
    		{
    			return false;
    		}else
    		{
    			return true;
    		}
    	}else
    	{

    		return true;
    	}
    } 
}; 





#endif

