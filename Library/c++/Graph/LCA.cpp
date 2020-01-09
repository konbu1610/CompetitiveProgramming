/*
    verify
    ABC014D
*/

struct LCA{
    static const int MAX_LOG = 20;
    const int V;
    int root;
    vector<vector<int>> es, parent;
    vector<int> depth;
    LCA(int V, int root) : V(V), es(V), parent(MAX_LOG, vector<int>(V)), depth(V), root(root){}
    void add_edge(int a, int b) {
        es[a].pb(b);
        es[b].pb(a);
    }
    void dfs(int cur, int par, int d) {
        parent[0][cur] = par;
        depth[cur] = d;
        for(auto& to: es[cur]) if(to != par) {
            dfs(to, cur, d+1);
        }
    }
    void init(){
        dfs(root, -1, 0);
        for(int k = 0; k+1 < MAX_LOG; ++k) {
            for(int v = 0; v < V; ++v) {
                if(parent[k][v] < 0) parent[k+1][v] = -1;
                else parent[k+1][v] = parent[k][parent[k][v]];
            }
        }
    }
    int lca(int u, int v) {
        if(depth[u] > depth[v]) swap(u, v);
        for(int k = 0; k < MAX_LOG; ++k) {
            if(((depth[v] -depth[u]) >> k) & 1) {
                v = parent[k][v];
            }
        }
        if(u == v) return u;
        for(int k = MAX_LOG-1; k >= 0; --k) {
            if(parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }
    int dist(int u, int v) {
        return depth[u] + depth[v] - 2*depth[lca(u, v)];
    }
};