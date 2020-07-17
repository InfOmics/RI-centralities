// Subgraph Centrality (SC)

#include "Graph.h"
#include "armadillo"
using namespace arma;

namespace rilib{
	class SubCentrality: public Centrality{
		public:
			
			SubCentrality(Graph& query, Graph& ref): Centrality(query, ref){

				int i1, i2;
				int n;
				int max_eig_index;

				double score = 0.0,
					   max_eig;

				mat A, eig_vec, eig_vec_square, lambda;
				vec eig_val, cen;
				
				//map<pair<int, int>, double> :: iterator pos1;
				A = mat(query.nof_nodes, query.nof_nodes, fill::zeros);
			
				for(int i = 0;i < query.nof_nodes;i++){
					for(int j = 0;j < query.out_adj_sizes[i];j++){						
						n = query.out_adj_list[i][j];
						i1 = *((int*)query.nodes_attrs[i]);	
						i2 = *((int*)query.nodes_attrs[n]);	
						//pos1 = ref.w.find(make_pair(i1,i2));
						
						/*if(pos1 != ref.w.end()){
							A(i,n) = ref.w[make_pair(i1,i2)];
							A(n,i) = ref.w[make_pair(i1,i2)];
						}
						else{
							A(i,n) = ref.w[make_pair(i2,i1)];
							A(n,i) = ref.w[make_pair(i2,i1)];
						}*/

						if(i1 != i2)
							A(i,n) = A(n,i) = query.label_adj_matrix[i1][i2] +
											  query.label_adj_matrix[i2][i1];
						else
							A(i,n) = A(n,i) = query.label_adj_matrix[i1][i2];
					}
				}

				//A.raw_print(cout);

				// computes eigenvalues and eigenvectors
				eig_sym(eig_val, eig_vec, A);

				// matrix of squares of eigenvectors
				eig_vec_square = square(eig_vec);

				// vector of centralities
				cen = eig_vec_square * exp(eig_val);

				for(int i = 0;i < query.nof_nodes;i++){
					scores[i] = cen(i);
					//cout << scores[i] << endl;
				}
		}
	};
}


