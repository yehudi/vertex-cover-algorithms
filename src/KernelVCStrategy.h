/*
 * KernelVCStrategy.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef KERNELVCSTRATEGY_H_
#define KERNELVCSTRATEGY_H_

#include "VertexCoverStrategy.h"

class KernelVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
	int k;
	int addedToCover;
	int oldAddedToCover;
	Graph ker;
public:
	KernelVCStrategy(Graph &g) : g(&g), k(g.N), addedToCover(0), oldAddedToCover(0){}

	virtual string getName(){
		return string("Kernel-VC");
	}

	virtual int getK(){
		return k;
	}

	virtual VertexCover findOptimalSolution(){
		VertexCover currentBest;
		VertexCover best;
		GreedyVCStrategy strategy(*this->g);
		strategy.setTimeLimit(this->time_limit);
		currentBest = strategy.findOptimalSolution();

		int minK = currentBest.vertices.size()/2;
		k = currentBest.vertices.size();
		int maxK = k;
		bool nDone = true;

		while(nDone){
			unsigned int k = minK + (maxK-minK)/2;
			if(maxK - minK<=1){
				nDone = false;
			}else{
				if(this->isAccrossTimeLimit()){
					break;
				}
				getGraphKer(*(this->g), k);
				ARBVCStrategy ipl(ker);
				ipl.setTimeLimit(this->time_limit);
				best = ipl.findOptimalSolution();
				bool res = best.vertices.size() + (g->N-ker.N) <= k;
				if(res){
					currentBest = best;
					for(unsigned int i=0;i<g->N-ker.N; ++i)
						currentBest.vertices.push_back(0);
					maxK = k;
					this->k = k;
				}else{
					minK = k;
				}
			}
		}
		return currentBest;
	}

	virtual void getGraphKer(Graph &g, unsigned int k){
		oldAddedToCover = addedToCover;
		vector<int> ids;
		for(unsigned int i=0; i< g.N; ++i){
			if(g.getDegree(i) <= 1 || g.getDegree(i) >= k+1)
				ids.push_back(i);
		}
		addedToCover += ids.size();
		Graph newG = g.removeVertices(ids);
		//if(addedToCover == oldAddedToCover){
			ker = newG;
			return;
		//}
		//this->getGraphKer(newG, k);
		//return ;
	}

	virtual ~KernelVCStrategy(){};
};




#endif /* KERNELVCSTRATEGY_H_ */
