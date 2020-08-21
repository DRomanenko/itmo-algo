def gcd(a, b):
    if a == 0:
        return b, 0, 1
    g, x, y = gcd(b % a, a)
    return g, y - (b // a) * x, x
  
N = int(input())
for _ in range(N):
    a, b, n, m = map(int, input().split())
    _, x, y = gcd(m, n)
    print((a * (x % n) * m + b * (y % m) * n) % (n * m))