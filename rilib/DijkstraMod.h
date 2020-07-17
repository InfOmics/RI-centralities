/* 
 Modified version of Dijkstra algorithm
 Considers the number of shortest paths to a node and
 the number of shortest paths that pass through a node z
*/

#include <queue>
#include <set>

#define INF 10000000000

double *D;

struct comp {
    bool operator() (const int &a, const int &b) {
        return D[a] > D[b];
    }
};

double dijkstraMod(Graph *query, Graph *ref, int source, int z){
	
	int u, v, i1, i2;
	int *count_sp, *count_spz;
	double w = 0.0, acc = 0.0;
	D = new double[query->nof_nodes];
	//map<pair<int, int>, double> :: iterator pos1;
	priority_queue <int, vector <int>, comp> Q;
	bool F[query->nof_nodes];

	set<int> visited;
	

	// count shortest paths
	count_sp = new int[query->nof_nodes];

	for(int i = 0;i < query->nof_nodes;i++)
		if(i == source) count_sp[i] = 1;
		else count_sp[i] = 0;
	
	// count shortest paths with z vertex
	count_spz = new int[query->nof_nodes];
	
	for(int i = 0;i < query->nof_nodes;i++)
		count_spz[i] = 0;

	// initialization
	D[source] = 0.0;
	F[source] = 0;
	Q.push(source);
	
	for(int i = 0;i < query->nof_nodes;i++){
		if(i == source) continue;	
		D[i] = INF;
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


			if(!F[v] && D[u] + w < D[v]) {
				
				if(u == z || v == z){
					count_spz[v] = count_sp[u];
				}
				else{
					count_spz[v] = count_spz[u];
				}
				
				count_sp[v] = count_sp[u];
				D[v] = D[u] + w;
                Q.push(v);
            }
			else if(!F[v] && D[u] + w == D[v]){
				
				if(u == z || v == z){
					count_spz[v] += count_sp[u];
				}
				else{
					count_spz[v] += count_spz[u];
				}
				count_sp[v] += count_sp[u];
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


			if(!F[v] && D[u] + w < D[v]) {
				
				if(u == z || v == z){
					count_spz[v] = count_sp[u];
				}
				else{
					count_spz[v] = count_spz[u];
				}
		
				count_sp[v] = count_sp[u];
                D[v] = D[u] + w;
                Q.push(v);
            }
			else if(!F[v] && D[u] + w == D[v]){
				
				if(u == z || v == z){
					count_spz[v] += count_sp[u];
				}
				else{
					count_spz[v] += count_spz[u];
				}
				count_sp[v] += count_sp[u];
			}
		}
		
		// definitive node
		F[u] = 1;

		if(u != z && u > source)
			acc += (double)count_spz[u]	/ count_sp[u];

	}
	return acc;
}

