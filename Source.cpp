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
	bool castvote() {

	}
};
class candidate
{
	string name;
	string party;
	int votes;
public:
	candidate()
	{
		name = " ";
		party = " ";
		votes = 0;
	}
	candidate(string n, string p, int v)
	{
		name = n;
		party = p;
		votes = v;
	}
	candidate(candidate& c)
	{
		name = c.name;
		party = c.party;
		votes = c.votes;
	}
	void operator =(candidate c)
	{
		this->name = c.name;
		this->party = c.party;
		this->votes = c.votes;
	}
	void setname(string n)
	{
		name = n;
	}
	void setparty(string p)
	{
		party = p;
	}
	string getname() const
	{
		return name;
	}
	string getparty() const
	{
		return party;
	}
	int getvotes() const
	{
		return votes;
	}
};
class administrator :public user {
public:
	administrator() {}
	administrator(string n, int p, long long c) :user(n, p, c) {}
	bool registeration() override {
		string n; int p; long long c;
		cout << "Enter name of admin"; cin >> n;
		cout << "Enter password"; cin >> p;
		cout << "Enter cnic"; cin >> c;
		ofstream file("admin.txt", ios::app);
		if (file) {
			file << n << " " << p << " " << c << "\n";
			return true;
		}
		return false;

	}
	bool login() override {
		ifstream file("admin.txt");
		string sn; int sp; long long sc;
		if (file) {
			string n; int p; long long c;
			cout << "Enter your name "; cin >> n;
			cout << "Enter password "; cin >> p;
			cout << "Enter your CNIC "; cin >> c;
			while (file >> sn >> sp >> sc) {
				if (n == sn && p == sp && c == sc) {
					return true;
				}
			}
		}
		else if (!file) {
			bool q = registeration();
			if (q) {
				cout << "Admin is register" << endl;
				return true;
			}
			else cout << "Registeration failed" << endl;
		}
		else return false;
	}
	void createElection(string electionName) {
		string filename = electionName + ".txt";
		ofstream file(filename);
		if (file) {
			cout << "Election created successfully." << endl;
			file.close();
		}
		else {
			cout << "Error creating election." << endl;
		}
	}
	void addcandidate(string filename) {
		candidate  c;
		ofstream file(filename, ios::app);
		if (file) {
			string n; string p; string r;
			cout << "Enter Candidate name"; cin >> n;
			cout << "Enter Party affiliation"; cin >> p;
			cout << "Enter the region in which in which candidate is running"; cin >> r;
			c.setname(n); c.setparty(p);
			file << c.getname() << " " << c.getparty() << " " << r << " " << c.getvotes() << "\n";
			file.close();
			cout << "Candidate added successfully." << endl;
		}
		else {
			cout << "Error adding candidate." << endl;
		}
	}
	void displayresult(string filename) {
		string region;
		cout << "Enter region name you wanna see result"; cin >> region;
		ifstream file(filename);
		string name, party, reg;
		if (file) {
			while (file >> name >> party >> reg) {
				if (reg == region) {
					cout << name << "    " << party << endl;
				}
			}
			file.close();
		}
		else {
			cout << "Error reading candidates." << endl;
		}
	}
};
class election {
public:

	virtual void displaycandidates() = 0;
	virtual void Result() = 0;
};
class LocalElection :public election {
	voters v;
public:
	void displaycandidates() override {
		string n, r;
		int p;
		long long c;
		ifstream file("voters.txt");
		if (file >> n >> p >> c >> r) {
			ifstream infile("candidates.txt");
			if (infile) {
				string name, party, region;
				int votes;
				cout << "The Candidates of " << r << " are following:\n";
				int i = 1;
				while (infile >> name >> party >> region >> votes) {
					if (region == r) {
						cout << i << ". " << name << " (" << party << ")" << endl;
						i++;
					}
				}
				if (i == 1) {
					cout << "No candidates found in this region." << endl;
				}
			}
			else {
				cout << "Error reading candidates file." << endl;
			}
		}
		else {
			cout << "Error reading voters file." << endl;
		}
	}
	void Result() {}
};
int main() {
	int mainChoice;

	do {
		cout << "\n========== Online Voting System ==========\n";
		cout << "1. Register as Voter\n";
		cout << "2. Login as Voter\n";
		cout << "3. Login as Admin\n";
		cout << "4.Login as Candidate\n";
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
					cout << "1.View Elections \n";
					cout << "2.Logout\n";
					cin >> choice;
					switch (choice)
					{
					case 1:
						int election;
						cout << "1. Local Elections\n";
						cout << "2. National Elections\n";
						cout << "3. Exit\n";
						cout << "Choose election \n ";
						cin >> election;
						if (election == 1) {
							int choose;
							cout << "1. Cast your vote\n";
							cout << "2.View your vote status\n";
							cout << "3.View Vote Statistics\n";
							cout << "4. Exit\n";
							cin >> choose;
							do {
								if (choose == 1) {

									LocalElection l;
									l.displaycandidates();
								}
								else if (choose == 2) {}
								else if (choose == 3) {}
								else break;
							} while (choose == 4);
						}
						else if (election == 2) {
							int choose1;
							cout << "1. Cast your vote\n";
							cout << "2.View your vote status\n";
							cout << "3.View Vote Statistics\n";
							cout << "4. Exit\n";
							cin >> choose1;
							do {
								if (choose1 == 1) {}
								else if (choose1 == 2) {}
								else if (choose1 == 3) {}
								else break;
							} while (choose1 == 4);
						}
						else break;
						break;
					case 2: break;
					}
				} while (choice == 2);
			}
			else {
				cout << "Login Failed";
			}
		}
		else if (mainChoice == 3) {
			administrator A;
			bool status = A.login();
			if (status) {
				ifstream localFile("Local.txt");
				ifstream nationalFile("National.txt");

				if (!localFile.is_open() || !nationalFile.is_open()) {
					string ename;
					cout << "Elections not created. Please create elections." << endl;
					cout << "Enter election name";
					cin >> ename;
					A.createElection(ename);
					A.createElection(ename);
				}

				int choice1;
				cout << "1. Local elections \n";
				cout << "2. National elections\n ";
				cin >> choice1;

				switch (choice1) {
				case 1: {
					ofstream file("Local.txt", ios::app);
					if (file) {
						int choice;
						cout << "1. Add Candidates" << endl;
						cout << "2. Display Results" << endl;
						cin >> choice;
						switch (choice) {
						case 1:
							A.addcandidate("Local.txt");
							break;
						case 2:
							A.displayresult("Local.txt");
							break;
						}
					}
					else {
						cout << "Error opening Local.txt" << endl;
					}
					file.close();
					break;
				}

				case 2: {
					ofstream file("National.txt", ios::app);
					if (file) {
						int choice;
						cout << "1. Add Candidates" << endl;
						cout << "2. Display Results" << endl;
						cin >> choice;
						switch (choice) {
						case 1:
							A.addcandidate("National.txt");
							break;
						case 2:
							A.displayresult("National.txt");
							break;
						}
					}
					else {
						cout << "Error opening National.txt" << endl;
					}
					file.close();
					break;
				}
				}
			}
			else {
				cout << "Login failed" << endl;
			}
		}
	} while (mainChoice == 5);
}