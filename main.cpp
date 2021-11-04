#include "List.h"

int main (int argc, const char **argv)
{
  List lst = {};

  LstInit (&lst, 10);

  ListInsert (&lst, 5);
  ListInsert (&lst, 8);
  ListPushBack (&lst, 9);
  ListPushBack (&lst, 10);
  ListPushFront (&lst, 11);
  ListPushBack (&lst, 12);

  LstDtor (&lst);
  return 0;
}
