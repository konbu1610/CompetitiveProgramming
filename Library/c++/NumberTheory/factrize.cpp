/*
  factrization
  O( sqrt(n) )
  verified yukicoder No. 2
*/

using Facts = map<ll, int>; 
void factrize(ll n, Facts& mp) {
  if(n < 2) return;
  ll m = sqrt(n);
  REP(i, 2, m+1) {
    int cnt = 0;
    while(n%i == 0) {
      n /= i;
      ++cnt;
    }
    mp[i] += cnt;
  }
  if(n != 1) mp[n]++;
}
