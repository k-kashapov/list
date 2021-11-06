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
  ListPushFront (&lst, 13);
  ListPushFront (&lst, 14);
  ListPushFront (&lst, 15);
  ListPushFront (&lst, 16);
  ListPushFront (&lst, 17);
  ListPushFront (&lst, 18);

  LstDtor (&lst);
  return 0;
}
