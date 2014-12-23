#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/stat.h>
#include<sys/types.h>


int rotations = 0;

int rot13cmp(const void *a, const void *b)
{
  //Loops through the array and updates each charater value according to the
  //appropiate decoding using ASCII
    rotations +=1;
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
    
    int sizeOfBuffer = 1000; //realloc
    int count = 0;          //size of array
    char *charArray;        //ptr to chars read in
    char charRead;          // char read with sys call
    
    int allocatedsize;
    
    //FILE META
    struct stat fileMeta;   //loads file info
    fstat(0, &fileMeta);
    int fsize = fileMeta.st_size;
    
    //if regular file
    if(S_ISREG(fileMeta.st_mode))
    {
        if (read(0, &charRead, 1)==0) //file is empty
        {
            fprintf(stderr, "Number of comparisons: %d\n", 0);
            return 0;
        }
        
        //else file is not empty, allocate memory to hold whole file
        charArray = (char*) malloc(sizeof(char) * fsize);
        allocatedsize = fsize;
        
        //check if memory allocation was successful
        if (charArray == NULL)
        {
            fprintf(stderr, "There was an error allocating dynamic memory.");
            exit(1);
        }
        
        int k=0;
        while(k < fsize)  //keep reading until file is done
        {
            
            read(0, &charRead, 1);
            charArray[count++] = charRead;
            
            //Reallocate more memory if full
            if (count == allocatedsize)
            {
                charArray = (char*) realloc(charArray, allocatedsize + sizeOfBuffer);
                
                if (charArray == NULL)
                {
                    fprintf(stderr, "There was an error re-allocating memory");
                    exit(1);
                }
                allocatedsize += sizeOfBuffer;
            }
            
            fstat(0, &fileMeta); //check if file has increased in size
            fsize = fileMeta.st_size;
            k++;
        }
        
    }

    else //not a regular file (i.e. just simple txt input from terminal
    {
        if (read(0, &charRead, 1)==0) //input is empty
        {
            fprintf(stderr, "Number of comparisons: %d\n", 0);
            return 0;
        }
        
        charArray = (char*) malloc(sizeof(char) * sizeOfBuffer);
        //check if memory allocation was successful
        if (charArray == NULL)
        {
            fprintf(stderr, "There was an error allocating dynamic memory.");
            exit(1);
        }
        
        charArray[count++] = charRead;
        
        while (read(0,&charRead,1)>0)
        {
            charArray[count++] = charRead;
            
            
            //Reallocate more memory if full
            if (count == sizeOfBuffer)
            {
                charArray = (char*) realloc(charArray, sizeOfBuffer*2);
                
                if (charArray == NULL)
                {
                    fprintf(stderr, "There was an error re-allocating memory");
                    exit(1);
                }
                sizeOfBuffer = sizeOfBuffer*2;
            }
        }
        
    }

    
    
    
    //Append newline if no newline at end of file
    if (charArray[count-1] != '\n')
        charArray[count++] = '\n';



    int wordCount = 0;
    for (int i = 0; i < count; i++)
    {
      if (charArray[i] == '\n')  //count how many newlines are there
          wordCount++;
    }

    //Create an tmpiliary array that points to the charArray created above with the
    //corresponding spacing by newlines

    char **tmp = (char**) malloc(sizeof(char*) * wordCount);
    int *lengths = (int*) malloc(sizeof(int) * wordCount);
    
    if (tmp == NULL | lengths == NULL)
    {
        fprintf(stderr, "Error allocating dynamic memory.");
        exit(1);
    }

    //Locate each word based on newlines, and assign a pointer to the first letter
    int tmpindex = 0;
    char *ptrTmp = charArray;
    for (int k = 0; k < wordCount; k++)
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

    //Qsort this tmp array
    qsort(tmp, tmpindex, sizeof(char*), rot13cmp);



  //Calculate lengths for printing
    int lindex = 0;
    int length = 1;
    int k;
    for (k = 0; k < wordCount; k++)
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
        
    for (k = 0; k < wordCount; k++)
    {
        char *c = tmp[k];
        for (int i = 0; i < lengths[k]; i++)
        {
            write(1, &(*c),1);
            c++;
        }
    }

    //free allocated memory
    free(charArray);
    free(tmp);
    free(lengths);

    fprintf(stderr, "Number of comparisons: %d\n", rotations);
    
    return 0;
}
