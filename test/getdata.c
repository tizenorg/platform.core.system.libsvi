#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <glib.h>

#define DBG(fmt, argc...)       printf("<%s:%d> "fmt"\n", __func__, __LINE__, ##argc)
#define ERR(fmt, argc...)       printf("<%s:%d> [ERROR] "fmt"\n", __func__, __LINE__, ##argc)

#define BUF_SIZE		1024

static int print_content(const char *filename)
{
	FILE *fp;
	unsigned char buf[BUF_SIZE];
	char *b64_de, *b64_en;
	unsigned int n;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		ERR("fopen fail");
		return -1;
	}

	while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
		printf("[[[");
		b64_en = g_base64_encode(buf, n);
		fwrite(b64_en, 1, n, stdout);
		printf("]]]\n(((");
		b64_de = (char*)g_base64_decode(b64_en, &n);
		fwrite(b64_de, 1, n, stdout);
		printf(")))\n");

		free(b64_en);
		free(b64_de);
	}

	fclose(fp);
	printf("\n");
	return 0;

}

static void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL) {
		ERR("opendir fail");
		return;
	}

	chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {
			if (strcmp(".", entry->d_name) == 0 ||
				strcmp("..", entry->d_name) == 0)
				continue;

			DBG("%*s%s", depth, "", entry->d_name);
			printdir(entry->d_name, depth+strlen(entry->d_name));
		} else {

			DBG("%*s%s", depth, "", entry->d_name);
			print_content(entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}

int main(int argc, char *argv[])
{
	printdir(argv[1], 0);
	DBG("done");
	return 0;
}
