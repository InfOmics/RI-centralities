#include "Graph.h"

namespace rilib{
	class FreqCentrality: public Centrality{
		public:			

		FreqCentrality(Graph& query, Graph& ref): Centrality(query, ref){
			int label;
			for(int i = 0;i < query.nof_nodes;i++){
				label = (*(int*)query.nodes_attrs[i]);
				scores[i] = 1.0/ref.freq[label];

				//cout << scores[i] << endl;
			}
		}
	};
}
