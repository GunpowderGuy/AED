#include <iostream>
#include <vector>

using namespace std;

const long long INF = 2e18;
long long global_max = -INF;

long long dfs(int u, int p, const vector<vector<int>>& adj, const vector<long long>& a) {
long long max1 = 0;
long long max2 = 0;

for (int v : adj[u]) {
    if (v == p) continue;
    
    long long val = dfs(v, u, adj, a);
    
    if (val > max1) {
        max2 = max1;
        max1 = val;
    } else if (val > max2) {
        max2 = val;
    }
}

long long max_actual = a[u] + max1 + max2;
if (max_actual > global_max) {
    global_max = max_actual;
}

return a[u] + max1;
}

int main() {
ios_base::sync_with_stdio(false);
cin.tie(nullptr);
int n;
if (!(cin >> n)) return 0;
vector<long long> a(n + 1);
for (int i = 1; i <= n; ++i) {
cin >> a[i];
}
vector<vector<int>> adj(n + 1);
for (int i = 0; i < n - 1; ++i) {
int u, v;
cin >> u >> v;
adj[u].push_back(v);
adj[v].push_back(u);
}
dfs(1, 0, adj, a);
cout << global_max << "\n";
return 0;
}
