#include <limits.h>
//INT_MAX

bool is_adn_O_fn(int (*d)(int), int (*f)(int), double a) {
    double c = 1.0;
    int n0 = 1;
    while (d(n0) > c * f(n0)) {
        n0++;
    }
    for (int n = n0; n < INT_MAX; n++) {
        if (a * d(n) > c * f(n)) {
            return false;
        }
    }
    return true;
}
