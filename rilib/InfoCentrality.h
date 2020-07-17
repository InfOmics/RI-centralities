// Information Centrality (IC)

#include "Graph.h"
#include "armadillo"
using namespace arma;

namespace rilib{
	class InfoCentrality: public Centrality {
		public:
			
			InfoCentrality(Graph& query, Graph& ref): Centrality(query, ref){

				mat D;	// Diagonal matrix of degrees
				mat J;	// Matrix with all ones
				mat A;	// Weighted Adjacency matrix
				mat R;	// R = inv(D - A + J)

				int i1, i2, n;
				//int flag = 0;

				double score = 0.0,
			   		   temp2 = 0.0,
			   		   acc = 0.0;
				
				//map<pair<int, int>, double> :: iterator pos1;
				D = mat(query.nof_nodes, query.nof_nodes, fill::zeros);

				for(int i = 0;i < query.nof_nodes;i++)
					D(i,i) = query.in_adj_sizes[i] + query.out_adj_sizes[i];

				J = mat(query.nof_nodes, query.nof_nodes, fill::ones);
				A = mat(query.nof_nodes, query.nof_nodes, fill::zeros);

				for(int i = 0;i < query.nof_nodes;i++){
					i1 = *((int*)query.nodes_attrs[i]);	
					for(int j = 0;j < query.out_adj_sizes[i];j++){		
						n = query.out_adj_list[i][j];
						i2 = *((int*)query.nodes_attrs[n]);
							
						if(i1 != i2)
							A(i,n) = A(n,i) = query.label_adj_matrix[i1][i2] +
											  query.label_adj_matrix[i2][i1];
						else
							A(i,n) = A(n,i) = query.label_adj_matrix[i1][i2];


						//A(i,n) = A(n,i) = 1;
					}
				}

				//R = inv_sympd(D - A + J);

				R = inv(D - A + J);


				for(int i = 0;i < query.nof_nodes;i++){
					for(int j = 0;j < query.nof_nodes;j++){
						if(i != j){
							temp2 = R(i,i) + R(j,j) - 2*R(i,j);
							acc += temp2;
						}
					}
					score = query.nof_nodes / acc;
					scores[i] = score;
					temp2 = 0;
					acc = 0;
				}
		}
	};
}
