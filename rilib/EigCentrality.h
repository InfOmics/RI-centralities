// Eigenvector Centrality (EC)

#include "Graph.h"
#include "armadillo"
using namespace arma;

namespace rilib{
	class EigCentrality: public Centrality{
		public:
			
			EigCentrality(Graph& query, Graph& ref): Centrality(query, ref){

				int i1, i2;
				int n;
				double score = 0.0,
					   max_eig;
				
				uword max_eig_index;

				mat A;
				mat eig_vec;
				vec eig_val;

				//sp_mat A;

				//map<pair<int, int>, double> :: iterator pos1;
				A = mat(query.nof_nodes, query.nof_nodes, fill::zeros);

				//A = sp_mat(query.nof_nodes, query.nof_nodes /*, fill::zeros*/);
				

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

				// computes eigenvalues and eigenvectors
				eig_sym(eig_val, eig_vec,A);

				//eigs_sym(eig_val, eig_vec, A, 1);					

				// computes max eigenvalue				
				max_eig = eig_val.max(max_eig_index);

				// centrality measures	
				for(int i = 0;i < query.nof_nodes;i++){
					//scores[i] = 0;
					scores[i] = fabs(eig_vec(i, max_eig_index));
					//cout << scores[i]<< endl;
				}	
		}
	};
}

