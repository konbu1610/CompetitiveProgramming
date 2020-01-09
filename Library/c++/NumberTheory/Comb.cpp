/*
    CombinationMod 
    using ModInt
    verify 
        ARC039B
*/

template <typename ModType, ModType MOD> struct Comb {
    using mint = ModInt<ModType, MOD>;
    vector<mint> fact;
    vector<mint> ifact;
    const int N;
    Comb(int n) : N(n + 1), fact(n + 1), ifact(n + 1) { makeFact(); }
    void makeFact() {
        fact[0] = ifact[0] = 1LL;
        for(int i = 1; i < N; i++) {
            fact[i] = fact[i - 1] * (mint)i;
            ifact[i] = fact[i].inverse();
        }
    }
    mint operator()(ModType n, ModType r) {
        if(n < 0 || r < 0 || r > n) return 0;
        if(r > n / 2) r = n - r;
        return fact[n] * ifact[n - r] % MOD * ifact[r] % MOD;
    }
};