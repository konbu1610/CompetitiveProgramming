typedef double D;
const D EPS = 1e-10;
using P = valarray<D>; // 使うときは、Originで初期化する！！
const P Origin = {(D)0.0, (D)0.0, (D)0.0};
#define x(P) (P)[0]
#define y(P) (P)[1]
#define z(P) (P)[2]
#define SQ(Q) ((Q)*(Q))
#define curr(P, idx) (P)[(idx)]
#define next(P, idx) (P)[((idx)+1)%(P).size()]
#define prev(P, idx) (P)[((idx)+(P).size()-1)%(P).size()]
namespace std {
    bool operator < (const P& a, const P& b) {
        if(x(a) != x(b)) return x(a) < x(b);
        if(y(a) != y(b)) return y(a) < y(b);
        return z(a) < z(b);
    }
}


struct L : array<P, 2> {
    L() {at(0) = at(1) = Origin;}
    L(const P& a, const P& b) {
        at(0) = a;
        at(1) = b;
    }
};
D dot(const P& a, const P& b) {
    auto p = a*b;
    return p.sum();
}
P cross(const P& a, const P& b) {
    P ret(3);
    x(ret) = y(a) * z(b) - y(b) * z(a);
    y(ret) = z(a) * x(b) - z(b) * x(a);
    z(ret) = x(a) * y(b) - x(b) * y(a);
    return ret;
}
D norm(const P& p) {
    return sqrt(dot(p, p));
}
//----------------------------------------
//直線・線分の当たり判定、円・直線、線分の当たり判定、距離等 
// verify aoj 2641 aoj1331
bool isHitLP(const L&l ,const P& p) {
    P v = l[1] - l[0], v1 = p - l[0];
    return abs(norm(cross(v, v1))) < EPS;
}
bool isHitSP(const L& l, const P& p) {
    P v = l[1] - l[0], v1 = p - l[0];
    P u = l[0] - l[1], u1 = p - l[1];
    return abs(norm(cross(v, v1))) < EPS && norm(v1) < norm(v) + EPS
            && abs(norm(cross(u, u1))) < EPS && norm(u1) < norm(u) + EPS; 
}
D distLP(const L& l, const P& p) {
    if(isHitLP(l, p)) return (D)0.0;
    P AB = l[1] - l[0];
    P AP = p - l[0];
    if(abs(norm(AB)) < EPS) return norm(AP);
    return norm(cross(AB, AP)) / norm(AB);
}
D distSP(const L& l, const P& p) {
    if(isHitSP(l, p)) return (D)0.0;
    return min({norm(p - l[0]), norm(p - l[1]), distLP(l, p)});
}
struct C{
    P p;
    D r;
};
bool isHitPC(const P& p, const C& c) {
    return norm(p - c.p) < c.r + EPS;
}
vector<P> crossPointLC(const L&l ,const C& cir) {
    P s = l[0] - cir.p;
    P d = l[1] - l[0];
    auto A = dot(d, d), B = dot(d, s), C = dot(s, s) - SQ(cir.r);
    auto DD = (SQ(B) - A * C);
    vector<D> t;
    if(abs(DD) < 0) t.pb(B / A);
    else if(DD < 0) ;
    else {
        t.pb((-B + sqrt(DD)) / A);
        t.pb((-B - sqrt(DD)) / A);
    }
    vector<P> ret;
    rep(i, t.size()) ret.push_back(s + d * t[i]);
    rep(i, ret.size()) ret[i] += cir.p;
    return ret;
}
vector<P> crossPointSC(const L&l ,const C& c) {
    vector<P> ps = crossPointLC(l, c);
    sort(all(ps));
    auto it = ps.begin();
    while(it != ps.end()) {
        assert(abs(norm(*it - c.p) - c.r) < EPS);
        if(isHitSP(l, *it)) ++it;
        else it = ps.erase(it);
    }
    return ps;
}
bool isHitLC(const L& l, const C& c) {
    return distLP(l, c.p) < c.r + EPS;
}
bool isHitSC(const L& l, const C& c) {
    return crossPointSC(l, c).size() > 0;
}
// 平面・三角形等 
// verify aoj 2641
struct Plane {
    P p;
    P n; // 法線ベクトル
    Plane(const P& _p, const P& _n) : p(_p), n(_n){ 
        n /= norm(n);
        assert(abs(norm(n) -1.0) < EPS);
    }
    Plane(const P& a, const P& b, const P& c) {
        p = a;
        n = cross(b - a, c - a);
        n /= norm(n);
        assert(abs(norm(n) -1.0) < EPS);
    }
};
struct Triangle : array<P, 3>{
    Triangle(){rep(i, 3) at(i) = Origin;}
    Triangle(const P& a, const P& b, const P& c) {
        at(0) = a;
        at(1) = b;
        at(2) = c;
    }
};
D distPlaneP(const Plane& pl, const P& p) {
    return abs(dot(p - pl.p, pl.n));
}
bool isHitPlaneS(const Plane& pl, const L& s) {
    return (dot(s[0] - pl.p, pl.n) * dot(s[1] - pl.p, pl.n) < EPS);
}
P crossPointPlaneS(const Plane& pl, const L& s) {
    assert(isHitPlaneS(pl, s)); //当たってない
    assert((abs(dot(s[0]-pl.p, pl.n))+abs(dot(s[1]-pl.p, pl.n))) > EPS); // 両端が平面状
    return s[0] + (s[1]-s[0])*(abs(dot(s[0]-pl.p, pl.n)) / (abs(dot(s[0]-pl.p, pl.n))+abs(dot(s[1]-pl.p, pl.n))));
}
bool isHitTriangleP(const Triangle& tri, const P& p) {
    Plane pl = {tri[0], tri[1], tri[2]};
    if(!(distPlaneP(pl, p) < EPS)) return false;
    vector<P> c(3);
    rep(i, 3) {
        c[i] = cross(next(tri, i) - curr(tri, i), p - next(tri, i));
    }
    auto a = dot(c[0], c[1]);
    auto b = dot(c[0], c[2]);
    return a > -EPS && b > -EPS;
}