// Matrix
// verified yukicoder 658
template <int SZ>
struct Mat {
    array<array<ll, SZ>, SZ> d;
    const int n;
    Mat() : n(SZ) {
        array<ll, SZ> tmp;
        tmp.fill(0);
        d.fill(tmp);
    }
    Mat<SZ> operator * (const Mat<SZ>& mt) const {
        Mat<SZ> ret;
        rep(i, SZ) rep(j, SZ) {
            ll sum = 0LL;
            rep(k, SZ) {
                (ret.d[i][j] += (d[i][k] * mt.d[k][j]) % MOD) %= MOD;
            }
        }
        return ret;
    }
    Mat<SZ>& operator = (const Mat<SZ>& mt) {
        d = mt.d;
        return *this;
    }
};
template <int SZ> 
Mat<SZ> pow(Mat<SZ> mt, ll n) {
    Mat<SZ> ret;
    rep(i, SZ) ret.d[i][i] = 1LL;
    while(n > 0) {
        if(n&1LL) ret = ret * mt;
        mt = mt*mt;
        n >>= 1;
    }
    return ret;
}