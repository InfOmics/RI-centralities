// Degree Centrality (DC)

#include "Graph.h"

namespace rilib{
	class DegCentrality: public Centrality{
		public:			

			DegCentrality(Graph& query, Graph& ref): Centrality(query, ref){
				double acc = 0.0;
				int i1, i2;
				//map<pair<int, int>, double> :: iterator pos1;
				

				for(int i = 0;i < query.nof_nodes;i++){
					acc = 0.0;
					i1 = *((int*)query.nodes_attrs[i]);
					for(int j = 0;j < query.out_adj_sizes[i];j++){		
						i2 = *((int*)query.nodes_attrs[query.out_adj_list[i][j]]);
						/*pos1 = ref.w.find(make_pair(i1,i2));
						
						if(pos1 != ref.w.end()){
							acc += ref.w[make_pair(i1,i2)];
						}
						else{
							acc += ref.w[make_pair(i2,i1)];
						}*/
						
						if(i1 != i2)
							acc += query.label_adj_matrix[i1][i2] +
								   query.label_adj_matrix[i2][i1];
						else
							acc += query.label_adj_matrix[i1][i2];
					}	

					for(int j = 0;j < query.in_adj_sizes[i];j++){
						i2 = *((int*)query.nodes_attrs[query.in_adj_list[i][j]]);
						
						//pos1 = ref.w.find(make_pair(i1,i2));
						
						//if(pos1 != ref.w.end()){
						//	acc += ref.w[make_pair(i1,i2)];
						//}
						//else{
						//	acc += ref.w[make_pair(i2,i1)];
						//}

						if(i1 != i2)
							acc += query.label_adj_matrix[i1][i2] +
								   query.label_adj_matrix[i2][i1];
						else
							acc += query.label_adj_matrix[i1][i2];

					}

					
					scores[i] = acc;
					//std::cout << acc << std::endl;
				}

			

			}	

	};
}
