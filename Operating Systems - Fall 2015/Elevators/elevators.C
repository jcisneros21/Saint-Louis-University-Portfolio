 /*
 * elevators.C
 *   Multithreaded elevator simulation
 *   usage: elevators [-e nelevators] [-s speed] [-g]
 * 
 *   Discovered issue with running elevator in /os/bin/ with a speed of .001 - elevators gets stuck, program doesn't end
 *   Issue always occurs with 100 people, and often occurs with 10 people
 *   More "stuck" issues occur when elevators are run with graphics, such as running with a speed of .01, 10 elevators, and 100 people
 *
 *   Jessie Cisneros and Joey Gao 10/14/15
 */
 
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include "building.h"
#include "elevators.h"
#include <semaphore.h>
#include <assert.h>
#include <algorithm>

using namespace std;

/* class: Rider
 *    Class for elevator rider objects
 */

class Rider
{
private:
  int sourceFloor;
  int destinationFloor;
  const Person *who;
  sem_t semaphore;
    
public:
  Rider()
  {
    sourceFloor = 0;
    destinationFloor = 0;
    sem_init(&semaphore,0,0);
  }
  
  int getSourceFloor()
  {
    return sourceFloor;
  }
  int getDestinationFloor()
  {
    return destinationFloor;
  }
  const Person* getPerson()
  {
    return who;
  }
  
  void setSourceFloor(int num)
  {
    sourceFloor = num;
  }
  void setDestinationFloor(int num)
  {
    destinationFloor = num;
  }
  void setPerson(const Person *other)
  {
    who = other;
  }
  void semWait()
  {
    sem_wait(&semaphore);
  }
  void semPost()
  {
    sem_post(&semaphore);
  }
};

/* class: Waitlist
 *    Class for data structure that holds pending elevator riders
 */

class Waitlist {
public:
  Waitlist();
  vector<Rider*> board_floor_of_first_rider_in_waitlist();
  vector<Rider *> board_riders_on_specific_floor(int floor);
  void add_rider_to_waitlist(Rider *it);
  vector<Rider *> remove_from_waitlist_and_board_elevator(int floorToBoardFrom);
  
private:
  vector<Rider *> waitlistVector;
  pthread_mutex_t lock; // protects waitlistVector
  sem_t Qsize;
  bool grabbingNext;;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
};

Waitlist::Waitlist()
{
  grabbingNext = false;
  pthread_mutex_init(&lock,NULL);
  sem_init(&Qsize,0,0);
}

/* function: board_floor_of_first_rider_in_waitlist
 *    Returns an array of the riders on the floor of the first rider on the waitlist
 *    Removes those riders from the waitlist
 */

vector<Rider*> Waitlist::board_floor_of_first_rider_in_waitlist()
{
  sem_wait(&Qsize); // Wait until a rider has been added to the waitlist (sem_post called by add_rider_to_waitlist)
  
  vector<Rider*> ridersToBoard; 
  
  pthread_mutex_lock(&lock);
  
  while (waitlistVector.size() == 0 || grabbingNext == true) // If waitlist is empty, wait; OR If another elevator is grabbing the first rider's floor, wait
  {
    pthread_cond_wait(&cond, &lock);
  }
    
  grabbingNext = true; // Got past the previous wait loops, which means it's my turn
  
  // Push everyone on waitlist that is on the first rider's floor to riderToBoard; erase them from the waitlist
  int first = waitlistVector[0]->getSourceFloor();
  
  for (int i = 0; i < waitlistVector.size(); i++)
  {
    if (waitlistVector[i]->getSourceFloor() == first)
    {
      ridersToBoard.push_back(waitlistVector[i]);	// push onto elevator
      waitlistVector.erase(waitlistVector.begin() + i); // erase from waitlist
      i--;
    }
  }
  
  grabbingNext = false; // I'm done grabbing the first rider from the waitlist
  
  if (waitlistVector.size() != 0) pthread_cond_broadcast(&cond); // Broadcast boarding of first rider's floor to other elevators
  
  pthread_mutex_unlock(&lock);
  
  return ridersToBoard;
}

/* function: board_riders_on_specific_floor
 *    Returns an array of the riders on a specific floor
 *    Removes those riders from the waitlist
 *    inputs: specific floor to board
 */

vector<Rider *>Waitlist::board_riders_on_specific_floor(int specificFloor)
{  
  vector<Rider *> ridersOnSpecificFloor;
    
  pthread_mutex_lock(&lock);
    
  // Push everyone on that is specificFloor to ridersOnSpecificFloor; erase them from the waitlist
  for (int i = 0; i < waitlistVector.size(); i++)
  {
    if (waitlistVector[i]->getSourceFloor() == specificFloor)
    {
      ridersOnSpecificFloor.push_back(waitlistVector[i]);
      waitlistVector.erase(waitlistVector.begin() + i);
      i--;
    }
  }
  
  if (waitlistVector.size() != 0) pthread_cond_broadcast(&cond); // Broadcast boarding specific floor to other elevators
  
  pthread_mutex_unlock(&lock);
  
  return ridersOnSpecificFloor;
}

/* function: add_rider_to_waitlist
 *    Adds a rider to the waitlist
 *    inputs: Rider to add
 */

void Waitlist::add_rider_to_waitlist(Rider *it)
{
  pthread_mutex_lock(&lock);

  waitlistVector.push_back(it);
  
  if (waitlistVector.size() != 0) pthread_cond_broadcast(&cond); // Broadcast adding a rider to other elevators

  pthread_mutex_unlock(&lock);
  
  sem_post(&Qsize);
}


Waitlist waitlistStruct;

// Elevator constructor
//   Called once for each elevator before the thread is created.
//
Elevator::Elevator()
{ 
}

//
// Elevator::display_passengers()
//
//  Call display() for each Person on the elevator.
//  Return the number of riders.
//
//  Beware: calling message() from this function will hang the simulation.
//
int Elevator::display_passengers()
{
  for (int i = 0; i < onboard.size(); i++)
  {
    onboard[i]->getPerson()->display();
  }
  
  return onboard.size();
}

//
// Elevator::run()
//
//   Main thread for an elevator.
//   Will be called at the beginning of the simulation, to put the
//   Elevator into operation.  run() should pick up and deliver Persons,
//   coordinating with other Elevators for efficient service.
//   run should never return.
//   
void Elevator::run() 
{
  // Pick up and drop off passengers.  
  message("running");
  while (1)
  {
    int direction = 0; // 0 is going up, 1 is going down
   
    vector<Rider*> firstRiders = waitlistStruct.board_floor_of_first_rider_in_waitlist();
    move_to_floor(firstRiders[0]->getSourceFloor());
    open_door();
    onboard.insert(onboard.end(), firstRiders.begin(), firstRiders.end());
    close_door();
   
    if (firstRiders[0]->getDestinationFloor() < onfloor())
    {
      direction = 1;
    }
    
    if (firstRiders[0]->getDestinationFloor() > onfloor())
    {
      direction = 0;
    }

    while (onboard.size() != 0)
    {
      // Move elevator one floor up or down
      if (onfloor() != 10 && onfloor() != 0)
      {
	if (direction == 0)
	{
	  move_up();
	}
	else // direction is 1
	{
	  move_down();
	}
      }
      else if (onfloor() == 10)
      {
	direction = 1;
	move_down();
      }
      else if (onfloor() == 0)
      {
	direction = 0;
	move_up();
      }
      
      // Boarding
      vector<Rider *> ridersOnFloor = waitlistStruct.board_riders_on_specific_floor(onfloor());
      if (ridersOnFloor.size() > 0)
      {
	if (door_is_open() == false) open_door();
	onboard.insert(onboard.end(), ridersOnFloor.begin(), ridersOnFloor.end());
      }
      
      // Debarking
      for (int i = 0; i < onboard.size(); i++)
      {
	if (onfloor() == onboard[i]->getDestinationFloor())
	{
	  if (door_is_open() == false) open_door();
	  onboard[i]->semPost();
	  onboard.erase(onboard.begin() + i);
          i--;
	}
      }
      if (door_is_open() == true) close_door();
    }
  }
}

//
//  take_elevator
//
//    A Person (who) calls this function to take an elevator from their
//    current floor (origin) to a different floor (destination).
//
void take_elevator(const Person *who, int origin, int destination) // DONT CHANGE THIS
{
  // Should not return until the person has taken an elevator
  // to their destination floor.
  Rider newRider;
  newRider.setSourceFloor(origin);
  newRider.setDestinationFloor(destination);
  newRider.setPerson(who);
  waitlistStruct.add_rider_to_waitlist(&newRider);
  newRider.semWait();
}

