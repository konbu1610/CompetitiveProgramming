#include <bits/stdc++.h>

using namespace std;


//----------基本部分-----------------------------------------------------------------------
#define X real()
#define Y imag()
#define x(p) real(p)
#define y(p) imag(p)
#define curr(P, i) P[i]
#define next(P, i) P[(i+1)%P.size()]
#define prev(P, i) P[(i+P.size()-1)%P.size()]

typedef D D;

const D EPS = 1e-8;
const D INF = 1e12;
const D PI = acos(-1);
typedef complex<D> P;
typedef complex<D> point;
namespace std {
    bool operator < (const P& a, const P& b) {
        return x(a) != x(b) ? x(a) < x(b) : y(a) < y(b);
    }
}
D cross(const P& a, const P& b) {
    return y(conj(a)*b);
}
D dot(const P& a, const P& b) {
    return x(conj(a)*b);
}

struct L : public vector<P> {
    L(const P &a, const P &b) {
        push_back(a); push_back(b);
    }
};

typedef vector<P> G;
typedef vector<P> polygon;

struct C {
    P p; D r;
    C(const P &p, D r) : p(p), r(r) { }
};
//点の進行方向
int ccw(P a, P b, P c) {
    b -= a; c -= a;
    if (cross(b, c) > 0)   return +1;       // counter clockwise
    if (cross(b, c) < 0)   return -1;       // clockwise
    if (dot(b, c) < 0)     return +2;       // c--a--b on line
    if (norm(b) < norm(c)) return -2;       // a--b--c on line
    return 0;
}
//------------------------------------------------------------------------------------------
//---------交差判定・距離等-------------------------------------------------------------------
bool intersectLL(const L &l, const L &m) {
    return abs(cross(l[1]-l[0], m[1]-m[0])) > EPS || // non-parallel
            abs(cross(l[1]-l[0], m[0]-l[0])) < EPS;   // same line
}
bool intersectLS(const L &l, const L &s) {
    return cross(l[1]-l[0], s[0]-l[0])*       // s[0] is left of l
            cross(l[1]-l[0], s[1]-l[0]) < EPS; // s[1] is right of l
}
bool intersectLP(const L &l, const P &p) {
    return abs(cross(l[1]-p, l[0]-p)) < EPS;
}
bool intersectSS(const L &s, const L &t) {
  return ccw(s[0],s[1],t[0])*ccw(s[0],s[1],t[1]) <= 0 &&
         ccw(t[0],t[1],s[0])*ccw(t[0],t[1],s[1]) <= 0;
}
bool intersectSP(const L &s, const P &p) {
    return abs(s[0]-p)+abs(s[1]-p)-abs(s[1]-s[0]) < EPS; // triangle inequality
}
P projection(const L &l, const P &p) {
    D t = dot(p-l[0], l[0]-l[1]) / norm(l[0]-l[1]);
    return l[0] + t*(l[0]-l[1]);
}
P reflection(const L &l, const P &p) {
    return p + 2.0 * (projection(l, p) - p);
}
D distanceLP(const L &l, const P &p) {
    return abs(p - projection(l, p));
}
D distanceLL(const L &l, const L &m) {
    return intersectLL(l, m) ? 0 : distanceLP(l, m[0]);
}
D distanceLS(const L &l, const L &s) {
    if (intersectLS(l, s)) return 0;
    return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}
D distanceSP(const L &s, const P &p) {
    const P r = projection(s, p);
    if (intersectSP(s, r)) return abs(r - p);
    return min(abs(s[0] - p), abs(s[1] - p));
}
D distanceSS(const L &s, const L &t) {
    if (intersectSS(s, t)) return 0;
    return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])),
            min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}
P crosspoint(const L &l, const L &m) {
    D A = cross(l[1] - l[0], m[1] - m[0]);
    D B = cross(l[1] - l[0], l[1] - m[0]);
    if (abs(A) < EPS && abs(B) < EPS) return m[0]; // same line
    if (abs(A) < EPS) assert(false); // !!!PRECONDITION NOT SATISFIED!!!
    return m[0] + B / A * (m[1] - m[0]);
}
//--------------------------------------------------------------------------------------
//点が多角形内にあるか判定(polygonは反時計回り)               CGL_3_C
enum { OUT, ON, IN };
int contains(const polygon& P, const point& p) {
    bool in = false;
    for (int i = 0; i < P.size(); ++i) {
        point a = curr(P,i) - p, b = next(P,i) - p;
        if (y(a) > y(b)) swap(a, b);
        if (y(a) <= 0 && 0 < y(b))
            if (cross(a, b) < 0) in = !in;
        if (cross(a, b) == 0 && dot(a, b) <= 0) return ON;
    }
    return in ? IN : OUT;
}
//点を内包しているか、polygonが反時計回りじゃなくても使えるが凸じゃないとダメ、外積の符号が一致しているか
enum { OUT, ON, IN };
int contains(const G& pol, const P& p){
    if(pol.size() < 2) return OUT;
    if(intersectSP({pol[0], pol[1]}, p)) return ON;
    bool f = cross(pol[1]-pol[0], p - pol[0]) > 0;
    for(int i = 1; i < pol.size(); i++) {
        auto& now = curr(pol, i);
        auto& nxt = next(pol, i);
        if(intersectSP({nxt, now}, p)) return ON;
        if(f^(cross(nxt-now, p-now)>0)) return OUT;
    }
    return IN;
}

//点が円の中にあるか判定
bool contains(const P& p, const C& c) {
    return abs(p-c.p) < c.r+EPS;
}
//円と線分の位置関係(返り値が正なら当たっている) positional relation
int posRel(const L&s, const C&c){
    if(abs(distanceSP(s,c.p)-c.r)<EPS)  return 1;   //接してる
    if(distanceSP(s,c.p) > c.r+EPS)     return 0;   //かすってもない
    if(contains(s[0], c) && contains(s[1], c))  return 2;   //円に内包
    if(contains(s[0], c)||contains(s[1], c))    return 3;   //片側だけ円の中
    return 4;   //円を貫通
}

// a in b
bool contains(const polygon& a, const polygon& b) {
    rep(i, a.size()) if(contains(b, a[i]) == OUT) return false;
    return true;
}

//ポリゴン同士の当たり判定(返り値が正ならあたり)
enum{NO_HIT, HIT, A_IN_B, B_IN_A};
int isHit(const polygon& a, const polygon& b){
    if(contains(a, b)) return A_IN_B;
    if(contains(b, a)) return B_IN_A;
    rep(i, a.size()) rep(j, b.size()){
        L la(curr(a, i), next(a, i)), lb(curr(b, j), next(b, j));
        if(intersectSS(la, lb)) return HIT;
    }
    return NO_HIT;
}
//pointをthetaだけ回転
point spinP(const point& p, D theta, D ext = 1.0) {
    return p*P(cos(theta),sin(theta))*ext;
}

point makeP(D r, D arg) {return P(r*cos(arg), r*sin(arg));}

//円と円の交点を求める                                    CGL_7_E
vector<P> crossPoint(const C& a, const C& b){
    D d = abs(b.p-a.p);
    D arg = atan2(y(b.p-a.p), x(b.p-a.p));
    D arg2 = acos((d*d+a.r*a.r-b.r*b.r)/(2*d*a.r));
    if(abs(arg2)<EPS) return {a.r*makeP(1.0, arg)+a.p};
    if(abs(a.r-b.r)<d && d < a.r+b.r) return {a.r*makeP(1.0, arg+arg2)+a.p, a.r*makeP(1.0, arg-arg2)+a.p};
    return {};
}


//凸包 - convex hull  同一直線上の点は除去（除去したくない場合は、ccw(...)<=0 ==> ccw(...)!=-1)             CGL_4_A
vector<point> convex_hull(vector<point> ps) {
    int n = ps.size(), k = 0;
    sort(ps.begin(), ps.end());
    vector<point> ch(2*n);
    for (int i = 0; i < n; ch[k++] = ps[i++]) // lower-hull
        while (k >= 2 && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
    for (int i = n-2, t = k+1; i >= 0; ch[k++] = ps[i--]) // upper-hull
        while (k >= t && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
    ch.resize(k-1);
    return ch;
}
//反時計回りのpolygonが凸かどうか判定         CGL_3_B
bool isHull(const G& g) {
    int n = g.size();
    if(n < 3) return false;
    rep(i, n) if(ccw(prev(g, i), curr(g, i), next(g, i)) == -1) return false;
    return true;
}

//反時計回り or 時計回りの多角形の面積を求める 凸じゃなくても良い        CGL_3_A
D getArea(const polygon& pol) {
    int n = pol.size();
    D s = 0.0;
    for(int i = 0; i < n; i++) {
        int now = i, nxt = (i + 1) % n;
        s += (pol[now].X - pol[nxt].X) * (pol[now].Y + pol[nxt].Y);
    }
    return abs(s/2.0);
}

//円の共通接線                            接線の本数は正しそう CGL_7_A
vector<L> comonTang(C c1, C c2) {
    vector<L> ret;
    if(c1.r > c2.r) swap(c1, c2);
    P sub = c1.p;
    c1.p -= sub;
    c2.p -= sub;
    D theta = atan2(c2.p.Y, c2.p.X);

    c2.p = spinP(c2.p, -theta);
    int signX = 0;
    if(c2.p.X > 0) signX = 1;
    if(c2.p.X < 0) signX = -1;

    D x = abs(c2.p.X), r1 = c1.r, r2 = c2.r;
    if(abs(x*x -(r2 - r1) * (r2 - r1))<EPS) {
        D theta2 = asin((r2-r1)/x);
        ret.pb({spinP({0, r1}, theta2), spinP({sqrt(x*x-(r2-r1)*(r2-r1)), r1}, theta2)});
    }
    else if(x*x > (r2 - r1) * (r2 - r1)) {
        D theta2 = asin((r2-r1)/x);
        ret.pb({spinP({0, r1}, theta2), spinP({sqrt(x*x-(r2-r1)*(r2-r1)), r1}, theta2)});
        ret.pb({spinP({0, -r1}, -theta2), spinP({sqrt(x*x-(r2-r1)*(r2-r1)), -r1}, -theta2)});
    }
    if(abs(r1+r2-x)<EPS) {
        D theta3 = PI / 2 - acos((r1+r2)/x);
        D len = sqrt((r1*x)*(r1*x)/(r1+r2)/(r1+r2)-r1*r1) + sqrt((r2*x)*(r2*x)/(r1+r2)/(r1+r2)-r2*r2);
        ret.pb({spinP({0, r1}, -theta3), spinP({len, r1}, -theta3)});
    }
    else if(x > r1+r2) {
        D theta3 = PI / 2 - acos((r1+r2)/x);
        D len = sqrt((r1*x)*(r1*x)/(r1+r2)/(r1+r2)-r1*r1) + sqrt((r2*x)*(r2*x)/(r1+r2)/(r1+r2)-r2*r2);
        ret.pb({spinP({0, r1}, -theta3), spinP({len, r1}, -theta3)});
        ret.pb({spinP({0, -r1}, -theta3), spinP({len, -r1}, theta3)});
    }

    for(auto& l : ret) {
        if(signX == -1) l[1] = {-l[1].X, l[1].Y};
        l[0] = spinP(l[0], theta);
        l[1] = spinP(l[1], theta);
        l[0] += sub;
        l[1] += sub;
    }
    return ret;
}


//円と直線の交点                           CGL_7_D posRelがおかしいかも？ => やばい SRM 401 DIV1 MED 通す
vector<P> crossPoint(C c, L l) {
    int hit = posRel(l, c);
    if(hit == 0) return {};
    
    D R = c.r;
    P sub = c.p;
    D theta;

    l[0] -= sub;
    l[1] -= sub;

    theta = atan2(y(l[1]-l[0]), x(l[1]-l[0]));

    l[0] = spinP(l[0], -theta);
    l[1] = spinP(l[1], -theta);

    assert(abs(y(l[0]-l[1])) < EPS);

    vector<P> ret;
    if(abs(abs(l[0].Y) - R) < EPS) ret.pb({0.0, l[0].Y});
    else {
        D y = l[0].Y;
        D x = sqrt(R*R-y*y);
        ret.pb({x, y});
        ret.pb({-x, y});
    }

    rep(i, ret.size()) {
        ret[i] = spinP(ret[i], theta);
        ret[i] += sub;
    }
    return ret;
}


int main(){

}
