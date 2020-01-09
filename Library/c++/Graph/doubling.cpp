// ダブリング
// cost 0 での遷移を書いてからpreCalcする！
// verify 
//  getCost ARC060E
//  getNode ABC136D
namespace Doubling {
    using NODE_TYPE = int;
    using COST_TYPE = long long;
    // d[i][0]はあらかじめ計算しておく！
    void preCalc(vector<vector<NODE_TYPE>>& d) {
        int n = d.size();
        int m = d[0].size();
        REP(j, 1, m) rep(i, n) {
            d[i][j] = d[d[i][j-1]][j-1];
        }
    }
    // 頂点uからcostでいける頂点を求める
    NODE_TYPE getNode(NODE_TYPE u, COST_TYPE cost, const vector<vector<NODE_TYPE>>& d) {
        int n = d.size();
        int m = d[0].size();
        NODE_TYPE cur = u;
        repr(j, m) {
            if(cost&(COST_TYPE(1)<<j)) {
                cur = d[cur][j];
            }
        }
        return cur;
    }
    // 頂点uから頂点vまでのコストを求める(iはトポロジカルソート後のidx)
    COST_TYPE getCost(NODE_TYPE u, NODE_TYPE v, const vector<vector<NODE_TYPE>>& d) {
        if(u >= v) assert(false);
        int n = d.size();
        int m = d[0].size();
        int cur = u;
        COST_TYPE ret = 0;
        while(cur < v) {
            if(d[cur][0] >= v) {
                ret += 1;
                break;
            }
            int j = lower_bound(all(d[cur]), v) - d[cur].begin() - 1;
            cur = d[cur][j];
            ret+= COST_TYPE(1) << j;
        }
        return ret;
    }
};