/*
    verified ABC130E
*/

// https://kopricky.github.io/code/DataStructure_Advanced/bit_2d.html
// add (i, j)
// sum(i, j) -> [0, i] [0, j]
// 閉区間に注意
template <typename T>
class BIT
{
private:
    int n, m;
    vector<vector<T>> bit;

public:
    void add(int i, int j, T val)
    {
        for (int i_ = i + 1; i_ < n; i_ += i_ & -i_)
            for (int j_ = j + 1; j_ < m; j_ += j_ & -j_)
                (bit[i_][j_] += val) %= MOD;
    }
    T sum(int i, int j)
    {
        T s = 0;
        for (int i_ = i + 1; i_ > 0; i_ -= i_ & -i_)
            for (int j_ = j + 1; j_ > 0; j_ -= j_ & -j_)
                (s += bit[i_][j_]) %= MOD;
        return s;
    }
    T sum(int lx, int rx, int ly, int ry)
    {
        return ((sum(rx - 1, ry - 1) - sum(lx - 1, ry - 1) - sum(rx - 1, ly - 1) + sum(lx - 1, ly - 1)) % MOD + MOD) % MOD;
    }
    BIT(int sz1, int sz2)
    {
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n, vector<T>(m, 0));
    }
    BIT(vector<vector<T>> &v)
    {
        n = (int)v.size() + 1, m = (int)v[0].size() + 1;
        bit.resize(n, vector<T>(m, 0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                add(i, j, v[i][j]);
    }
    void print()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << sum(i - 1, i, j - 1, j) << " ";
            }
            cout << "\n";
        }
    }
    void print_sum()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << sum(i - 1, j - 1) << " ";
            }
            cout << "\n";
        }
    }
};

