#include <ctime>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>
#define N 1000005
#define M 9000005
#define depth_lim 4
#define INTERVAL 15
#define SPLIT 24 * 60 / INTERVAL
using namespace std;
int tot, head[N], h, t;
stack<string> st;
map<string, int> mp;
vector<string> message;
struct Edge {
    int v, w, nxt;
    string info;
} edge[M];
struct QElem {
    int u, depth, from;
    string info;
} queue[20000005];
struct Node{
    vector<string> resinfo;
    int restime;
};
vector<Node> res;
void addedge(int u, int v, int w, string info) {
    edge[++tot] = (Edge){v, w, head[u], info};
    head[u] = tot;
}
void backward(int x) {
    if (x == -1)
        return;
    st.push(queue[x].info);
    backward(queue[x].from);
}
void BFS(int S, int T) {
    h = 1;
    t = 0;
    queue[++t] = QElem{S, 0, -1};
    while (h <= t && queue[h].depth < depth_lim) {
        QElem u = queue[h];
        ++h;
        for (int i = head[u.u]; i; i = edge[i].nxt) {
            int v = edge[i].v;
            if (v == u.from)
                continue;
            else if (v == T) {
                message.clear();
                st.push(edge[i].info);
                backward(h - 1);
                while (!st.empty()) {
                    message.push_back(st.top());
                    st.pop();
                }
                res.push_back(Node{message, 0});
            } else
                queue[++t] = QElem{v, u.depth + 1, h - 1, edge[i].info};
        }
    }
}
int main() {
    clock_t start, end;
    start = clock();
    freopen("in.txt", "r", stdin);
    int m, id;
    string station;
    cin >> m;
    while (m--) {
        cin >> station >> id;
        mp[station] = id;
    }
    int u, v, w;
    string info;
    while (cin >> u >> v >> w >> info) {
        addedge(u, v, w, info);
        // if(u==316&&v==46) cout<<"1";
        // if(u==46&&v==143) cout<<"2";
        // if(u==143&&v==437) cout<<"3";
        // if(u==437&&v==1154) cout<<"4";
    }
    // 计时
    end = clock();
    cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    string ST = "北京南", ED = "上海";
    int st = mp[ST], ed = mp[ED];
    for (int i = st * SPLIT * 2 + SPLIT; i < (st + 1) * SPLIT * 2; i++) {
        for (int j = ed * SPLIT * 2; j < ed * SPLIT * 2 + SPLIT; j++) {
            BFS(i, j);
        }
    }
    for(int i=0;i<res.size();i++){
        for(int j=0;j<res[i].resinfo.size();j++){
            cout<<res[i].resinfo[j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}