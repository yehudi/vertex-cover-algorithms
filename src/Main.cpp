//============================================================================
// Name        : Vertex-Cover.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include "Graph.h"
#include "VertexCoverStrategy.h"
#include "ARBVCStrategy.h"
#include "GreedyVCStrategy.h"
#include "IPLVCStrategy.h"
#include "LPVCStrategy.h"
#include "KernelVCStrategy.h"
#include "MonHeurVCStrategy.h"
#include "table_printer.h"
#include "ResultSet.h"

#include <assert.h>

using namespace std;

map<double, vector<double> > times;
int TIME_LIMIT = 180;
void runAlgos(int N, Graph &g, vector<VertexCoverStrategy *> &strategies, ostream &o){
		ResultSet resultSet(N,TIME_LIMIT);
		resultSet.addProba(3.0/(double)N);
		resultSet.addProba(4.0/(double)N);
		resultSet.addProba(5.0/(double)N);
		resultSet.addProba(log(N)/(double)N);
		resultSet.addProba(1/sqrt(N));
		resultSet.addProba(0.1);
		resultSet.addProba(0.2);

		for(unsigned int i = 0; i<strategies.size() ; ++i)
			resultSet.addStrategie(strategies[i]);

		resultSet.compute(g,times);

		resultSet.printResult(cout);
		resultSet.printResultcsv(o);
		o.flush();
}

void runAlgos(int N, ostream &o){

	Graph g1(N);
	ARBVCStrategy arb(g1);
	GreedyVCStrategy greedy(g1);
	IPLVCStrategy ipl(g1);
	KernelVCStrategy ker(g1);
	MonHeurVCStrategy heur(g1);
	LPVCStrategy lp(g1);

	ResultSet resultSet(N,180);
	resultSet.addProba(3.0/(double)N);
	resultSet.addProba(4.0/(double)N);
	resultSet.addProba(5.0/(double)N);
	resultSet.addProba(log(N)/(double)N);
	resultSet.addProba(1/sqrt(N));
	resultSet.addProba(0.1);
	resultSet.addProba(0.2);

	resultSet.addStrategie(&arb);
	resultSet.addStrategie(&greedy);
	resultSet.addStrategie(&ipl);
	resultSet.addStrategie(&lp);
	resultSet.addStrategie(&ker);
	resultSet.addStrategie(&heur);

	resultSet.compute(g1,times);

	resultSet.printResult(cout);
	resultSet.printResultcsv(o);
	o.flush();
}

void tests(){
	Graph g(5);
	g.bind(0,1);
	g.bind(0,2);


	cout<<"Begin test on graph"<<endl;
	cout<<"==================="<<endl;

	assert(g.edgeExist(0,1));
	assert(!g.edgeExist(0,3));
	assert(g.getDegree(0) == 2);
	assert(g.getDegree(1) == 1);
	assert(g.getDegree(3) == 0);
	assert(g.getDegree() == 2);

	vector<int> ids;
	ids.push_back(0);
	ids.push_back(1);
	Graph p = g.removeVertices(ids);
	assert(p.getDegree() == 0);
	assert(p.getDegree(0) == 0);
	cout<<"end of tests"<<endl;
	cout<<"==================="<<endl;

}

void printMenu(){
	cout<<"Run one of the following commands :" <<endl;
	cout<<" - menu or help (to show this menu)"<<endl;
	cout<<" - tests (to run tests)"<<endl;
	cout<<" - exit (to quit the application)"<<endl;
	cout<<" - run algo N (to run an algo with the parameter N a positive integer)"<<endl;
	cout<<"   algos are : greedy, ip, lp, ker, arb, heur, all (to run them all)"<<endl;
}

void App(){
	string command;
	cout<<"====================================="<<endl;
	cout<<"             Vertex-Cover            "<<endl;
	cout<<"                                     "<<endl;
	cout<<" by HOLLEVOET Yehudi and ZOLO Moudif "<<endl;
	cout<<"====================================="<<endl;
	cout<<"-> type 'help' for more info"<<endl;
	cout<<endl;
	while(true){
		cout<<">>> ";
		getline(cin,command);

		if(command.compare("tests") == 0){
			tests();
		}else if(command.compare("menu") == 0 || command.compare("help") == 0 ){
			printMenu();
		}
		else if (command.compare(0,3,"run") == 0){
			vector<string> parsed;
			string token;
			istringstream ss(command);
			while(getline(ss, token, ' ')){
				parsed.push_back(token);
			}
			int N = atoi(parsed[parsed.size()-1].c_str());
			if(N > 0 && parsed.size() >= 3){
				bool correct = true;
				vector<VertexCoverStrategy *> strategies;
				map<string, VertexCoverStrategy *> mappedStrategies;
				Graph g1(N);
				ARBVCStrategy arb(g1);
				GreedyVCStrategy greedy(g1);
				IPLVCStrategy ipl(g1);
				KernelVCStrategy ker(g1);
				MonHeurVCStrategy heur(g1);
				LPVCStrategy lp(g1);
				mappedStrategies["arb"] = &arb;
				mappedStrategies["greedy"] = &greedy;
				mappedStrategies["ip"] = &ipl;
				mappedStrategies["lp"] = &lp;
				mappedStrategies["ker"] = &ker;
				mappedStrategies["heur"] = &heur;
				for(unsigned int i=1; i< parsed.size()-1; ++i){
					if(parsed[i].compare("greedy") == 0
							|| parsed[i].compare("ip") == 0
							|| parsed[i].compare("lp") == 0
							|| parsed[i].compare("ker") == 0
							|| parsed[i].compare("arb") == 0
							|| parsed[i].compare("heur") == 0){
						strategies.push_back(mappedStrategies[parsed[i]]);
					}else if(parsed[i].compare("all") == 0){
						strategies.clear();
						for(map<string, VertexCoverStrategy *>::iterator it = mappedStrategies.begin(); it != mappedStrategies.end(); ++it)
							strategies.push_back((*it).second);
						break;
					}else{
						cout<<"Unknown algorithm <"<<parsed[1]<<">"<<endl;
						cout<<"\t Algorithmes are : greedy, ip, lp, ker, arb, heur, all"<<endl;
						correct = false;
					}
				}
				if(correct){
					if(system("mkdir -p results") < 0){
						cout<<"unable to create folder"<<endl;
					}
					ofstream f;
					ostringstream fname;
					fname<<"results/vc";
					for(unsigned int i=1; i< parsed.size(); ++i)
						fname<<"_"<<parsed[i];
					fname<<".csv";
					f.open(fname.str().c_str());
					runAlgos(N, g1, strategies, f);
					f.close();
				}
			}else{
				cout<<"wrong command format, type 'help' to get more information"<<endl;
			}
		}else if (command.compare("exit") == 0){
			break;
		}else if(command.empty()){

		}else{
			cout<<"Unknown command"<<endl;
		}
	}
}

int main() {
	App();
	/*ofstream f;
	f.open("results180.csv");

	for(int i=10;i<=100;i+=10){
		runAlgos(i,f);
	}

	for(int i=100;i<=500;i+=20){
		runAlgos(i,f);
	}

	for(int i=500;i<=1000;i+=50){
		runAlgos(i,f);
	}
	f.close();
	*/

	return 0;
}
