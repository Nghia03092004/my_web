template <int MOD>
struct ModInt {
    int value;

    ModInt(long long v = 0) { value = normalize(v); }

    static int normalize(long long v) {
        v %= MOD;
        if (v < 0) v += MOD;
        return (int)v;
    }

    ModInt& operator+=(const ModInt& other) {
        value += other.value;
        if (value >= MOD) value -= MOD;
        return *this;
    }

    ModInt& operator-=(const ModInt& other) {
        value -= other.value;
        if (value < 0) value += MOD;
        return *this;
    }

    ModInt& operator*=(const ModInt& other) {
        value = (int)((long long)value * other.value % MOD);
        return *this;
    }

    friend ModInt operator+(ModInt a, const ModInt& b) { return a += b; }
    friend ModInt operator-(ModInt a, const ModInt& b) { return a -= b; }
    friend ModInt operator*(ModInt a, const ModInt& b) { return a *= b; }

    static ModInt power(ModInt base, long long exp) {
        ModInt result(1);
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }
};
