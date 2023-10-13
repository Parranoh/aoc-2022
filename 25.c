#include <stdio.h>
#include <stdlib.h>

long dec2snafu(void)
{
    long sum = 0, n = 0;
    int c;
    while ((c = getchar()) != EOF)
        switch(c)
        {
            case '=': n = 5 * n - 2; break;
            case '-': n = 5 * n - 1; break;
            case '0': n = 5 * n;     break;
            case '1': n = 5 * n + 1; break;
            case '2': n = 5 * n + 2; break;
            case '\n':
                sum += n;
                n = 0;
                break;
            default:
                exit(1);
        }
    return sum;
}

void snafu2dec(long n)
{
    if (n == 0)
        return;
    char c; long d = n % 5;
    switch (d)
    {
        case 0: c = '0'; break;
        case 1: c = '1'; break;
        case 2: c = '2'; break;
        case 3: c = '='; d = -2; break;
        case 4: c = '-'; d = -1; break;
        default: exit(1);
    }
    snafu2dec((n - d) / 5);
    putchar(c);
}

int main(void)
{
    snafu2dec(dec2snafu());
    putchar('\n');
    return 0;
}
