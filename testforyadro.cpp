#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <queue>
using namespace std;



bool checkTime(string s) {
	if (s.size() != 5) {
		return false;
	}
	for (int i = 0;i < 5;++i) {
		if (i != 2 && (s[i]>57 || s[i]<48)) {
			return false;
		}
		if (i == 2 && s[i] != ':') {
			return false;
		}
		if (i == 0 && s[i] > 50) {
			return false;
		}
		if (i == 1 && s[i] > 52) {
			if (s[0] == '2') {
				return false;
			}
		}
		if (i == 3 && s[i] > 53) {
			return false;
		}
	}
	return true;
}

bool checkName(string name) {
	for (int i = 0;i < name.size();++i) {
		if (!((name[i] > 47 && name[i] < 58) || (name[i] == '_' || name[i] == '-') || (name[i] > 96 && name[i] < 123))) {
			return false;
		}
	}
	return true;
}

bool checkNum(string s) {
	for (int i = 0;i < s.size();++i) {
		if (s[i] < 48 || s[i]>57) {
			return false;
		}
	}
	return true;
}

class CyberClub {
	struct mtime {
		short min, hours;
		mtime(string s) {
			min = 0;
			hours = 0;
			if (s[0] != '0') {
				hours += 10 * (s[0] - 48);
			}
			hours += s[1] - 48;
			if (s[3] != '0') {
				min += 10 * (s[3] - 48);
			}
			min += s[4] - 48;
		}
		mtime(short h, short m) {
			hours = h;
			min = m;
		}
		mtime() {
			hours = 0;
			min = 0;
		}
		bool operator <(mtime& t) {
			if (hours < t.hours) {
				return 1;
			}
			else if (hours == t.hours) {
				if (min < t.min) {
					return 1;
				}
				return 0;
			}
			return 0;
		}

		bool operator ==(mtime& t) {
			if (hours == t.hours && min == t.min) {
				return 1;
			}
			return 0;
		}

		mtime operator =(mtime t) {
			min = t.min;
			hours = t.hours;
			return *this;
		}

		mtime operator +(mtime t) {
			short m, h;
			h = hours + t.hours;
			if (min + t.min > 59) {
				++h;
				m = min + t.min - 60;
			}
			else {
				m = min + t.min;
			}
			return mtime{ h,m };
		}

		mtime operator -(mtime& t) {
			short m, h;
			h = hours - t.hours;
			if (min - t.min < 0) {
				--h;
				m = min - t.min + 60;
			}
			else {
				m = min - t.min;
			}
			return mtime{ h,m };
		}

		friend ostream& operator <<(ostream& os, mtime t) {
			string h, m;
			if (t.hours < 10) {
				h = "0";
			}
			if (t.min < 10) {
				m = "0";
			}
			os << h << t.hours << ':' << m << t.min;
			return os;
		}

	};
	mtime* topen;
	mtime* tclose;
	string close;
	vector<pair<bool,mtime>> tables;
	map<string, pair<bool,int>> clients;
	vector<pair<int,mtime>> incomes;
	queue<string> line;
	int waitingLine;
	int tableCount;
	int price;
public:
	CyberClub(int table, string open, string close, int price)
	{
		tableCount = table;
		this->close = close;
		topen = new mtime(open);
		tclose = new mtime(close);
		this->price = price;
		tables.resize(tableCount);
		waitingLine = 0;
		incomes.resize(tableCount);
	}
	~CyberClub() {
		delete topen;
		delete tclose;
	}



	vector<string> separate(string s) {
		if (s.empty()) {
			return vector<string>{};
		}
		string g;
		vector<string> vec;
		for (int i = 0;i < s.size();++i) {
			if (s[i] == ' ' && !g.empty()) {
				vec.push_back(g);
				g.clear();
			}
			if (s[i] != ' ') {
				g.push_back(s[i]);
			}
		}
		if (!g.empty()) {
			vec.push_back(g);
		}
		return vec;
	}

	bool isClientHere(string name) {
		
		if (clients.find(name) != clients.end() && clients[name].first==1) {
			return 1;
		}
		return 0;
	}

	bool isTableFree(int table) {
		if (table > tables.size()) {
			return 0;
		}
		if (tables[table - 1].first) {
			return 0;
		}
		return 1;
	}

	bool isHereFreeTables() {
		for (int i = 0;i < tables.size();++i) {
			if (!tables[i].first) {
				return 1;
			}
		}
		return 0;
	}

	bool checkRequest(vector<string> vec) {
		mtime tm(vec[0]);
		if (vec[1] == "1" && tm < *topen ) {
			cout << vec[0]<<" " << vec[1]<<" " << vec[2] << endl;
			cout << vec[0] << " " << "13 NotOpenYet" << endl;
			return 0;
		}
		if (vec[1] == "1" && !(tm < *tclose)) {
			cout << vec[0] << " " << vec[1] << " " << vec[2] << endl;
			cout <<vec[0]<<" "<< "13 NotOpenYet" << endl;
			return 0;
		}

		if (vec[1] == "1" && isClientHere(vec[2])) {
			cout << vec[0] << " " << vec[1] << " " << vec[2] << endl;
			cout << vec[0] << " " << "13 YouShallNotPass" << endl;
			return 0;
		}

		if (vec[1] == "2" && !isTableFree(stoi(vec[3]))) {
			cout << vec[0] << " " << vec[1] << " " << vec[2] <<" "<<vec[3]<< endl;
			cout << vec[0] << " " << "13 PlaceIsBusy" << endl;
			return 0;
		}

		if (vec[1] == "2" && !isClientHere(vec[2])) {
			cout << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << endl;
			cout << vec[0] << " " << "13 ClientUnknown" << endl;
			return 0;
		}

		if (vec[1] == "4" && !isClientHere(vec[2])) {
			cout << vec[0] << " " << vec[1] << " " << vec[2]  << endl;
			cout << vec[0] << " " << "13 ClientUnknown" << endl;
			return 0;
		}
		

		return 1;
	}

	void id1(string name,string tm) {
		if (clients.find(name) == clients.end()) {
			clients.emplace(make_pair(name, make_pair(1, -1)));
		}else{
			clients[name].first = 1;
		}
	}

	void id2(int table,string tm,string name,string id) {
		cout << tm << " " << id <<" "<< name <<" "<<table << endl;
		if (id == "12") {
			--waitingLine;
			line.pop();
		}
		
		tables[table - 1].first = 1;
		mtime t(tm);

		if (clients[name].second>-1) {
			tables[clients[name].second - 1].first = 0;
			mtime cleartime = t - tables[clients[name].second - 1].second;
			incomes[clients[name].second - 1].second = incomes[clients[name].second - 1].second + cleartime;
			short rounded = cleartime.hours;
			if (cleartime.min > 0) {
				++rounded;
			}
			incomes[clients[name].second - 1].first += price * rounded;
		}

		tables[table - 1].second = t;
		clients[name].second = table;
	}

	void id12(int table,string tm) {
		id2(table,tm,line.front(),"12");

	}

	void id11(string name,string tm,string id) {
		cout << tm << " " << id << " " << name << endl;;
		if (clients[name].second>-1) {
			mtime timenow(tm);
			timenow=timenow-tables[clients[name].second-1].second;
			short t = timenow.hours;
			if (timenow.min > 0) {
				++t;
			}
			incomes[clients[name].second-1].first += t * price;
			clients[name].first = 0;
			tables[clients[name].second-1].first = 0;
			incomes[clients[name].second-1].second = incomes[clients[name].second-1].second + timenow;
			if (waitingLine > 0 && tm!=close) {
				id12(clients[name].second,tm);
			}
			clients[name].second = -1;
			
			
		}
		else {
			clients[name].first = 0;
		}
	}

	void id3(vector<string> vec) {
		if (isHereFreeTables()) {
			cout <<vec[0] << " " << "13 ICanWaitNoLonger!" << endl;
		}else if (waitingLine >= tableCount) {
			id11(vec[2],vec[0],"11");
		}
		else {
			++waitingLine;
			line.push(vec[2]);
		}
		
	}

	void id4(string name,string tm) {
		id11(name, tm, "4");
	}

	bool checkCorrect(vector<string> s) {
		if (s.size() > 4 || s.size() < 3) {
			return 0;
		}
		if (!checkTime(s[0])) {
			return 0;
		}
		if (!checkNum(s[1])) {
			return 0;
		}
		if (!checkName(s[2])) {
			return 0;
		}
		if (s.size() == 4 && !checkNum(s[3])) {
			return 0;
		}
		if (s[1] != "1" && s[1] != "2" && s[1] != "3" && s[1] != "4" ) {
			return 0;
		}
		if ((s[1] == "1" || s[1] == "3" || s[1] == "4") && s.size() != 3) {
			return 0;
		}
		else if(s[1]=="2" && s.size()!=4) {
			return 0;
		}

		return 1;
	}

	void closeClub(){
		for (auto it = clients.begin();it != clients.end();++it) {
			if (it->second.first) {
				id11(it->first, close, "11");
			}
		}
		cout << close<<endl;
		for (int i = 0;i < tableCount;++i) {
			cout << i + 1 << " " << incomes[i].first <<" "<< incomes[i].second << endl;
		}
	}

	friend bool work(int argc, char* argv[]);

};

bool work(int argc, char* argv[]) {
	string open, close, table, price;
	string filename = argv[1];
	ifstream inputFile(filename);
	if (!inputFile) {
		cerr << "fault: notopen " << filename << std::endl;
		return 1; 
	}
	string s;
	getline(inputFile, s);
	table = s;
	if (table == "0") {
		cout << "0 tables in the club";
		return 0;
	}
	getline(inputFile, s);
	int i = 0;
	while (i < s.size() && s[i] != ' ') {
		open += s[i];
		++i;
	}
	++i;
	while (i < s.size() && s[i] != ' ') {
		close += s[i];
		++i;
	}
	getline(inputFile, s);
	price = s;
	if (!checkNum(table)) {
		cout << table;
		return 1;
	}
	if (!(checkTime(open) && checkTime(close))) {
		cout << open << " " << close;
		return 1;
	}
	if (!checkNum(price)/*цена должна быть целым числом*/) {
		cout << price;
		return 1;
	}
	CyberClub club(stoi(table), open, close, stoi(price));
	cout << *club.topen << endl;
	while (getline(inputFile,s)) {
		vector<string> vec = club.separate(s);
		if (!club.checkCorrect(vec)) {
			cout << s;
			return 1;
		}
		if (!club.checkRequest(vec)) {
			continue;
		}
		if (vec[1] == "1") {
			cout << s << endl;
			club.id1(vec[2], vec[0]);
		}
		else if (vec[1] == "2") {
			club.id2(stoi(vec[3]), vec[0], vec[2], vec[1]);
		}
		else if (vec[1] == "3") {
			cout << s << endl;
			club.id3(vec);
		}
		else {
			club.id4(vec[2], vec[0]);
		}
	} 
	club.closeClub();
	return 0;
}

int main(int argc, char* argv[]) {
	
	return work(argc,argv);
}

