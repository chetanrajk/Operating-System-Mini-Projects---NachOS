#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "employee.h"
#include "list.h"
#include <fstream>
#include <sstream>

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("\n*** thread %d looped %d times", which, num);
        kernel->currentThread->Yield();
    }
}

// Function for writing records to employee.dat file
void writFile(List<Employee*> *empList) {

	Employee* emObj = new Employee();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);

	std::ofstream outfile("../threads/employee.dat");

	outfile.flush();

	for (; !iter->IsDone(); iter->Next()) {
		outfile << iter->Item()->getEmpid() << "\t" << iter->Item()->getName() << "\t" << iter->Item()->getPosition() << "\t" << iter->Item()->getDepartment() << "\t" << iter->Item()->getPay() << std::endl;
	}
}

// Function for entering record
void enterRecord(List<Employee*>* empList, int id)
{
	Employee* emObj = new Employee();
	std::string name;
	std::string position;
	std::string department;
	double pay;

	std::cout << "\n\tEnter new employee record with following details: \n";
	std::cout << "\n\tName: "; 
	std::cin >> name;
	std::cout << "\tPosition:";
	std::cin >> position;
	std::cout << "\tDepartment:";
	std::cin >> department;
	std::cout << "\tPay:";
	std::cin >> pay;

	emObj->setEmpid(id);
	emObj->setName(name);
	emObj->setPosition(position);
	emObj->setDepartment(department);
	emObj->setPay(pay);

	empList->Append(emObj);
	writFile(empList);

	std::cout << "\n\tNew Employee record added successfully...!!! \n";
}

// Function for display records
void PrintRecord(Employee* rec) {

	Employee* emObj = new Employee();
	emObj = rec;

	std::cout << "\n\n\t" << emObj->getEmpid() << "\t\t" << emObj->getName() << "\t\t" << emObj->getPosition() << "\t\t" << emObj->getDepartment() << "\t\t" << emObj->getPay();
}

// Function for sort employees
int sortComparer(Employee* emp1, Employee* emp2) 
{
	if (emp1->getDepartment() == emp2->getDepartment())
	{
		return emp1->getPosition().compare(emp2->getPosition());
	}
	else 
	{
		return emp1->getDepartment().compare(emp2->getDepartment());
	}
}

// Function for display
void display(List<Employee*> *empList)
{
	SortedList<Employee*> *tempList = new SortedList<Employee*>(sortComparer);
	//List<Employee*> *tempList = new List<Employee*>();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);

	for (; !iter->IsDone(); iter->Next()) {
		tempList->Insert(iter->Item());
	}

	std::cout << "\n\n\tID" << "\t\tName" << "\t\tPosition" << "\tDepartment" << "\tPay Rate";

	tempList->Apply(PrintRecord);
}

// Function for searching
void searchEmp(List<Employee*> *empList)
{
	int ch;  bool flag;
	int cont = 1;
	while (cont == 1) {

	Employee* emObj = new Employee();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);
	
	flag = false;
	std::cout << "\n\n\tSearch Employee(s) by: ID (0) or Department(1)\n";
	std::cout << "\n\tEnter your choice: ";
	std::cin >> ch;

	if (ch == 0) {
		int id;
		std::cout << "\n\tEnter employee ID to search: ";
		std::cin >> id;
		
		for (; !iter->IsDone(); iter->Next()) {
			if (iter->Item()->getEmpid() == id) {
				flag = true;
				emObj = iter->Item();
				std::cout << "\n\n\t==> Result:";
				std::cout << "\n\tID: " << emObj->getEmpid();
				std::cout << "\n\tName: " << emObj->getName();
				std::cout << "\n\tPosition: " << emObj->getPosition();
				std::cout << "\n\tDepartment: " << emObj->getDepartment();
				std::cout << "\n\tHourly Pay Rate: " << emObj->getPay();
				break;
			}
		}
		if (!flag) {
			std::cout << "\n\t ...Enterned ID not exists in database...";
		}
	}
	else if(ch == 1){
		std::string deptIn;
		std::cout << "\n\tEnter department name: ";
		std::cin >> deptIn;
		const char *dept = deptIn.c_str();
		std::cout << "\n\n\t==> Result:";

		for (; !iter->IsDone(); iter->Next()) {

			if (strcmp(iter->Item()->getDepartment().c_str(),dept) == 0) {
				flag = true;
				emObj = iter->Item();
				std::cout << "\n\n\tID: " << emObj->getEmpid();
				std::cout << "\n\tName: " << emObj->getName();
				std::cout << "\n\tPosition: " << emObj->getPosition();
				std::cout << "\n\tDepartment: " << emObj->getDepartment();
				std::cout << "\n\tHourly Pay Rate: " << emObj->getPay();
			}
		}
		if (!flag) {
			std::cout << "\n\t ...Enterned Department not exists in database...";
		}
	}
	else {
		std::cout << "\tSorry wrong input... \n";
	}

	std::cout << "\n\n\tDo you want to continue search: Yes(1) or No(0) : ";
	std::cin >> cont;
	}
}

// Function for update records
void updateEmp(List<Employee*> *empList)
{
	int id; bool flag;
	std::cout << "\n\tEnter ID of employee to update: ";
	std::cin >> id;
	flag = false;

	Employee* emObj = new Employee();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);

	for (; !iter->IsDone(); iter->Next()) {
		if (iter->Item()->getEmpid() == id) {
			flag = true;
			std::string name;
			std::string position;
			std::string department;
			double pay;

			std::cout << "\n\tEnter new information to update record: \n";
			std::cout << "\n\tName: ";
			std::cin >> name;
			std::cout << "\tPosition:";
			std::cin >> position;
			std::cout << "\tDepartment:";
			std::cin >> department;
			std::cout << "\tPay:";
			std::cin >> pay;

			iter->Item()->setName(name);
			iter->Item()->setPosition(position);
			iter->Item()->setDepartment(department);
			iter->Item()->setPay(pay);
		}
	}
	if (flag) {
		writFile(empList);
		std::cout << "\n\tEmployee details has been updated..: \n";
	}
	else {
		std::cout << "\n\tError: Id not found.. \n";
	}

}

// Function for delete records
void deleteEmp(List<Employee*> *empList)
{
	int id; bool flag;
	std::cout << "\n\tEnter ID of employee to delete: ";
	std::cin >> id;
	flag = false;
	int ch;

	Employee* emObj = new Employee();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);

	for (; !iter->IsDone(); iter->Next()) {
		if (iter->Item()->getEmpid() == id) {
			flag = true;
			std::cout << "\n\tDo you want to delete. Please confirm:  Yes(1) or No(0) : ";
			std::cin >> ch;
			if (ch == 1) {
				empList->Remove(iter->Item());
				std::cout << "\n\tEmployee record deleted successfully...!!! \n";
			}
			else {
				std::cout << "\n\t Please continue with next operation...";
			}
		}
	}
	if (!flag) {
		std::cout << "\n\t ...Enterned Employee ID not exists in database...";
	}
	else {
		writFile(empList);
	}
}

// Function for display paycheck
void PrintPaycheck(Employee* working) {

	Employee* emObj = new Employee();
	emObj = working;
	static int orderno = 1;

	std::cout << "\n\n\t" << orderno << "\t\t" << emObj->getEmpid() << "\t\t" << emObj->getName() << "\t\t" << emObj->getWeekHrs() << "\t\t" << emObj->getWeekPay();

	orderno++;
}

// Function for compare weekly pay 
int payComparer(Employee* emp1, Employee* emp2)
{
	if (emp1->getWeekPay() > emp2->getWeekPay()) {
		return -1;
	}
	else if (emp1->getWeekPay() == emp2->getWeekPay()) {
		return 0;
	}
	else {
		return 1;
	}
}

// Function for schedule weekly jobs
void scheduleJobs(List<Employee*> *empList)
{
	double weekHrs;
	double weekPay;
	double totalPay = 0.00;

	Employee* emObj = new Employee();
	ListIterator<Employee*>* iter = new ListIterator<Employee*>(empList);

	SortedList<Employee*> *tempList = new SortedList<Employee*>(payComparer);

	for (; !iter->IsDone(); iter->Next()) {

		emObj = iter->Item();
		weekHrs = rand() % 21 + 20;
		weekPay = weekHrs * emObj->getPay();
		totalPay = totalPay + weekPay;

		emObj->setWeekHrs(weekHrs);
		emObj->setWeekPay(weekPay);

		tempList->Insert(emObj);
	}

	std::cout << "\n\n\tOrder#" << "\t\tID" << "\t\tName" << "\t\tWeek Hours" << "\tWeekly Paycheck";
	tempList->Apply(PrintPaycheck);

	std::cout << "\n\n\t Total amount of Paycheck for the week:  " << totalPay;
}

// Function for handling all employee management system
void EmployeeManag()
{
	std::cout << "\n ==== Employee Management ==== \n";
	
	List<Employee*> *empList = new List<Employee*>();
	int ch;
	//empObj.display();
	static int id = 8000;

	std::ifstream infile("../threads/employee.dat");
	std::string line;

	while (std::getline(infile, line)) {

		Employee* empObj = new Employee();
		//std::cout << line << "\n";

		std::stringstream   streamLn(line);

		std::string empId; int idt;
		std::string name;
		std::string position;
		std::string department;
		std::string payStr; double pay;

		std::getline(streamLn, empId, '\t');
		
		std::istringstream(empId) >> idt;
		std::getline(streamLn, name, '\t');
		std::getline(streamLn, position, '\t');
		std::getline(streamLn, department, '\t');
		std::getline(streamLn, payStr, '\t');
		std::istringstream(payStr) >> pay;

		empObj->setName(name);
		empObj->setEmpid(idt);
		empObj->setPosition(position);
		empObj->setDepartment(department);
		empObj->setPay(pay);
		
		//std::cout << empObj.getEmpid() << empObj.getName() << empObj.getPosition() << empObj.getDepartment() << empObj.getPay() << "\n";
		empList->Append(empObj);
		id++;
	}

	int cont = 1;

	while (cont == 1) {

	std::cout << "\n\n   ***** Menu: *****\n\n";
	std::cout << "   1.	Enter new record\n";
	std::cout << "   2.	Display all employees \n";
	std::cout << "   3.	Search employee(s) \n";
	std::cout << "   4.	Update employee info \n";
	std::cout << "   5.	Delete employee info \n";
	std::cout << "   6.	Schedule weekly jobs \n";
	std::cout << "   7.	Exit\n";

	//enterRecord(empList);
	//display(empList);
	//searchEmp(empList);

	std::cout << "\n\tEnter choice from above menu : ";
	std::cin >> ch;
	std::cout << "\n";

	
	switch (ch)
	{
		case 1: std::cout << "\n\t ==> Enter new Record <==..\n";
			    id++;
				enterRecord(empList, id);
				break;

		case 2: std::cout << "\n\t ==> Display Records <==..\n";
				display(empList);
				break;

		case 3: std::cout << "\n\t ==> Search employee(s) <==..\n";
				searchEmp(empList);
				break;

		case 4: std::cout << "\n\t ==> Update employee <==..\n";
			    updateEmp(empList);
				break;

		case 5: std::cout << "\n\t ==> Delete employee <==..\n";
				deleteEmp(empList);
				break;

		case 6: std::cout << "\n\t ==> Schedule Weekly Paycheck <==..\n";
				scheduleJobs(empList);
				break;

		case 7: std::cout << "\n\t..... Exit .....\n";
				writFile(empList);
				cont = 0;
				break;

		default:std::cout << "\n...Wrong input from menu..."; 
				break;
	}

	}
}

void
ThreadTest()
{
    Thread *t = new Thread("forked thread");
    t->Fork((VoidFunctionPtr)EmployeeManag, (void *) 1);
    //SimpleThread(0);
}
