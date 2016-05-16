
// assumes ascending sorted array
int binary_search(int lo, int hi, int *a, int elm) {
	int mid;
	if(lo > hi)
		return -1;
	else {
		mid = (lo + hi) / 2;
		if(a[mid] < elm)
			return binary_search(mid + 1, hi, a, elm);
		else if(a[mid] > elm)
			return binary_search(lo, mid - 1, a, elm);
		else
			return mid;
	}
}

int main() {
	int a[10];
	int t[10];
	int i, j, elm;

	i = 0;
	while(i < 10) {
		a[i] = i * i;
		i++ ;
	}
	
	printf("Here is the array\n");
	for(i = 0 ; i < 10 ; i++) {
		printf(a[i], "\n");
	}
	elm = 49;
	printf(binary_search(0, 9, a, elm), "\n");
}