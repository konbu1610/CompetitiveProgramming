/*
    最大流
    verified AOJ GLR7-A, ABC010D
*/
struct MaxFlow {
    struct Edge {
        int to, cap, rev;
        Edge() {};
        Edge(int to, int cap, int rev) : to(to), cap(cap), rev(rev) {};
    };
    int V;
    vector<vector<Edge>> G;
    vector<int> used;

    MaxFlow(int V) : V(V), G(V, vector<Edge>()), used(V){}

    void add_edge(int from, int to, int cap) {
        G[from].push_back({to, cap, (int)G[to].size()});
        G[to].push_back({from, 0, (int)G[from].size() - 1});
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        used[v] = true;
        for (int i = 0; i < G[v].size(); i++) {
            Edge& e = G[v][i];
            if (!used[e.to] && e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (1) {
            // memset(used, 0, sizeof(used));
            used = vector<int>(V, 0);
            int f = dfs(s, t, INF);
            if (f == 0) return flow;
            flow += f;
        }
    }
};