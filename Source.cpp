#include<iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
class user {
private:
	string name;
	int password;
	long long cnic;
public:
	user() {
		name = " ";
		password = 0;
		cnic = 0;
	}
	user(string n, int p, long long c) {
		name = n;
		password = p;
		cnic = c;
	}
	user(user& u) {
		name = u.name;
		password = u.password;
		cnic = u.cnic;
	}
	string getname() { return name; }
	int getpassword() { return password; }
	long long getcnic() { return cnic; }
	virtual bool registeration() = 0;
	virtual bool login() = 0;
};
class voters :public user {
private:
	string regions[4] = { "Lahore","Gujranwala","Multan","Faislabad" };
	string region;
	int age;
public:
	voters() { region = ""; age = 0; }
	voters(string n, int p, int a, long long c) :user(n, p, c) {
		age = a;
		int lastdigit = c % 10;
		region = regions[lastdigit % 4];

		bool status = registeration();
		if (status == true) {
			cout << "Registered successfully" << endl;
		}
		else cout << "Registration Failed" << endl;
	}
	bool registeration() override {
		string n = getname(); int p = getpassword(); long long c = getcnic();
		ofstream file("voters.txt", ios::app);
		if (file) {
			file << n << " " << p << " " << c << " " << region << "\n";
			return true;
		}
		return false;
	}
	bool login() override {
		string n; int p; long long c;
		cout << "Enter your name "; cin >> n;
		cout << "Enter password "; cin >> p;
		cout << "Enter your CNIC "; cin >> c;
		ifstream file("voters.txt");
		string sn, sr; int sp; long long sc;
		if (file) {
			while (file >> sn >> sp >> sc >> sr) {
				if (n == sn && p == sp && c == sc) {
					return true;
				}
			}
		}
		else return false;
	}
};

int main() {
	int mainChoice;

	do {
		cout << "\n========== Online Voting System ==========\n";
		cout << "1. Register as Voter\n";
		cout << "2. Login as Voter\n";
		cout << "3. Login as Admin\n";
		cout << "4.Enter as Candidate\n";
		cout << "5. Exit\n";
		cout << "Enter choice: ";
		cin >> mainChoice;
		if (mainChoice == 1) {
			string name; int password; int age;  long long cnic;
			cout << "Enter your name ";
			cin >> name;
			cout << "Enter your age ";
			cin >> age;
			if (age >= 18) {
				cout << "Enter password ";
				cin >> password;
				cout << "Enter your CNIC ";
				cin >> cnic;
				ifstream file("voters.txt");
				string sn, sr; int sp; long long sc;
				bool alreadyRegistered = false;
				if (file) {
					while (file >> sn >> sp >> sc >> sr) {
						if (cnic == sc) {
							alreadyRegistered = true;
							break;
						}
					}
					file.close();
					if (alreadyRegistered == true) {
						cout << "You have already been registered " << endl;
					}
					else {
						voters v(name, password, age, cnic);
					}
				}
			}
			else cout << "You are not eligible for voting" << endl;
		}
		else if (mainChoice == 2) {
			voters v;
			bool status = v.login();
			if (status == true) {
				cout << "Login successfull" << endl;
				int choice;
				do
				{
					cout << "Select the Election \n";
					cout << "1.National Election\n";
					cout << "2.Local Election\n";
					cout << "3.Logout\n";
					cin >> choice;
					switch (choice)
					{

					}
				} while (choice == 3);
			}
			else {
				cout << "Login Failed";
			}
		}
	} while (mainChoice == 5);
}