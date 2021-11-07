#include "List.h"
#include "Tests.h"

int main (int argc, const char **argv)
{
    List lst = {};
    ListInit (&lst, 3);

    int mode = get_arg (argc, argv);
    if (mode == TEST_MODE)
    {
        Testing (&lst);
        ListDtor (&lst);
        return 0;
    }

    ListPushBack (&lst, 5);
    ListPushBack (&lst, 6);
    ListPushBack (&lst, 7);

    ListDtor (&lst);

    return 0;
}
