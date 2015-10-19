//my regular expression

// TO DO : 加上 std:: 以及把 hash_map 改成 unordered_map

#ifndef REGEX_H
#define REGEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <hash_map>
#include <utility>
#include <set>

using namespace std;

namespace zl
{

#define LargestChar 255
#define BIG_MOD 100007;

	const char epsilon = '\0';

	struct Edge;
	struct Status;
	void StatLink();
	void oprtCalc();


	struct Edge
	{
		char MatchContent;
		Status* Start;
		Status* End;

		Edge(Status* s1, char key, Status* s2);
	};

	struct Status
	{
		//	list<Edge> InEdges;
		list<Edge> OutEdges;
		bool BeginStatus = false;
		bool FinalStatus = false;

		//	void AddIn(Status* S,char key);
		void AddOut(char key, Status* S);
		void e_LinkStat(Status* S);
	};

	struct Expr
	{
		Status* Start;
		Status* End;

		Expr(Status* in1 = NULL, Status* in2 = NULL);
		void Link(Expr follow);
		void Union(Expr follow);
		void Closure();
		void Optional();
	};

	struct NFA
	{
		Status* Start;
		vector<Status*> ValidStats;
		vector<Status*> unValidStats;

		NFA(char* InputStr);
		~NFA();
		void GetStatsList();
		vector<Edge> e_closure(Status* start);
		void DeleteEpsilon();
	};

	struct DFA;

	struct DetStat
	{
		DFA* from;
		bool BeginStatus;
		bool FinalStatus;
		int CompressedTableLength;
		vector<int>* CompressedTable;

		DetStat(DFA* dfa, int length);
		void SetAsBegin();
		void SetAsFinal();
		void AddEdge(int CompressedStatNum, int End);
	};

	struct DFA
	{
		NFA* nfa;
		vector<DetStat> StatusList;
		int CharDict[LargestChar + 2];
		int CompressedTableLength;

		int BuildCharDict();
		int AddStatus();//return the number of Stat in StatusList
		unsigned int sethash(set<Status*>& S);
		DFA(char* InputStr);
		int parser(char* src);
	};
}
#endif
