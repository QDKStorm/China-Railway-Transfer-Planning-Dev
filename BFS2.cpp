#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#define N 1000005
#define M 9000005
#define INTERVAL 15
#define SPLIT 24 * 60 / INTERVAL
#define BADLIM 2.0
using namespace std;
int tot, head[N], h, t, depth_lim = 3, best = 0x3fffffff, times[300000];
stack<string> st;
map<pair<string, int>, pair<int, int>> mp;
vector<string> message;
struct Edge {
    int v, w, nxt;
    string info;
    bool T, Y;
} edge[M];
struct QElem {
    int u, depth, from, cumtime;
    string info;
}queue[20000005];
struct Node {
    vector<string> resinfo;
    int restime, estitime;
};
vector<Node> res;
int timecalc(string st, string ed) {
    int sth = (st[0] - '0') * 10 + (st[1] - '0');
    int stm = (st[3] - '0') * 10 + (st[4] - '0');
    int edh = (ed[0] - '0') * 10 + (ed[1] - '0');
    int edm = (ed[3] - '0') * 10 + (ed[4] - '0');
    int ret = (edh - sth) * 60 + (edm - stm);
    if (ret > 0)
        return ret;
    else
        return ret + 24 * 60;
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
void addedge(int u, int v, int w, string info, char y) {
    edge[++tot] = (Edge){v, w, head[u], info, false, y == 'Y' ? true : false};
    head[u] = tot;
    if (info == "T")
        edge[tot].T = true;
}
void backward(int x) {
    if (x == -1)
        return;
    st.push(queue[x].info);
    backward(queue[x].from);
}
void BFS(int S, int T, bool Ttag, bool Ytag, bool Gban, bool Gonly, bool lingchen) {
    h = 1;
    t = 0;
    queue[++t] = QElem{S, 0, -1, 0, ""};
    while (h <= t && queue[h].depth < depth_lim) {
        QElem u = queue[h];
        ++h;
        if (u.cumtime > BADLIM * best)
            continue;
        for (int i = head[u.u]; i; i = edge[i].nxt) {
            if (edge[i].T && !Ttag)
                continue;
            if (edge[i].Y && !Ytag)
                continue;
            if (Gban && edge[i].info.length() > 4 && (edge[i].info[0] == 'G' || edge[i].info[0] == 'D'))
                continue;
            if (Gonly && edge[i].info.length() > 4 && edge[i].info[0] != 'G' && edge[i].info[0] != 'D')
                continue;
            int infolength = edge[i].info.length();
            if (!lingchen && edge[i].info.length() > 4 && (timecalc("00:00", edge[i].info.substr(infolength - 13, 5)) <= 6 * 60 || timecalc("00:00", edge[i].info.substr(infolength - 5, 5)) <= 6 * 60))
                continue;
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
                res.push_back(Node{message, 0, 0});
                res[res.size() - 1].estitime = u.cumtime + edge[i].w;
                best = min(best, res[res.size() - 1].estitime);
            } else
                queue[++t] = QElem{v, u.depth + 1, h - 1, edge[i].w + u.cumtime, edge[i].info};
        }
    }
}
int main() {

    freopen("graph.txt", "r", stdin);
    int n, m, io, st, len;
    string station;
    cin >> n >> m;
    while (m--) {
        cin >> station >> io >> st >> len;
        mp[make_pair(station, io)] = make_pair(st, len);
    }
    for (int i = 0; i < n; i++)
        cin >> times[i];
    int u, v, w;
    string info;
    char y;
    while (cin >> u >> v >> w >> info >> y) {
        addedge(u, v, w, info, y);
    }

    string ST = "万象", ED = "阿克陶";
    bool T = true, Y = true, Gban = false, Gonly = false, lingchen = true;
    int stt1 = 0, stm1 = 0, stt2 = 23, stm2 = 59;
    int edt1 = 0, edm1 = 0, edt2 = 23, edm2 = 59;
    int transt = 2;

    res.clear();
    depth_lim = 2 * transt + 1;
    best = 0x3fffffff;
    vector<string> ststations, edstations;
    Stringsplit(ST, " ", ststations);
    Stringsplit(ED, " ", edstations);
    clock_t start, end;
    start = clock();
    for (int i = 0; i < ststations.size(); i++) {
        for (int j = 0; j < edstations.size(); j++) {
            if (mp.find(make_pair(ststations[i], 1)) == mp.end() || mp.find(make_pair(edstations[j], 0)) == mp.end())
                continue;
            pair<int, int> st = mp[make_pair(ststations[i], 1)], ed = mp[make_pair(edstations[j], 0)];
            for (int i = st.first; i < st.first + st.second; i++) {
                if (times[i] < stt1 * 60 + stm1 || times[i] > stt2 * 60 + stm2)
                    continue;
                for (int j = ed.first; j < ed.first + ed.second; j++) {
                    if (times[j] < edt1 * 60 + edm1 || times[j] > edt2 * 60 + edm2)
                        continue;
                    BFS(i, j, T, Y, Gban, Gonly, lingchen);
                }
            }
        }
    }
    end = clock();
    cout << "搜索用时：" + to_string((double)(end - start) / CLOCKS_PER_SEC) + "s" << endl;
    for (int i = 0; i < res.size(); i++) {
        int len = res[i].resinfo.size();
        string lastime = "";
        for (int j = 0; j < len; j++) {
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
        while (res[i].restime < res[i].estitime)
            res[i].restime += 24 * 60;
    }
    sort(res.begin(), res.end(), [](Node a, Node b) { return a.restime < b.restime; });
    for (int i = 0; i < res.size(); i++) {
        if (res[i].restime > BADLIM * res[0].restime)
            break;
        bool tag = true;
        for (int j = 1; j < res[i].resinfo.size() - 2; j += 2) {
            size_t colonpos1 = res[i].resinfo[j].find("：");
            size_t colonpos2 = res[i].resinfo[j + 2].find("：");
            if (colonpos1 != string::npos && colonpos2 != string::npos && res[i].resinfo[j].substr(0, colonpos1) == res[i].resinfo[j + 2].substr(0, colonpos2)) {
                tag = false;
            }
        }
        if (!tag)
            continue;
        for (int j = 0; j < res[i].resinfo.size(); j++) {
            if (res[i].resinfo[j].length() > 4)
                cout << res[i].resinfo[j] << ' ';
            else if (res[i].resinfo[j] == "T")
                cout << "同城换乘" << ' ';
            else if (res[i].resinfo[j] == "W")
                cout << "候车" << ' ';
        }
        cout << endl;
    }
    return 0;
}
/*
隆回 北京西
三亚 阿克陶
渭南西 襄阳
*/