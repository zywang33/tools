#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct firmware_header {
	unsigned int size_bytes; /* size of the entire header+image(s) in bytes */
	unsigned int header_size_bytes; /* size of just the header in bytes */
	unsigned int iram_size_bytes; /* size of iram */
	unsigned int dram_size_bytes; /* size of dram */
};

int pack_fw(FILE *iram_in, FILE *dram_in, FILE *fp_out)
{
	unsigned int reg;
	struct firmware_header header;
	int readlen = 0;

	fwrite(&header, sizeof(header), 1, fp_out);
	while (fscanf(iram_in, "%x\n", &reg) > 0)
	{
		readlen += 4;
		fwrite(&reg, 4, 1, fp_out);
	}

	printf("iram length = %x\n", readlen);
	header.iram_size_bytes = readlen;
	readlen = 0;

	while (fscanf(dram_in, "%x\n", &reg) > 0)
	{
		readlen += 4;
		fwrite(&reg, 4, 1, fp_out);
	}

	printf("dram length = %x\n", readlen);
	header.dram_size_bytes = readlen;

	header.header_size_bytes = sizeof(header);
	header.size_bytes = header.header_size_bytes + header.iram_size_bytes +
			    header.dram_size_bytes;

	fseek(fp_out, 0, SEEK_SET);
	fwrite(&header, sizeof(header), 1, fp_out);

	return 0;
}

int main(int argc, char **argv)
{
	FILE *dram_in, *iram_in, *fp_out;

	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <iram.data> <dram.data> <output>\n", argv[0]);
		return 1;
	}

	iram_in = fopen(argv[1], "rb");
	if (!iram_in)
	{
		fprintf(stderr, "can't open input file '%s': %s\n", argv[1], strerror(errno));
		return 1;
	}

	dram_in = fopen(argv[2], "rb");
	if (!dram_in)
	{
		fprintf(stderr, "can't open input file '%s': %s\n", argv[2], strerror(errno));
		return 1;
	}

	fp_out = fopen(argv[3], "wb");
	if (!fp_out)
	{
		fprintf(stderr, "can't open output file '%s': %s\n", argv[3], strerror(errno));
		return 1;
	}

	pack_fw(iram_in, dram_in, fp_out);

	return 0;
}
