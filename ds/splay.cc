/*
给定一个长度为 n 的整数序列，初始时序列为 {1,2,…,n−1,n}。
序列中的位置从左到右依次标号为 1~n。
我们用 [l,r] 来表示从位置 l 到位置 r 之间（包括两端点）的所有数字构成的子序列。
现在要对该序列进行 m 次操作，每次操作选定一个子序列 [l,r]，并将该子序列中的所有数字进行翻转。
例如，对于现有序列 1 3 2 4 6 5 7，如果某次操作选定翻转子序列为 [3,6]，那么经过这次操作后序列变为 1 3 5 6 4 2 7。
请你求出经过 m 次操作后的序列。
*/
const int N = 100010;
int n, m;
struct Node {
    int s[2], p, v; //s[0]左子节点 s[1]右子节点, p父亲节点 v当前点的编号
    int size, flag; //当前点为根的子树大小 flag是否反转
    void init(int _v, int _p) { //初始化节点
        v = _v, p = _p;
        size = 1;
    }
} tr[N];
int root, idx;
void pushup(int x) { //旋转之后pushup
    //此题只需要维护size
    //左子树size+右子树size+root
    tr[x].size = tr[tr[x].s[0]].size + tr[tr[x].s[1]].size +  1;
}
void pushdown(int x) { //递归之前向下传lazytag
    if (tr[x].flag) {
        //递归交换子节点可以实现反转整个区间
        swap(tr[x].s[0], tr[x].s[1]);
        tr[tr[x].s[0]].flag ^= 1;
        tr[tr[x].s[1]].flag ^= 1;
        tr[x].flag = 0; //下传之后清空当前点的flag
    }
}
void rotate(int x) { //左旋+右旋
    //拓扑序是 Z->Y->X
    int y = tr[x].p, z = tr[y].p;
    int k = tr[y].s[1] == x; // k=0表示x是y的左儿子；k=1表示x是y的右儿子 k=1右旋 否则左旋
    tr[z].s[tr[z].s[1] == y] = x, tr[x].p = z; //z的子节点改为x x的父节点改为z
    //y的左节子点是x的右子节点，或者右子节点是x的左子节点 根据左右旋(k)而定
    tr[y].s[k] = tr[x].s[k ^ 1], tr[tr[x].s[k ^ 1]].p = y; //x的右(左)子节点的父亲是y
    tr[x].s[k ^ 1] = y, tr[y].p = x;
    pushup(y), pushup(x);  //旋转之后先pushup(y) 后pushup(x)
    //此处不用push(z)因为pushup本题的操作只是维护size，而z的子树size没有改变
}
void splay(int x, int k) { //将x旋转到k的下边
    while (tr[x].p != k) {
        //拓扑序是Z->Y->X
        int y = tr[x].p, z = tr[y].p;
        if (z != k)
            //如果是折线状就旋转2次X
            //直线形状就先y后x
            if ((tr[y].s[1] == x) ^ (tr[z].s[1] == y)) rotate(x);
            else rotate(y);
        rotate(x);
    }
    if (!k) root = x; //特判旋转后是root的情况
}
void insert(int v) {
    int u = root, p = 0;
    /*
    当前编号比u的编号大，就插在右子树，反之插在左子树
    splay所维护的信息一定满足BST性质
    比如此题维护的就是数组元素的下标满足BST
    那么下标对应元素的值就不满足BST了
    */
    while (u) p = u, u = tr[u].s[v > tr[u].v];
    u = ++idx;
    if (p) tr[p].s[v > tr[p].v] = u;
    tr[u].init(v, p);
    splay(u, 0); //插入后旋到root
}
int get_k(int k) { //返回中序中的第k个数
    int u = root;
    while (true) {
        pushdown(u);
        //左子树size大于k就在左子树迭代找
        if (tr[tr[u].s[0]].size >= k) u = tr[u].s[0];
        else if (tr[tr[u].s[0]].size + 1 == k) return u;
        else k -= tr[tr[u].s[0]].size + 1, u = tr[u].s[1];
    }
    return -1; //没有找到
}
void output(int u) { //从根节点开始中序输出
    pushdown(u);
    if (tr[u].s[0]) output(tr[u].s[0]);
    if (tr[u].v >= 1 && tr[u].v <= n) printf("%d ", tr[u].v);
    if (tr[u].s[1]) output(tr[u].s[1]);
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n + 1; i++) insert(i); //0,n+1是哨兵 放置RE
    while (m --) { //翻转区间的操作
        int l, r;
        scanf("%d%d", &l, &r);
        //反转的是[l,r]那么就要splay(l-1) splay(r+1)
        //但是有开头哨兵节点的存在要加上1
        l = get_k(l), r = get_k(r + 2);
        splay(l, 0), splay(r, l);
        tr[tr[r].s[0]].flag ^= 1; //旋转之后r的左子树就是要旋转的区间了 更新flag即可
    }
    output(root); //输出中序遍历
    return 0;
}