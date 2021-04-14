/*各操作复杂度：插入一个数O(logN),求堆顶O(1),删除堆顶O(logN),合并2棵左偏树O(logN)
模板题：维护一个小根堆的集合，初始时集合是空
该集合需要支持如下四种操作：
1 a，在集合中插入一个新堆，堆中只包含一个数 a。
2 x y，将第 x 个插入的数和第 y 个插入的数所在的小根堆合并。数据保证两个数均未被删除。若两数已在同一堆中，则忽略此操作。
3 x，输出第 x 个插入的数所在小根堆的最小值。数据保证该数未被删除。
4 x，删除第 x 个插入的数所在小根堆的最小值（若最小值不唯一，则优先删除先插入的数）。数据保证该数未被删除。
DSU+左偏树 DSU不支持删除，还要使用DSU的换根操作
同时维护DSU和左偏树的集合,每棵左偏树的root和DSU的root的编号是相同的，但是两者没有其他的联系*/
const int N = 200010; //假设所有操作都是增加节点 节点数量上界
int n; //操作数量
int v[N], dist[N], l[N], r[N], idx;
int p[N]; //DSU father
//双关键字排序，val相同比较编号
bool cmp(int x, int y) {
    if (v[x] != v[y]) return v[x] < v[y];
    return x < y;
}
int find(int x) {
    if (p[x] != x) p[x] = find(p[x]);
    return p[x];
}
//合并x为根的子树和y为根的子树 返回合并后的root节点编号
int merge(int x, int y) {
    if (!x || !y) return x + y; //空节点直接返回不空的那个
    if (cmp(y, x)) swap(x, y); //x是较小的点的编号
    r[x] = merge(r[x], y); //让x的右子树变为x的右子树和y合并之后的子树
    //回溯维护dist
    if (dist[r[x]] > dist[l[x]]) swap(l[x], r[x]); //维护左偏
    dist[x] = dist[r[x]] + 1;//每个节点dist为右儿子dist+1
    return x;
}
int main() {
    scanf("%d", &n);
    v[0] = 2e9; //边界0不会用到 x,y从1开始防止0变成root
    while (n --) {
        int t, x, y;
        scanf("%d%d", &t, &x);
        if (t == 1) {
            v[++idx] = x;
            dist[idx] = 1; //插入的时候要初始化该节点的dist 空节点dist从1开始
            p[idx] = idx;
        } else if (t == 2) {
            scanf("%d", &y);
            x = find(x), y = find(y);
            if (x != y) {
                if (cmp(y, x)) swap(x, y);
                p[y] = x;
                merge(x, y);
            }
        } else if (t == 3) {
            printf("%d\n", v[find(x)]);
        } else {
            x = find(x);
            if (cmp(r[x], l[x])) swap(l[x], r[x]);
            p[x] = l[x], p[l[x]] = l[x]; //并查集换根
            merge(l[x], r[x]);
        }
    }
    return 0;
}