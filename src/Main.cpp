//============================================================================
// Name        : Vertex-Cover.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
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

void runAlgos(int N){

	Graph g1(N);
	ARBVCStrategy arb(g1);
	GreedyVCStrategy greedy(g1);
	IPLVCStrategy ipl(g1);
	KernelVCStrategy ker(g1);
	MonHeurVCStrategy heur(g1);
	LPVCStrategy lp(g1);

	ResultSet resultSet(N,2);
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

	resultSet.compute(g1);

	resultSet.printResult(cout);
}

int main() {
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


	for(int i=10;i<=100;i+=10){
		runAlgos(i);
	}

	for(int i=100;i<=200;i+=20){
		runAlgos(i);
	}
	return 0;
}
