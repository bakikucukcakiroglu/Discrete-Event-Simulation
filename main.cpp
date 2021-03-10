
#include <iostream>
#include <fstream>
#include <queue> 
#include <vector>
#include "Hackers.h"
#include "Events.h"
#include "StickerDesk.h"
#include "HoodieDesk.h"
#include <iomanip>

//auto start = chrono::steady_clock::now();

using namespace std;

int max_sticker_queue = 0; 		// keeps maximum sticker queue length
int max_hoodie_queue = 0;		// keeps maximum hoodie queue length
float hackathon = 0;			// keeps total hackathon time
float total_timearound_time = 0;// keeps total timearound time

int main(int argc, char* argv[]) 
{

    ifstream read(argv[1]); 	// provide us to read input file
    ofstream write(argv[2]);	// provide us to write to output file

    int hackerNumber;		// hacker number in the hackaton
    read>>hackerNumber;	

    vector<Hackers> hackers;												// the vector keeps all hackers
    priority_queue< Events, vector<Events>, CompareEvents> events;			// the priority queue keeps all events and sorts them 
    
    vector<StickerDesk> stickerDesks;										// the vector keeps all sticker desks
    vector<HoodieDesk> hoodieDesks;											// the vector keeps all hoodie desks

    priority_queue<Hackers, vector<Hackers>, CompareHackers> hoodieQueue;	// the priority queue keeps all hackers on hoodie queue and sorts them
    queue<Hackers> stickerQueue;											// the queue keeps all hackers on sticker queue 
    
    for(int i=0; i<hackerNumber; i++) 	// creates hacker objects due to hacker number
    {

    	int hackerID = i+1;		// keeps hacker's id
    	
    	float arrival_time;		// keeps arrival time of the hacker
    	read>>arrival_time;		

    	hackers.push_back(Hackers(hackerID, arrival_time)); 	// creates the hacker object and pushes it to hackers vector
    
    }

    int commitNumber;		//keeps total commit number in hackaton
    read>>commitNumber;

    for(int i=0; i<commitNumber; i++)	// creates commit events due to commit number
    {
    	
    	int hackerID;		// id of the hacker who makes a commit
    	int commited_line;	// number of changed lines of the commit
    	float time;			// event time

    	read>>hackerID>>commited_line>>time;

    	events.push(Events(hackerID, 1, time, commited_line)); 	// creates a commit event(event number:1) and pushes it to events priority queue

    }

    int queueAttempts;		// total attempts to enter a queue
    read>>queueAttempts;

    for(int i=0; i<queueAttempts; i++)	// creates queue entry events due to total queue entry attempts
    {    	

    	int hackerID;			// id of the hacker who tries to enter a queue
    	float time;				// event time

    	read>>hackerID>>time;

    	events.push(Events(hackerID, 2, time, 0)); // creates a queue entry event(event number:2) and pushes it to events priority queue

    }

    int stickerDesksNum;	// number of sticker desks 
    read>>stickerDesksNum;

    for(int i=0; i<stickerDesksNum; i++)	// creates sticker desks due to sticker desks number
    {

    	int deskID = i+1;		// id of the sticker desk
    	float service_time;		// service time of the sticker desk 

    	read>>service_time;

    	stickerDesks.push_back(StickerDesk(deskID, service_time));	// creates a sticker desk and pushes it to sticker desks vector

    }

    int hoodieDesksNum;		// number of hoodie desks
    read>>hoodieDesksNum;

    for(int i=0; i<hoodieDesksNum; i++)	// creates hoodie desks due to hoodie desks number
    {

    	int deskID = i+1;		// id of the hoodie desk 
    	float service_time;		//service time of the hoodie desk

    	read>>service_time;

    	hoodieDesks.push_back(HoodieDesk(deskID, service_time));	// crates a hoodie desk and pushes it to hoodie desks vector

    }

    while(events.size()!= 0)	// excutes time sorted events one by one
    {

    	Events a=events.top();	// keeps the first event 
    	events.pop();			// removes the first event from events priority queue

    	switch(a.eventID)	// this switch case executes events according to their event id (1=commit, 2=sticker queue entry, 3=sticker desk exit, 5= hoodie desk exit)
    	{

    		case 1: // executes commit events
    		{

    			hackers[a.hackerID-1].commit(a.commit_length); 	// calls hacker's commit method 

    			break;
    		}
    		case 2:	// executes sticker queue entry events
    		{
    			
    			if(hackers[a.hackerID-1].valid_commit_count >= 3)	// checks whether hacker has 3 valid comment or not
    			{

    				if(hackers[a.hackerID-1].gift_count<3)		// checks whether hacker has less than 3 gifts or not
    				{
		    			
		    			hackers[a.hackerID-1].gift_count++;		// when a hacker enter a queue increases her/his gift number by one
		    			
		    			hackers[a.hackerID-1].sticker_queue_enterance_time=a.event_time;	// sets hacker's sticker queue enterance time as event time

		    			if(stickerQueue.size()==0)	// checks whether queue is empty or not
		    			{

		    				int fullDeskNumber=0;	// counter for full desks
		    				
		    				for(int i=0; i<stickerDesksNum;i++)		// traverse all sticker desks to find a empty desk
		    				{

		    					if(!stickerDesks[i].isFull)		// checks whether the sticker desk is full or not
		    					{
		    						
		    						hackers[a.hackerID-1].current_sticker_desk_index=i;		// set hacker's current sticker desk as empty desk

		    						hackers[a.hackerID-1].sticker_queue_exit_time = hackers[a.hackerID-1].sticker_queue_enterance_time;	// since queue is empty and there is an empty desk, 
		    																															// sets hacker's sticker queue exit time as sticker queue entry time

		    						stickerDesks[i].isFull=true;	// sets the sticker desk as full

		    						events.push(Events(a.hackerID,3,hackers[a.hackerID-1].sticker_queue_exit_time+stickerDesks[i].service_time,0)); // creates sticker queue exit event and pushes it evenets priority queue
		    						
		    						break;
		    					}
		    					else
		    					{
		    					
		    						fullDeskNumber++; // if the desk is full, increases full desk number by one

		    					}
		    				}
		    				if(fullDeskNumber==stickerDesksNum)	// this case means all desks are full
		    				{

								stickerQueue.push(hackers[a.hackerID-1]);	// pushes current hacker to sticker queue

		    				}
		    			}
		    			else // this case means sticker queue is not empty
		    			{
		    				
		    				stickerQueue.push(hackers[a.hackerID-1]);	// pushes current hacker to sticker queue
		    				
		    			}
		    		}
		    		else 	// this case means hacker has more than 3 gifts
		    		{
		    			
		    			hackers[a.hackerID-1].total_invalid_by_gift++;	// increases total invalid attempts by one
		    	
		    		}	

    			}else	// this case means hacker doesn't have enough attempts
    			{

    				hackers[a.hackerID-1].invalid_attempts++;	// increases total invalid attempts by one

    			}	

    			if(stickerQueue.size()>max_sticker_queue)	// if current sticker queue size is larger than maximum sticker queue size
    			{

		    		max_sticker_queue=stickerQueue.size();	// sets maximum queue size as current sticker size
		    	}

    			break;
    		}
    		case 3: // executes sticker desk exit event
    		{
    				
    			//hackers[a.hackerID-1].gift_count++;

				stickerDesks[hackers[a.hackerID-1].current_sticker_desk_index].isFull=false;	// sets hacker's currents sticker desk as false
				
				int emptyIndex;	// keeps the empty desk with minimum id

				for(int i=0; i<stickerDesksNum; i++ )	// traverse sticker desks due to sticker desk number
				{

					if(!stickerDesks[i].isFull)	// checks wheter currents desk is full or not
					{

						emptyIndex=i;	// set empty index as current index
						break;

					}
				}
				//!!!!
				hackers[a.hackerID-1].total_sticker_queue_time+= (hackers[a.hackerID-1].sticker_queue_exit_time-hackers[a.hackerID-1].sticker_queue_enterance_time);

				if(stickerQueue.size()!=0)	// if sticker queue is not empty 
		    	{

					hackers[stickerQueue.front().id-1].current_sticker_desk_index=emptyIndex;	// set current sticker desk of the first hacker on the sticker queue as empty index
		    						
		    		hackers[stickerQueue.front().id-1].sticker_queue_exit_time = a.event_time;	// sets hacker's sticker queue exit time as event time

		    		stickerDesks[emptyIndex].isFull=true;										// sets hacker's current desk as full

		    		events.push(Events(stickerQueue.front().id,3,hackers[stickerQueue.front().id-1].sticker_queue_exit_time+stickerDesks[emptyIndex].service_time,0));
		    		// creates sticker desk exit event for the hacker and pushes it to events priority queue

		    		stickerQueue.pop();	// remove the hacker from sticker queue

		    	}

		    	hackers[a.hackerID-1].hoodie_queue_enterance_time=a.event_time;	// sets hacker's hoodie queue enterance time as event time

		    	if(hoodieQueue.size()==0)	// if hoodie queue is empty
		    	{

		    		int fullDeskNum=0;	// counter for full hoodie desks
		    		
		    		for(int i=0; i<hoodieDesksNum;i++)	// traverse hoodie desks due to hoodie desks number
		    		{

		    			if(!hoodieDesks[i].isFull)	// if there is an empty hoodie desk
		    			{

		    				hackers[a.hackerID-1].current_hoodie_desk_index=i;													// sets hacker's current hoodie desk index as current index

		    				hackers[a.hackerID-1].hoodie_queue_exit_time = hackers[a.hackerID-1].hoodie_queue_enterance_time ;	// sets hacker's hoodie queue exit time as hoodie queue enterance time

		    				hoodieDesks[i].isFull=true;																			// sets hacker's current hoodie desk as full

		    				events.push(Events(a.hackerID,5,hackers[a.hackerID-1].hoodie_queue_exit_time+hoodieDesks[i].service_time,0)); 
		    				// creates hoodie desk exit event for hacker and pushes it to events priority queue

		    				break;
		    			}
		    			else
		    			{

		    				fullDeskNum++;	// increases full desk number by one

		    			}
		    		}

		    		if(fullDeskNum==hoodieDesksNum)	// checks whether hoodie desk is full or not
		    		{

		    			hoodieQueue.push(hackers[a.hackerID-1]);	// pushes hacker to hoodie queue

		    		}
		    	}else								// if hoodie queue is not empty
		    	{
		    			
		    		hoodieQueue.push(hackers[a.hackerID-1]);	// pushes hacker to hoodie queue
		    	
		    	}
						if(hoodieQueue.size()>max_hoodie_queue)	// checks current hoodie queue size is larger than maximum hoodie queue size or not
						{

		    			max_hoodie_queue=hoodieQueue.size();	// sets maximum hoodie queue size as current hoodie queue size
		    			
		    			}

		    	break;
		    }
		    
		    case 5:	// executes hoodie desk exit event
		    {
		    	
		    	//hackers[a.hackerID-1].gift_count++;
				
				hoodieDesks[hackers[a.hackerID-1].current_hoodie_desk_index].isFull=false;	// sets hacker's current hoodie desk as empty
			
				hackers[a.hackerID-1].total_hoodie_queue_time+= (hackers[a.hackerID-1].hoodie_queue_exit_time-hackers[a.hackerID-1].hoodie_queue_enterance_time);
				// increments total hoodie queue time by difference between hoodie queue exit time and hoodie queue entrance time
				
				hackers[a.hackerID-1].total_tour_time+= (a.event_time-hackers[a.hackerID-1].sticker_queue_enterance_time);
				// increments total tour time by difference between event time and sticker queue enterance time
				
				total_timearound_time+=(a.event_time-hackers[a.hackerID-1].sticker_queue_enterance_time);
				// increments total timearound time by difference between event time and sticker queue enterane time
				
				int emptyIndex2;	// keeps the empty desk with minimum id

				for(int i=0; i<hoodieDesksNum; i++ )	// traverse sticker desks due to sticker desk number
				{
					
					if(!hoodieDesks[i].isFull)	// checks wheter currents desk is full or not
					{

						emptyIndex2=i;	// set empty index as current index
						break;

					}
				}

				if(hoodieQueue.size()!=0)	// checks whether hoodie queue is empty or not
		    	{
		    						
		    		Hackers p= hoodieQueue.top(); 							// sets a hacker to first hacker on the hoodie queue
		    		hoodieQueue.pop();

		    		hackers[p.id-1].hoodie_queue_exit_time = a.event_time ;	// sets hacker's hoodie queue exit time as event time

		    		hoodieDesks[emptyIndex2].isFull=true;					// sets hacker's current desk as full			

		    		hackers[p.id-1].current_hoodie_desk_index=emptyIndex2;	// sets hacker's current hoodie desk index as empty index

		    		events.push(Events(p.id,5,hackers[p.id-1].hoodie_queue_exit_time+hoodieDesks[emptyIndex2].service_time,0)); // creates hoodie desk exit event for hacker and pushes it to events priority queue

		    	}

		    	break;
		    }
		    default:
		    {


		    break;
			}

			
		
		}	
		hackathon=a.event_time;	// keeps last event's time

	}
    
   	int total_gift=0;				// keeps total gifts
   	float total_wait_sticker=0;		// keeps total sticker queue time
   	float total_wait_hoodie=0;		// keeps total hoodie queue time
   	int total_commit=0;				// keeps total commit number
   	int total_changed_line=0;		// keeps total changed lines
   	float total_waiting_time=0;		// keeps total wiaitng time
   	int total_invalid_sticker=0;	// keeps total invalid attempts casused by commit number
   	int total_invalid_gift=0;		// keeps total invalid attempts caused by gift number

   	float most_spend_queue=-1;		// keeps most sepended time on queues
   	int most_spend_queue_id=-1;		// keeps id of the hacker who has the most spended time on queues
   	float least_spend_queue=-1;		// keeps least spended time on queues
   	int least_spend_queue_id=-1;	// keeps id of the hacker who has the most spended time on queues
   	
   	for(int i=0;i<hackerNumber;i++)	//traverse hackers due to hacker number
   	{

   		if(hackers[i].gift_count==3)	// checks if the current hacker has 3 gift or not
   		{

   			least_spend_queue=hackers[i].total_sticker_queue_time+hackers[i].total_hoodie_queue_time;	// sets least spended time on queues as sum of total sticker queue time and total hoodie queue time
   			least_spend_queue_id=i+1; 	// sets id of the hacker 
   			break;
   		}
   	}


   
   	
   	for(auto &Hackers: hackers)
   	{	

   		total_gift+=Hackers.gift_count;							// total gifts equal sum of hackers' gifts

   		total_wait_sticker+=Hackers.total_sticker_queue_time;	// total waiting time on sticker queues equals sum of hackers' waiting times on queues

   		total_wait_hoodie+= Hackers.total_hoodie_queue_time;	// total waiting time on hoodie queues equals sum of hackers's waiting times on queues

   		total_commit+= Hackers.commit_count;					// total commit number equals sum of hackers' commit numbers

   		total_changed_line+=Hackers.total_changed_lines;		// total changed lines equals sum of hackers' changed lines

   		total_invalid_sticker+= Hackers.invalid_attempts;		// total invalid attempts caused by commit number equals hackers' total invalid attempts caused by commit number

   		total_invalid_gift+= Hackers.total_invalid_by_gift;		// total invalid attempts caused by gift number equals hackers' total invalid attempts caused by commit number

   		total_waiting_time+= Hackers.total_tour_time;			// total waiting time equals sum of hackers' total tour times


   		if(Hackers.total_sticker_queue_time+Hackers.total_hoodie_queue_time-most_spend_queue>0.00001)	// if the hacker's waiting time on queues is larger than most spended on queues
   		{

   			most_spend_queue=(Hackers.total_sticker_queue_time+Hackers.total_hoodie_queue_time);	// sets most spended on queues as sum of hacker's sticker queue time and hoodie queue time
   			most_spend_queue_id=Hackers.id;
   		}
		
		if(Hackers.gift_count==3)	// if the hacker's gift equals 3 or not
		{

			if(least_spend_queue-(Hackers.total_sticker_queue_time+Hackers.total_hoodie_queue_time)>0.00001)	// if the hacker's waiting time on queues is less than least spended on queues
			{

   				least_spend_queue=Hackers.total_sticker_queue_time+Hackers.total_hoodie_queue_time;	// sets least spended on queues as sum of hacker's sticker queue time and hoodie queue time
   				least_spend_queue_id=Hackers.id;
   			}
   		}

   	}


	write << setprecision(3) << fixed;

	write<<max_sticker_queue<<endl;
	write<<max_hoodie_queue<<endl;
	write<<(((float)total_gift)/hackerNumber)<<endl;
	write<< total_wait_sticker/total_gift<<endl;
	write<< total_wait_hoodie/total_gift<<endl;
	write<< ((float)total_commit)/hackerNumber<<endl;
	write<< ((float)total_changed_line)/total_commit<<endl;
	write<<total_timearound_time/total_gift<<endl;
	write<< total_invalid_sticker<<endl;
	write<< total_invalid_gift<<endl;
	write<< most_spend_queue_id<<" "<< most_spend_queue<<endl;
	write<< least_spend_queue_id<<" "<< least_spend_queue<<endl;
	write<< hackathon<<endl;

	//auto end = chrono::steady_clock::now();
	//auto diff = end - start;
	//cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;

   	return 0;

}
