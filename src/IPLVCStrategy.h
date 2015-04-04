/*
 * IPLVCStrategy.h
 *
 *  Created on: Mar 18, 2015
 *      Author: wolkom
 */

#ifndef IPLVCSTRATEGY_H_
#define IPLVCSTRATEGY_H_

#include "VertexCoverStrategy.h"
#include "Graph.h"
#include "glpk.h"
#include <vector>
#include <iostream>
#include <sstream>

class IPLVCStrategy : public virtual VertexCoverStrategy{
	Graph *g;
public:
	IPLVCStrategy(Graph &g) : g(&g){}

	virtual string getName(){
		return string("IPL-VC");
	}

	virtual VertexCover findOptimalSolution(){

		vector<Edge> edges = g->getEdges();
		int number_edges = edges.size();
		int number_vertex = g->N;
		vector<int> ia;
		vector<int> ja;
		vector<double> ra;
		glp_prob *lp;
		lp = glp_create_prob();
		glp_set_obj_dir(lp, GLP_MIN);
		glp_set_prob_name(lp, "min_vertex");
		glp_add_rows(lp, number_edges);
		for(int i=0; i < number_edges; ++i){
			ostringstream os;
			os<<"cs"<<i+1;
			glp_set_row_name(lp, i+1, os.str().c_str());
			glp_set_row_bnds(lp, i+1, GLP_LO, 1.0, 0.0);
		}

		glp_add_cols(lp,number_vertex);
		for(int i=0; i < number_vertex; ++i){
			ostringstream os;
			os<<"x"<<i+1;
			glp_set_col_name(lp, i+1, os.str().c_str());
			glp_set_col_bnds(lp, i+1, GLP_DB, 0.0, 1.0);
			glp_set_col_kind(lp, i+1, GLP_BV);
			glp_set_obj_coef(lp, i+1, 1.0);
		}

		for(int i=0; i<number_edges; ++i){
			ia.push_back( i+1  );
			ja.push_back( ((int) edges[i].first) +1);
			ra.push_back( 1.0 );
			ia.push_back( i+1  );
			ja.push_back( ((int) edges[i].second) +1);
			ra.push_back( 1.0 );
		}

		int * ias = new int[ia.size()+1];
		int * jas = new int[ja.size()+1];
		double * ras = new double[ra.size()+1];

		for(unsigned int i=0; i< ia.size() ;++i){
			ias[i+1] = ia[i];
			jas[i+1] = ja[i];
			ras[i+1] = ra[i];
		}

		glp_load_matrix(lp, ia.size(), ias, jas, ras);

		glp_term_out(GLP_OFF);

		glp_iocp parm;
		glp_init_iocp(&parm);
		parm.presolve = GLP_ON;
		parm.tm_lim = this->time_limit * 1000;
		glp_intopt(lp, &parm);


		VertexCover res;
		for(int i=0; i<number_vertex; ++i){
			if( glp_mip_col_val(lp, i+1) > 0)
				res.vertices.push_back(i);
		}
		delete[] ias;
		delete[] jas;
		delete[] ras;
		return res;
	}

	virtual ~IPLVCStrategy(){};
};


#endif /* IPLVCSTRATEGY_H_ */
