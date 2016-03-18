/*
   This file is NOT BUILT by default.  Together with addon.h, it
   provides an example of how to add new builtins to rc.
*/

#include "rc.h"
#include "addon.h"

#ifdef	DEMO_ADDON
void b_sum(char **av) {
	long sum = 0;

	while (*++av)
		sum += atol(*av);
	fprint(1, "%ld\n", sum);
	set(TRUE);
}

void b_prod(char **av) {
	long sum = 1;

	while (*++av)
		sum *= atol(*av);
	fprint(1, "%ld\n", sum);
	set(TRUE);
}
#endif

/* From code written by Alan Watson <alan@oldp.astro.wisc.edu>. */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#define	SIZE_T	size_t

/*
 b_reads -- read a single line, terminated by \n or EOF, from the standard
 input, and assign the line without the terminator to av[1].
 */

static int
readchar (int fd)
{
   unsigned char c;

   if (read (fd, &c, 1) == 1)
      return c;
   else
      return EOF;
}

void
b_read (char **av)
{
   char *name;
   int c;
   char *line;
   SIZE_T len;
   SIZE_T max_len;
   SIZE_T max_len_quantum;

   /* check usage is "read name" */
   if (av[1] == NULL)
      rc_error ("missing variable name");
   if (av[2] != NULL)
      rc_error ("too many arguments to read");
   name = av[1];

   /* read a single line from stdin */
   line = NULL;
   len = 0;
   max_len = 0;
   max_len_quantum = 256;
   do {
      c = readchar (0);
      if (len == 0 && c == EOF)
	 break;
      if (c == '\n' || c == EOF)
	 c = 0;
      if (len == max_len) {
	 max_len += max_len_quantum;
	 line = erealloc (line, max_len);
      }
      line[len] = c;
      len++;
   } while (c);

   /* if we successfully read a line assign it to name and return TRUE,
    * otherwise unset name and return FALSE */
   if (line != NULL) {
      assign (word (name, NULL), word (line, NULL), FALSE);
      efree (line);
      set (TRUE);
   } else {
      assign (word (name, NULL), NULL, FALSE);
      set (FALSE);
   }

   return;
}
