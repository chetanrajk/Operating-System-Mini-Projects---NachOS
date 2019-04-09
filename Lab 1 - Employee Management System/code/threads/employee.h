
class Employee{
   
public:
	int getEmpid() { return empId; }
	void setEmpid(int empId1) { empId = empId1; }
	std::string getName() { return name; }
	void setName(std::string name1) { name = name1; }
	std::string getPosition() { return position; }
	void setPosition(std::string position1) { position = position1; }
	std::string getDepartment() { return department; }
	void setDepartment(std::string department1) { department = department1;}
	double getPay() { return pay; }
	void setPay(double pay1) { pay = pay1; }
	double getWeekHrs() { return weekHrs; }
	void setWeekHrs(double weekHrs1) { weekHrs = weekHrs1; }
	double getWeekPay() { return weekPay; }
	void setWeekPay(double weekPay1) { weekPay = weekPay1; }
	bool operator==(Employee& empObj );

private:
	int empId;
	std::string name;
	std::string position;
	std::string department;
	double pay;
	double weekHrs;
	double weekPay;

	int uid;
	int rooms;
	std::vector<int> roomNums;
	int checkIn;
	int checkOut;

	public int getUid()
	{
		return this.uid;
	}

	public void setUid(int uid)
	{
		this.uid = uid;
	}

	public int getRooms()
	{
		return this.rooms;
	}

	public void setRooms(int rooms)
	{
		this.rooms = rooms;
	}

	public std::vector<int> getRoomnums()
	{
		return this.roomNums;
	}

	public void setRoomnums(std::vector<int> roomNums)
	{
		this.roomNums = roomNums;
	}

	public int getCheckin()
	{
		return this.checkIn;
	}

	public void setCheckin(int checkIn)
	{
		this.checkIn = checkIn;
	}

	public int getCheckout()
	{
		return this.checkOut;
	}

	public void setCheckout(int checkOut)
	{
		this.checkOut = checkOut;
	}

};