//２部グラフ判定
//veriffied ARC 099E

bool checkBipartite(int n) {
    vector<int> color(n, -1);
    rep(i, n) {
        if(color[i] != -1) continue;
        color[i] = 0;
        queue<int> q;
        q.push(i);
        while(!q.empty()) {
            int p = q.front();
            q.pop();
            for(auto& to : es[p]) {
                if(color[to] == -1) {
                    color[to] = !color[p];
                    q.push(to);
                }
                else if(color[p] == color[to]) {
                    return false;
                }
            }
        }
    }
    return true;
}