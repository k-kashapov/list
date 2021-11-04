#include "List.h"

int main (int argc, const char **argv)
{
  List lst = {};

  LstInit (&lst, 10);

  ListInsert (&lst, 5);
  ListInsert (&lst, 8);
  ListPushBack (&lst, 9);
  long jojo = ListPushBack (&lst, 10);
  ListPushFront (&lst, 11);
  ListPushFront (&lst, 12);

  int pop_err = 0;
  type_t res = ListPopFront (&lst, &pop_err);
  printf ("%ld\n", res);

  printf ("%ld\n", ListPop (&lst, jojo, &pop_err));

  LstDtor (&lst);
  return 0;
}
