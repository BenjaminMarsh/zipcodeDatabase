#ifdef ZIPFILTER_H
#define ZIPFILTER_H

#include <strings.h>
#include "zipfed.hpp"

ssize_t readln_cs2303(char **line_pointer, size_t *n, FILE *stream);
bool comparator_function(Zipfed* pZip1,Zipfed* pZip2);
bool comparatorstring_function(const char* str1, const char* str2);

#endif
