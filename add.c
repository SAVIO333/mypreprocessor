#include "add.h"

#define NUM1 10
#define NUM2 20
#define SUM(x, y) ((x) + (y))

int main() {
    int res1 = add(NUM1, NUM2);
    int res2 = SUM(NUM1, NUM2);
    return 0;
}
