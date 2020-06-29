/* Matrix
    verified
        yukicoder 658
        aoj ALDS1_10
*/
template <class T, int SZ>
struct Mat {
    array<array<T, SZ>, SZ> d;
    const int n;
    Mat() : n(SZ) {
        array<T, SZ> tmp;
        d.fill(tmp);
    }
    Mat operator * (const Mat& mt) const {
        Mat ret;
        rep(i, SZ) rep(j, SZ) {
            rep(k, SZ) {
                ret.d[i][j] += (d[i][k] * mt.d[k][j]);
            }
        }
        return ret;
    }
    Mat& operator *= (const Mat& mt) {
        *this = *this * mt;
        return *this;
    }
    Mat& operator = (const Mat& mt) {
        d = mt.d;
        return *this;
    }
    array<T, SZ>& operator [](const int n) {
        return d[n];
    }
};

template <class T, int SZ> 
Mat<T, SZ> pow(Mat<T, SZ> mt, ll n) {
    Mat<T, SZ> ret;
    rep(i, SZ) ret.d[i][i] = 1LL;
    while(n > 0) {
        if(n&1LL) ret = ret * mt;
        mt = mt*mt;
        n >>= 1;
    }
    return ret;
}

template <typename T, typename U> T pow_fast(T x, U n) {
    T ret = (T)1;
    while(n) {
        if(n & (U)1) ret *= x;
        x *= x;
        n >>= 1;
    }
    return ret;
}