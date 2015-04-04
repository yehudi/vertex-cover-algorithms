/*
 * MonHeurVCStrategy.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef MONHEURVCSTRATEGY_H_
#define MONHEURVCSTRATEGY_H_

#include "VertexCoverStrategy.h"
#include "Graph.h"

class MonHeurVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
public:
	MonHeurVCStrategy(Graph &g) : g(&g){}

	virtual string getName(){
		return string("MonHeur-VC");
	}

	virtual VertexCover findOptimalSolution(){
		return VertexCover();
	}
	virtual ~MonHeurVCStrategy(){};
};



#endif /* MONHEURVCSTRATEGY_H_ */
