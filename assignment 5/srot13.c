#include <stdio.h>
#include <stdlib.h>

int rot13cmp(const void *a, const void *b)
{
  //Loops through the array and updates each charater value according to the
  //appropiate decoding usind ASCII
  const char *first = *(const char **)a;
  const char *second = *(const char **)b;

  while (1)
    {

      int firstNum = (int) *first;
      int secondNum = (int) *second;

      //Decode ROT13

      //Base cases
      //10 == \n (newline char in ASCII)
      if (firstNum == 10 && secondNum == 10)
	return 0;
      else if (firstNum == 10)
	return -1;
      else if (secondNum == 10)
	return 1;


      //Move 13 positions based on numeric value
      if ((firstNum > 64 && firstNum < 78) || (firstNum > 96 && firstNum < 110))
	firstNum += 13;
      else if ((firstNum > 77 && firstNum < 91) || (firstNum > 109 && firstNum < 123))
	firstNum -= 13;

      if ((secondNum > 64 && secondNum < 78) || (secondNum > 96 && secondNum < 110))
	secondNum += 13;
      else if ((secondNum > 77 && secondNum < 91) || (secondNum > 109 && secondNum < 123))
	secondNum -= 13;


      int diff = firstNum - secondNum;
      if (diff != 0)
	return diff;
      first++;
      second++;
    }
}

int main()
{
  //allocate an array to store the input
  int sizeOfBuffer = 2048;
  int count = 0;
  char *charArray = (char*) malloc(sizeof(char) * sizeOfBuffer);

  if (charArray == NULL){
    fprintf(stderr, "There was an error allocating dynamic memory.");
    exit(1);
  }

  int letter;
  int emptyFile = 1;
  while(1)
    {
      letter = getchar();
      if (letter == EOF)
	break;
      emptyFile = 0;
      charArray[count++] = (char) letter;

      //Reallocate more memory if full
      if (count == sizeOfBuffer)
	{
	  charArray = (char*) realloc(charArray, sizeOfBuffer * 2);

	  if (charArray == NULL)
	    {
	      fprintf(stderr, "There was an error re-allocating memory");
	      exit(1);
	    }
	  sizeOfBuffer *= 2;
	}
    }

  //If an empty file was passed as argument
  if (emptyFile == 1)
    return 0;

  //Append newline if no newline at end of file
  if (charArray[count-1] != '\n')
    charArray[count++] = '\n';



  int newlinesCount = 0;
  for (int i = 0; i < count; i++)
    {
      if (charArray[i] == '\n')  //count how many newlines are there
	newlinesCount++;
    }

  //Create an tmpiliary array that points to the charArray created above with the
  //corresponding spacing by newlines

  char **tmp = (char**) malloc(sizeof(char*) * newlinesCount);
  int *lengths = (int*) malloc(sizeof(int) * newlinesCount);
  if (tmp == NULL | lengths == NULL)
    {
      fprintf(stderr, "Error allocating dynamic memory.");
      exit(1);
    }

  //Locate each word based on newlines, and assign a pointer to the first letter
  int tmpindex = 0;
  char *ptrTmp = charArray;
  for (int k = 0; k < newlinesCount; k++)
    {
      if (k == 0 && *ptrTmp == '\n')
	{
	  tmp[tmpindex++] = ptrTmp;
	  k++;
	  ptrTmp++;
	}
      tmp[tmpindex++] = ptrTmp;

      while (*ptrTmp != '\n')
	ptrTmp++;

      ptrTmp++;
    }

  //Qsort this tmpiliary array
  qsort(tmp, tmpindex, sizeof(char*), rot13cmp);



  //Calculate lengths for printing
  int lindex = 0;
  int length = 1;
  for (int k = 0; k < newlinesCount; k++)
    {
      ptrTmp = tmp[k];
      while (*ptrTmp != '\n')
	{
	  ptrTmp++;
	  length++;
	}
      lengths[lindex++] = length;
      length = 1;
    }

  //Sorted array is ready to be printed
  for (int k = 0; k < newlinesCount; k++)
    {
      char *printable = tmp[k];
      for (int i = 0; i < lengths[k]; i++)
	{
	  printf("%c", *printable);
	  printable++;
	}
    }

  //free allocated memory
  free(charArray);
  free(tmp);
  free(lengths);

  return 0;
}
