#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char temp[16] = "";

    for(int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0)
            strcat(temp, "Fizz");
        
        if (i % 5 == 0)
            strcat(temp, "Buzz");

        if (i % 3 != 0 && i % 5 != 0)
            printf("%d", i);
        
        printf("%s ", temp);
        memset(temp, 0, strlen(temp));
    }

    return 0;
}