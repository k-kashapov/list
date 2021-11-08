#include "List.h"
#include "Tests.h"

int main (int argc, const char **argv)
{
    int mode = get_arg (argc, argv);
    if (mode == TEST_MODE)
    {
        Testing ();
        return 0;
    }

    List lst = {};
    ListInit (&lst, 5, 3, 1, 2, 8);

    ListPushBack (&lst, 5);
    ListPushBack (&lst, 6);
    ListPushFront (&lst, 7);
    ListPushFront (&lst, 8);
    ListPushBack (&lst, 9);
    ListPopPhys (&lst, 2);
    ListPushFront (&lst, 10);

    ListDtor (&lst);

    return 0;
}
