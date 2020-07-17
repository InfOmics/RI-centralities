// Base class centrality measures

#include "Graph.h"

namespace rilib{
	class Centrality{
		public:
			Graph *query, *ref;
			double *scores;

			Centrality(Graph& q, Graph& r){
				query = &q;
				ref = &r;
				scores = new double[query->nof_nodes];	
			}

			bool isNeig(int n, int z){
				for(int i = 0;i < query->in_adj_sizes[n];i++){
					if(query->in_adj_list[n][i] == z)
						return true;
				}

				for(int i = 0;i < query->out_adj_sizes[n];i++){
					if(query->out_adj_list[n][i] == z)
						return true;
				}
				return false;
			}

	};
}
