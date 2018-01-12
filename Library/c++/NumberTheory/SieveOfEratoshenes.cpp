/*
  エラトステネスの篩
  O( n log log n )
  verified yukicoder No. 6
 */
const int MAX_N = 1e6+10;
vector<int> primes;
vector<int> isPrime(MAX_N, 1);
void buildPrimes() {
    isPrime[0] = isPrime[1] = 0;
    REP(i, 2, MAX_N) {
        if(isPrime[i]) {
            primes.push_back(i);
            for(int j = i+i; j < MAX_N; j += i) isPrime[j] = false;
        }
    }
}
