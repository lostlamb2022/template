## This is only an additionary temporary cheat sheet for 10/17/2021 BUPT Campus Contest Final

### 最大流
- 二分图无权匹配
- 上下界可行流
- 多源汇最大流
- 最大流关键边
- 拆点最大流

### 最小割
- 最大权闭合图
- 最大密度子图
- 最小点权覆盖集
- 最大点权独立集

### 费用流
- 二分图带权匹配
- 最大权不相交路径
- 网格图模型
- 上下界费用流
- 费用流拆点

### 图论杂项
- 2-SAT
- 朱刘算法
- Prufer编码

### 二分图杂项

- 最小点覆盖，最大独立集，最小路径覆盖（最小路径重复覆盖）
- 最大匹配数 = 最小点覆盖 = 总点数 - 最大独立集 = 总点数 - 最小路径覆盖
- 最小路径重复覆盖：DAG 跑 Floyd 后再求最小路径覆盖即可

### 计算几何
- 凸多边形面积交 ---- 半平面交 S&I 算法/极角排序 $O(N \times logN)$
- 最小圆覆盖 ---- `random_shuffle`/三角形增量算法 $O(N)$
- 三维凸包 ---- 点增量算法 $O(N^2)$
- 平面最远点对 ---- 旋转卡壳/凸包 $O(N \times logN)$
- 最小矩形覆盖 ---- 旋转卡壳/凸包 $O(N \times logN)$
- 圆和简单多边形面积交 ---- 三角剖分 $O(N)$
- 矩形面积并 ---- 扫描线 $O(N^2 \times logN)$
- 三角形面积并 ---- 扫描线 $O(N^3 \times logN)$
- 圆的面积并 ---- 自适应辛普森积分 $O((R-L) \times \sum (H(x)))$ $H(x)$ 为求出 $F(x)$ 的复杂度
- 多面体欧拉定理：顶点数 - 棱长数 + 表面数 = 2
- 皮克定理：点阵中顶点在格点上的多边形面积公式该公式可以表示为:
           $ S = a + b/2 - 1$
其中 $a$ 表示多边形内部的点数，$b$ 表示多边形边界上的点数，$S$ 表示多边形的面积

#### 简单快速记忆（从必要条件特例思考）
- 最小点覆盖：用最少的点覆盖所有的边，使得每条边都最少有一个端点被覆盖。考虑最普通的二分图：

```md
*
*   *
*   *
*   *
*
```
假设右列三个点有3条边连向左边，答案显示就是右列的点的最大匹配数就是3。

- 最大独立集：最大的点的集合，集合所有点之间没有边相连
依然考虑上图，那么一个最大独立集就是左列所有点，就是所有点减去最大匹配数，而最大匹配数又是右列的点数

- 最小路径覆盖：选出不重复的路径，也就是所有路径都不能有相同的点，覆盖所有DAG上的点
- 最小路径重复覆盖：可以有重复的点，也就是多个路径经过同一个点
考虑下图：

```md
    *
*   *
    *
```
左边的点向右边三个点都连接有向边，这是一个DAG。最小路径覆盖显然三条边都要选择，而此图的最大匹配数是1,
同时最小路径覆盖数3等于总点数减去最大匹配数。


#### 多源汇最大流

$n$ 个点 $m$ 条边的有向图，给定每条边的容量，边的容量非负。
 $S_c$ 个源点，$T_c$ 个汇点,图中可能存在重边和自环,求最大流。
输入格式:
第一行四个整数 $n,m,S_c,T_c$,第二行包含 $S_c$ 个整数，表示所有源点的编号。第三行包含 $T_c$ 个整数，表示所有汇点的编号。
接下来 $m$ 行，每行三个整数 $u,v,c$，表示从点 $u$ 到点 $v$ 一条有向边，容量为 $c$,点的编号从 $1$ 到 $n$。


解法:建立虚拟远点和虚拟汇点，虚拟源点向所有源点连边，容量无穷大，汇点也一样
然后在新图上 Dinic 求一遍最大流即可，因为新图的可行流集合对应于原图的可行流集合
所以新图的最大流就是原图的最大流。

```cpp
const int N = 10010, M = (1e5 + N) * 2, INF = 0x3f3f3f3f;
int n, m, S, T;
int h[N], e[M], f[M], ne[M], idx;
int q[N], d[N], cur[N];
void add(int a, int b, int c) {
    e[idx] = b, ne[idx] = h[a],  f[idx] = c, h[a] = idx ++;
    e[idx] = a,  f[idx] = 0, ne[idx] = h[b], h[b] = idx ++;
}
bool bfs() {
    int hh = 0, tt = 0;
    memset(d, - 1, sizeof d);
    q[0] = S, d[S] = 0, cur[S] = h[S];
    while (hh <= tt) {
        int t = q[hh++];
        for (int i = h[t]; i != -1; i = ne[i]) {
            int ver = e[i];
            if (d[ver] == -1 && f[i]) {
                d[ver] = d[t] + 1;
                cur[ver] = h[ver];
                if (ver == T) return true;
                q[++tt] = ver;
            }
        }
    }
    return false;
}
int find(int u, int limit) {
    if (u == T) return limit;
    int flow = 0;
    for (int i = h[u]; i != -1 && flow < limit; i = ne[i]) {
        cur[u] = i;
        int ver = e[i];
        if (d[ver] == d[u] + 1 && f[i]) {
            int t = find(ver, min(f[i], limit - flow));
            if (!t) d[ver] = -1;
            f[i] -= t, f[i ^ 1] += t, flow += t;
        }
    }
    return flow;
}
int dinic() {
    int r = 0, flow;
    while (bfs()) while (flow = find(S, INF)) r += flow;
    return r;
}
int main() {
    int sc, tc;
    scanf("%d%d%d%d", &n, &m, &sc, &tc);
    memset(h, -1, sizeof h);
    S = 0, T = n +  1; //虚拟源汇点
    while (sc--) {
        int x;
        scanf("%d", &x);
        add(S, x, INF);
    }
    while (tc--) {
        int x;
        scanf("%d", &x);
        add(x, T, INF);
    }
    while (m--) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c);
    }
    printf("%d\n", dinic());
    return 0;
}
```

#### 最大流关键边


增加一个边的容量能让整个图的最大流变大，就是一个关键边。
首先当前最大流的图里面，不满的边直接不用考虑，因为增大了容量依然还是流不满，被前边的容量小的边卡脖子了,所以只需要考虑满流的边即可。考虑简单路径 $s-u-v-t$ 如果 $s-u$ 和 $v-t$ 边都没有满流，只有 $u-v$ 满流，那么 $u-v$ 边就是关键边,
如果一条边是满流，而且两个端点都同时存在到起点和终点的增广路径，那么增大这条边的容量可以构造出一个增广路径，让最大流变大。

POJ3204：第一行包含 $N$ 和 $M$，表示城市和道路的数量。
接下来 $M$ 行，每行包含三个整数 $a,b,c$，表示存在一条道路从城市 $a$ 通往城市 $b$，且运输能力为 $c$。
所有道路都是有方向的。城市编号从 $0$ 到 $N - 1$。起点 $0$ 号点，终点为 $N - 1$ 号城市。

输出格式:
输出一个整数 $K$，表示存在 $K$ 条关键边。

```cpp
const int N = 510, M = 5010 * 2, INF = 0x3f3f3f3f;
int n, m, S, T;
int h[N], e[M], ne[M];
int q[N], f[M], d[N], cur[N], idx;
bool vis_s[N], vis_t[N]; //能到S和能到T的点
void add(int a, int b, int c) {
    e[idx] = b, f[idx] = c, ne[idx] = h[a], h[a] = idx ++;
    e[idx] = a, f[idx] = 0, ne[idx] = h[b], h[b] = idx ++;
}
bool bfs() {
    int hh = 0, tt = 0;
    memset(d, -1, sizeof d);
    q[0] = S, d[S] = 0, cur[S] = h[S];
    while (hh <= tt) {
        int t = q[hh++];
        for (int i = h[t]; i != -1; i = ne[i]) {
            int ver = e[i];
            if (d[ver] == -1 && f[i]) {
                d[ver] = d[t] + 1; //层数
                cur[ver] = h[ver];
                if (ver == T) return true;
                q[++tt] = ver;
            }
        }
    }
    return false;
}
int find(int u, int limit) { //当前u 从S到u最大的可增加流量是limit
    if (u == T) return limit;
    int flow = 0; //从当前点往后流的流量最多是多少
    for (int i = cur[u]; i != -1 && flow < limit; i = ne[i]) {
        cur[u] = i; //当前搜的弧
        int ver = e[i];
        if (d[ver] == d[u] + 1 && f[i]) { //只搜下一层的点 防止环的干扰
            int t = find(ver, min(f[i], limit - flow));
            if (!t) d[ver] = -1; //当前边到终点没有增广路 就层数设为-1，之后不会再搜到了
            f[i] -= t, f[i ^ 1] += t, flow += t; //存在增广路 就更新残留网络
        }
    }
    return flow;
}
int dinic() {
    int r = 0, flow;
    while (bfs()) while (flow = find(S, INF)) r += flow;
    return r;
}
void dfs(int u, bool st[], int t) {
    st[u] = true; //标记已经走过
    for (int i = h[u]; i != -1; i = ne[i]) {
        int j = i ^ t; //i^t 作用是从S搜是正向边 从T搜是反向边
        int ver = e[i];
        if (f[j] && !st[ver]) //当前邻边没有满流并且没有被搜过
            dfs(ver, st, t);
    }
}
int main() {
    scanf("%d%d", &n, &m);
    S = 0, T = n - 1;
    memset(h, -1, sizeof h);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c);
    }
    dinic();
    dfs(S, vis_s, 0); //不需要反向边
    dfs(T, vis_t, 1); //从汇点反向搜需要用到反向边 因为汇点开始走的边都是反向边，而关键边是正向边
    int res = 0;
    for (int i = 0; i < m * 2; i += 2)
        //一个边是满流并且起点能到S，终点能到T，就是一条关键边
        if (!f[i] && vis_s[e[i ^ 1]] && vis_t[e[i]]) //!f[i]就是满流，此时f是残留网络的流量
            res++;
    printf("%d\n", res);
    return 0;
}
```
#### 最大流拆点
USACO 2007 Open Gold: 农夫约翰一共烹制了 $F$ 种食物，并提供了 $D$ 种饮料。
约翰共有 $N$ 头奶牛，其中第 $i$ 头奶牛有 $F_i$ 种喜欢的食物以及 $D_i$ 种喜欢的饮料。
约翰需要给每头奶牛分配一种食物和一种饮料，并使得有吃有喝的奶牛数量尽可能大。
每种食物或饮料都只有一份，所以只能分配给一头奶牛食用（即一旦将第 $2$ 种食物分配给了一头奶牛，就不能再分配给其他奶牛了）

输入格式:
第一行包含三个整数 $N,F,D$

接下来 $N$ 行，其中第 $i$ 行描述第 $i$ 头奶牛的饮食喜好，首先包含两个整数 $F_i$ 和 $D_i$，表示其喜欢的食物和饮料数量，然后包含 $F_i$ 个整数表示其喜欢的食物的种类编号，最后包含 $D_i$ 个整数表示其喜欢的饮料的种类编号。食物编号从 $1$ 到 $F$，饮料编号从 $1$ 到 $D$

输出格式:输出一个整数，表示能够有吃有喝的奶牛的最大数量。

最大流拆点求三分图匹配：将食物，牛，饮料都分别看成一竖排，再加上源点 $S$ 和汇点 $T$
源点 $S$ 向所有食物连边，所有饮料向 $T$ 连边，每个牛匹配的食物向牛连边,每个牛向其匹配的饮料向后连边。

考虑如果每个牛可以多进多出边，那么就不能满足题目的 “给每头奶牛分配一种食物和一种饮料” 条件，所以必须把牛的点拆开
拆点后注意编号问题，以及数组的点数计算问题。

```cpp
const int N = 410, M = 41000, INF = 0x3f3f3f3f;
int n, F, D, S, T;
int h[N], e[M], ne[M], f[M], idx;
int q[N], d[N], cur[N];
//建图点的顺序：所有牛的入点1~n，所有牛的出点n + 1 ~ 2n, 食物2n + 1~ 2n + F, 饮料2n + F + 1 ~ 2n + F  + 1 + D
void add(int a, int b, int c) {
    e[idx] = b, ne[idx] = h[a], f[idx] = c, h[a] = idx ++;
    e[idx] = a, f[idx] = 0, ne[idx] = h[b], h[b] = idx ++;
}
bool bfs() {
    int hh = 0, tt = 0;
    memset(d, -1, sizeof d);
    q[0] = S, d[S] = 0, cur[S] = h[S];
    while (hh <= tt) {
        int t = q[hh++];
        for (int i = h[t]; i != -1; i = ne[i]) {
            int ver = e[i];
            if (d[ver] == -1 && f[i]) {
                d[ver] = d[t] + 1; //层数
                cur[ver] = h[ver];
                if (ver == T) return true;
                q[++tt] = ver;
            }
        }
    }
    return false;
}
int find(int u, int limit) { //当前u 从S到u最大的可增加流量是limit
    if (u == T) return limit;
    int flow = 0; //从当前点往后流的流量最多是多少
    for (int i = cur[u]; i != -1 && flow < limit; i = ne[i]) {
        cur[u] = i; //当前搜的弧
        int ver = e[i];
        if (d[ver] == d[u] + 1 && f[i]) { //只搜下一层的点 防止环的干扰
            int t = find(ver, min(f[i], limit - flow));
            if (!t) d[ver] = -1; //当前边到终点没有增广路 就层数设为-1，之后不会再搜到了
            f[i] -= t, f[i ^ 1] += t, flow += t; //存在增广路 就更新残留网络
        }
    }
    return flow;
}
int dinic() {
    int r = 0, flow;
    while (bfs())  //bfs同时建立分层图 以及返回是否有增广路
        while (flow = find(S, INF)) r += flow; //所有能够增广的流量累加
    return r;
}
int main() {
    scanf("%d%d%d", &n, &F, &D);
    S = 0, T = n * 2 + F + D + 1;
    memset(h, -1, sizeof h);
    for (int i = 1; i <= F; i++) add(S, n * 2 + i, 1); //源点向所有食物连边
    for (int i = 1; i <= D; i++) add(2 * n + F + i, T, 1); //饮料向汇点连边
    for (int i = 1; i <= n; i++) {
        add(i, n + i, 1); //牛的拆点后的入点到出点连边权为1的边
        int a, b, t;
        scanf("%d%d", &a, &b);
        while (a --) {
            scanf("%d", &t);
            add(n * 2 + t, i, 1); //牛匹配的食物向牛的入点连边
        }
        while (b --) {
            scanf("%d", &t);
            add(i + n, n * 2 + F + t, 1); //牛的出点向牛匹配的饮料连边
        }
    }
    printf("%d\n", dinic());
    return 0;
}
```
#### 最小割之最大权闭合图

NOI2006最大获利：