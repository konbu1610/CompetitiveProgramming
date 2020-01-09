/*
    パスカルの三角形を求める
        precalc: O(n^2)
        combination: O(1)

    verify:
        ABC057 D
*/

template<typename T, unsigned int MAX_SIZE, unsigned long long mod>
struct PascalTriangle {
    T d[MAX_SIZE][MAX_SIZE];

    void precalc() {
        rep(i, MAX_SIZE) {
            d[i][0] = d[i][i] = 1;
            REP(j, 1, i) {
                d[i][j] = d[i-1][j-1]+d[i-1][j];
                if(mod != 0) d[i][j] %= mod;
            }
        }
    }

    PascalTriangle() {
        precalc();
    }

    T comb(int n, int r) {
        return d[n][r];
    }
};