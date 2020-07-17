// Network Centrality (NC)

#include <cmath>

namespace rilib{
	class NetCentrality: public Centrality{
		
		public:
		NetCentrality(Graph& query, Graph& ref): Centrality(query, ref){
			//map<pair<int, int>, double> :: iterator pos1;	
			double S1 = 0, S2 = 0, T1 = 0, T2 = 0;
			double w1, w2, w3, acc = 0;
			int i1, i2, i3, j, s;

			for(int i = 0;i < query.nof_nodes;i++){
				i1 = *((int*)query.nodes_attrs[i]);
				
				T1 = 0;
				acc = 0;

				for(int h = 0;h < query.in_adj_sizes[i];h++){
					j = query.in_adj_list[i][h];
					i2 = *((int*)query.nodes_attrs[j]);	
					//pos1 = ref.w.find(make_pair(i1,i2));					

					/*if(pos1 != ref.w.end()){
						w1 = ref.w[make_pair(i1,i2)];
					}
					else{
						w1 = ref.w[make_pair(i2,i1)];
					}*/

					if(i1 != i2)
						w1 = query.label_adj_matrix[i1][i2] +
							 query.label_adj_matrix[i2][i1];
					else
						w1 = query.label_adj_matrix[i1][i2];	
					
					T1 += w1 - 1;

				}
				for(int h = 0;h < query.out_adj_sizes[i];h++){
					j = query.out_adj_list[i][h];
					i2 = *((int*)query.nodes_attrs[j]);	
					//pos1 = ref.w.find(make_pair(i1,i2));					

					/*if(pos1 != ref.w.end()){
						w1 = ref.w[make_pair(i1,i2)];
					}
					else{
						w1 = ref.w[make_pair(i2,i1)];
					}*/

					if(i1 != i2)
						w1 = query.label_adj_matrix[i1][i2] +
							 query.label_adj_matrix[i2][i1];
					else
						w1 = query.label_adj_matrix[i1][i2];					
					T1 += w1 - 1;

				}

				for(int h = 0;h < query.in_adj_sizes[i];h++){
					j = query.in_adj_list[i][h];				
					i2 = *((int*)query.nodes_attrs[j]);

					T2 = 0;					
					S1 = 0;
					S2 = 0;

					for(int h1 = 0;h1 < query.in_adj_sizes[j];h1++){
						s = query.in_adj_list[j][h1];
						i3 = *((int*)query.nodes_attrs[s]);

						//pos1 = ref.w.find(make_pair(i2,i3));					
						
						/*if(pos1 != ref.w.end()){
							w2 = ref.w[make_pair(i2,i3)];
						}
						else{
							w2 = ref.w[make_pair(i3,i2)];
						}*/

						if(i2 != i3)
							w2 = query.label_adj_matrix[i2][i3] +
							 	 query.label_adj_matrix[i3][i2];
						else
							w2 = query.label_adj_matrix[i2][i3];
						
						
						T2 += w2 - 1;

						if(isNeig(i, s)){
							//pos1 = ref.w.find(make_pair(i1,i3));
							
							/*if(pos1 != ref.w.end()){
								w3 = ref.w[make_pair(i1,i3)];
							}
							else{
								w3 = ref.w[make_pair(i3,i1)];
							}*/

							if(i1 != i3)
								w3 = query.label_adj_matrix[i1][i3] +
									 query.label_adj_matrix[i3][i1];
							else
								w3 = query.label_adj_matrix[i1][i3];	



							S1 += w3;
							S2 += w2;

						}
					}

					for(int h1 = 0;h1 < query.out_adj_sizes[j];h1++){
						s = query.out_adj_list[j][h1];
						i3 = *((int*)query.nodes_attrs[s]);

						//pos1 = ref.w.find(make_pair(i2,i3));					
						
						/*if(pos1 != ref.w.end()){
							w2 = ref.w[make_pair(i2,i3)];
						}
						else{
							w2 = ref.w[make_pair(i3,i2)];
						}*/

						if(i2 != i3)
							w2 = query.label_adj_matrix[i2][i3] +
								 query.label_adj_matrix[i3][i2];
						else
							w2 = query.label_adj_matrix[i2][i3];

						T2 += w2 - 1;

						if(isNeig(i, s)){
							//pos1 = ref.w.find(make_pair(i1,i3));												

							/*if(pos1 != ref.w.end()){
								w3 = ref.w[make_pair(i1,i3)];
							}
							else{
								w3 = ref.w[make_pair(i3,i1)];
							}*/

							if(i1 != i3)
								w3 = query.label_adj_matrix[i1][i3] +
									 query.label_adj_matrix[i3][i1];
							else
								w3 = query.label_adj_matrix[i1][i3];


							S1 += w3;
							S2 += w2;

						}
					}
					
					if(T1 < T2){
						acc += sqrt(S1*S2)/T1;
					}
					else{
						acc += sqrt(S1*S2)/T2;
					}
				}
				// ------------------------------------------------------


				// ------------------------------------------------------
				for(int h = 0;h < query.out_adj_sizes[i];h++){
					j = query.out_adj_list[i][h];					
					i2 = *((int*)query.nodes_attrs[j]);	
				
					T2 = 0;	
					S1 = 0;
					S2 = 0;

					for(int h1 = 0;h1 < query.in_adj_sizes[j];h1++){
						s = query.in_adj_list[j][h1];
						i3 = *((int*)query.nodes_attrs[s]);

						//pos1 = ref.w.find(make_pair(i2,i3));					
						
						/*if(pos1 != ref.w.end()){
							w2 = ref.w[make_pair(i2,i3)];
						}
						else{
							w2 = ref.w[make_pair(i3,i2)];
						}*/
					
						if(i2 != i3)
							w2 = query.label_adj_matrix[i2][i3] +
								 query.label_adj_matrix[i3][i2];
						else
							w2 = query.label_adj_matrix[i2][i3];

						T2 += w2 - 1;

						if(isNeig(i, s)){
							//pos1 = ref.w.find(make_pair(i1,i3));					
							
							/*if(pos1 != ref.w.end()){
								w3 = ref.w[make_pair(i1,i3)];
							}
							else{
								w3 = ref.w[make_pair(i3,i1)];
							}*/

							if(i1 != i3)
								w3 = query.label_adj_matrix[i1][i3] +
							 		 query.label_adj_matrix[i3][i1];
							else
								w3 = query.label_adj_matrix[i1][i3];			

							S1 += w3;
							S2 += w2;
						}
					}

					for(int h1 = 0;h1 < query.out_adj_sizes[j];h1++){
						s = query.out_adj_list[j][h1];
						i3 = *((int*)query.nodes_attrs[s]);

						//pos1 = ref.w.find(make_pair(i2,i3));					
						
						/*if(pos1 != ref.w.end()){
							w2 = ref.w[make_pair(i2,i3)];
						}
						else{
							w2 = ref.w[make_pair(i3,i2)];
						}*/

						w2 = query.label_adj_matrix[i2][i3] +
						 	 query.label_adj_matrix[i3][i2];

						T2 += w2 - 1;

						if(isNeig(i, s)){
							//pos1 = ref.w.find(make_pair(i1,i3));					
							
							/*if(pos1 != ref.w.end()){
								w3 = ref.w[make_pair(i1,i3)];
							}
							else{
								w3 = ref.w[make_pair(i3,i1)];
							}*/

							if(i1 != i3)
								w3 = query.label_adj_matrix[i1][i3] +
							 		 query.label_adj_matrix[i3][i1];
							else
					  			w3 = query.label_adj_matrix[i1][i3];	

							S1 += w3;
							S2 += w2;
						}
					}

					if(T1 < T2){
						acc += sqrt(S1*S2)/T1;
					}
					else{
						acc += sqrt(S1*S2)/T2;
					}
				}
				// -----------------------------------------------

				scores[i] = acc;
				//cout << scores[i] << endl;
			}
		}
	};
}
