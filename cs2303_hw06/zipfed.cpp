/** Functions supporting manipulation and management of zip code 
 *
 * @author Blake Nelson
 */

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>
#include "zipfed.hpp"

/** Default ctor for Zipfed. 
 *  Initialize members to default values.
 */
Zipfed::Zipfed () {
  zipcode = "";
  zctype = INVALID;
  city = "";
  state = "";
  lat = 0.0;
  lon = 0.0;
}

/** Method to parse a line of data as read and intialize object instance
 *
 * This method assumes the input file is a CSV format file of ZIP CODE data
 * as supplied by the federal government at:
 *
 *  http://federalgovernmentzipcodes.us/free-zipcode-database-Primary.csv
 * 
 * The function uses strtok library function to parse the file using commas as 
 * tokens.
 *
 * @param csv is a pointer to the comma separated value string to parse
 * @return zero (0) if success or non-zero on error.
 */
int Zipfed::parse_zip_federal (char *csv) {
  const char* delim = ","; // comma is the delimiter
  const int QUOTE = '\"';  // the quote character, we strip this from input.
  std::string token;       // This  will be the token we read using strtok
  int recnum = 0;          // Each record in CSV is sequentially numbered
  
  // verify we have a string to parse. Return if we don't
  if ((csv == NULL) || (strlen(csv) == 0)) {
    return -1;
  }

  /* The string should be delimited by commas (',') as the token separator.
   * We call the strtok to get the tokens between the delimiters and process 
   * accordingly for initializing the ZIP_FEDERAL struct
   *
   * NOTE: The very first line of the CSV file contains the column headers.
   * our stucture can't do anything with that, so object instance is not modified
   *
   * NOTE2: Each token is quoted in the input file. We will strip/remove them 
   */
  token = strtok(csv, delim);
  if (strcmp(token.c_str(), "\"RecordNumber\"") == 0) {
    // The string "RecordNumber" (with quotes) means this is the first line
    // of the input file containing column identification labels. We can't
    // handle that so just return an emtpy struct
    return -3;
  }
  // first column is the record num. We don't store this
  recnum = atoi(token.c_str());

  // second column is the 5-digit zip code as string.
  // strip the surrounding double quotes
  // If the zip code is < 5 digits, we prepend '0' to make it a legic zipcode
  token = strtok(NULL, delim);
  token.erase(std::remove(token.begin(), token.end(), QUOTE), token.end());
  while (token.size() < 5) {
    token.insert(0, 1, '0');
  }
  zipcode = token;

  // third column is zip code type as string, we convert to enum after
  // stripping the surrounding double quotes
  token = strtok(NULL, delim);
  token.erase(std::remove(token.begin(), token.end(), QUOTE), token.end());
  if (token.compare("STANDARD") == 0) {
    zctype = STANDARD;
  } else if (token.compare("PO_BOX") == 0) {
    zctype = PO_BOX;
  } else if (token.compare("UNIQUE") == 0) {
    zctype = UNIQUE;
  } else if (token.compare("MILITARY") == 0) {
    zctype = MILITARY;
  } else {
    zctype = INVALID;
  }

  // fourth column is the city
  token = strtok(NULL, delim);
  token.erase(std::remove(token.begin(), token.end(), QUOTE), token.end());
  city = token;

  // fifth column is the 2-character State code as string
  token = strtok(NULL, delim);
  token.erase(std::remove(token.begin(), token.end(), QUOTE), token.end());
  state = token;

  // sixth column is LocationType, we don't want that - don't process input
  token = strtok(NULL, delim);

  // seventh & eigth columns are Lat/Lon - these fields are not quoted
  token = strtok(NULL, delim);
  lat = std::stof(token);
  token = strtok(NULL, delim);
  lon = std::stof(token);

  // Columns 9-11 (Zaxis. Yaxis, Zaxis) we don't care about
  token = strtok (NULL, delim);
  token = strtok (NULL, delim);
  token = strtok (NULL, delim);

  return 0;
}

//New Parse Function




/** Method to parse a line of data as read and intialize object instance
 *
 * This method assumes the input file is a CSV format file of ZIP CODE data
 * as supplied by the previous function fed2cs2303
 *
 * 
 * The function uses strtok library function to parse the file using commas as 
 * tokens.
 *
 * @param csv is a pointer to the comma separated value string to parse
 * @return zero (0) if success or non-zero on error.
 */




int Zipfed::parse_zip (char *csv) {
  const char* delim = ","; // comma is the delimiter
  const int QUOTE = '\"';  // the quote character, we strip this from input.
  std::string token;       // This  will be the token we read using strtok
  int recnum = 0;          // Each record in CSV is sequentially numbered
  
  // verify we have a string to parse. Return if we don't
  if ((csv == NULL) || (strlen(csv) == 0)) {
    return -1;
  }
  /* The string should be delimited by commas (',') as the token separator.
   * We call the strtok to get the tokens between the delimiters and process 
   * accordingly for initializing the ZIP_FEDERAL struct
   */
  token = strtok(csv, delim);
  zipcode = token;
  token = strtok(NULL, delim);
  if (token.compare("STANDARD") == 0) {
    zctype = STANDARD;
  } else if (token.compare("PO_BOX") == 0) {
    zctype = PO_BOX;
  } else if (token.compare("UNIQUE") == 0) {
    zctype = UNIQUE;
  } else if (token.compare("MILITARY") == 0) {
    zctype = MILITARY;
  } else {
    zctype = INVALID;
  }

  // fourth column is the city
  token = strtok(NULL, delim);
  city = token;
  // fifth column is the 2-character State code as string
  token = strtok(NULL, delim);
  state = token;
  token = strtok(NULL, delim);
  

  lat = std::stof(token);
  token = strtok(NULL, delim);
  lon = std::stof(token);



  return 0;
}


/** Function to print zipcode structure to stdout
 *
 * @param pzip is a pointer to ZIP_FEDERAL struct
 */
void Zipfed::print (FILE* output) {
  std::string isMass("MA");
  if(isMass == state.c_str()){
  printf ("%s,", zipcode.c_str());
  fprintf(output, "%s,",zipcode.c_str()); //outputing zipcode
  switch (zctype) {
  case STANDARD:
    printf("STANDARD,");
    fprintf(output, "STANDARD,");
    break;
  case PO_BOX:
    printf("PO_BOX,");
    fprintf(output, "PO_BOX,");
    break;
  case UNIQUE:
    printf("UNIQUE,");
    fprintf(output, "UNIQUE,");
    break;
  case MILITARY:
    printf("MILITARY,");
    fprintf(output, "MILITARY,");
    break;
  case INVALID:
    printf("INVALID,");
    fprintf(output, "INVALID,");
    break;
  default:
    printf(",");
    fprintf(output, ",");
    break;
  }
  printf("%s,%s,", city.c_str(), state.c_str());
  fprintf(output, "%s,%s,", city.c_str(), state.c_str());
  printf("%f,%f\n", lat, lon);
  fprintf(output, "%f,%f\n", lat, lon);
  
  }
  return;
}

std::string Zipfed::getCity(){
	return city;
}

std::string Zipfed::getZipcode(){
	return zipcode;
}

