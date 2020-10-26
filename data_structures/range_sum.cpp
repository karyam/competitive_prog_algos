#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
// #include <template>

/*
Segment tree (ST): data structure which answers range queries on a dynamic array.
Fenwick trees are an effective substiture but only when the operatos is ... (min, max are not included).

Queries which are allowed by STs:
- update one pos 
- update a range
- query a range

Can be easily generalised to larger dimensions.
Time complexity O(log(n)^#dim)
Query time complexity analysis: Prove by induction that the query function
visits at most 4 nodes on each level which gives the log complexity.
Space complexity: O(n) the standard ST requires 4n vertices

To think about when applying ST to your problem:
- what gets stored in a node
- how to merge 2 nodes; should be wary that this operation might not be constant for some problems
*/

using namespace std;
using ll = long long;

ll aggregator(ll x, ll y) {
    return 1LL*x + y;
};

class SegmentTree {
    public:

    SegmentTree(int n, ll initValue) {
        this->n = n;
        tree = vector<ll>(4*n+2, initValue);
    }

    void build(const vector<ll> &arr, int curNode, int left, int right) {
        if (left == right) {
            tree[curNode] = arr[left];
            return;
        }
        int mid = (left + right) >> 1;
        build(arr, leftSon(curNode), left, mid);
        build(arr, rightSon(curNode), mid+1, right);
        tree[curNode] = aggregator(tree[leftSon(curNode)], tree[rightSon(curNode)]);
    }

    ll query(int curNode, int left, int right, int l, int r) {
        //cout << left << " " << right << " " << l << " " << r << "\n";
        if (r < l) { return 0; }
        if (left == l && right == r) { //completely contained case
            return tree[curNode];
        }
        int mid = (left + right) >> 1;
        ll leftResult = query(leftSon(curNode), left, mid, l, min(mid, r));
        ll rightResult = query(rightSon(curNode), mid+1, right, max(mid+1, l), r);
        return aggregator(leftResult, rightResult);
    }

    void update(int curNode, int left, int right, int pos, ll value) {
        if (left == right) {
            tree[curNode] = value;
            return;
        }
        int mid = (left + right) >> 1;
        if (pos <= mid) update(leftSon(curNode), left, mid, pos, value);
        else update(rightSon(curNode), mid+1, right, pos, value);
        tree[curNode] = aggregator(tree[leftSon(curNode)], tree[rightSon(curNode)]);
    }

    int leftSon(int node) {
        return (node << 1) + 1;
    }

    int rightSon(int node) {
        return (node << 1) + 2;
    }
    
    private:
    int n;
    vector<ll> tree;
};

int main() {
    int n, q; 
    cin >> n >> q;
    vector<ll> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    SegmentTree st = SegmentTree(n, 0LL);
    st.build(arr, 0, 0, n-1);
    
    while (q--) {
        int type; cin >> type; 
        if (type == 1) {
            int a; cin >> a;
            ll b; cin >> b;
            st.update(0, 0, n-1, a-1, b);
        } else {
            int a, b; cin >> a >> b;
            cout << ll(st.query(0, 0, n-1, a-1, b-1)) << "\n";
        }
    }
    return 0;
}