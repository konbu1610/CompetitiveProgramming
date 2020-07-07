/* Matrix
    verified
        yukicoder 658
        yukicoder 1105
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
    Mat(Mat&&)=default;
    array<T, SZ>& operator [](const int n) {
        return d[n];
    }
    Mat pow(ll n) const {
        Mat tmp;
        tmp.d = this->d;
        Mat<T, SZ> ret;
        rep(i, SZ) ret.d[i][i] = 1LL;
        while(n > 0) {
            if(n&1LL) ret = ret * tmp;
            tmp *= tmp;
            n >>= 1;
        }
        return ret;
    }
};