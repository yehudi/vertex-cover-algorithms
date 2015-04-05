/*
 * Graph.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include <limits>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Edge{
public:
	int first;
	int second;
	Edge(int i, int j) : first(i), second(j){}
};

class Graph{
private:
	bool ** _adjancyMatrix = NULL;
	int * _degrees = NULL;
public:
	unsigned int N;

	Graph() : N(0){

	}

	Graph(unsigned int nodes) : N(nodes){
		this->_degrees = new int[nodes];
		this->_adjancyMatrix = new bool*[nodes];
		for(unsigned int i=0; i<nodes; ++i){
			this->_degrees[i] = 0;
			this->_adjancyMatrix[i] = new bool[i+1];
			for(unsigned int j=0; j<i+1; ++j){
				this->_adjancyMatrix[i][j] = 0;
			}
		}
	}

	void operator=(const Graph &D ){
		delete[] _degrees;
		for(unsigned int i=0; i<N; ++i){
			bool * val = _adjancyMatrix[i];
			_adjancyMatrix[i] = NULL;
			delete[] val;
		}
		delete[] _adjancyMatrix;
		this->N = D.N;
		this->_degrees = new int[this->N];
		this->_adjancyMatrix = new bool*[this->N];
		for(unsigned int i=0; i<this->N; ++i){
			this->_degrees[i] = 0;
			this->_adjancyMatrix[i] = new bool[i+1];
			for(unsigned int j=0; j<i+1; ++j){
				this->_adjancyMatrix[i][j] = 0;
			}
		}

		vector<Edge> edges = D.getEdges();

		for(unsigned int i=0; i<edges.size(); ++i){
			this->bind(edges[i].first, edges[i].second);
		}
	}

	int getUniqueIdentifier(Edge e) const{
		return e.first * this->N + e.second;
	}

	Edge getEdge(int id){
		return Edge(id/this->N, id % this->N);
	}

	Edge getEdge(int i, int j) const{
		int node1 = i;
		int node2 = j;
		if(i < j){
			node1 = j;
			node2 = i;
		}
		return Edge(node1, node2);
	}

	vector< vector<int> > getAdjancyList() const{
		vector< vector<int> > res;
		for(unsigned int i=0;i <N ;++i){
			res.push_back(vector<int>());
		}
		vector<Edge> edges = this->getEdges();
		for(unsigned int i=0; i< edges.size(); ++i){
			res[edges[i].second].push_back(edges[i].first);
			res[edges[i].first].push_back(edges[i].second);
		}
		return res;
	}

	Graph removeVertices(vector<int> vertices){
		vector< int > newIds;
		vector <bool> mask;
		vector< vector<int> > adj = this->getAdjancyList();
		for(unsigned int i=0; i< adj.size();++i)
			mask.push_back(true);
		for(unsigned int i=0; i< vertices.size();++i)
			mask[vertices[i]] = false;
		int k=0;
		for(unsigned int i=0; i< adj.size();++i){
			if(mask[i])
				newIds.push_back(k++);
			else
				newIds.push_back(-1);
		}
		vector<Edge> edges = getEdges();
		Graph g(k);

		for(unsigned int i=0; i<edges.size(); ++i){
			if(mask[edges[i].first] && mask[edges[i].second])
				g.bind(newIds[edges[i].first],newIds[edges[i].second]);
		}
		return g;
	}


	vector<Edge> getEdges() const{
		vector<Edge> res;
		for(unsigned int i=0; i<N; ++i){
			for(unsigned int j=0; j<N; ++j){
				if(i<j && this->edgeExist(i,j))
					res.push_back(this->getEdge(i,j));
			}
		}
		return res;
	}

	vector<int> getSuccessors(int i) const{
		vector<int> res;
		for(unsigned int j=0; j<N; ++j){
			if(this->edgeExist(i,j)){
				res.push_back(j);
			}
		}
		return res;
	}

	/*
	 * Bind node i to node j
	 */
	void bind(int i, int j){
		Edge edge = this->getEdge(i,j);
		if(!this->_adjancyMatrix[edge.first][edge.second]){
			this->_adjancyMatrix[edge.first][edge.second] = true;
			this->_degrees[i]+=1;
			this->_degrees[j]+=1;
		}
	}

	void unbind(int i, int j){
		Edge edge = this->getEdge(i,j);
		if(this->_adjancyMatrix[edge.first][edge.second]){
			this->_adjancyMatrix[edge.first][edge.second] = 0;
			this->_degrees[i]-=1;
			this->_degrees[j]-=1;
		}
	}

	/*
	 * Verify if nodes are linked by an edge
	 */
	bool edgeExist(int i, int j) const{
		Edge edge = this->getEdge(i, j);
		return this->_adjancyMatrix[edge.first][edge.second];
	}

	unsigned int getDegree() const{
		unsigned int res = 0;
		for(unsigned int i= 0; i< N; ++i){
			res+=this->_degrees[i];
		}
		return res/2;
	}

	unsigned int getDegree(int i) const{
		return this->_degrees[i];
	}

	void randomise(float p){
		assert(p <= 1.0);
		assert(p >= 0);
		srand(time(NULL));
		for( unsigned int i=0; i<N; ++i ){
			for( unsigned int j=0; j<N; ++j){
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if(r<p && i!=j){
					this->bind(i,j);
				}else{
					this->unbind(i,j);
				}
			}
		}
	}

	virtual ~Graph(){
		delete[] _degrees;
		for(unsigned int i=0; i<N; ++i){
			bool * val = _adjancyMatrix[i];
			_adjancyMatrix[i] = NULL;
			delete[] val;
		}
		delete[] _adjancyMatrix;
	}
};


ostream& operator<<(ostream& s, const Graph &g){
	s<<g.N<<endl;
	for(unsigned int i = 0; i<g.N; ++i){
		vector<int> nodes = g.getSuccessors(i);
		s<<i<<" \t->\t ";
		for(unsigned j=0; j<nodes.size(); ++j){
			s<<nodes[j]<<"  ";
		}
		s<<endl;
	}
	return s;
}

#endif /* GRAPH_H_ */
