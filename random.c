#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
	int n, max;
	time_t tt;
	scanf("%d%d", &n, &max);
	srandom(time(&tt));
	while(n) {
		printf("%ld\n", random() % max);
		n--;
	}
	return 0;
}

