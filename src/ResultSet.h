/*
 * ResultSet.h
 *
 *  Created on: Mar 19, 2015
 *      Author: wolkom
 */

#ifndef RESULTSET_H_
#define RESULTSET_H_

#include "Graph.h"
#include "VertexCoverStrategy.h"
#include "table_printer.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

class Result{
public:
	double time;
	int val;
	int nb_exec;
	int k;
	Result(double t, int v, int n, int k) : time(t), val(v), nb_exec(n), k(k){
	}
};

class ResultSet{
public:
	double timeLimit;
	int n;
	vector<VertexCoverStrategy *> strategies;
	vector<double> proba;
	vector< vector<Result> > results;

	ResultSet(int n, double timeLimit){
		this->n = n;
		this->timeLimit = timeLimit;
	}

	void addStrategie(VertexCoverStrategy * s){
		strategies.push_back(s);
	}

	void addProba(double p){
		proba.push_back(p);
		results.push_back(vector<Result>());
	}

	void addResult(int proba, double t, int v, int n,int k){
		results[proba].push_back(Result(t,v,n,k));
	}

	void compute(Graph &g){
		for(unsigned int k=0; k<proba.size();++k){

			g.randomise(proba[k]);

			cout<<"N["<<n<<"], P["<<proba[k]<<"] ";

			for(unsigned int i=0; i<strategies.size(); ++i){
				cout<<strategies[i]->getName()<<" ";
				cout.flush();
				VertexCover cover = strategies[i]->getSolution(timeLimit);
				this->addResult(k,strategies[i]->getTotalTime(),cover.vertices.size(),1,strategies[i]->getK());
			}
			cout << endl;

		}
	}

	void printResult(ostream &s){
		bprinter::TablePrinter tp(&s);
		ostringstream os;
		os<<"n = "<<n;
		tp.AddColumn(os.str(),15);
		for(unsigned int i=0; i<this->strategies.size(); ++i){
			tp.AddColumn(strategies[i]->getName(), 25);
		}
		tp.PrintHeader();
		for(unsigned int i=0; i< this->proba.size(); ++i){
			ostringstream st1;
			st1<< std::setprecision( 3 )<<this->proba[i];
			tp<<st1.str();
			for(unsigned int j=0; j< this->strategies.size(); ++j){
				ostringstream st;
				if(this->results[i][j].time >= timeLimit){
					st<<" > limit |";
				}else{
					st<<this->results[i][j].time<<" s"<<"  |";
				}
				if(this->results[i][j].k>0)
					st<<std::fixed << std::setw( 3 )<<"(k="<<this->results[i][j].k<<")";
				st<<std::fixed << std::setw( 5 );
				st<<this->results[i][j].val;
				tp<<st.str();
			}
		}
		tp.PrintFooter();
	}
};

#endif /* RESULTSET_H_ */
