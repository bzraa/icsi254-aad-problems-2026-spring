#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> adj(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    a = --a;
    b = --b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  // for (int i = 0; i < n; i++) {
  //   cout << i << " : ";
  //   for (int j = 0; j < adj[i].size(); j++) {
  //     cout << adj[i][j] << " ";
  //   }
  //   cout << endl;
  // }

  vector<int> teams(n, 0);
  for (int i = 0; i < n; i++) {
    if (teams[i] != 0) {
      continue;
    }
    teams[i] = 1;
    queue<int> q;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();

      for (int j = 0; j < adj[cur].size(); j++) {
        if (teams[adj[cur][j]] == 0) {
          teams[adj[cur][j]] = 3 - teams[cur];
          q.push(adj[cur][j]);
        } else if (teams[cur] == teams[adj[cur][j]]) {
          // cout << cur << " " << teams[cur] << " " << j << " " << teams[j]
          //      << "\n";
          cout << "Bipartite bish" << endl;
          return 0;
        }
      }

      // for (int j = 0; j < n; j++) {
      //   cout << teams[j] << " ";
      // }
      // cout << endl;
    }
  }
  for (int i = 0; i < n; i++) {
    cout << teams[i] << " ";
  }
  cout << endl;
}
