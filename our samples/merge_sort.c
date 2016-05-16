
void mergesort(int lo, int hi, float * a, float * tmp) {
	int mid, i, j, k;
	if(lo < hi) {
		mid = (lo + hi) / 2;
		mergesort(lo, mid, a, tmp);
		mergesort(mid + 1, hi, a, tmp);
		for(i = lo ; i <= hi ; i++)
			tmp[i] = a[i];

		i = lo;
		j = mid + 1;
		for(k = lo ; k <= hi ; k++) {
			if(i == mid + 1)
				a[k] = tmp[j++];
			else if(j == hi + 1)
				a[k] = tmp[i++];
			else if(tmp[i] < tmp[j])
				a[k] = tmp[i++];
			else
				a[k] = tmp[j++];
		}
	}
	else {
		;
	}
}

int main() {
	float a[10];
	float t[10];
	int i, j;
	for(i = 0 ; i < 10 ; i++)
		a[i] = 10.934 - i * 0.99;
	printf("Here is the array\n");
	for(i = 0 ; i < 10 ; i++) {
		printf(a[i], "\n");
	}
	mergesort(0, 10 - 1, a, t);
	printf("Here is the sorted array\n");
	for(i = 0 ; i < 10 ; i++) {
		printf(a[i], "\n");
	}
}