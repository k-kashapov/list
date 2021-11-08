#include "List.h"
#include "Tests.h"

int main (int argc, const char **argv)
{
    List lst = {};
    ListInit (&lst, 5, 8, 1, 1, 1, 1, 1, 1, 1, 1);

    int mode = get_arg (argc, argv);
    if (mode == TEST_MODE)
    {
        Testing (&lst);
        ListDtor (&lst);
        return 0;
    }

    ListPushBack (&lst, 5);
    ListPushBack (&lst, 6);
    ListPushFront (&lst, 7);
    ListPushFront (&lst, 8);
    ListPushBack (&lst, 9);
    ListPushFront (&lst, 10);
    ListLinearize (&lst);

    ListDtor (&lst);

    return 0;
}
