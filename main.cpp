#include "big.h"

using namespace std;

int main() {
    big a;
    cin >> a;
    cout << a.primefact();
    big b;
    cin >> b;
    cout << a.gcd(b);
    return 0;
}