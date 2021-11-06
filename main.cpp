#include "List.h"

int main (int argc, const char **argv)
{
  List lst = {};

  ListInit (&lst, 10);

  ListPushBack (&lst, 9);
  ListPushBack (&lst, 10);
  ListPushFront (&lst, 11);
  ListInsert (&lst, 2, 2);
  printf ("%ld\n", ListPop (&lst, 2));
  printf ("%ld\n", ListPopFront (&lst));
  printf ("%ld\n", ListPopBack (&lst));


  ListDtor (&lst);
  return 0;
}
