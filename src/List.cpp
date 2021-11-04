#include "List.h"

static FILE *Log_file = NULL;

static int OpenLogFile ()
{
  Log_file = fopen ("ListLog.html", "w");
  if (!Log_file)
  {
    printf ("OPENING LOG FILE FAILED\n");
    return OPEN_FILE_FAIL;
  }
  fprintf (Log_file, "<style>\
                      table, th, td\
                      {\
                        border:1px solid black;\
                        margin-left:100px;\
                        margin-right:auto;\
                        margin-top:1%%;\
                        table-layout:fixed;\
                        width:100px;\
                        text-align:center;\
                      }\
                      rect\
                      {\
                        position: relative;\
                        overflow: hidden;\
                        border:1px solid black;\
                        margin-left:100px;\
                        margin-top:10px;\
                        font-size:18px;\
                        width:80px;\
                        max-width:80px;\
                        text-align:center;\
                        display: inline-block;\
                      }\
                      </style>\
                      <body>\
                      <pre>");
  return OK;
}

static int CloseLogFile ()
{
  if (Log_file)
  {
    fprintf (Log_file, "</pre></body>");
    fclose (Log_file);
    Log_file = NULL;
  }

  return OK;
}

int LstInit (List *lst, long init_size)
{
  #ifdef LIST_LOGS
    int open_err = OpenLogFile ();
    if (open_err) return OPEN_FILE_FAIL;
  #endif

  if (init_size < 2) init_size = 2;

  type_t *data_mem = (type_t *) calloc ((size_t) init_size, sizeof (type_t));
  if (!data_mem)
  {
    LOG_FATAL("LOCATING MEMORY FAIL\n");
    return MEM_ALLOC_ERR;
  }

  long *next_mem = (long *) calloc ((size_t) init_size, sizeof (long));
  if (!next_mem)
  {
    LOG_PRINT ("\n#######\n\
                  FATAL: ALLOCATING MEMORY FAIL\n");
    return MEM_ALLOC_ERR;
  }

  for (long elem = 1; elem < init_size; elem++)
  {
    next_mem[elem] = -1;
  }

  lst->data     =  data_mem;
  lst->next     =  next_mem;
  lst->capacity = init_size;
  lst->tail     =         0;
  lst->head     =         0;

  LOG_PRINT ("List Initialized\n");
  return OK;
}

long FindFree (List *lst)
{
  for (long elem = 0; elem < lst->capacity; elem++)
  {
    if (lst->next[elem] == -1)
      return elem;
  }
  return -1;
}

long ListInsert (List *lst, type_t value, long place)
{
  LstDump (lst, 0, __FUNCTION__);
  if (place > lst->capacity)
  {
    LOG_ERROR ("INVALID INSERT ARG\nplace > capacity!\n");
    return INVALID_INS_ARG;
  }
  if (place == -1)
  {
    place = lst->tail;
  }
  if (place < 1 || lst->tail == 0)
  {
    LOG_ERROR ("INVALID INSERT ARG\nplace < 0!\n");
    return INVALID_INS_ARG;
  }

  long dest = FindFree (lst);
  if (dest < 1)
  {
    LOG_ERROR ("NO FREE SPACE\n");
    return -1;
  }

  lst->data[dest]  = value;
  lst->next[dest]  = lst->next[place];
  lst->next[place] = dest;

  LstDump (lst, 0, __FUNCTION__);

  return dest;
}

long ListPushBack (List *lst, type_t value)
{
  LstDump (lst, 0, __FUNCTION__);
  long dest = FindFree (lst);
  if (dest < 1)
  {
    LOG_ERROR ("NO FREE SPACE\n");
    return -1;
  }

  if (lst->tail == 0)
  {
    lst->tail = dest;
    lst->head = dest;
    lst->data[dest] = value;
    lst->next[dest] = 0;
    return dest;
  }

  lst->data[dest] = value;
  lst->next[dest] = 0;
  lst->next[lst->tail] = dest;
  lst->tail = dest;

  LstDump (lst, 0, __FUNCTION__);
  return dest;
}

long ListPushFront (List *lst, type_t value)
{
  LstDump (lst, 0, __FUNCTION__);
  long dest = FindFree (lst);
  if (dest < 1)
  {
    LOG_ERROR ("NO FREE SPACE\n");
    return -1;
  }

  if (lst->tail == 0)
  {
    lst->tail = dest;
    lst->head = dest;
    lst->data[dest] = value;
    lst->next[dest] = 0;
    return dest;
  }

  lst->data[dest] = value;
  lst->next[dest] = lst->head;
  lst->head = dest;

  LstDump (lst, 0, __FUNCTION__);
  return dest;
}

long ListPopBack ()
{
  

}

int LstDtor (List *lst)
{
  LstDump (lst, 0, __FUNCTION__);
  LOG_PRINT ("List destructed\n");

  #ifdef LIST_LOGS
    CloseLogFile ();
  #endif

  if (lst->data)
  {
    free (lst->data);
    lst->data = NULL;
  }

  if (lst->next)
  {
    free (lst->next);
    lst->next = NULL;
  }

  return OK;
}

uint64_t LstDump (List *lst, uint64_t err, const char *called_from)
{
  #ifdef LIST_LOGS
    if (!Log_file) return -1;

    const char *err_string = err ? "<em style = \"color : red\">ERROR</em>" :
                                   "<em style = \"color : #00FA9A\">ok</em>";
    fprintf (Log_file, "[%s] [%s] List &#60%s&#62 [&%p] %s ; called from %s\n",\
                 __DATE__, __TIME__, _type_name, lst, err_string, called_from);

    #ifdef MAX_INFO
        if (1)
    #else
        if (err)
    #endif
    {
      fprintf (Log_file, "<pre>{\n\tcapacity = %ld;\n\tsize = %ld;",\
               lst->capacity, (lst->tail - lst->head + lst->capacity) % lst->capacity);

      #ifdef HASH_PROTECTION
        unsigned int lst_hash = MurmurHash (lst, (unsigned int) sizeof (List));

        unsigned int data_hash = MurmurHash (lst->data, (int)((unsigned long)lst->capacity * sizeof (type_t)));

        fprintf (Log_file, "\n\tlist_hash = %u;\n\texpected list_hash = %u;\n\tdata_hash = %u;\n\texpected data_hash = %u;",
			    lst_hash, lst->list_hash, data_hash, lst->data_hash);
      #endif

      if (!lst->data)
      {
          fprintf (Log_file, "\t}\n}\n </pre>");
          return err;
      }

      fprintf (Log_file, "<table> <tr> <th>Position</th>");

      for (long data_iter = 0; data_iter < lst->capacity; data_iter++)
      {
        fprintf (Log_file, "<th>%4ld</th>", data_iter);
      }

      fprintf (Log_file, "</tr> <tr> <td>Data</td>");

      for (long data_iter = 0; data_iter < lst->capacity; data_iter++)
      {
        fprintf (Log_file, "<td>%4ld</td>", lst->data[data_iter]);
      }

      fprintf (Log_file, "</tr> <tr> <td>Next</td>");

      for (long next_iter = 0; next_iter < lst->capacity; next_iter++)
      {
        fprintf (Log_file, "<td>%4ld</td>", lst->next[next_iter]);
      }

      fprintf (Log_file, "</tr></table>\n"); //TODO LOG VERTICAL LINE

      fprintf (Log_file, "<rect>Head\n%ld</rect>\n\n<rect>Tail\n%ld</rect>\
                          \n}</pre>", lst->head, lst->tail);
      //
      // fprintf (Log_file, HLINE (900, 0) "\n");
    }
  #endif

  return 0;
}

unsigned int MurmurHash (void *ptr, int len)
{
    if (!ptr) return 0;

    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)ptr;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
