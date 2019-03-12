/*
    verify
    aoj DSL 2F
*/

using ull = unsigned long long;


constexpr ll initVal = (1LL<<31)-1;     //TODO: fix type and value
constexpr ll initLazy = (1LL<<60)-1;    //TODO: fix type and value
template <typename T>
struct Node {
    T val;
    T lazy;
    ull l, r;
    Node<T>* l_node = nullptr;
    Node<T>* r_node = nullptr;
    Node(ull l, ull r, const T& val = initVal) : l(l), r(r), val(val), lazy(initLazy) {}
    T getVal() {
        return (lazy != initLazy ? lazy : val); //TODO: fix
    }
    T f1(const T& a, const T& b) {
        return min(a, b);               //TODO: fix
    }
    T f2(const T& a, const T& b) {
        return b;                       //TODO: fix
    }
    void push() {
        if(r - l > 1) {
            auto m = (l+r)/2;
            {
                if(l_node == nullptr) l_node = new Node(l, m);
                if(lazy != initLazy) l_node->lazy = f2(l_node->lazy, this->lazy);
            }
            {
                if(r_node == nullptr) r_node = new Node(m, r);
                if(lazy != initLazy) r_node->lazy = f2(r_node->lazy, this->lazy); 
            }
        }
        val = getVal();
        lazy = initLazy;
    }
    void fix() {
        if(r - l == 1) return;
        val = f1(l_node->getVal(), r_node->getVal());
    }
    T query(ull qL, ull qR) {
        if(qR <= l || r <= qL) return initVal;
        if(qL <= l && r <= qR) return getVal();
        if(r - l == 1)  return getVal();

        push();
        T l_val = l_node->query(qL, qR), r_val = r_node->query(qL, qR);
        return f1(l_val, r_val);
    }
    void update(ull uL, ull uR, const T& uVal) {
        if(uR <= l || r <= uL) return;
        if(uL <= l && r <= uR) lazy = f2(lazy, uVal);
        else {
            push();
            l_node->update(uL, uR, uVal);
            r_node->update(uL, uR, uVal);
            fix();
        }
    }
};

template <typename T>
struct DynamicLazySegTree {
    Node<T> root;
    DynamicLazySegTree(ull l, ull r, const T& val) : root(l, r, val) {}
    T query(ull l, ull r) {
        return root.query(l, r);
    }
    void update(ull l, ull r, const T& val) {
        root.update(l, r, val);
    }
};