// Betweenness Centrality (BC)
// Uses a modified version of the Dijkstra algorithm

#include "Graph.h"
#include "DijkstraMod.h"

namespace rilib{
	class BetCentrality: public Centrality{
		
		public:

		BetCentrality(Graph& query, Graph& ref): Centrality(query, ref){
			double acc = 0.0;

			for(int i = 0;i < query.nof_nodes;i++){
				acc = 0.0;
				for(int j = 0;j < query.nof_nodes;j++){
					if(j != i)
						acc += dijkstraMod(&query, &ref, j, i);
				}
				scores[i] = acc;
				//cout << i << " " << acc << endl;
			}
		}
	};
}
