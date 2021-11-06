#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_LOGS
#define MAX_INFO

typedef int64_t type_t;
#define _type_name "int64_t"

const long LIST_INIT_CAP = 2;

enum EXIT_CODES
{
	OK					=  0,
	MEM_ALLOC_ERR		= -1,
	INVALID_INS_ARG		= -2,
	OPEN_FILE_FAIL		= -3,
	POP_FIND_ERR		= -4,
};

#define LST_ERR(name, code) name = code,
enum LIST_STATES
{
	#include "lst_errs.h"
};
#undef LST_ERR

#ifdef LIST_LOGS
	#define LOG_PRINT(string) fprintf (Log_file, string)
	#define HLINE(width, height)	"<div><style scoped>"						\
										"hr#w" #width							\
										"{"										\
										"display: inline-block;"				\
										"position: relative;"				 	\
										"margin-top: -"#height"px;"			 	\
										"margin-left: -240px;"					\
										"border-style: inset;"					\
										"border-width: 1px;"					\
										"width: " #width "px;"					\
										"}"										\
									"</style>"									\
									"<hr id = \"w" #width "\"></div>"

	#define LOG_FATAL(string) fprintf (stderr, "\n\n<<img src = \"img/fatal.jpg\""		\
												 "width = 150px>\nem style = \"color:"	\
												 "red\">FATAL: " string "</em>")

	#define LOG_ERROR(string)	"\n\n<img src = \"img/cat.jpg\" width = 150px>\n"	 	\
								"<em style = \"color : red\">ERROR: </em>"				\
								#string "\n" HLINE (1000, 0)							\

	#define LIST_OK()															\
	{																			\
		int64_t err = ListOK (lst);												\
		ListDump (lst, err, __FUNCTION__);										\
	}
#else
	#define LIST_OK()
	#define HLINE(width, height)
	#define LOG_PRINT(string)
#endif


#define REALLOC(arr, type, size)												\
	{																			\
		type *tmp_ptr = (type *) realloc (arr, sizeof (type) * (size_t)size);	\
		if (!tmp_ptr)															\
		{																		\
			LOG_FATAL ("ALLOCATING MEMORY FAIL\n");								\
			return MEM_ALLOC_ERR;												\
		}																		\
		arr = tmp_ptr;															\
	}

#define KILL_PTR(ptr, type)													 	\
	if (ptr)																	\
	{																			\
		free (ptr);																\
		ptr = (type *)POISON;													\
	}

struct List
{
	type_t *data;
	long *next;
	long *prev;
	long head;
	long tail;
	long free;
	long capacity;
	long size;
};


int ListInit (List *lst, long init_size = LIST_INIT_CAP);

long LogicalToPhysicalAddr (List *lst, long num);

long ListInsert (List *lst, type_t value, long place = -1);

long ListPushBack (List *lst, type_t value);

long ListPushFront (List *lst, type_t value);

type_t ListPopBack (List *lst, int *pop_err = NULL);

type_t ListPopFront (List *lst, int *pop_err = NULL);

type_t ListPop (List *lst, long place, int *pop_err = NULL);

int64_t ListOK (List *lst);

int64_t ListResize (List *lst, long new_capacity);

int ListDtor (List *lst);

int64_t ListDump (List *lst, int64_t err, const char *called_from);
