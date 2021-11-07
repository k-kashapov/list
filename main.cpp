#include "List.h"

int main ()
{
    List lst = {};

    ListInit (&lst, 3);

    ListPushBack (&lst, 10);
    ListPushBack (&lst, 20);
    ListPushFront (&lst, 30);
    ListPushFront (&lst, 40);
    ListPushBack (&lst, 50);

    ListLinearize (&lst);

    ListDtor (&lst);
    return 0;
}
