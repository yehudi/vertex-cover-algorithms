/*
 * GreedyVCStrategy.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef GREEDYVCSTRATEGY_H_
#define GREEDYVCSTRATEGY_H_

#include "VertexCoverStrategy.h"
#include "Graph.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class GreedyVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
public:
	GreedyVCStrategy(Graph &g) : g(&g){}

	virtual string getName(){
		return string("Greedy-VC");
	}

	virtual VertexCover findOptimalSolution(){
		vector<Edge> edges = g->getEdges();
		std::map<int, int> edgesDegree;
		for(unsigned int i=0; i< edges.size(); ++i){
			edgesDegree[g->getUniqueIdentifier(edges[i])] = 2;
		}
		vector< vector<int> > adjList = g->getAdjancyList();
		VertexCover cover;
		for(unsigned int i=0; i< adjList.size(); ++i){
			bool isRemovable = true;
			for(unsigned int j=0; j< adjList[i].size(); ++j){
				Edge edge = g->getEdge(i, adjList[i][j]);
				if(edgesDegree[g->getUniqueIdentifier(edge)] < 2){
					isRemovable = false;
					break;
				}
			}
			if(isRemovable){
				for(unsigned int j=0; j< adjList[i].size(); ++j){
					Edge edge = g->getEdge(i, adjList[i][j]);
					edgesDegree[g->getUniqueIdentifier(edge)] -= 1;
				}
			}else{
				cover.vertices.push_back(i);
			}
		}
		return cover;
	}
	virtual ~GreedyVCStrategy(){};
};



#endif /* GREEDYVCSTRATEGY_H_ */
