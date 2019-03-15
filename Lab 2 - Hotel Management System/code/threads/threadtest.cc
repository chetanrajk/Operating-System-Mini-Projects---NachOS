#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "guest.h"
#include "thread.h"
#include "list.h"
#include "bitmap.h"
#include <vector>


/*
void
SimpleThread(int which)
{
int num;

for (num = 0; num < 5; num++) {
printf("*** thread %d looped %d times\n", which, num);
kernel->currentThread->Yield();
}
}

*/

struct GueThrStruct {
	Guest* guObj;
	Thread *thr;
};


static int id = 5000;
static int currentDate = 1;
static std::vector<Bitmap*> availability;
static int notGrant = 0;
static int dailyOccu = 0;

List<GueThrStruct*>* Confirmed = new List<GueThrStruct*>();
List<GueThrStruct*>* Staying = new List<GueThrStruct*>();
List<GueThrStruct*>* CheckedOut = new List<GueThrStruct*>();
List<GueThrStruct*>* Discarded = new List<GueThrStruct*>();

void displayVector(std::vector<int> tempVect) {

	for (int i = 0; i < tempVect.size(); i++) {
		std::cout << tempVect[i] << "  ";
	}
}

bool checkAvailability(Guest* guObj) {

	//checking availability of room
	int roomsAvail;
	int tempN, chk;
	bool flag = true;
	tempN = guObj->getNights();
	chk = guObj->getCheckin() - 1;

	while (tempN > 0) {

		if (chk >= 10) {
			flag = false;

			break;
		}
		roomsAvail = availability[chk]->NumClear();

		if (roomsAvail < guObj->getRooms()) {
			flag = false;
			break;
		}
		//std::cout << "\n Disp...";
		chk++;
		tempN--;
	}
	return flag;
}

void assignRooms(Guest* guObj) {

	int tempN, chk;
	int index; 
	int tempRooms;
	std::vector<int> tempRoomSet;

	tempN = guObj->getNights();
	chk = guObj->getCheckin() - 1;

	while (tempN > 0) {

		tempRooms = guObj->getRooms();

		while (tempRooms > 0)
		{
			index = availability[chk]->FindAndSet();
			if (tempN == 1) {
				tempRoomSet.push_back(index+1);
			}
			tempRooms--;
		}

		chk++;
		tempN--;
	}

	guObj->setRoomnums(tempRoomSet);
	std::cout << "\n Assigned room numbers for this Guest: ";
	displayVector(tempRoomSet);

}


void GuestThread() {


	IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);

	Guest* guObj1 = new Guest();
	GueThrStruct* guthreadObj = new GueThrStruct();

	id++;

	std::cout << "\n==> Guest request thread created with following details: ";

	int rooms = rand() % 5 + 1;
	int nights = rand() % 4 + 1;
	int checkIn = rand() % 10 + 1;

	while (checkIn < currentDate) {

		checkIn = rand() % 10 + 1;

	}

	int checkOut = checkIn + nights;
	std::vector<int> roomNums;

	guObj1->setUid(id);
	guObj1->setRooms(rooms);
	guObj1->setNights(nights);
	guObj1->setCheckin(checkIn);
	guObj1->setCheckout(checkOut);

	// displaying guest details

	std::cout << "\n Id: " << id;
	std::cout << "\n Number of rooms: " << rooms;
	std::cout << "\n Number of nights: " << nights;
	std::cout << "\n CheckIn date: " << checkIn;
	std::cout << "\n CheckOut date: " << checkOut << "\n";
	
	bool flag = checkAvailability(guObj1);

	guthreadObj->thr = kernel->currentThread;

	if (flag == true) {

		std::cout << "\n Request granted for this Guest thread";
		
		// todo : assign room numbers
		assignRooms(guObj1);

		guthreadObj->guObj = guObj1;
	
		if (checkIn == currentDate) {
			std::cout << "\n Guest is checking-In today. Adding Guest to Staying List";
			std::cout << "\n";
			Staying->Append(guthreadObj);
			kernel->currentThread->Sleep(FALSE);
		}
		else {
			std::cout << "\n Adding Guest to Confirmed List";
			std::cout << "\n";
			Confirmed->Append(guthreadObj);
			kernel->currentThread->Sleep(FALSE);
		}
	}
	else {
		guthreadObj->guObj = guObj1;

		std::cout << "\n Request can not be granted for this Guest thread. Discarding thread.";
		std::cout << "\n Adding Guest to Discarded List";
		std::cout << "\n";
		notGrant++;
		Discarded->Append(guthreadObj);
		kernel->currentThread->Finish();
	}

	if (checkIn == currentDate) {

		std::cout << "\n ***Check-in date is reached for Guest:";
		std::cout << "\n \t ID: " << guthreadObj->guObj->getUid();
		std::cout << "\n \t No.of rooms: " << guthreadObj->guObj->getRooms();
		std::cout << "\n \t Reserved room numbers: "; displayVector(guthreadObj->guObj->getRoomnums());
																
		Staying->Append(guthreadObj);
		std::cout << "\n Added Guest to Staying List..!!\n";
		kernel->currentThread->Sleep(FALSE);
	}

	if (guthreadObj->guObj->getCheckout() == currentDate) {

		//std::cout << "\n Got it" << currentDate;
		std::cout << "\n ***Checkout date is reached for Guest-Id:  ";
		std::cout << "\n \t ID: " << guthreadObj->guObj->getUid();
		std::cout << "\n \t Room numbers returning: "; displayVector(guthreadObj->guObj->getRoomnums());
		CheckedOut->Append(guthreadObj);
		std::cout << "\n Added Guest to Checked-Out List and thread is Finished..!!\n";
		kernel->currentThread->Finish();
	}

	std::cout << "\n";
}

void Concierge() {

	std::cout << "\n ==== Hotel Reservation System ==== \n";
	int occupancy[10];
	
	IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);

	for (int i = 0; i < 10; i++) {
		Bitmap* bitObj = new Bitmap(30);
		availability.push_back(bitObj);
	}

	while (currentDate <= 10) {

	for (int i = 0; i < 5; i++) {

	Thread *t = new Thread("forked thread");
	t->Fork((VoidFunctionPtr)GuestThread, (void *)1);

	}

	// Check checkout date reached for any guests
	ListIterator<GueThrStruct*>* iter = new ListIterator<GueThrStruct*>(Staying);

	for (; !iter->IsDone(); iter->Next()) {
		if (iter->Item()->guObj->getCheckout() == currentDate) {
			
			kernel->scheduler->ReadyToRun(iter->Item()->thr);
		}
	}

	//Check whether checkin date reached for any guests
	ListIterator<GueThrStruct*>* iter1 = new ListIterator<GueThrStruct*>(Confirmed);

	for (; !iter1->IsDone(); iter1->Next()) {
		if (iter1->Item()->guObj->getCheckin() == currentDate) {

			kernel->scheduler->ReadyToRun(iter1->Item()->thr);
		}
	}

	kernel->currentThread->Yield();

	dailyOccu = (30 - (availability[currentDate-1]->NumClear()));
	occupancy[currentDate - 1] = dailyOccu;

	currentDate++;
	}

	if (!Staying->IsEmpty()) {

	std::cout << "\n ***  11th day - Checking out all staying guests...!!!\n";
	// Checkout remaining guests
	ListIterator<GueThrStruct*>* iter2 = new ListIterator<GueThrStruct*>(Staying);

	for (; !iter2->IsDone(); iter2->Next()) {
		if (iter2->Item()->guObj->getCheckout() == currentDate) {
			kernel->scheduler->ReadyToRun(iter2->Item()->thr);
		}
	}
	}

	kernel->currentThread->Yield();

	double notGrantPe = ((50 - notGrant) / (double)50);

	std::cout << "\n ====> Summary of Hotel Reservation System <==== \n";

	std::cout << "\n\t Total Number of requests over 10 days:  50";
	std::cout << "\n\t Number of requests successfully granted:  " << (50 - notGrant);
	std::cout << "\t Number of requests not granted:  " << notGrant;
	std::cout << "\n\t Request Granting rate:  " << (notGrantPe * 100) << " %";

	std::cout << "\n\t Occupancy for each day is as follows:";
	for (int i = 0; i < 10; i++) {
		std::cout << "\n\tDay " << i + 1 << " : " << occupancy[i];
	}

	kernel->currentThread->Yield();
	std::cout << "\n\t**** End of System **** \n";
}

void
ThreadTest()
{
    Thread *t = new Thread("forked thread");
    t->Fork((VoidFunctionPtr)Concierge, (void *) 1);
    //SimpleThread(0);
}


