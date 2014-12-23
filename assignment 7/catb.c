#include <stdio.h>

int main()
{
  int ch;
  do
    {
      ch = getchar();
      if (ch != EOF)
	putchar(ch);

    } while (ch != EOF);
  return 0;
}
