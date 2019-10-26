/*
    verify
    ABC126E
*/
struct UF{ //O(loga(n))
    int n;
    int m;
    vi d, r;
    UF(int n) : n(n), m(n), d(n, -1), r(n, 0){};
    int root(int i){
        if(d[i] < 0) return i;
        return d[i] = root(d[i]);
    }
    bool same(int x, int y){
        return root(x) == root(y);
    }
    bool unite(int x, int y){
        x = root(x);
        y = root(y);
        if(x == y) return false;
 
        if(r[x] < r[y]) swap(x, y);
        else if(r[x] == r[y]) r[x]++;
        d[x] += d[y];
        d[y] = x;
        --m;
        return true;
    }
    int size(int i){
        return -d[root(i)];
    }
    int size() {
        return m;
    }
};