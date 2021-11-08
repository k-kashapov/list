#include "Tests.h"
#include <string.h>

const int TEST_FAILED = -42;

static int RunTest (List *lst, int checks_num, ...)
{
    va_list checks;
    va_start (checks, checks_num);

    printf ("Starting a test...\n");

    for (long check = 1; check <= checks_num; check++)
    {
        type_t key = va_arg (checks, type_t);
        if (lst->nodes[check].data != key)
        {
            printf ("\nTEST FAILED: expected %ld at %ld, got %ld\n",
                    key, check, lst->nodes[check].data);
        }
    }

    va_end (checks);

    ListDtor (lst);
    *lst = {};
    ListInit (lst, 3);

    printf ("Test complete\n");

    return OK;
}

int Testing ()
{
    List lst = {};
    ListInit (&lst, 3);

    printf ("Running in test mode\n");

    int err = 0;

    ListPushBack (&lst, 5);
    ListPushBack (&lst, 6);
    ListPushBack (&lst, 7);

    err = RunTest (&lst, 3, 5, 6, 7);


    ListPushBack (&lst, 9);
    long pos8 = ListPushFront (&lst, 8);
    ListPushBack (&lst, 7);
    ListInsertPhys (&lst, 6, pos8);
    ListPopFront (&lst);

    err = RunTest (&lst, 4, 9, 0, 7, 6);

    printf ("Tests finished\n");

    if (err) return err;

    return OK;
}

int get_arg (int argc, const char **argv)
{
    for (; argc > 1; argc--)
    {
        if (!strncmp (*++argv, "-t", 3))
            return TEST_MODE;
    }

    return NORM_MODE;
}
