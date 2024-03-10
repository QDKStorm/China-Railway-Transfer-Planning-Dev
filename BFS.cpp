#include <algorithm>
#include <cstring>
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
#define BADLIM 2.0
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
struct Node {
    vector<string> resinfo;
    int restime;
};
vector<Node> res;
int timecalc(string st, string ed) {
    int sth = (st[0] - '0') * 10 + (st[1] - '0');
    int stm = (st[3] - '0') * 10 + (st[4] - '0');
    int edh = (ed[0] - '0') * 10 + (ed[1] - '0');
    int edm = (ed[3] - '0') * 10 + (ed[4] - '0');
    if ((edh - sth) * 60 + (edm - stm) > 0)
        return (edh - sth) * 60 + (edm - stm);
    else
        return (edh - sth + 24) * 60 + (edm - stm);
}
void Stringsplit(const string &str, const string &split, vector<string> &res) {
    char *strc = new char[str.size() + 1];
    strcpy(strc, str.c_str());
    char *temp = strtok(strc, split.c_str());
    while (temp != NULL) {
        res.push_back(string(temp));
        temp = strtok(NULL, split.c_str());
    }
    delete[] strc;
}

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
    }
    end = clock();
    cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    vector<string> ststations, edstations;
    string ST = "临潼", ED = "北京西";
    Stringsplit(ST, " ", ststations);
    Stringsplit(ED, " ", edstations);
    for (int i = 0; i < ststations.size(); i++) {
        for (int j = 0; j < edstations.size(); j++) {
            if (mp.find(ststations[i]) == mp.end() || mp.find(edstations[j]) == mp.end())
                continue;
            int st = mp[ststations[i]], ed = mp[edstations[j]];
            for (int i = st * SPLIT * 2 + SPLIT; i < (st + 1) * SPLIT * 2; i++) {
                for (int j = ed * SPLIT * 2; j < ed * SPLIT * 2 + SPLIT; j++) {
                    BFS(i, j);
                }
            }
        }
    }
    for (int i = 0; i < res.size(); i++) {
        string lastime = "";
        for (int j = 0; j < res[i].resinfo.size(); j++) {
            if (res[i].resinfo[j].length() <= 4)
                continue;
            int infolength = res[i].resinfo[j].length();
            string stime = res[i].resinfo[j].substr(infolength - 13, 5);
            string etime = res[i].resinfo[j].substr(infolength - 5, 5);
            if (lastime != "")
                res[i].restime += timecalc(lastime, stime);
            res[i].restime += timecalc(stime, etime);
            lastime = etime;
        }
    }
    sort(res.begin(), res.end(), [](Node a, Node b) { return a.restime < b.restime; });
    for (int i = 0; i < res.size(); i++) {
        if (res[i].restime > BADLIM * res[0].restime)
            break;
        for (int j = 0; j < res[i].resinfo.size(); j++) {
            cout << res[i].resinfo[j] << ' ';
        }
        cout << endl;
    }
    return 0;
}