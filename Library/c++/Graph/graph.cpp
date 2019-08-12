/*
    Graph用クラス：隣接リストを用いる
    Graph : 無向、有向で共通のアルゴリズムを記述、それぞれのグラフでこのグラフを継承する
    GraphUD: 無向グラフ用のクラス
    GraphD: 有効グラフ用のクラス
    
    verify
    Graph:
        dijkstra        ABC035D
        warshall_floyd  ABC051D

    GraphD:
        bellmanFord2 ABC137E
*/

template<typename T> T chmax(T& a, const T& b) {return a = (a > b ? a : b);}
template<typename T> T chmin(T& a, const T& b) {return a = (a < b ? a : b);}


//有向、無向グラフ共通クラス(隣接リスト)
struct Graph {
    int n;
    using WEIGHT_TYPE = ll;
    static const WEIGHT_TYPE INF = 1e18;
    struct Edge {
        int to;
        WEIGHT_TYPE weight;
    };
    struct Edge2 {
        int from;
        int to;
        WEIGHT_TYPE weight;
    };
    vector<vector<Edge>> es;
    Graph(int n) : n(n), es(n) {}

    // dijkstra O(E log V)
    vector<WEIGHT_TYPE> dijkstra(int s) {
        vector<WEIGHT_TYPE> d(n, INF);
        d[s] = 0;
        using P = pair<WEIGHT_TYPE, int>;
        priority_queue<P, vector<P>, greater<P> > q;
        q.push({0LL, s});
        while (!q.empty()) {
            auto p = q.top(); q.pop();
            int cur = p.se;
            auto cost = p.fi;
            if (d[cur] < p.fi) continue;
            for (auto& e : es[cur]) {
                int to = e.to;
                auto dist = e.weight + cost;
                if (dist < d[to]) {
                    d[to] = dist;
                    q.push({dist, to});
                }
            }
        }
        return d;
    }
    // warshall_floyd O(n^3)
    vector<vector<WEIGHT_TYPE>> warshall_floyd() {
        vector<vector<WEIGHT_TYPE>> d(n, vector<WEIGHT_TYPE>(n, INF));
        rep(i, n) d[i][i] = 0LL;
        rep(i, n) for (auto && e : es[i]) {
            int j = e.to;
            chmin(d[i][j], e.weight);
        }
        rep(k, n) rep(i, n) rep(j, n) {
            chmin(d[i][j], d[i][k] + d[k][j]);
        }
        return d;
    }
    // 頂点sから到達できるか
    vector<bool> getVisitable(int s) {
        vector<bool> ret(n);
        queue<int> q;
        q.push(s);
        ret[s] = true;
        while(!q.empty()) {
            auto cur = q.front();
            q.pop();
            for(auto&& e : es[cur]) {
                if(!ret[e.to]) {
                    ret[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return ret;
    }
};

// 無向グラフ
struct GraphUD : public Graph {
    GraphUD(int n) : Graph(n) {}
    void add_edge(int from, int to, WEIGHT_TYPE weight) {
        es[from].push_back({to, weight});
        es[to].push_back({from, weight});
    }
    vector<Edge2> getEdge2() {
        vector<Edge2> ret;
        rep(i, n) for (auto && e : es[i]) {
            if (i < e.to) ret.push_back({i, e.to, e.weight});
        }
        return ret;
    }
};


// 有向グラフ
struct GraphD : public Graph {
    GraphD(int n) : Graph(n) {}
    void add_edge(int from, int to, WEIGHT_TYPE weight) {
        es[from].push_back({to, weight});
    }
    vector<Edge2> getEdge2() {
        vector<Edge2> ret;
        rep(i, n) for (auto && e : es[i]) {
            ret.push_back({i, e.to, e.weight});
        }
        return ret;
    }
    GraphD getReverseGraph() {
        GraphD g(n);
        rep(i, n) for (auto && e : es[i]) {
            g.add_edge(e.to, i, e.weight);
        }
        return g;
    }
    vector<vector<int>> scc() {
        vector<vector<int>> res;
        vector<int> cmp(n);
        vector<int> vs;
        vector<vector<int>> r_es(n);
        rep(i, n) for (auto && e : es[i]) {
            int j = e.to;
            r_es[j].push_back(i);
        }

        vector<bool> used(n);
        function<void(int)> dfs = [&](int v) {
            used[v] = true;
            for (auto && e : es[v]) {
                int to = e.to;
                if (!used[to]) dfs(to);
            }
            vs.push_back(v);
        };
        function<void(int, int)> rdfs = [&](int v, int k) {
            used[v] = true;
            cmp[v] = k;
            for (auto && to : r_es[v]) {
                if (!used[to]) rdfs(to, k);
            }
        };

        fill(all(used), 0);
        vs.clear();
        for (int v = 0; v < n; v++) {
            if (!used[v]) dfs(v);
        }
        fill(all(used), 0);
        int k = 0;
        for (int i = vs.size() - 1; i >= 0; i--) {
            if (!used[vs[i]]) rdfs(vs[i], k++);
        }
        res.clear();
        res.resize(k);
        for (int i = 0; i < n; i++) {
            res[cmp[i]].push_back(i);
        }
        return res;
    }
    // bellmanFord 負閉路があるなら, dist[s] = INF | O(VE)
    vector<WEIGHT_TYPE> bellmanFord(int s) {
        vector<WEIGHT_TYPE> dist(n, INF);
        dist[s] = 0;
        auto es = getEdge2();
        rep(i, n) {
            for(auto&& e : es) {
                if(dist[e.to] > dist[e.from] + e.weight) {
                    dist[e.to] = dist[e.from] + e.weight;
                    if(i == n-1) {
                        dist[s] = INF;
                        return dist;
                    }
                }
            }
        }
        return dist;
    }
    // bellmanFord s->tの経路上に負閉路があるなら, dist[s] = INF | O(VE)
    vector<WEIGHT_TYPE> bellmanFord2(int s, int t) {
        vector<WEIGHT_TYPE> dist(n, INF);
        auto f1 = getVisitable(s);
        auto f2 = getReverseGraph().getVisitable(t);
        dist[s] = 0;
        auto es = getEdge2();
        rep(i, n) {
            for(auto&& e : es) {
                if(!(f1[e.from]&&f2[e.to])) continue;
                if(dist[e.to] > dist[e.from] + e.weight) {
                    dist[e.to] = dist[e.from] + e.weight;
                    if(i == n-1) {
                        dist[s] = INF;
                        return dist;
                    }
                }
            }
        }
        return dist;
    }
};
