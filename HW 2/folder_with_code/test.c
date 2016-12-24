#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cilk/cilk.h>

int fib(int n)
{
    if (n < 2)
        return n;
    int x = cilk_spawn fib(n-1);
    int y = fib(n-2);
    cilk_sync;
    return x + y;
}



int main() {
	
	int f = fib(3);
	printf("f=%d\n", f);
	int i = 0;
	cilk_for (i=0; i<10; i++){
		printf("I:%d\n", i);
	}


return 0;
	 } 