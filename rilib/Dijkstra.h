/* 
 Dijkstra algorithm
 return the sum of costs from the source
 to the other nodes
*/

#include <queue>
#include <set>
#define INF 10000000000

double *D1;

struct comp1 {
    bool operator() (const int &a, const int &b) {
        return D1[a] > D1[b];
    }
};

double dijkstra(Graph *query, Graph *ref, int source){
	
	int u, v, i1, i2;
	double w = 0, acc = 0;
	D1 = new double[query->nof_nodes];
	//map<pair<int, int>, double> :: iterator pos1;
	priority_queue <int, vector <int>, comp1> Q;
	bool F[query->nof_nodes];

	set<int> visited;

	// initialization
	D1[source] = 0.0;
	F[source] = 0;
	Q.push(source);
	
	for(int i = 0;i < query->nof_nodes;i++){
		if(i == source) continue;	
		D1[i] = INF;
		F[i] = 0;
	}

	// dijkstra
    while(!Q.empty()) {
        u = Q.top();
		i1 = *((int*)query->nodes_attrs[u]);

        Q.pop();
        if(F[u]) continue;

		visited.clear();

        for(int i = 0;i < query->in_adj_sizes[u];i++){
			v = query->in_adj_list[u][i];

			visited.insert(v);

			i2 = *((int*)query->nodes_attrs[v]);
			
			/*pos1 = ref->w.find(make_pair(i1,i2));
			if(pos1 != ref->w.end()){
				w = 1/ref->w[make_pair(i1,i2)];
			}
			else{
				w = 1/ref->w[make_pair(i2,i1)];
			}*/

			if(i1 != i2)
				w = 1/(query->label_adj_matrix[i1][i2] +
					   query->label_adj_matrix[i2][i1]);
			else
				w = 1/query->label_adj_matrix[i1][i2];


			//cout << w << endl;


			if(!F[v] && D1[u] + w < D1[v]) {
                D1[v] = D1[u] + w;
                Q.push(v);
            }
		}
		for(int i = 0;i < query->out_adj_sizes[u];i++){
			v = query->out_adj_list[u][i];

			if(visited.find(v) != visited.end()){
				continue;
			}

			
			i2 = *((int*)query->nodes_attrs[v]);
			/*pos1 = ref->w.find(make_pair(i1,i2));
			if(pos1 != ref->w.end()){
				w = 1/ref->w[make_pair(i1,i2)];
			}
			else{
				w = 1/ref->w[make_pair(i2,i1)];
			}*/

			if(i1 != i2)
				w = 1/(query->label_adj_matrix[i1][i2] +
					   query->label_adj_matrix[i2][i1]);
			else
				w = 1/query->label_adj_matrix[i1][i2];


			if(!F[v] && D1[u] + w < D1[v]) {
                D1[v] = D1[u] + w;
                Q.push(v);
            }
		}
		
		F[u] = 1;
		acc += D1[u];

	}
	return acc;

}
