/*  double omp_get_wtick(void);

    Gets the timer precision, i.e., the number of seconds between two successive
    clock ticks.
*/

#include <stdio.h>
#include <omp.h>

#ifdef WIN32
#include <windows.h>
#define sleep(a) Sleep(a*1000)
#else //linux
#include <unistd.h>
#endif //WIN32

int main(int argc, char** argv)
{
    double start = omp_get_wtime ();
    sleep (1);
    double end = omp_get_wtime ();
    double wtick = omp_get_wtick ();

    printf ("start = %.16g\n"
            "end   = %.16g\n"
            "diff  = %.16g\n",
            start, end, end - start);
    printf ("wtick = %.16g\n"
            "1/wtick = %.16g\n",
            wtick, 1.0/wtick);

    return 0;
}

/*
$> ./10_omp_get_wtime
start = 31352.738506449
end   = 31353.738726453
diff  = 1.000220003999857
wtick = 1e-09
1/wtick = 999999999.9999999
*/
