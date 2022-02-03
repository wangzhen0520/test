#ifndef union_find_h
#define union_find_h

#include <vector>

class UF {
public:
    UF(int n)
    {
        _count = n;
        _parent.resize(n);
        _size.resize(n);
        for (int i = 0; i < n; i++) {
            _parent[i] = i;
            _size[i] = 1;
        }
    }

    /* 将 p 和 q 连通 */
    void uion(int p, int q)
    {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ)
            return;

        // 小树接到大树下面，较平衡
        if (_size[rootP] > _size[rootQ]) {
            _parent[rootQ] = rootP;
            _size[rootP] += _size[rootQ];
        }
        else {
            _parent[rootP] = rootQ;
            _size[rootQ] += _size[rootP];
        }
        // 两个分量合二为一
        _count--;
    }

    /* 判断 p 和 q 是否互相连通 */
    bool connected(int p, int q)
    {
        int rootP = find(p);
        int rootQ = find(q);
        // 处于同一棵树上的节点，相互连通
        return rootP == rootQ;
    }

    int count()
    {
        return _count;
    }

private:
    /* 返回节点 x 的根节点 */
    int find(int x)
    {
        // 根节点的 _parent[x] == x
        while (_parent[x] != x) {
            // 进行路径压缩
            _parent[x] = _parent[_parent[x]];
            x = _parent[x];
        }
        return x;
    }

private:
    // 记录连通分量个数
    int _count;
    // 存储若干棵树
    std::vector<int> _parent;
    // 记录树的“重量”
    std::vector<int> _size;
};

#endif // union_find_h