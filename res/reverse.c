#include <stdio.h>

int main(void)
{
	char in_f[128], out_f[128];
	FILE *in, *out;
	char ch;

	printf("\nInput filename:   ");
	scanf("%s", in_f);
	printf("Output filename:  ");
	scanf("%s", out_f);

	in = fopen(in_f, "rb");
	out = fopen(out_f, "wb");
	if (!in) {
		printf("\nFile %s open error!\n", in_f);
		return -1;
	}
	if (!out) {
		printf("\nFile %s open error!\n", out_f);
		return -1;
	}

	printf("\nProcessing...\n");
	while (!feof(in)) {
		ch = fgetc(in);
		ch = ~ch;
		fputc(ch, out);
	}
	printf("Done!\n");

out:
	fclose(in);
	fclose(out);
	return 0;
}