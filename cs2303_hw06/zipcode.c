/** Program demonstrating parsing and linked lists. This program 
 * supports zip code lookup for any town in Massachusetts
 *
 * @author Blake Nelson
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <forward_list>
#include "zipfed.hpp" // include header file for Zipfed class
#include "zipfilter.h" // include header file for comparator functions

#define FILENAME (1488)

//Functions


/** Function to read a line from the input file and return pointer to the 
 * line of data represented as a C-String (i.e. NULL terminated string)
 *
 * @lineptr is a pointer to the dynamically allocated buffer to fill.
 *   If the pointer is NULL, a buffer will be allocated to hold the line.
 *   If the buffer is too small to hold the line, realloc will be called
 *   to allocate a larger buffer. If the buffer is reallocated, n will
 *   be set to new size.
 * @n is a pointer to the size of the buffer pointer to be lineptr. If
 *   memory is reallocated for lineptr, the size of n will be incremented
 *   accordingly. The parameter n will always specify the size of buffer
 *   pointed at by lineptr.
 * @fptr is the FILE pointer to the open file to read.
 * @return number bytes (chars) read or -1 on error
 */



ssize_t readln_cs2303(char **line_pointer, size_t *n, FILE *stream) 
{
  ssize_t bytes_read = -1;  // -1 will signify error reading line
  const char *delim = "\r\n";   // each line terminated with \r\n

  // Verify the file is open
  if (stream == NULL) 
  {
    return -1;
  }

  // Read a line from the file, including the delimiter
  bytes_read = getline(line_pointer, n, stream);

  // Check if the line was read successfully
  if (bytes_read < 0) 
  {
    return bytes_read;
  }

  // Remove the delimiter from the end of the line
  int len = strlen(*line_pointer);
  if (len >= 2 && strcmp(*line_pointer + len - 2, delim) == 0)
  {
    (*line_pointer)[len - 2] = '\0';
  }

  return bytes_read;
}


/** Function to compare two Zipfeds city variable
 *
 * @param pZip1 and pZip2 are Zipfeds 
 * @return a boolean based on whether the two city variables are equal
 */

bool comparator_function(Zipfed* pZip1,Zipfed* pZip2)
{
   if(strcmp(pZip1->getCity().c_str(),pZip2->getCity().c_str())<=0)
     return true;
   else
     return false;
}



/** Function to compare two strings that is case insensitive
 *
 * @param str1 and str2 are pointers to a strings
 * @return a boolean based on whether the two characters are equal
 */

bool comparatorstring_function(const char* str1, const char* str2)
{
const size_t max_len = 30;
char s1[max_len + 1], s2[max_len + 1];
for (size_t i = 0; i < max_len; i++) {
    if (str1[i] == '\0') {
        s1[i] = '\0';
        break;
    }
    s1[i] = toupper(str1[i]);
}

for (size_t i = 0; i < max_len; i++) {
    if (str2[i] == '\0') {
        s2[i] = '\0';
        break;
    }
    s2[i] = toupper(str2[i]);
}

return strcmp(s1, s2) == 0;
  }




//Main Function




int main (int argc, char *argv[]) {

//Checks if theres only one arguement
  if(argc != 2)
  {
    fprintf(stderr, "Input file missing!\n");
    return 1;
  }

  //Copy the input file name into the array
  char input_file[FILENAME];
  strncpy(input_file, argv[1], FILENAME -1);

  //Opens the file for reading mode and returns error if fails
  FILE* file = fopen(input_file, "r");
  if(file == NULL)
  {
    fprintf(stderr, "Cannot Open File\n");
    return 1;
  }

  //Variables and Lists
  ssize_t characters_read;
  char* input_buffer = NULL;
  size_t input_buffer_size = 0;
  std::forward_list<char*> mainlist;
  std::forward_list<char*>::iterator iter;
  std::forward_list<char*>::iterator iter_end;

   for (iter = mainlist.begin(), iter_end = mainlist.end(); iter != iter_end; ++iter)
    {
    free(*iter);
    }

  std::forward_list<Zipfed*> llist;
  std::forward_list<Zipfed*>::iterator it;
  std::forward_list<Zipfed*>::iterator it_end;

    for (it = llist.begin(), it_end = llist.end(); it != it_end; ++it) 
    {
    free(*it);
    }

  //Reads lines from the input file, parses, and then adds it to the list
  while((characters_read = readln_cs2303(&input_buffer, &input_buffer_size, file)) != EOF)
    {
      Zipfed* pZipfed = (Zipfed*) malloc(sizeof(Zipfed));
      if(pZipfed == NULL)
      {
        fprintf(stderr, "Failed to Allocate Memory\n");
        fclose(file);
        return 1;
      }
      if(pZipfed -> parse_zip(input_buffer) != 0)
      {
        fprintf(stderr, "Failed to Process Input Record\n");
        fclose(file);
        return 1;
      }
      llist.push_front(pZipfed);
    }

  //Sorts list of zipcodes 
  llist.sort(comparator_function);


  printf("Input City/Town Names.\nUse Ctrl+D When Done\n");

  //Reads the city names entered by user and adds it to list

  
  char * inputstring;
  size_t len = 0;
  
    while(getline(&inputstring, &len, stdin) != -1){
    	inputstring[strlen(inputstring)-1] = '\0';
  	char* new_city_town = (char*)malloc(strlen(inputstring)+1); // allocates exact space
	strcpy(new_city_town, inputstring);
  	if (new_city_town == NULL) {
        	fprintf(stderr, "Failed to Allocate Memory \n");
        	fclose(file);
        	return 1;
	}
    	mainlist.push_front(new_city_town);
    }
  
  
  
  
  
  

  bool flag;
  for (iter = mainlist.begin(), iter_end = mainlist.end(); iter != iter_end; ++iter) 
  {
      flag = false;
      printf("%s :", *iter);
  for (it = llist.begin(), it_end = llist.end(); it != it_end; ++it)        {
      Zipfed* pTmpZipfed = *it;
      
      if (comparatorstring_function(pTmpZipfed->getCity().c_str(), *iter)) {
        flag = true;
        printf("%s ", pTmpZipfed->getZipcode().c_str());
      }
    }

if (flag)
{
  printf("\n");
}      
else
printf(" No Zip Code Found\n");
}

for (iter = mainlist.begin(), iter_end = mainlist.end(); iter != iter_end; ++iter) {
free(*iter);
}

for (it = llist.begin(), it_end = llist.end(); it != it_end; ++it) {
free(*it);
}

fclose(file);
return 0;
}

