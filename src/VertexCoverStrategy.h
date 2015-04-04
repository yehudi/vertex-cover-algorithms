/*
 * VertexCoverStrategy.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef VERTEXCOVERSTRATEGY_H_
#define VERTEXCOVERSTRATEGY_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

class VertexCover{
public:
	vector<int> vertices;
	VertexCover(){
	}
};

class VertexCoverStrategy{
private:
	clock_t start;
	double total_time;
	bool finish;

protected:
	VertexCover cover;
	double time_limit;
public:
	virtual int getK(){
		return -1;
	}
	virtual string getName() = 0;
	virtual VertexCover findOptimalSolution() = 0;

	virtual VertexCover getSolution(double timeLimit){
		finish = false;
		setTimeLimit(timeLimit);

		startChrono();
		cover = findOptimalSolution();
		stopChrono();
		return cover;
	}

	virtual void setTimeLimit(double time){
		time_limit = time;
	}

	virtual void startChrono(){
		start = clock();
	}

	virtual double getElapsedTime(){
		clock_t finish = clock();
		return ((double)(finish - start))/CLOCKS_PER_SEC;
	}

	virtual bool isAccrossTimeLimit(){
		return getElapsedTime() >= time_limit;
	}
	virtual void stopChrono(){
		total_time = getElapsedTime();
	}

	virtual double getTotalTime(){
		return total_time;
	}

	virtual ~VertexCoverStrategy(){};
};


ostream& operator<<(ostream& s, const VertexCover& c){
	s<<"cover [ "<<c.vertices.size()<<" ] : ";
	for(unsigned int i=0; i< c.vertices.size(); ++i){
		s<<c.vertices[i]<<" ";
	}
	return s;
}
#endif /* VERTEXCOVERSTRATEGY_H_ */
