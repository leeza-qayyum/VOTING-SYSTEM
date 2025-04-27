#include<iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
string stop = "\033[0m";
string crimson = "\033[38;5;197m";
string green = "\033[32m";
bool c = false;
bool c1 = false;
class user {
protected:
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
	string region, status, status1;
	int age;
	long long loggedINcnic;
public:
	voters() { region = ""; age = 0; status = "Null"; status1 = "Null"; loggedINcnic = 0; }
	voters(string n, int p, int a, long long c) :user(n, p, c) {
		age = a;
		int lastdigit = c % 10;
		status = "Null";
		status1 = "Null";
		region = regions[lastdigit % 4];
		loggedINcnic = 0;
		bool status = registeration();
		if (status == true) {
			cout << green << "Registered successfully" << stop << endl;
		}
		else cout << crimson << "Registration Failed" << stop << endl;
	}
	bool registeration() override {
		ofstream file("voters.txt", ios::app);
		if (file) {
			file << name << " " << password << " " << cnic << " " << region << " " << status << " " << status1 << endl;
			return true;
		}
		return false;
	}
	bool login() override {
		string n; int p; long long c;
		cout << "Enter your name "; cin >> n;
		cout << "Enter password "; cin >> p;
		cout << "Enter your CNIC "; cin >> c;
		loggedINcnic = c;
		ifstream file("voters.txt");
		string sn, sr; int sp; long long sc; string stat, stat1;
		if (file) {
			while (file >> sn >> sp >> sc >> sr >> stat >> stat1) {
				if (n == sn && p == sp && c == sc) {
					return true;
				}
			}
		}
		else return false;
	}
	void castvote(string filename) {
		string n, p, r;
		int v;
		string name;
		ifstream file(filename);
		ofstream temp("temp.txt");

		if (!file || !temp) {
			cout << crimson << "Error opening file.\n" << stop;
			return;
		}
		cout << "Enter name of canidate you wanna cast vote for "; cin >> name;
		while (file >> n >> p >> r >> v) {
			if (n == name) {
				v++;
				cout << "Vote casted for " << n << " in region " << r << ".\n";
			}
			temp << n << " " << p << " " << r << " " << v << "\n";
		}

		file.close();
		temp.close();

		remove(filename.c_str());
		rename("temp.txt", filename.c_str());
	}
	void updateVoteStatus(string fl) {
		ifstream file("voters.txt");
		ofstream temp("temp.txt");
		string sn, sr, currentStatus, currentStatus1;
		int sp;
		long long sc;
		bool found = false;
		if (!file) {
			cout << crimson << "Error opening file.\n" << stop;
			return;
		}
		else if (!temp) {
			cout << crimson << "Error in temp\n" << stop;
		}
		else {
			while (file >> sn >> sp >> sc >> sr >> currentStatus >> currentStatus1) {
				if (sc == loggedINcnic) {
					if (fl == "Local.txt") {
						temp << sn << " " << sp << " " << sc << " " << sr << " Voted " << currentStatus1 << endl;
						found = true;
					}
					else if (fl == "National.txt") {
						temp << sn << " " << sp << " " << sc << " " << sr << " " << currentStatus << " Voted" << endl;
						found = true;
					}
				}
				else {
					temp << sn << " " << sp << " " << sc << " " << sr << " " << currentStatus << " " << currentStatus1 << "\n";
				}
			}
			file.close();
			temp.close();
		}
		if (found == true) {
			remove("voters.txt");
			rename("temp.txt", "voters.txt");
			cout << green << "Voter status updated successfully." << stop << endl;
		}
		else {
			cout << crimson << "Voter not found with CNIC: " << loggedINcnic << stop << endl;
			remove("temp.txt");
		}
	}
	bool viewvotestatus(string fl) {
		ifstream file("voters.txt");
		if (!file) {
			cout << crimson << "Error opening file.\n" << stop;
			return false;
		}
		else {
			string sn, sr, currentStatus, currentStatus1;
			int sp;
			long long sc;
			while (file >> sn >> sp >> sc >> sr >> currentStatus >> currentStatus1) {
				if (sc == loggedINcnic) {
					if (fl == "Local.txt") {
						if (currentStatus == "Voted") {
							return true;
						}
						else return false;
					}
					else if (fl == "National.txt") {
						if (currentStatus1 == "Voted") {
							return true;
						}
						else return false;
					}
				}
			}
		}
	}

};
class candidate {
	string name;
	string party;
	int votes;
public:
	candidate() {
		name = " ";
		party = " ";
		votes = 0;
	}
	candidate(string n, string p, int v) :name(n), party(p), votes(v) {}
	candidate(candidate& c) {
		name = c.name;
		party = c.party;
		votes = c.votes;
	}
	void operator =(candidate c) {
		this->name = c.name;
		this->party = c.party;
		this->votes = c.votes;
	}
	void setname(string n) {
		name = n;
	}
	void setparty(string p) {
		party = p;
	}
	string getname() const {
		return name;
	}
	string getparty() const {
		return party;
	}
	int getvotes() const {
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
				cout << green << "Admin is register" << stop << endl;
				return true;
			}
			else cout << crimson << "Registeration failed" << stop << endl;
		}
		else return false;
	}
	void createElection(string electionName) {
		string filename = electionName + ".txt";
		ofstream file(filename);
		if (file) {
			cout << green << "Election created successfully." << stop << endl;
			file.close();
		}
		else {
			cout << crimson << "Error creating election." << stop << endl;
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
			cout << green << "Candidate added successfully." << stop << endl;
		}
		else {
			cout << crimson << "Error adding candidate." << stop << endl;
		}
	}
	bool EndElections() {
		cout << "Do ypu want to end the elections?\n";
		cout << "1.YES" << "\n 2.NO" << endl;
		cin >> c;
		if (c) {
			return true;
		}
		else return false;
	}
};
class election {
public:
	virtual void displaycandidates(string fl) = 0;
	virtual void Result(string fl) = 0;
};
class LocalElection :public election {
	voters v;
public:
	void displaycandidates(string filename) override {
		string n, r;
		int p;
		long long c;
		ifstream file("voters.txt");
		if (file >> n >> p >> c >> r) {
			ifstream infile(filename);
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
					cout << crimson << "No candidates found in this region." << stop << endl;
				}
			}
			else {
				cout << crimson << "Error reading candidates file." << stop << endl;
			}
		}
		else {
			cout << crimson << "Error reading voters file." << stop << endl;
		}
	}
	void Result(string filename) override {
		string n, p, r;
		int v;
		ifstream file(filename);
		string currentRegion = "";
		string winnerName;
		string winnerParty;
		int maxVotes = 0;
		if (file) {
			while (file >> n >> p >> r >> v) {
				if (r != currentRegion) {
					if (!currentRegion.empty()) {
						cout << "Winner of " << currentRegion << " REGION: " << winnerName << " (" << winnerParty << ") with " << maxVotes << " votes" << endl << endl;
					}
					currentRegion = r;
					cout << currentRegion << " REGION" << endl;
					winnerName = n;
					winnerParty = p;
					maxVotes = v;
				}
				cout << n << " " << p << " " << v << endl;
				if (v > maxVotes) {
					winnerName = n;
					winnerParty = p;
					maxVotes = v;
				}
			}
			cout << "Winner of " << currentRegion << " REGION: " << winnerName << " (" << winnerParty << ") with " << maxVotes << " votes" << endl;
		}
	}

};
class nationalElection :public election {
	voters v;
public:
	void displaycandidates(string filename) override {
		string n, r;
		int p;
		long long c;
		ifstream file("voters.txt");
		if (file >> n >> p >> c >> r) {
			ifstream infile(filename);
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
					cout << crimson << "No candidates found in this region." << stop << endl;
				}
			}
			else {
				cout << crimson << "Error reading candidates file." << stop << endl;
			}
		}
		else {
			cout << crimson << "Error reading voters file." << stop << endl;
		}
	}
	void Result(string filename)override {

	}
};
void header() {
	cout << "	     _ _                               _" << endl;
	cout << "            | (_)                         _   (_)                               _               " << endl;
	cout << "  ___  ____ | |_ ____  ____    _   _ ___ | |_  _ ____   ____     ___ _   _  ___| |_  ____ ____" << endl;
	cout << " / _ \\|  _ \\| | |  _ \\/  _ )  | | | / _ \\| _ )| |  _ \\ / _  |   /___) | | |/___)  _)/ _  )    \\" << endl;
	cout << "| |_| | | | | | | | | ( (/ /   \\ V / |_| | |__| | | | ( ( | |  |___ | |_| |___ | |_( (/ /| | | |" << endl;
	cout << " \\___/|_| |_|_|_|_| |_|\\____)   \\_/ \\___/ \\___)_|_| |_|\\_|| |  (___/ \\__  (___ /\\___)____)_|_|_|" << endl;
	cout << "                                                       (____|       (____/" << endl;
}
int main() {
	int mainChoice;
	header();
	do {
		cout << "\n========== Online Voting System ==========\n";
		cout << "1. Register as Voter\n";
		cout << "2. Login as Voter\n";
		cout << "3. Login as Admin\n";
		cout << "4. Exit\n";
		cout << "Enter choice: ";
		cin >> mainChoice;
		system("cls"); header();
		switch (mainChoice) {
		case 1: {
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
				int count = 0;
				long long c = cnic;
				while (c != 0) {
					c /= 10;
					count++;
				}
				if (count <= 12) {
					cout << crimson << "ERROR!!! CNIC must be of 13 numbers" << stop << endl;
				}
				else {
					ifstream file("voters.txt");
					string sn, sr; int sp; long long sc;
					bool alreadyRegistered = false;
					if (file) {
						while (file >> sn >> sp >> sc >> sr) {
							if (cnic == sc) {
								alreadyRegistered = true;
								break;
							}system("cls"); header();
						}
						file.close();
						if (alreadyRegistered == true) {
							cout << green << "You have already been registered " << stop << endl;
						}
						else {
							voters v(name, password, age, cnic);
						}
					}
				}
			}
			else cout << crimson << "You are not eligible for voting" << stop << endl;
		}
			  break;
		case 2: {
			voters v;
			bool status = v.login();
			if (status == true) {
				cout << green << "Login successfull" << stop << endl;
				int choice;
				do {
					cout << "1.View Elections \n";
					cout << "2.Logout\n";
					cin >> choice;
					system("cls"); header();
					switch (choice) {
					case 1: {
						int election;
						cout << "1. Local Elections\n";
						cout << "2. National Elections\n";
						cout << "3. Exit\n";
						cout << "Choose election \n ";
						cin >> election;
						system("cls"); header();
						switch (election) {
						case 1: {
							int choose;
							cout << "1. Cast your vote\n";
							cout << "2.View your vote status\n";
							cout << "3. Exit\n";
							cin >> choose;
							do {
								if (choose == 1) {
									bool s = v.viewvotestatus("Local.txt");
									if (s) {
										cout << "You have already catsed vote, Voter van Vote only ONCE";
									}
									else {
										LocalElection l;
										if (c == false) {
											l.displaycandidates("Local.txt");
											v.castvote("Local.txt");
											v.updateVoteStatus("Local.txt");
										}
										else cout << "Local elections has been ended, Wait for the result" << endl;
									}
								}
								else if (choose == 2) {
									bool s = v.viewvotestatus("Local.txt");
									if (s) {
										cout << "Your vote has been casted\n";
									}
									else cout << "You have not casted vote\n";


								}
								else break;
							} while (choose == 3);
						}
							  break;
						case 2: {
							int choose1;
							cout << "1. Cast your vote\n";
							cout << "2.View your vote status\n";
							cout << "3. Exit\n";
							cin >> choose1;
							do {
								if (choose1 == 1) {
									bool s = v.viewvotestatus("National.txt");
									if (s) {
										cout << "you have already casted the vote, Voter can only Vote ONCE" << endl;
									}
									else {
										if (c1 == false) {
											nationalElection n;
											n.displaycandidates("National.txt");
											v.castvote("National.txt");
											v.updateVoteStatus("National.txt");
										}
										else cout << "National elections have been ended, Wait for the result";
									}
								}
								else if (choose1 == 2) {
									bool s1 = v.viewvotestatus("National.txt");
									if (s1) {
										cout << "Your vote has been casted\n";
									}
									else cout << "You have not casted vote\n";

								}

								else break;
							} while (choose1 == 3);
						}
							  break;
						}
					}
						  break;
					case 2:
						cout << "Logging Out\n";
						break;
					}
				} while (choice != 2);
			}
			else {
				cout << crimson << "Login Failed" << stop << endl;
			}
		}
			  break;
		case 3: {
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
				cout << "3.Exit" << endl;
				cin >> choice1;
				system("cls"); header();
				switch (choice1) {
				case 1: {
					ofstream file("Local.txt", ios::app);
					if (file) {
						int choice;
						cout << "1. Add Candidates" << endl;
						cout << "2.End Elections" << endl;
						cin >> choice;
						system("cls"); header();
						switch (choice) {
						case 1:
							A.addcandidate("Local.txt");
							break;
						case 2:
							bool c = A.EndElections();
							if (c) {
								int choice;
								system("cls"); header();
								cout << "1.Display Result" << endl;
								cin >> choice;
								system("cls"); header();
								LocalElection l;
								l.Result("Local.txt");
								int k;
								cout << "\n\nPress 1 to exit." << endl;
								cin >> k;
								system("cls"); header();
								if (k == 1)
									break;
							}
						}
					}
					else {
						cout << crimson << "Error opening Local.txt" << stop << endl;
					}
					file.close();
					break;
				}

				case 2: {
					ofstream file("National.txt", ios::app);
					if (file) {
						int choice;
						cout << "1. Add Candidates" << endl;
						cout << "2. End Elections" << endl;
						cin >> choice;
						system("cls"); header();
						switch (choice) {
						case 1: {
							A.addcandidate("National.txt");
							break;
						}
						case 2: {
							bool c1 = A.EndElections();
							if (c1) {
								int choice;
								system("cls"); header();
								cout << "1.Display Result" << endl;
								cin >> choice;
								system("cls"); header();
								nationalElection n;
								n.Result("National.txt");
								int k;
								cout << "\n\nPress 1 to exit." << endl;
								cin >> k;
								system("cls"); header();
								if (k == 1)
									break;
							}
						}
						}
					}
					else {
						cout << crimson << "Error opening National.txt" << stop << endl;
					}
					file.close();
					break;
				}break;
				case 3:
					cout << "Exiting\n";
				}
			}
			else {
				cout << crimson << "Login failed" << stop << endl;
			}
		}
		}
	} while (mainChoice != 4);
}