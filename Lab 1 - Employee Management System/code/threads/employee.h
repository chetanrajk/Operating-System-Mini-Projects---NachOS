
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
};