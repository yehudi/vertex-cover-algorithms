/*
 * ARBVC.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef ARBVCSTRATEGY_H_
#define ARBVCSTRATEGY_H_

#include <stack>
#include <map>
#include <vector>

#include "VertexCoverStrategy.h"
#include "GreedyVCStrategy.h"
#include "Graph.h"

class ARBVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
	int k;
	int maxK;
public:
	VertexCover best;
	ARBVCStrategy(Graph &g) : g(&g), k(0),maxK(g.N){}

	ARBVCStrategy(Graph &g, int k) : g(&g), k(k),maxK(g.N){}

	virtual string getName(){
		return string("ARB-VC");
	}

	virtual int getK(){
		return maxK;
	}

	virtual VertexCover findOptimalSolution(){
		vector< vector <int> > adj = g->getAdjancyList();
		vector<Edge> edges = g->getEdges();
		map<int, int> m;
		for(unsigned int i=0; i< edges.size(); ++i){
			m[g->getUniqueIdentifier(edges[i])] = g->getUniqueIdentifier(edges[i]);
		}

		GreedyVCStrategy strategy(*this->g);

		VertexCover currentBest = strategy.findOptimalSolution();
		maxK = currentBest.vertices.size();
		int minK = maxK/2;

		bool nDone = true;
		while(nDone){
			int k = minK + (maxK-minK)/2;
			if(maxK - minK<=1){
				nDone = false;
			}else{
				if(this->isAccrossTimeLimit()){
					break;
				}
				best = VertexCover();
				bool res = findOptimalSolutionForKRec(k, adj, m);
				if(res){
					maxK = k;
					currentBest = best;
				}else{
					minK = k;
				}
			}
		}
		return currentBest;
	}

	virtual bool findOptimalSolutionForKRec(int k, Graph &g){
		vector< vector <int> > adj = g.getAdjancyList();
		vector<Edge> edges = g.getEdges();
		map<int, int> m;
		for(unsigned int i=0; i< edges.size(); ++i){
			m[g.getUniqueIdentifier(edges[i])] = g.getUniqueIdentifier(edges[i]);
		}
		return findOptimalSolutionForKRec(k,adj,m);
	}

	virtual bool findOptimalSolutionForKRec(int k, vector< vector<int> > &adj, map<int, int> &m){
		if(m.empty()){
			return true;
		}
		else if(k==0){
			return false;
		}
		else{
			if(this->isAccrossTimeLimit()){
				return false;
			}
			Edge e = g->getEdge(m.begin()->first);
			stack<int> vertices;
			for(unsigned int i=0; i<adj[e.first].size(); ++i){
				int id = g->getUniqueIdentifier(g->getEdge(e.first,adj[e.first][i]));
				if(m.find(id) != m.end()){
					vertices.push(id);
					m.erase(id);
				}
			}
			bool res = findOptimalSolutionForKRec(k-1, adj, m);

			while(!vertices.empty()){
				m[vertices.top()] = vertices.top();
				vertices.pop();
			}

			if(res){
				best.vertices.push_back(e.first);
				return true;
			}

			for(unsigned int i=0; i<adj[e.second].size(); ++i){
				int id = g->getUniqueIdentifier(g->getEdge(e.second,adj[e.second][i]));
				if(m.find(id) != m.end()){
					vertices.push(id);
					m.erase(id);
				}
			}

			res = findOptimalSolutionForKRec(k-1, adj, m);

			while(!vertices.empty()){
				m[vertices.top()] = vertices.top();
				vertices.pop();
			}
			if(res){
				best.vertices.push_back(e.second);
			}
			return res;
		}
	}

	virtual void setK(int k){
		this->k = k;
	}
	virtual ~ARBVCStrategy(){};
};

#endif /* ARBVCSTRATEGY_H_ */
