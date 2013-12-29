#include <boost/python.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


using namespace boost::python;

class UserBase {

public:
static UserBase& getInstance() {
	static UserBase instance;
	return instance;
}


void addUser(std::string userName) {
	std::ofstream myfile;
  	myfile.open("users.txt", std::ios_base::app);
  	myfile << userName << "\n";
}


std::string showUsers() {
	std::string line, userList;
	std::ifstream myfile ("users.txt");
  	if (myfile.is_open())
  	{
    	while ( getline (myfile,line) ) userList += line + " ";
    }
    myfile.close();
	return userList; 
}

};

void addToBase(std::string user) {
	UserBase::getInstance().addUser(user);
}

std::string showBase() {
	return UserBase::getInstance().showUsers();
}

BOOST_PYTHON_MODULE(userbase)
{
    def("addToBase", addToBase);
	def("showBase", showBase);
};



