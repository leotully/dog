#ifndef _GETOPTEX_H_
#define _GETOPTEX_H_

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

char **get_args(int argc, char **argv, int *arg_count)
{
	char **args;
	char **newptr;
	char *s;
	int i;
	int count = 1;

	args = malloc(sizeof(char*));
	s = malloc(1);
	args[0] = '\0';
	free(s);

	while (optind < argc)
	{
		i = optind++;
		newptr = realloc(args, (count + 1) * sizeof(char*));

		if (newptr != NULL)
		{
			args = newptr;
		}
		else
		{
			*arg_count = 0;
			return NULL;
		}
		
		s = malloc(strlen(argv[i]) + 1);
		strcpy(s, argv[i]);
		args[count] = s;
		count++;
	}

	*arg_count = count;

	return args;
}

void free_args(int argc, char **argv)
{
	int i;
	for (i = 0; i < argc; i++)
		free(argv[i]);
	
	free(argv);
}

#endif /* _GETOPTEX_H_ */

