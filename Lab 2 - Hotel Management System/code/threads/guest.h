
#include <vector>

class Guest{

private:

	int uid;
	int rooms;
	std::vector<int> roomNums;
	int nights;
	int checkIn;
	int checkOut;

public:

	int getUid()
	{
		return uid;
	}

	void setUid(int uid1)
	{
		uid = uid1;
	}

	int getRooms()
	{
		return rooms;
	}

	void setRooms(int rooms1)
	{
		rooms = rooms1;
	}

	int getNights()
	{
		return nights;
	}

	void setNights(int nights1)
	{
		nights = nights1;
	}

	std::vector<int> getRoomnums()
	{
		return roomNums;
	}

	void setRoomnums(std::vector<int> roomNums1)
	{
		roomNums = roomNums1;
	}

	int getCheckin()
	{
		return checkIn;
	}

	void setCheckin(int checkIn1)
	{
		checkIn = checkIn1;
	}

	int getCheckout()
	{
		return checkOut;
	}

	void setCheckout(int checkOut1)
	{
		checkOut = checkOut1;
	}
};