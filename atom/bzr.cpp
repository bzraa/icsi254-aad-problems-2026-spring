#include <iostream>
using namespace std;

long long binomial(int n, int r) {
  if (r > n || r < 0)
    return 0;
  if (r == 0 || r == n)
    return 1;
  if (r > n - r)
    r = n - r;

  long long result = 1;
  for (int i = 0; i < r; i++) {
    result *= (n - i);
    result /= (i + 1);
  }
  return result;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;

    if (m > n - 3 || m < 0) {
      cout << 0 << endl;
      continue;
    }
    long long ways_to_choose_3 = binomial(n, 3);

    long long ways_to_partition = binomial(n - 3, m);

    long long result = ways_to_choose_3 * ways_to_partition;

    cout << result << endl;
  }
}
