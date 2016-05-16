struct pair {
	int x;
	float y;
};


// assumes ascending sorted array
struct pair * binary_search(int lo, int hi, struct pair * a, struct pair elm, struct pair* ret) {
	int mid;
	ret -> y = -1 * 1.1;
	printf("lo = ", lo, ", hi = ", hi, "\n");
	// printf("Function ke andar\n");
	if(lo > hi) {
		return ret;
		// printf("lo > hi case", "\n");
	}
	else {
		// printf("else case\n");
		mid = (lo + hi) / 2;
		if(a[mid].x < elm.x)
			return binary_search(mid + 1, hi, a, elm, ret);
		else if(a[mid].x > elm.x)
			return binary_search(lo, mid - 1, a, elm, ret);
		else {
			ret -> y = mid * 1.1;
			return ret;
		}
	}
}

int main() {
	struct pair a[10];
	struct pair elm;
	struct pair ret;
	struct pair * tmp;
	int i, j;
	for(i = 0 ; i < 10 - 1 ; i++) {
		a[i].x = i * i;
		a[i].y = 1;
	}

	printf("Here is the array\n");
	for(i = 0 ; i < 10 - 1 ; i++) {
		printf(a[i].x, " ", a[i].y , "\n");
	}
	elm.x = 36;
	elm.y = 1;
	
	tmp = binary_search(0, 10 - 1, a, elm, &ret);
	elm = *tmp;
	printf(elm.y, "\n");
}