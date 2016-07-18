#include "rechner.h"
#include <stack>

rechner::rechner() {
}

rechner::~rechner() {
}

// checks if term is correct
bool rechner::check(string& term) {
	char token;
	short ope = 0;
	short clo = 0;
	bool oper = false;
	bool ok = true;
	bool br = false;
	string partial;

	for(unsigned int ii=0;ii<term.size();ii++) {
		token=term[ii];
		if(token==')') {
			br = true;
			continue;
		}
		if(token=='('&&br==true) {
			partial = term.substr(0,ii-1);
			partial.append("*");
			partial.append(term.substr(ii,term.size()-1));
			term = partial;
			br =false;
			continue;
		}
		br=false;
	}

	for(unsigned int ii=0;ii<term.size();ii++) {
		token=term[ii];
		if(token=='+'||token=='-'||token=='*'||token=='/'||token=='^') {
			oper = true;
			continue;
		}
		if(token=='+'||token=='-'||token=='*'||token=='/'||token=='^' && oper==true) {
			ok = false;
			break;
		}
		if(token=='(') {
			ope++;
			continue;
		}
		if(token==')') {
			clo++;
			continue;
		}
		if(token==')'&&ope<=clo) {
			ok = false;
			break;
		}
		oper=false;
	}
	if(clo!=ope) {
		ok = false;
	}
	return ok;
}

// seperates numbers and operators with "|"-token
string rechner::modify(string term) {
	string modified;
	char token;
	bool op = false;
	bool inC = false;

	for(unsigned int ii=0;ii<term.size();ii++) {
		token=term[ii];
		// checks if next token is an operator
		if(token=='+'||token=='-'||token=='*'||token=='/'||token=='('||token==')'||token=='^') {
			op = true;
			inC = false;
		}
		else{
			op = false;
		}
		// if next token is a number and it is not the second number
		if(op==false && inC==false) {
			modified.push_back('|');
			modified.push_back(token);
			modified.push_back('|');
			inC = true;
			continue;
		}
		// if next token is a number and it is the second number
		if(op==false && inC==true) {
			modified.pop_back();
			modified.push_back(token);
			modified.push_back('|');
			continue;
		}
		// if next token is an operator
		else{
			modified.push_back('|');
			modified.push_back(token);
			modified.push_back('|');
			continue;
		}
	}
	
	return modified;
}

// convertes seperated term in reversed polnish form
string rechner::convert(string term) {
	std::stack<string> operators;
	string converted;
	string partial;
	char token;
	short start = 0;
	bool inChar = false;
	short op = 0;
	short opo = 0;

	for(unsigned int ii=0;ii<term.size();ii++) {
		token=term[ii];

		// checks if next token is part of a other token
		if(inChar==false) {
			if(token=='|') {
				start = ii;
				inChar = true;
				continue;
			}
		}
		// next token is part of a other token
		else {
			// next token is an operator with rank 1
			if(token=='+'||token=='-'||token=='(') {
				op = 1;
				continue;
			}
			// next token is an operator with rank 2
			if(token=='*'||token=='/') {
				op = 2;
				continue;
			}
			// next token is an operator with rank 3
			if(token=='^') {
				op = 3;
				continue;
			}
			// next token is an ")"-operator
			if(token==')') {
				op = 4;
				continue;
			}
			// next token is not part of an operator
			if(token=='|' && op==0 ) {
				converted.append(term.substr(start,ii-start+1));
				inChar = false;
				continue;
			}
			// next token is part of an operator and end of a bracket
			if(token=='|' && op==4 && op>=opo) {
				inChar = false;
				op = 0;
				// push everything from stack to result till '('
				while(operators.top()!="|(|") {
					partial = operators.top();
					operators.pop();
					converted.append(partial);
				}
				operators.pop();
				continue;
			}
			// next token is part of an operator and is bigger or same than rank of saved one
			if(token=='|' && op>0 && op>=opo) {
				operators.push(term.substr(start,ii-start+1));
				inChar = false;
				op = 0;
				// save rank of the operator
				if(term[ii-1]=='+'||term[ii-1]=='-') {
					opo = 1;
					continue;
				}
				if(term[ii-1]=='*'||term[ii-1]=='/') {
					opo = 2;
					continue;
				}
				if(term[ii-1]=='^') {
					opo = 3;
					continue;
				}
			}
			// next token is part of an operator and is smaller than rank of saved one
			if(token=='|' && op>0 && op<opo) {
				while(operators.size()!=0) {
					partial = operators.top();
					operators.pop();
					if(partial != "|(|") {
						converted.append(partial);
					}
				}
				operators.push(term.substr(start,ii-start+1));
				inChar = false;
				op = 0;
				if(term[ii-1]=='+'||term[ii-1]=='-') {
					opo = 1;
					continue;
				}
				if(term[ii-1]=='*'||term[ii-1]=='/') {
					opo = 2;
					continue;
				}
				if(term[ii-1]=='^') {
					opo = 3;
					continue;
				}
			}
		}
	}
	if(operators.empty()==false) {
		unsigned int len = operators.size();
		for(unsigned int ii=0;ii<len;ii++) {
			partial = operators.top();
			operators.pop();
			converted.append(partial);
		}
	}
	return converted;
}

// gets the corresponding number as int
float rechner::getNum(string term) {
	float f = atof(term.c_str());
	return f;
}

// gets corresponding number as string
string rechner::getString(float num) {
	string s = to_string(num); 
	return s;
}

// gets position of first char in the area
void rechner::getSPos(string term, int end, vector<int> & vec) {
	char token;
	bool inC = false;
	unsigned int start;

	for(int ii=end-2;ii>=0;ii--) {
		token=term[ii];
		if(token=='|'&&inC==false) {
			inC = true;
			vec[1] = ii;
			continue;
		}
		if (token=='|'&&inC==true) {
			vec[0]=ii;
			break;
		}
	}
}

// gets position of second char in the area
void rechner::getFPos(string term, int end, vector<int> & vec) {
	char token;
	bool inC = false;
	int count = 0;
	unsigned int start;

	for(int ii=end-2;ii>=0;ii--) {
		token=term[ii];
		if(token=='|'&&count<2) {
			count++;
			continue;
		}
		if(token=='|'&&inC==false&&count>=2) {
			inC = true;
			vec[1] = ii;
			continue;
		}
		if (token=='|'&&inC==true) {
			vec[0] = ii;
			break;
		}
	}
}

// modifies string with add operation
string rechner::add(string term, vector<int> fir, vector<int> sec) {
	string f = term.substr(fir[0]+1, fir[1]-fir[0]-1);
	string s = term.substr(sec[0]+1, sec[1]-sec[0]-1);
	float fi = rechner::getNum(f);
	float si = rechner::getNum(s);
	float res = fi + si ;
	string partial = rechner::getString(res);
	string newTerm = "";

	for(int ii=0;ii<fir[0];ii++) {
		newTerm.append(term.substr(ii,1));
	}
	newTerm.append("|"+partial+"|");
	for(unsigned int ii=sec[1]+4;ii<term.size();ii++) {
		newTerm.append(term.substr(ii,1));
	}
	return newTerm;
}

string rechner::sub(string term, vector<int> fir, vector<int> sec) {
	string f = term.substr(fir[0]+1, fir[1]-fir[0]-1);
	string s = term.substr(sec[0]+1, sec[1]-sec[0]-1);
	float fi = rechner::getNum(f);
	float si = rechner::getNum(s);
	float res = fi - si ;
	string partial = rechner::getString(res);
	string newTerm = "";

	for(int ii=0;ii<fir[0];ii++) {
		newTerm.append(term.substr(ii,1));
	}
	newTerm.append("|"+partial+"|");
	for(unsigned int ii=sec[1]+4;ii<term.size();ii++) {
		newTerm.append(term.substr(ii,1));
	}
	return newTerm;
}

string rechner::mult(string term, vector<int> fir, vector<int> sec) {
	string f = term.substr(fir[0]+1, fir[1]-fir[0]-1);
	string s = term.substr(sec[0]+1, sec[1]-sec[0]-1);
	float fi = rechner::getNum(f);
	float si = rechner::getNum(s);
	float res = fi * si ;
	string partial = rechner::getString(res);
	string newTerm = "";

	for(int ii=0;ii<fir[0];ii++) {
		newTerm.append(term.substr(ii,1));
	}
	newTerm.append("|"+partial+"|");
	for(unsigned int ii=sec[1]+4;ii<term.size();ii++) {
		newTerm.append(term.substr(ii,1));
	}
	return newTerm;
}

string rechner::diff(string term, vector<int> fir, vector<int> sec) {
	string f = term.substr(fir[0]+1, fir[1]-fir[0]-1);
	string s = term.substr(sec[0]+1, sec[1]-sec[0]-1);
	float fi = rechner::getNum(f);
	float si = rechner::getNum(s);
	float res = fi / si ;
	string partial = rechner::getString(res);
	string newTerm = "";

	for(int ii=0;ii<fir[0];ii++) {
		newTerm.append(term.substr(ii,1));
	}
	newTerm.append("|"+partial+"|");
	for(unsigned int ii=sec[1]+4;ii<term.size()-1;ii++) {
		newTerm.append(term.substr(ii,1));
	}
	return newTerm;
}

string rechner::exp(string term, vector<int> fir, vector<int> sec) {
	string f = term.substr(fir[0]+1, fir[1]-fir[0]-1);
	string s = term.substr(sec[0]+1, sec[1]-sec[0]-1);
	float res = (float)pow(rechner::getNum(f),rechner::getNum(s));
	string partial = rechner::getString(res);
	string newTerm = "";

	for(int ii=0;ii<fir[0];ii++) {
		newTerm.append(term.substr(ii,1));
	}
	newTerm.append("|"+partial+"|");
	for(unsigned int ii=sec[1]+4;ii<term.size();ii++) {
		newTerm.append(term.substr(ii,1));
	}
	return newTerm;
}

// calculates result from term in reversed polnish form
float rechner::calculate(string term) {
	char token;
	unsigned int start = 0;
	float result = 0.0f;
	int op = 0;
	vector<int> posF;
	posF.resize(2);
	vector<int> posS;
	posS.resize(2);
	string partial;

	for(unsigned int ii=0;ii<term.size();ii++) {
		token=term[ii];
			if(token=='+'||token=='-'||token=='*'||token=='/'||token=='^') {
				op++;
			}
	}

	while(op!=0) {
		for(unsigned int ii=0;ii<term.size();ii++) {
			token=term[ii];
			if(token=='+') {
				rechner::getFPos(term, ii, posF);
				rechner::getSPos(term, ii, posS);
				term = rechner::add(term, posF, posS);
				op--;
				ii=0;
			}
			if(token=='-') {
				rechner::getFPos(term, ii, posF);
				rechner::getSPos(term, ii, posS);
				term = rechner::sub(term, posF, posS);
				op--;
				ii=0;
			}
			if(token=='*') {
				rechner::getFPos(term, ii, posF);
				rechner::getSPos(term, ii, posS);

				term = rechner::mult(term, posF, posS);
				op--;
				ii=0;
			}
			if(token=='/') {
				rechner::getFPos(term, ii, posF);
				rechner::getSPos(term, ii, posS);

				term = rechner::diff(term, posF, posS);
				op--;
				ii=0;
			}
			if(token=='^') {
				rechner::getFPos(term, ii, posF);
				rechner::getSPos(term, ii, posS);

				term = rechner::exp(term, posF, posS);
				op--;
				ii=0;
			}
		}
	}
	result = rechner::getNum(term.substr(1,term.size()-2));
	return result;
}
