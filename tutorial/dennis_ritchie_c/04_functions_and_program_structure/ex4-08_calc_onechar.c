/* Cap.4 Functions and Program Structure, pag.79

   Exercise 4-8: Suppose that there will never be more than one character of
                 pushback. Modify getch and ungetch accordingly.
*/


#include<stdio.h>

char buf = 0;

int getch();
void ungetch(int c);

int main(void)
{
    int c;

    c = '*';

    ungetch(c);

    while ((c = getch()) != EOF)
        putchar(c);

    return 0;
}

/* getch: get a (possibly) pushed back character */
int getch(void)
{
    int c;

    if (buf != 0)
        c = buf;
    else
        c = getchar();

    buf = 0;
    return c;
}

/* ungetch: push a character back into input */
void ungetch(int c)
{
    if (buf != 0)
        printf("ungetch: too many characters\n");
    else
        buf = c;
}