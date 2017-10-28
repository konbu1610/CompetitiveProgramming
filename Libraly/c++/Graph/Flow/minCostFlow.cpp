/*
    最小費用流 (ダイクストラ) O(F |E| logV)
    verified : AOJ GRL6 B
*/
struct MinCostFlow
{
    typedef pair<int, int> P;   // first -> minDist, second -> v
    struct Edge
    {
        int to, cap, cost, rev;
    };
    const int V;
    vector<vector<Edge>> G;
    vector<int> h, dist, prevv, preve;
    MinCostFlow(int v) : V(v), G(v), h(v), dist(v), prevv(v), preve(v){}
    void add_edge(int from, int to, int cap, int cost) {
        G[from].push_back({to, cap, cost, (int)G[to].size()});
        G[to].push_back({from, 0, -cost, (int)G[from].size()-1});
    }
    // minCostFlow s -> t
    // noexist flow return -1
    int min_cost_flow(int s, int t, int f) {
        int res = 0;
        for(int i = 0; i < V; ++i) h[i] = 0;
        while(f > 0) {
            priority_queue<P,vector<P>,greater<P> > q;
            for(int i = 0; i < V; ++i) dist[i] = 1e9;
            dist[s] = 0;
            q.push(P(0, s));
            while(!q.empty()) {
                P p = q.top();    q.pop();
                int v = p.second;
                if(dist[v] < p.first) continue;;
                for(int i = 0; i < G[v].size(); ++i) {
                    auto& e = G[v][i];
                    if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        q.push(P(dist[e.to], e.to));
                    }
                }
            }
            if(dist[t] == 1e9) return -1;
            for(int v = 0; v < V; ++v) h[v] += dist[v];
            int d = f;
            for(int v = t; v != s; v = prevv[v]) {
                d = min(d, G[prevv[v]][preve[v]].cap);
            }
            f -= d;
            res += d * h[t];
            for(int v = t; v != s; v = prevv[v]) {
                Edge& e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
        }
        return res;
    } 
};


/*
    最小費用流 (ベルマンフォード) O(F |E| |V|)
    verified : AOJ GRL6 B
*/
struct MinCostFlow
{
    typedef pair<int, int> P;   // first -> minDist, second -> v
    struct Edge
    {
        int to, cap, cost, rev;
    };
    const int V;
    vector<vector<Edge>> G;
    vector<int> dist, prevv, preve;
    MinCostFlow(int v) : V(v), G(v), dist(v), prevv(v), preve(v){}
    void add_edge(int from, int to, int cap, int cost) {
        G[from].push_back({to, cap, cost, (int)G[to].size()});
        G[to].push_back({from, 0, -cost, (int)G[from].size()-1});
    }
    // minCostFlow s -> t
    // noexist flow return -1
    int min_cost_flow(int s, int t, int f) {
        int res = 0;
        while(f > 0) {
            for(int i = 0; i < V; ++i) dist[i] = INF;
            dist[s] = 0;
            bool update = true;
            while(update) {
                update = false;
                for(int v = 0; v < V; ++v) {
                    if(dist[v] == INF) continue;
                    for(int i = 0; i < G[v].size(); ++i) {
                        Edge& e = G[v][i];
                        if(e.cap > 0 && dist[e.to] > dist[v] + e.cost) {
                            dist[e.to] = dist[v] + e.cost;
                            prevv[e.to] = v;
                            preve[e.to] = i;
                            update = true;
                        }
                    }
                }
            }
            if(dist[t] == INF) return -1;
            int d = f;
            for(int v = t; v != s; v = prevv[v]) {
                d = min(d, G[prevv[v]][preve[v]].cap);
            }
            f -= d;
            res += d * dist[t];
            for(int v = t; v != s; v = prevv[v]) {
                Edge& e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
        }
        return res;
    } 
};