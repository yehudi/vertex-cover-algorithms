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
#include <stdlib.h>

class MonHeurVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
public:
	MonHeurVCStrategy(Graph &g) : g(&g){}

	virtual string getName(){
		return string("MonHeur-VC");
	}

	virtual VertexCover findOptimalSolution(){
		VertexCover best;
		for(unsigned int i=0; i< g->N;++i)
			best.vertices.push_back(i);
		for(unsigned int i=0; i<1000; ++i){
			map<int, bool> forbiden;
			int vertices = ( rand() % (i+1) )%(int)(g->N/2);
			for (int k=0; k<vertices; ++k){
				forbiden[rand()%g->N] = true;
			}
			VertexCover current = findOptimalSolutionWithBlock(forbiden);
			if(current.vertices.size() < best.vertices.size())
				best = current;
		}
		return best;
	}

	virtual VertexCover findOptimalSolutionWithBlock(map<int,bool> &forbiden){
		vector<Edge> edges = g->getEdges();
		std::map<int, int> edgesDegree;
		for(unsigned int i=0; i< edges.size(); ++i){
			edgesDegree[g->getUniqueIdentifier(edges[i])] = 2;
		}
		vector< vector<int> > adjList = g->getAdjancyList();
		VertexCover cover;
		for(unsigned int i=0; i< adjList.size(); ++i){
			bool isRemovable = true;
			if(forbiden.find(i) != forbiden.end())
				isRemovable = false;
			else{
				for(unsigned int j=0; j< adjList[i].size(); ++j){
					Edge edge = g->getEdge(i, adjList[i][j]);
					if(edgesDegree[g->getUniqueIdentifier(edge)] < 2){
						isRemovable = false;
						break;
					}
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
		return VertexCover();
	}


	virtual ~MonHeurVCStrategy(){};
};



#endif /* MONHEURVCSTRATEGY_H_ */
