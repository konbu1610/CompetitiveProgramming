/*
    遅延伝搬 segment tree
    (区間更新、最小値取得)
    
    verified : AOJ DSL2 F
*/
template <typename T>
struct SegTree {
    int n;
    vector<T> dat, lazy;
    const T NIL = 2e18;
    const T DFLT;    // 範囲外で返す値 minQuery -> 1e18, maxQuery -> -1e18
    SegTree(int n_, const T& initV, const T& dflt) : DFLT(dflt) {
        n = 1;
        while (n < n_) n *= 2;
        dat.assign(2 * n - 1, initV);
        lazy.assign(2 * n - 1, NIL);
    }
    void add(int l, int r, const T& t) {
        add(l, r, t, 0, 0, n);
    }
    T query(int l, int r) {
        return query(l, r, 0, 0, n);
    }
private:
    T f(const T& a, const T& b) {
        return min(a, b);
    }
    void fix(int k) {
        dat[k] = f(dat[k * 2 + 1], dat[k * 2 + 2]);
    }
    void setLazy(int k, const T& v) {
        lazy[k] = v;
        dat[k] = v;
    }
    void push(int k) {
        if (lazy[k] == NIL) return;
        setLazy(k * 2 + 1, lazy[k]);
        setLazy(k * 2 + 2, lazy[k]);
        lazy[k] = NIL;
    }
    void add(int queryL, int queryR, const T& val, int k, int nodeL, int nodeR) {
        // クエリ区間とノード区間が交差していないなら、これ以上、処理する意味はない
        if (nodeR <= queryL || queryR <= nodeL) {
            return;
        }
        // ノード区間がクエリ区間に完全に覆われたら、遅延命令をセットして、さっさと帰る
        if (queryL <= nodeL && nodeR <= queryR) {
            setLazy(k, val);
            return;
        }
        // ノードが下がるときには命令をpushする
        push(k);
        int nodeM = (nodeL + nodeR) / 2;
        add(queryL, queryR, val, k * 2 + 1, nodeL, nodeM);
        add(queryL, queryR, val, k * 2 + 2, nodeM, nodeR);

        // ノードが上がるときには情報をfixする
        fix(k);
    }
    T query(int queryL, int queryR, int k, int nodeL, int nodeR) {
        // クエリ区間とノード区間が交差していない
        if (nodeR <= queryL || queryR <= nodeL) {
            return DFLT;
        }
        // ノード区間がクエリ区間に完全に覆われた
        if (queryL <= nodeL && nodeR <= queryR) {
            return dat[k];
        }
        // ノードが下がるときには命令をpushする
        push(k);
        int nodeM = (nodeL + nodeR) / 2;
        T vl = query(queryL, queryR, k * 2 + 1, nodeL, nodeM);
        T vr = query(queryL, queryR, k * 2 + 2, nodeM, nodeR);
        return f(vl, vr);
    }
};