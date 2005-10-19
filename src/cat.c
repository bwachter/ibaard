#include <stdarg.h>
#include <string.h>

#ifdef __WIN32__
#include <windows.h>
#include <io.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif 

#include "ibaard_cat.h"

int cat(char **dest, char *str, ...){
	va_list ap;
	int len;
	char *ptr, *tmp;
	
	if (*dest != NULL)
		free(*dest);
	
	len = strlen(str);
	va_start(ap, str);
	while ((tmp = va_arg(ap, char*)))
		len += strlen(tmp);
	va_end(ap);

	if (!(*dest=malloc(len+1)))
		return -1;

	ptr = *dest;
	for (tmp=str; *tmp; tmp++) *ptr++ = *tmp;
	va_start(ap, str);
	while ((tmp = va_arg(ap, char*)))
		while (*tmp) *ptr++ = *tmp++;
	va_end(ap);
	*ptr = '\0';
	return 0;
}


