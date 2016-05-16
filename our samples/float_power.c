float power(float a, int b) {
	float tmp;
	if(b == 0) {
		// printf("Here\n");
		return 1;
	}
	else {
		// printf("Here1\n");
		tmp = power(a, b - 1);

		return a * tmp;
	}
}

int main() {
	float a;
	int b;
	a = 2.4;
	b = 9;
	// printf("I am here\n");
	printf(power(a, b), "\n");
}