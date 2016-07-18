#ifndef RECHNER_H
#define RECHNER_H

#include <vector>
#include <math.h>
#include <stdlib.h> 
#include  <string>
using namespace std;

class rechner {
public:
	rechner();
	~rechner();

	static bool check(string& term);
	static string modify(string term);
	static string convert(string term);
	static float getNum(string term);
	static string getString(float num);
	static float calculate(string term);
	static void getFPos(string term, int end, vector<int> & vec);
	static void getSPos(string term, int end, vector<int> & vec);
	static string add(string term, vector<int> fir, vector<int> sec);
	static string sub(string term, vector<int> fir, vector<int> sec);
	static string mult(string term, vector<int> fir, vector<int> sec);
	static string diff(string term, vector<int> fir, vector<int> sec);
	static string exp(string term, vector<int> fir, vector<int> sec);

private:
	string term;

};

#endif





