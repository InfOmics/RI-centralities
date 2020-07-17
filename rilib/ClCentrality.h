// Closeness Centrality (CC)

#include "Graph.h"
#include "Dijkstra.h"

namespace rilib{
	class ClCentrality: public Centrality{
		
		public:

		ClCentrality(Graph& query, Graph& ref): Centrality(query, ref){
			double acc = 0.0; 
			
			for(int i = 0;i < query.nof_nodes;i++){
				acc = dijkstra(&query, &ref, i);
				scores[i] = 1 / acc;
				//cout << scores[i] << endl;
			}
		}
	};
}
