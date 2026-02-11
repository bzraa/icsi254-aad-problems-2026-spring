#include <iostream>
#include <map>
using namespace std;

map<pair<long long, long long>, long long> memo;

long long binomial(long long n, long long r) {
  if (r > n || r < 0)
    return 0;
  if (r == 0 || r == n)
    return 1;
  if (r > n - r)
    r = n - r;

  pair<long long, long long> key = {n, r};
  if (memo.find(key) != memo.end())
    return memo[key];

  long long result = 1;
  for (long long i = 0; i < r; i++) {
    result *= (n - i);
    result /= (i + 1);
  }

  memo[key] = result;
  return result;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    long long n, m;
    cin >> n >> m;

    if (m > n - 3 || m < 0) {
      cout << 0 << endl;
      continue;
    }

    cout << binomial(n, 3) * binomial(n - 3, m) << endl;
  }
}
