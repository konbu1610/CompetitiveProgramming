/*
    nCk mod m
    n, k <= 10^18
    m <= 10^6

    verified codeforces (http://codeforces.com/gym/100633/problem/J)
*/

struct CRT {
    vector<pair<ll, ll>> v;
    void add(ll a, ll m) { v.push_back({ a, m }); } // x = a (mod m)

    set<ll> enumpr(ll n) {
        set<ll> V;
        for (ll i = 2; i * i <= n; i++) while (n % i == 0) V.insert(i), n /= i;
        if (n > 1) V.insert(n); return V;
    }
    ll ext_gcd(ll p, ll q, ll& x, ll& y) {
        if (q == 0) return x = 1, y = 0, p; ll g = ext_gcd(q, p % q, y, x); y -= p / q * x; return g;
    }
    ll inv(ll p, ll q) { ll xx, yy, g = ext_gcd(p, q, xx, yy); if (xx < 0) xx += q, yy -= p; return xx; }

    ll solve(ll mo = 1e9+7) {
        string s; int N = v.size(); set<ll> P; vector<int> num;
        REP(i, 0, N) { set<ll> S = enumpr(v[i].second); for (ll r : S) P.insert(r);}
        num = vector<int>(N, 0);
        for (ll r : P) {
            int y = 0;
            REP(x, 0, N) {
                num[x] = 1; int j = v[x].second; while (j % r == 0) j /= r, num[x] *= r;
                if (num[y] < num[x]) y = x;
            }
            REP(x, 0, N) if (x != y) {
                if (v[x].first % num[x] != v[y].first % num[x]) return -1;
                v[x].second /= num[x]; v[x].first %= v[x].second;
            }
        }
        vector<pair<ll, ll> > V2;
        REP(x, 0, N) if (v[x].second > 1) V2.emplace_back(v[x].second, v[x].first);
        sort(V2.begin(), V2.end()); v = V2; N = v.size();
        REP(y, 0, N) REP(x, 0, y) {
            ll k = v[y].second - v[x].second; if (k < 0) k += v[y].first;
            v[y].second = k * inv(v[x].first, v[y].first) % v[y].first;
        }
        ll ret = 0; for (int x = N - 1; x >= 0; x--) ret = (ret * v[x].first + v[x].second) % mo;
        return ret;
    }
};


ll gcd(ll a, ll b) {
    return b == 0LL ? a : gcd(b, a % b);
}
ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}
ll extgcd(ll a, ll b, ll &x, ll &y) {
    ll g = a;
    x = 1LL;
    y = 0LL;
    if (b != 0) g = extgcd(b, a % b, y, x), y -= (a / b) * x;
    return g;
}
ll mod_inverse(ll a) {
    ll x, y;
    extgcd(a, MOD, x, y);
    return (MOD + x % MOD) % MOD;
}
void factorization(ll n, map<ll, ll>& mp) {
    for (ll i = 2; i * i <= n; ++i) {
        while (n % i == 0) mp[i]++, n /= i;
    }
    if (n != 1) mp[n]++;
}

namespace CombMod {
vector<ll> fact, ifact;
void makeFF(int p, int q) {
    int P = 1;
    for (int i = 0; i < q; i++)P *= p;
    fact.assign(P + 1, 0);
    ifact.assign(P + 1, 0);
    fact[0] = 1;
    for (int i = 1; i <= P; i++) {
        if (i % p == 0) {
            fact[i] = fact[i - 1];
        } else {
            fact[i] = fact[i - 1] * i % P;
        }
    }
    for (int i = 0; i <= P; i++) {
        ll ret = 1;
        ll mul = fact[i];
        for (ll n = P / p * (p - 1) - 1; n > 0; n >>= 1) {
            if ((n & 1) == 1) {
                ret = (ret * mul) % P;
            }
            mul = (mul * mul) % P;
        }
        ifact[i] = ret;
    }
}
// Lucas Theory
// m = pow(p, q) p := prime number
// 前処理としてmakeFFする
ll C(ll n, ll r, int p, int q) {
    if (n < 0 || r < 0 || r > n) return 0;
    ll P = 1;
    rep(i, q) P *= p;

    ll z = n - r;
    ll e0 = 0;
    for (ll u = n / p; u > 0; u /= p) e0 += u;
    for (ll u = r / p; u > 0; u /= p) e0 -= u;
    for (ll u = z / p; u > 0; u /= p) e0 -= u;

    ll em = 0;
    for (ll u = n / p; u > 0; u /= p) em += u;
    for (ll u = r / p; u > 0; u /= p) em -= u;
    for (ll u = z / p; u > 0; u /= p) em -= u;

    ll ret = 1;
    while (n > 0) {
        ret = ret * fact[n % P] % P * ifact[r % P] % P * ifact[z % P] % P;
        n /= p;
        r /= p;
        z /= p;
    }
    rep(i, e0) ret = ret * p % P;
    if (!(p == 2 && q >= 3) && (em & 1LL) == 1LL) ret = (P - ret) % P;
    return ret;
}

ll comb_mod(ll n, ll k, ll m) {
    CRT crt;
    map<ll, ll> mp;
    factorization(m, mp);

    for (const auto& p : mp) {
        ll mod = 1;
        rep(i, p.se) mod *= p.fi;
        makeFF(p.fi, p.se);
        ll c = C(n, k, p.fi, p.se);
        crt.add(c, mod);
    }
    return crt.solve(m);
}
}