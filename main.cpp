#include "List.h"

int main (int argc, const char **argv)
{
  List lst = {};

  printf ("main1 = %u\n", MurmurHash (&lst, sizeof (List)));
  printf ("main2 = %u\n", MurmurHash (&lst, sizeof (List) - 3 * sizeof (int)));

  LstInit (&lst, 10);

  printf ("main3 = %u\n", sizeof (List));
  printf ("main4 = %u\n", sizeof (List) - 3 * sizeof (int));

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
