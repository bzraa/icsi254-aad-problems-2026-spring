import sys

MOD = 998244353

def is_palindrome(s):
    return s == s[::-1]

def is_double_palindrome(s):
    n = len(s)
    if n == 0: return False
    if is_palindrome(s):
        return True
    for i in range(1, n):
        if is_palindrome(s[:i]) and is_palindrome(s[i:]):
            return True
    return False

def solve_brute_force(n, k):
    def generate_all(current_s, length):
        nonlocal count
        if len(current_s) > 0:
            if is_double_palindrome(current_s):
                count = (count + 1) % MOD
        
        if len(current_s) == length:
            return
            
        for i in range(k):
            generate_all(current_s + chr(ord('a') + i), length)

    generate_all("", n)
    return count

if __name__ == "__main__":
    print(f"n=2, k=2 Test: {solve_brute_force(2, 2)}")
