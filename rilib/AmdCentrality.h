#include <cmath>
#include "Graph.h"
#include "armadillo"
using namespace arma;

namespace rilib{
	class AmdCentrality : public Centrality {
		
		public:
		mat Q;
		double **G;

		// Amoeboid centrality
		AmdCentrality(Graph& query, Graph& ref) : Centrality(query,ref){
			double acc = 0.0;
			int i1, i2, n;
			//map<pair<int, int>, double> :: iterator pos1;
			
			Q = mat(query.nof_nodes, query.nof_nodes, fill::zeros);
			G = new double*[query.nof_nodes];
			
			// crea la matrice di adiacenza pesata
			for(int i = 0;i < query.nof_nodes;i++)
  				G[i] = new double[query.nof_nodes];

			for(int i = 0;i < query.nof_nodes;i++){
				for(int j = 0;j < query.out_adj_sizes[i];j++){			
					n = query.out_adj_list[i][j];
					if(n < i) continue;
	
					i1 = *((int*)query.nodes_attrs[i]);	
					i2 = *((int*)query.nodes_attrs[n]);
							
					//pos1 = ref.w.find(make_pair(i1,i2));
														
					/*if(pos1 != ref.w.end()){
						G[i][n] = G[n][i] = ref.w[make_pair(i1,i2)];
					}
					else{
						G[n][i] = G[i][n] = ref.w[make_pair(i2,i1)];
					}*/

					if(i1 != i2)
						G[i][n] = G[n][i] = query.label_adj_matrix[i1][i2] +
											query.label_adj_matrix[i2][i1];
					else
						G[i][n] = G[n][i] = query.label_adj_matrix[i1][i2];	
				}
			}		

			// per tutte le coppie di nodi
			for(int s = 0;s < query.nof_nodes;s++){
				//acc = 0.0;
				for(int e = s+1;e < query.nof_nodes;e++){
					//cout << "Source: " << s << " End: " << e << endl;
					physarum(query, ref, s, e);
					//break;
				}
				//break;
			}

			// centralità misurata in base alla
			// criticità degli archi
			for(int i = 0;i < query.nof_nodes;i++){
				acc = 0.0;
				for(int j = 0;j < query.nof_nodes;j++){
					acc += Q(i,j);	
				}								
				scores[i] = acc;
				//cout << scores[i] << endl;
			}
		}

		double f(double x){
			return (3*pow(x,27)) / (1+2*pow(x,27));
		}	

		// Physarum model
		void physarum(Graph& query, Graph& ref, int s, int e){

			// matrice dei coefficienti del
			// sistema lineare per ricavare 
			// le pressioni Pi
			mat A = mat(query.nof_nodes-1, query.nof_nodes-1, fill::zeros);
	
			// matrice con le conduttività
			mat D = mat(query.nof_nodes, query.nof_nodes);

			// matrice flussi finali sugli archi
			mat Qt = mat(query.nof_nodes, query.nof_nodes, fill::zeros);

			// vettore pressioni
			vec P(query.nof_nodes);

			// vettore termini noti sistema lineare
			vec b(query.nof_nodes-1);
			b.fill(0.0);

			int count = 0, c;
			double coeff, S;
			int map_node[query.nof_nodes];

			// la conduttività iniziale di tutti
			// gli archi è settata a 0.5
			D.fill(0.5);

			P.fill(0.0);

			// mappo i nodi con indici univoci che
			// mi serviranno per creare la matrice A
			c = 0;
			for(int j = 0;j < query.nof_nodes;j++){
				if(j != e){
					map_node[j] = c;
					c++;
				}
				else
					map_node[j] = -1;
			}

			do{
				// Per calcolare i flussi sugli archi e aggiornare
				// le conduttività occorre calcolare le pressioni sui
				// nodi. Per farlo occorre risolvere il seguente sistema
				// lineare, considerando che la pressione sul nodo 
				// destinazione del modello physarum è nulla, P_e = 0:

				// \sum_{i} \frac{D_{i,j}}{L_{i,j}}(P_i - P_j) = \begin{cases} -1, & \mbox{se }j=s \\ 0, & \mbox{altrimenti } \end{cases}


				// costruisco la matrice A dei coefficienti
				for(int i = 0;i < query.nof_nodes;i++){
					if(i == s)
						b(map_node[i]) = -1;

					S = 0.0;	
					if(i != e){
						for(int j = 0;j < query.nof_nodes;j++){
							if(j != i && G[i][j] != 0){
								coeff = D(i,j) * G[i][j];
								S += coeff;
								if(j != e)
									A(map_node[i], map_node[j]) = coeff;
							}
						}
						A(map_node[i], map_node[i]) = -S;
					}
				}

				vec X(query.nof_nodes-1);

				// risolvo sistema lineare e ricavo le
				// pressioni P_i
				X = solve(A, b);

				// inserisco le pressioni trovate
				// nel vettore X, P_e = 0
				for(int i = 0;i < query.nof_nodes;i++){
					if(i == e) continue;
					P(i) = X(map_node[i]);
				}
	
				for(int i = 0;i < query.nof_nodes;i++){
					for(int j = i+1;j < query.nof_nodes;j++){
						if(G[i][j] != 0){
							// aggiorno flusso negli archi
							// Formula:
							// Q_{i,j} = D_{i,j} / L_{i,j} * (P_i - P_j)
							Qt(i,j) = D(i,j) * (P(i) - P(j)) * G[i][j];
						
							// lavoriamo con archi non orientati
							Qt(j,i) = Qt(i,j);
	
							// aggiorno la conduttività degli archi
							// la conduttività degli archi varia secondo
							// la formula d/dt D_{i,j} = f(|Q_{i,j}|) - \alpha D_{i,j}
							// si utilizza il metodo di eulero implicito per risolverla:
							// y_{n+1} = y_{n} + h * f(x_{n+1}, y_{n+1})
							D(i,j) += 0.1*(f(fabs(Qt(i,j))) - D(i,j));
							D(j,i) = D(i,j);
						}
					}
				}

				count++;
			}while(count < 4*log(query.nof_nodes));
			

			// accumulo in Q tutte le matrici
			// ottenute facendo variare s ed e
			Q += Qt;

		}
	};
}

