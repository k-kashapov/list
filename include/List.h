#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_LOGS
#define HASH_PROTECTION
#define MAX_INFO

typedef int64_t type_t;
#define _type_name "int64_t"

const long LIST_INIT_CAP = 2;

enum EXIT_CODES
{
  OK              =  0,
  MEM_ALLOC_ERR   = -1,
  INVALID_INS_ARG = -2,
  OPEN_FILE_FAIL  = -3,
  POP_FIND_ERR    = -4,
};

#ifdef LIST_LOGS
  #define LOG_PRINT(string) fprintf (Log_file, string)
  #define HLINE(width, height) "<div><style scoped>\
                                  hr#w" #width "\
                                  {\
                                    display: inline-block;\
                                    position: relative;\
                                    margin-top: -"#height"px;\
                                    margin-left: -240px;\
                                    border-style: inset;\
                                    border-width: 1px;\
                                    width: " #width "px;\
                                  }\
                                </style>\
                                <hr id = \"w" #width "\"></div>"

  #define POINTER(width) "<div style=\"\
                              display: block;\
                              position: relative;\
                              border-right: 2px solid black;\
                              margin-top: 0px;\
                              height: 40px;\
                              width: "#width"px;\
                              \">\
                          </div>"
#else
  #define HLINE(width, height)
  #define POINTER(width)
  #define LOG_PRINT(string) printf (string)
#endif

#define LOG_FATAL(string) fprintf (stderr, "\n\n<<img src = \"src/fatal.jpg\" width = 150px>\nem style = \"color : red\">FATAL: " string "</em>")
#define LOG_ERROR(string) "\n\n<img src = \"src/cat.jpg\" width = 150px>\n<em style = \"color : red\">ERROR: </em>" #string "\n"

struct List
{
  type_t *data;
  long *next;
  long head;
  long tail;
  long capacity;

  #ifdef HASH_PROTECTION
    unsigned int list_hash;
    unsigned int data_hash;
    unsigned int next_hash;
  #endif
};

#define GET_HASHES unsigned int lst_hash = MurmurHash (lst, (unsigned int) sizeof (List));   \
                   int lst_buff_len = (int)((unsigned long)lst->capacity * sizeof (type_t)); \
                   unsigned int data_hash = MurmurHash (lst->data, lst_buff_len);            \
                   unsigned int next_hash = MurmurHash (lst->next, lst_buff_len)

int LstInit (List *lst, long init_size = LIST_INIT_CAP);

long FindByNext (List *lst, long key);

long ListInsert (List *lst, type_t value, long place = -1);

long ListPushBack (List *lst, type_t value);

long ListPushFront (List *lst, type_t value);

type_t ListPopBack (List *lst, int *pop_err = NULL);

type_t ListPopFront (List *lst, int *pop_err = NULL);

type_t ListPop (List *lst, long place, int *pop_err = NULL);

int64_t ListResize (List *lst, long new_capacity);

int LstDtor (List *lst);

int64_t LstDump (List *lst, int64_t err, const char *called_from);

unsigned int MurmurHash (void *ptr, int len);
