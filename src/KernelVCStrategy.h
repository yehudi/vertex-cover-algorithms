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
	int _k;
	Graph ker;
public:
	KernelVCStrategy(Graph &g) : g(&g), k(g.N), _k(g.N){}

	virtual string getName(){
		return string("Kernel-VC");
	}

	virtual int getK(){
		return k;
	}

	virtual VertexCover findOptimalSolution(){
		VertexCover currentBest;
		GreedyVCStrategy strategy(*this->g);
		strategy.setTimeLimit(this->time_limit);
		currentBest = strategy.findOptimalSolution();

		int minK = currentBest.vertices.size()/2;
		k = currentBest.vertices.size();
		int maxK = k;
		bool nDone = true;

		while(nDone){
			int k = minK + (maxK-minK)/2;
			if(maxK - minK<=1){
				nDone = false;
			}else{
				bool res = false;
				VertexCover best;
				if(this->isAccrossTimeLimit()){
					break;
				}
				getGraphKer(*(this->g), k);
				if(_k >= 0 ){
					ARBVCStrategy ipl(ker);
					ipl.setTimeLimit(this->time_limit);
					best = ipl.findOptimalSolution();
					res = (int)best.vertices.size() + k -_k <= k;
				}
				if(res){
					currentBest = best;
					for( int i=0;i < k-_k; ++i)
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

	virtual void getGraphKer(Graph &g, int k){
		vector<int> ids;
		_k  = k;
		for(unsigned int i=0; i < g.N; ++i){
			if((int)g.getDegree(i) >= k+1){
				ids.push_back(i);
				_k-=1;
			}
			else if ( g.getDegree(i) == 1 ){
		    	ids.push_back(g.getSuccessors(i)[0]);
		    	_k-=1;
			}if(_k<0){
				return;
			}
		}
		Graph newG = g.removeVertices(ids);
		if(_k == k){
			ker = newG;
			return;
		}
		this->getGraphKer(newG, _k);
	}

	virtual ~KernelVCStrategy(){};
};




#endif /* KERNELVCSTRATEGY_H_ */
