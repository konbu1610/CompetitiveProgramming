/*
    ModInt
    verify
        ARC039 B

*/

template <typename T, typename U> T pow_fast(T x, U n) {
    T ret = (T)1;
    while(n) {
        if(n & (U)1) ret *= x;
        x *= x;
        n >>= 1;
    }
    return ret;
}

template <typename ModType, ModType MOD> struct ModInt {
    ModType val;
    ModInt() : val(0) {}
    ModInt(ModType x) : val(x % MOD) {
        if(val < 0) val += MOD;
    };
    operator ModType() const { return val; }
    ModInt &operator+=(const ModInt &m) {
        val += m.val;
        if(val >= MOD) val -= MOD;
        return *this;
    }
    ModInt &operator-=(const ModInt &m) {
        val -= m.val;
        if(val < 0) val += MOD;
        return *this;
    }
    ModInt &operator*=(const ModInt &m) {
        (val *= m.val) %= MOD;
        return *this;
    }
    ModInt &operator/=(const ModInt &m) {
        *this *= m.inverse();
        return *this;
    }
    ModInt operator+(const ModInt &m) const { return ModInt(*this) += m; }
    ModInt operator-(const ModInt &m) const { return ModInt(*this) -= m; }
    ModInt operator*(const ModInt &m) const { return ModInt(*this) *= m; }
    ModInt operator/(const ModInt &m) const { return ModInt(*this) /= m; }
    ModInt inverse() const { return pow_fast(*this, MOD - 2); }

    friend std::ostream &operator<<(std::ostream &os, const ModInt &rhs) {
        os << rhs.val;
        return os;
    }
    friend std::istream &operator>>(std::istream &is, ModInt &rhs) {
        ModType value;
        is >> value;
        rhs = ModInt(value);
        return is;
    }
};
