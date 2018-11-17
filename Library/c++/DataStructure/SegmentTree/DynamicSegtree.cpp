using ull = unsigned long long int;
template <typename T>
struct Node {
    T val;
    ull l, m, r;
    Node* l_node;
    Node* r_node;
    Node() : l_node(nullptr), r_node(nullptr) {};
    Node(const T& v, ull l, ull r) : val(v), l(l), m((l + r) / 2), r(r), l_node(nullptr), r_node(nullptr) {};
};
template <typename T>
struct DynamicSegTree {
    ull n;
    Node<T> root;
    const T Default;
    DynamicSegTree(ull N, const T& Default) : Default(Default) {
        n = 1;
        while (n < N) n *= 2;
        root = Node<T>(Default, 0, n);
    }
    void update(ull id, const T& x) {
        update(&root, id, x);
    }
    T query(ull l, ull r) {
        return query(l, r, &root);
    }
    T update_f(T& a, const T& b) {return a = b;}
    T query_f(const T& l, const T& r) {return min(l, r);}
private:
    void criateNode(Node<T>* par, Node<T>* chl, ull id, const T& x) {
        // cout << "  create " << id << ", " << x << ";  par[" << par->l << ", " << par->r << ", " << par->val << "] chl[" << chl->l << ", " << chl->r << ", " << chl->val <<"]" << endl;
        auto pl = par->l, pr = par->r;
        auto cl = chl->l, cr = chl->r;
        auto pm = par->m;
        Node<T>* newNode;
        if (id < pm) {
            par->l_node = new Node<T>(Default, pl, pm);
            newNode = par->l_node;
        } else {
            par->r_node = new Node<T>(Default, pm, pr);
            newNode = par->r_node;
        }
        if (chl->r <= newNode->m) newNode->l_node = chl;
        else newNode->r_node = chl;
        update(newNode, id, x);
    }
    void update(Node<T>* node, ull id, const T& x) {
        // cout << "  update " << id << ", " << x << "; node [" << node->l << ", " << node->r << "] =>" << node->val << endl; 
        auto l = node->l, r = node->r;
        if (r - l == 1) {
            assert(l == id);
            update_f(node->val, x);
            return;
        }
        auto m = (l + r) / 2;
        if (id < m) {
            if (node->l_node == nullptr) {
                node->l_node = new Node<T>(x, id, id + 1);
            } else {
                auto nxt = node->l_node;
                auto L = nxt->l, R = nxt->r;
                if (L <= id && id < R) update(nxt, id, x);
                else criateNode(node, node->l_node, id, x);
            }
        } else {
            if (node->r_node == nullptr) {
                node->r_node = new Node<T>(x, id, id + 1);
            } else {
                auto nxt = node->r_node;
                auto L = nxt->l, R = nxt->r;
                if (L <= id && id < R) update(nxt, id, x);
                else criateNode(node, node->r_node, id, x);
            }
        }
        node->val = query_f(node->l_node == nullptr ? Default : node->l_node->val,
                            node->r_node == nullptr ? Default : node->r_node->val);
    }
    T query(ull a, ull b, Node<T>* node) {
        // cout << "query [" << a << ", " << b << "] node[" << node->l << ", " << node->m << ", " << node->r << ", " << node->val <<"]" << endl;
        if (node->r <= a || b <= node->l) return Default;
        if (a <= node->l && node->r <= b) return node->val;
        T vl = node->l_node == nullptr ?  Default : query(a, b, node->l_node);
        T vr = node->r_node == nullptr ?  Default : query(a, b, node->r_node);
        return query_f(vl, vr);
    }
};

void outputNode(Node<ll>* node) {
    cout << "[" << node->l << ", " << node->r << ") => " << node->val << endl;
    if (node->l_node != nullptr) outputNode(node->l_node);
    if (node->r_node != nullptr) outputNode(node->r_node);
}
