#include <bits/stdc++.h>
using namespace std;

/*
  Given A, B length N.
  Choose integer X, add to all A: A[i] += X
  Then you may change at most K elements of A to any values (optimal: set them to B[i]) -> diff becomes 0.

  Let C[i] = A[i] - B[i]
  After adding X: |C[i] + X|
  Changing up to K elements means we can remove up to K largest absolute diffs.
  So for a fixed X, cost = sum of (N-K) smallest values of |C[i] + X|.

  Let p = -X. We want min over p of sum of (N-K) smallest |C[i] - p|.
  After sorting C, the best (N-K) elements form a contiguous window; optimal p for a window is its median.
  Evaluate all windows of length M = N-K.
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<long long> A(N), B(N);
    for (int i = 0; i < N; i++) cin >> A[i];
    for (int i = 0; i < N; i++) cin >> B[i];

    int M = N - K;                 // how many elements must remain (not "fixed")
    if (M <= 0) {                  // we can change all elements -> 0
        cout << 0 << "\n";
        return 0;
    }

    vector<long long> C(N);
    for (int i = 0; i < N; i++) C[i] = A[i] - B[i];
    sort(C.begin(), C.end());

    // prefix sums: ps[i] = sum(C[0..i-1])
    vector<long long> ps(N + 1, 0);
    for (int i = 0; i < N; i++) ps[i + 1] = ps[i] + C[i];

    auto window_cost = [&](int l) -> long long {
        int r = l + M - 1;
        int m = (l + r) / 2;      // median index
        long long p = C[m];

        long long left  = p * (m - l) - (ps[m] - ps[l]);
        long long right = (ps[r + 1] - ps[m + 1]) - p * (r - m);
        return left + right;
    };

    long long ans = LLONG_MAX;
    for (int l = 0; l + M - 1 < N; l++) {
        ans = min(ans, window_cost(l));
    }

    cout << ans << "\n";
    return 0;
}
