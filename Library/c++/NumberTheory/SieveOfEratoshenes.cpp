/*
    エラトステネスの篩
    O( n log log n )
    verified 
        yukicoder No. 6
        ABC084D
*/

struct Sieve {
    vector<int> primes;
    vector<int> _isPrime;
    Sieve(int n) : _isPrime(n+1, 1) {
        buildPrimes();
    }
    void buildPrimes() {
        _isPrime[0] = _isPrime[1] = 0;
        int n = _isPrime.size();
        REP(i, 2, n) {
            if(_isPrime[i]) {
                primes.push_back(i);
                for(int j = i+i; j < n; j += i) _isPrime[j] = false;
            }
        }
    }
    bool isPrime(int i) {
        return _isPrime[i];
    }
};
