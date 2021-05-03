template <typename T>
inline T
read() { //支持负数 不支持浮点数
    T sum = 0, fl = 1;
    int ch = getchar();
    for (; !isdigit(ch); ch = getchar())
        if (ch == '-') fl = -1;
    for (; isdigit(ch); ch = getchar()) sum = sum * 10 + ch - '0';
    return sum * fl;
}
//int a = read<int>();
//long long b = read<long long>();