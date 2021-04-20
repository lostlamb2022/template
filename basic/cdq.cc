/*给定 n 个元素（编号 1~n），其中第 i 个元素具有 ai,bi,ci 三种属性。设 f(i) 表示满足以下 4 个条件：aj<=ai,bj<=bi,cj<=ci,j!=i的j的数量。对于 d∈[0,n)，求满足 f(i)=d 的i的数量。
输入格式:第一行n,k元素数量和最大属性值。接着n行第i行包含三个整数 ai,bi,ci。输出格式:n行每行一个整数，第 d+1行表示满足f(i)=d的i的数量。
数据范围：1<=n<=1e5,1<=ai,bi,ci<=k<=2e5
一维直接排序a 二维归并排序b 三维树状数组c,排序是三关键字排序,归并排序是为了排b，此时ab都已单调，归并过程中双指针扫描BIT查询c更新答案。
归并过程中处理了ab之后，将c插入树状数组，以c的权值建立树状数组，ci数据范围小不需要离散化。时间复杂度O(N*log^2N)*/
const int N = 100010, M = 200010;
int n, m;
struct Data {
    //res是比当前元素小的元素个数
    //s是和该元素完全相同元素的个数
    int a, b, c, s, res;
    bool operator< (const Data& t) const {
        if (a != t.a) return a < t.a;
        if (b != t.b) return b < t.b;
        return c < t.c;
    }
    bool operator== (const Data& t) const {
        return a == t.a && b == t.b && c == t.c;
    }
} q[N], w[N]; //w[]归并的临时数组
int tr[M], ans[N];
int lowbit(int x) {
    return x & -x;
}
void add(int x, int v) {
    for (int i = x; i < M; i += lowbit(i)) tr[i] += v;
}
int query(int x) {
    int res = 0;
    for (int i = x; i; i -= lowbit(i)) res += tr[i];
    return res;
}
void merge_sort(int l, int r) {
    if (l >= r) return;
    int mid = l + r >> 1;
    merge_sort(l, mid), merge_sort(mid + 1, r);
    int i = l, j = mid + 1, k = 0;
    while (i <= mid && j <= r)
        //左一半来add  右一半来查询答案
        if (q[i].b <= q[j].b) add(q[i].c, q[i].s), w[k++] = q[i++];
    //b已经有序了，所以i后边的b肯定比q[j].b都大
    //此时q[j]就可以求出符合小于c的元素个数了
        else q[j].res += query(q[j].c), w[k++] = q[j++];
    //扫描剩下的元素
    while (i <= mid) add(q[i].c, q[i].s), w[k++] = q[i++];
    while (j <= r) q[j].res += query(q[j].c), w[k++] = q[j++];
    //每次递归一层结束将左边add的树状数组清空
    for (int i = l; i <= mid; i++) add(q[i].c, -q[i].s);
    for (int i = l, j = 0; j < k; i++, j++) q[i] = w[j];
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i++) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        q[i] = {a, b, c, 1};
    }
    sort(q, q + n);
    int k = 1;
    for (int i = 1; i < n; i++) //去重记录相同元素的数量
        if (q[i] == q[k - 1]) q[k - 1].s++;
        else q[k++] = q[i];
    merge_sort(0, k - 1);
    for (int i = 0; i < k; i++)
        //res存去重之后的结果，还要加上相同的配对的s-1个
        ans[q[i].res + q[i].s - 1] += q[i].s;
    for (int i = 0; i < n; i++) printf("%d\n", ans[i]);
    return 0;
}