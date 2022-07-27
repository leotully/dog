#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> /* for access */
#include <limits.h> /* for INT_MAX */
#include <getopt.h>
#include "getoptex.h"

#define BUF_SIZE 4097

char **loadfile(char *filename, int *count);
int intlen(unsigned n);
void showhelp(void);

const char *tryhelp = "try 'dog --help' for more information.";

int main (int argc, char **argv)
{
	int i;
	int c;
	int count;
	int int_len;
	int arg_c;
	char **arg_v;
	char **text_lines;
	int sort = 0;
	int reverse = 0;
	int show_count = 0;
	int show_line_num = 0;
	static int help;
	int start_number = 1;
	int end_number = INT_MAX;
	
	char *option_str = "ce:lrsn:";

	static struct option long_options[] = {
		{"help", no_argument, &help, 1},
		{0}
	};

/*
printf("%d\n", end_number);
return 0;
*/

	opterr = 0;

	while ((c = getopt_long(argc, argv, option_str, long_options, 0)) != -1)

	if (help) showhelp();

	optind = 0; /* reset */
	
	while ((c = getopt(argc, argv, option_str)) != -1)
	{
		switch (c)
		{
			case '?':
				printf("-%c not an option.\n", optopt);
				puts(tryhelp);
				return 1;
			case 'c':
				show_count = 1;
				break;
			case 'e':
				end_number = atoi(optarg);
				break;
			case 'l':
				show_line_num = 1;
				break;
			case 'r':
				reverse = 1;
				break;
			case 's':
				sort = 1;
				break;
			case 'n':
				start_number = atoi(optarg);
				break;
			default:
				break;
		}
	} 

	/* test -n is blank or less than 1 */
	if (start_number < 1)
	{
		printf("start line number (-n) needs to be 1 or greater.\n");
		puts(tryhelp);
		return 1;
	}

	if (end_number < 1)
	{
		printf("end line number (-e) needs to be 1 or greater.\n");
		puts(tryhelp);
		return 1;
	}

	if (end_number < start_number)
	{
		printf("end line number (-e) needs to be\n");
		printf("greater than start number (-n)\n");
		puts(tryhelp);
		return 1;
	}

	arg_v = get_args(argc, argv, &arg_c);

	if (arg_c != 2)
	{
		printf("error: filename required\n");
		puts(tryhelp);
		
		return 1;
	}

	if ((text_lines = loadfile(arg_v[1], &count)) == NULL) return 1;

	if (show_count)
	{
		printf("file '%s' contains %d lines\n", arg_v[1], count);
		return 0;
	}

	if (start_number > count)
	{
		printf("file '%s' only contains %d lines\n", arg_v[1], count);
		return 1;
	}

	int_len = intlen(count);
	while (start_number - 1 < count)
	{
		if (show_line_num)
			printf("%*d ", int_len, start_number);
		

		printf("%s\n", text_lines[start_number - 1]);

		if (start_number == end_number)
			break;

		start_number++;
	}

	return 0;
}

char **loadfile(char *filename, int *count)
{
	const int chunk = 100;
	int array_size = chunk;
	int line_count = 0;
	char **lines;
	char *s;
	char buffer[BUF_SIZE];
	FILE *fp;

	if (access(filename, 0) != 0)
	{
		fprintf(stderr, "file '%s' not found\n", filename);
		return NULL;
	}
	
	fp = fopen(filename, "r");

	if (fp == NULL)
	{
		fprintf(stderr, "can't open file '%s'\n", filename);
		return NULL;
	}
	
	lines = malloc(array_size * sizeof(char*));
	
	while (fgets(buffer, BUF_SIZE, fp) != NULL)
	{
		if (line_count == array_size)
		{
			char **newptr;
			
			array_size += chunk;			
			newptr = realloc(lines, array_size * sizeof(char*));

			if (newptr == NULL)
			{
				fprintf(stderr, "file too big to fit in memory\n");
				return NULL;
			}

			lines = newptr;
		}
		
		if(strchr(buffer, '\n') == NULL)
		{
			char discard;
			while((discard = fgetc(fp)) != '\n' && discard != EOF);
		}
		
		buffer[strcspn(buffer, "\n")] = '\0';
		s = malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s, buffer);
		lines[line_count] = s;

		line_count++;
	}

	*count = line_count;

	return lines;
}

int intlen(unsigned n)
{
	/* return floor(log10(abs(i))) + 1;	 */
    
	if (n >= 1000000000) return 10;
	if (n >= 100000000) return 9;
	if (n >= 10000000) return 8;
	if (n >= 1000000) return 7;
	if (n >= 100000) return 6;
	if (n >= 10000) return 5;
	if (n >= 1000) return 4;
	if (n >= 100) return 3;
	if (n >= 10) return 2;

    return 1;
}

void showhelp(void)
{
	char *msg = 	
"Usage: dog [options] file\n\
options:\n\
  -c                       show line count of file\n\
  -e <number>              end at given line number\n\
  -l                       print line numbers\n\
  -n <number>              start at given line number\n\
  -r                       reverse sort order (use with -s)\n\
  -s                       sort file\n\
  --help                   display this help message and exit\n\
";
/*
Concatenate FILE(s) to standard output.\n\
\n\
With no FILE, or when FILE is -, read standard input.\n\
\n\
  -A, --show-all           equivalent to -vET\n\
  -b, --number-nonblank    number nonempty output lines, overrides -n\n\
  -e                       equivalent to -vE\n\
  -E, --show-ends          display $ at end of each line\n\
  -n, --number             number all output lines\n\
  -s, --squeeze-blank      suppress repeated empty output lines\n\
  -t                       equivalent to -vT\n\
  -T, --show-tabs          display TAB characters as ^I\n\
  -u                       (ignored)\n\
  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB\n\
      --help     display this help and exit\n\
      --version  output version information and exit\n\
\n\
Examples:\n\
  dog f - g  Output f's contents, then standard input, then g's contents.\n\
  dog        Copy standard input to standard output.\n\
  ";
*/

	printf("%s", msg);
	exit(0);
}
