// Local Average connectivity-based Centrality

#include "Graph.h"

namespace rilib{
	class LacCentrality: public Centrality{
		public:			

			LacCentrality(Graph& query, Graph& ref): Centrality(query, ref){
				double acc;
				int i1, i2, n;
				//map<pair<int, int>, double> :: iterator pos1;
				
				for(int i = 0;i < query.nof_nodes;i++){
					acc = 0;
					
					for(int s = 0;s < query.out_adj_sizes[i];s++){				
						n = query.out_adj_list[i][s];
						i1 = *((int*)query.nodes_attrs[n]);						
						
						for(int t = 0;t < query.out_adj_sizes[n];t++){
							if(isNeig(i, query.out_adj_list[n][t])){
								i2 = *((int*)query.nodes_attrs[query.out_adj_list[n][t]]);
								//pos1 = ref.w.find(make_pair(i1,i2));
								
								/*if(pos1 != ref.w.end()){
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
						}
							
						for(int t = 0;t < query.in_adj_sizes[n];t++){
							if(isNeig(i, query.in_adj_list[n][t])){
								i2 = *((int*)query.nodes_attrs[query.in_adj_list[n][t]]);
								//pos1 = ref.w.find(make_pair(i1,i2));
								
								/*if(pos1 != ref.w.end()){
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
						}
					}
					// -----------------------------------------------------------------------------------------


					for(int s = 0;s < query.in_adj_sizes[i];s++){		
						n = query.in_adj_list[i][s];
						i1 = *((int*)query.nodes_attrs[n]);						
						for(int t = 0;t < query.out_adj_sizes[n];t++){
							if(isNeig(i, query.out_adj_list[n][t])){
								i2 = *((int*)query.nodes_attrs[query.out_adj_list[n][t]]);
								//pos1 = ref.w.find(make_pair(i1,i2));
								
								/*if(pos1 != ref.w.end()){
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
						}

						for(int t = 0;t < query.in_adj_sizes[n];t++){
							if(isNeig(i, query.in_adj_list[n][t])){
								i2 = *((int*)query.nodes_attrs[query.in_adj_list[n][t]]);
								//pos1 = ref.w.find(make_pair(i1,i2));
								
								/*if(pos1 != ref.w.end()){
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
						}
					}
					scores[i] = acc / (query.out_adj_sizes[i] + query.in_adj_sizes[i]);
					//cout << scores[i] << endl;
				}

			}	

		};
}

