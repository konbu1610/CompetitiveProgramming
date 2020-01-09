/*
    Graph用クラス：隣接リストを用いる
    Graph :
   無向、有向で共通のアルゴリズムを記述、それぞれのグラフでこのグラフを継承する
    GraphUD: 無向グラフ用のクラス
    GraphD: 有効グラフ用のクラス

    verify
    Graph:
        dijkstra        ABC035D, aoj1383
        dijkstra2       ABC022C
        warshall_floyd  ABC051D, ABC143E, ABC022C, ABC012D
        isBipartile     code-festival2017QualB C
        getEdgeMat      ABC022C

    GraphUD:
        getBridges      aoj1383

    GraphD:
        bellmanFord2 ABC137E
        scc ABC142F
*/

#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) REP(i, 0, n)
#define REP(i, s, e) for(int i = (s); i < (int)(e); i++)
#define all(r) r.begin(), r.end()

template <typename T> T chmax(T &a, const T &b) { return a = (a > b ? a : b); }
template <typename T> T chmin(T &a, const T &b) { return a = (a < b ? a : b); }

//有向、無向グラフ共通クラス(隣接リスト)
struct Graph {
    int n;
    using WEIGHT_TYPE = long long;
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
        priority_queue<P, vector<P>, greater<P>> q;
        q.push({0LL, s});
        while(!q.empty()) {
            auto p = q.top();
            q.pop();
            int cur = p.second;
            auto cost = p.first;
            if(d[cur] < p.first) continue;
            for(auto &e : es[cur]) {
                int to = e.to;
                auto dist = e.weight + cost;
                if(dist < d[to]) {
                    d[to] = dist;
                    q.push({dist, to});
                }
            }
        }
        return d;
    }
    // dijkstra O(V^2)
    vector<WEIGHT_TYPE> dijkstra2(int s) {
        vector<WEIGHT_TYPE> d(n, INF);
        d[s] = 0;
        vector<int> used(n);
        auto mat = getEdgeMat();
        while(1) {
            int cur = -1;
            rep(i, n) {
                if(used[i]) continue;
                if(cur == -1 || d[i] < d[cur]) cur = i;
            }
            if(cur == -1) break;
            used[cur] = 1;
            rep(i, n) { chmin(d[i], d[cur] + mat[cur][i]); }
        }
        return d;
    }
    // warshall_floyd O(n^3)
    vector<vector<WEIGHT_TYPE>> warshall_floyd() {
        // vector<vector<WEIGHT_TYPE>> d(n, vector<WEIGHT_TYPE>(n, INF));
        // rep(i, n) d[i][i] = 0LL;
        // rep(i, n) for (auto && e : es[i]) {
        //     int j = e.to;
        //     chmin(d[i][j], e.weight);
        // }
        auto d = getEdgeMat();
        rep(k, n) rep(i, n) rep(j, n) { chmin(d[i][j], d[i][k] + d[k][j]); }
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
            for(auto &&e : es[cur]) {
                if(!ret[e.to]) {
                    ret[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return ret;
    }
    // 2部グラフ判定
    bool isBipartile() {
        vector<int> memo(n, -1);
        rep(i, n) {
            if(memo[i] != -1) continue;
            queue<int> q;
            q.push(i);
            memo[i] = 0;
            while(!q.empty()) {
                auto v = q.front();
                q.pop();
                for(auto &&e : es[v]) {
                    auto u = e.to;
                    if(memo[u] == -1) {
                        memo[u] = !memo[v];
                        q.push(u);
                    } else if(memo[u] == memo[v]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    vector<vector<WEIGHT_TYPE>> getEdgeMat() {
        vector<vector<WEIGHT_TYPE>> mat(n, vector<WEIGHT_TYPE>(n, INF));
        rep(i, n) mat[i][i] = 0;
        rep(i, n) {
            for(auto &&e : es[i]) chmin(mat[i][e.to], e.weight);
        }
        return mat;
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
        rep(i, n) for(auto &&e : es[i]) {
            if(i < e.to) ret.push_back({i, e.to, e.weight});
        }
        return ret;
    }
    // 橋の検出
    // http://nupioca.hatenadiary.jp/entry/2013/11/03/200006
    // Calculate bridges in a undirected graph.
    // Assume graph is connected and has no parallel edges or self-loops.
    vector<Edge2> getBridges() {
        int V = n;
        // res: bridges
        vector<Edge2> res;
        // assume at least the first vertex exists
        vector<int> low(V, -1); // lowest reacheable index
        vector<int> pre(V, -1); // pre-order index
        int count = 0;          // pre-order index counter

        // v: current node
        // from: parent node
        function<int(int, int)> dfs = [&](int v, int from) {
            pre[v] = count++;
            low[v] = pre[v];
            for(auto &&e : es[v]) {
                int to = e.to;
                if(pre[to] == -1) {
                    // destination has not been visited
                    // visit destination and update low[v]
                    low[v] = min(low[v], dfs(to, v));
                    if(low[to] == pre[to]) {
                        // edge is not contained in a closed path -> bridge
                        res.push_back({v, to, e.weight});
                    }
                } else {
                    if(from == to) {
                        // ignore a path to parent
                        continue;
                    }
                    low[v] = min(low[v], low[to]);
                }
            }
            return low[v];
        };

        dfs(0, -1); // start dfs from vertex 0

        return res;
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
        rep(i, n) for(auto &&e : es[i]) { ret.push_back({i, e.to, e.weight}); }
        return ret;
    }
    GraphD getReverseGraph() {
        GraphD g(n);
        rep(i, n) for(auto &&e : es[i]) { g.add_edge(e.to, i, e.weight); }
        return g;
    }
    vector<vector<int>> scc() {
        vector<vector<int>> res;
        vector<int> cmp(n);
        vector<int> vs;
        vector<vector<int>> r_es(n);
        rep(i, n) for(auto &&e : es[i]) {
            int j = e.to;
            r_es[j].push_back(i);
        }

        vector<bool> used(n);
        function<void(int)> dfs = [&](int v) {
            used[v] = true;
            for(auto &&e : es[v]) {
                int to = e.to;
                if(!used[to]) dfs(to);
            }
            vs.push_back(v);
        };
        function<void(int, int)> rdfs = [&](int v, int k) {
            used[v] = true;
            cmp[v] = k;
            for(auto &&to : r_es[v]) {
                if(!used[to]) rdfs(to, k);
            }
        };

        fill(all(used), 0);
        vs.clear();
        for(int v = 0; v < n; v++) {
            if(!used[v]) dfs(v);
        }
        fill(all(used), 0);
        int k = 0;
        for(int i = vs.size() - 1; i >= 0; i--) {
            if(!used[vs[i]]) rdfs(vs[i], k++);
        }
        res.clear();
        res.resize(k);
        for(int i = 0; i < n; i++) {
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
            for(auto &&e : es) {
                if(dist[e.to] > dist[e.from] + e.weight) {
                    dist[e.to] = dist[e.from] + e.weight;
                    if(i == n - 1) {
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
            for(auto &&e : es) {
                if(!(f1[e.from] && f2[e.to])) continue;
                if(dist[e.to] > dist[e.from] + e.weight) {
                    dist[e.to] = dist[e.from] + e.weight;
                    if(i == n - 1) {
                        dist[s] = INF;
                        return dist;
                    }
                }
            }
        }
        return dist;
    }
};

// int main() {

//     return 0;
// }
