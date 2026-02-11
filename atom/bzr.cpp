#include <iostream>
using namespace std;

// Function to calculate nCr (binomial coefficient)
long long binomial(int n, int r) {
  if (r > n || r < 0)
    return 0;
  if (r == 0 || r == n)
    return 1;
  if (r > n - r)
    r = n - r; // Optimization: C(n,r) = C(n,n-r)

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

    // We choose 3 electrons to destroy, leaving n-3 electrons
    // We need at least one part to have exactly m electrons

    // Check if it's possible
    if (m > n - 3 || m < 0) {
      cout << 0 << endl;
      continue;
    }

    // Number of ways to choose 3 electrons from n
    long long ways_to_choose_3 = binomial(n, 3);

    // Number of ways to partition the remaining (n-3) electrons
    // such that one side has exactly m electrons
    long long ways_to_partition = binomial(n - 3, m);

    // Total distinct ways
    long long result = ways_to_choose_3 * ways_to_partition;

    cout << result << endl;
  }
}
