/*
    verified ABC145D
    calc combination_mod nCk % MOD
    pre-calc: O(n)
    calc nCk: O(1)

*/

const int MAX_N=200010;
 
ll gcd(ll a, ll b){
    return b == 0LL ? a : gcd(b, a%b);
}
ll lcm(ll a, ll b){
    return a / gcd(a, b) * b;
}
ll extgcd(ll a, ll b, ll &x, ll &y){
    ll g = a;
    x = 1LL;
    y = 0LL;
    if(b != 0) g = extgcd(b, a%b, y, x), y-= (a/b)*x;
    return g;
}
ll fact[MAX_N], ifact[MAX_N];
ll mod_inverse(int a){
    ll x, y;
    extgcd(a, MOD, x, y);
    return(MOD+x%MOD)%MOD;
}
 
void makeFact(){
    fact[0]=ifact[0] = 1LL;
    for(int i = 1; i < MAX_N; i++){
        fact[i] = fact[i-1]*i%MOD;
        ifact[i] = mod_inverse(fact[i]);
    }
}
 
ll c(int n, int r){
    if(n < 0 || r < 0 || r > n) return 0;
    if(r > n / 2) r = n-r;
    return fact[n]*ifact[n-r]%MOD*ifact[r]%MOD;
}