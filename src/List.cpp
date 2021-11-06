#include "List.h"

#ifdef LIST_LOGS
	static int OpenLogFile ();
	static int CloseLogFile ();
	static FILE *Log_file = NULL;
	static FILE *Graph_file = NULL;
	static int	GRAPH_NUM = 0;
#else
	static FILE *Log_file = stderr;
#endif

const int POISON = 0x42;

int ListInit (List *lst, long init_size)
{
	#ifdef LIST_LOGS
		int open_err = OpenLogFile ();
		if (open_err) return OPEN_FILE_FAIL;
	#endif

	if (init_size < 2) init_size = 2;

	REALLOC (lst->data, type_t, init_size);
	REALLOC (lst->next,	 long, init_size);
	REALLOC (lst->prev,	 long, init_size);

	for (long elem = 0; elem < init_size; elem++)
	{
		lst->data[elem] = 0;
		lst->next[elem] = (elem + 1) % init_size;
		lst->prev[elem] = -1;
	}

	lst->next[0]			 = 0;
	lst->prev[0]			 = 0;

	lst->capacity = init_size;
	lst->tail	 = 0;
	lst->head	 = 0;
	lst->size	 = 0;
	lst->free	 = 1;

	LOG_PRINT ("<em style = \"color : #16c95e\">List Initialized</em>\n");
	LIST_OK();

	return OK;
}

long LogicalToPhysicalAddr (List *lst, long num)
{
	long elem = lst->head;
	for (; elem < num; elem = lst->next[elem]) ;
	return elem;
}

long ListInsert (List *lst, type_t value, long place)
{
	LIST_OK();
	if (place > lst->capacity)
	{
		fprintf (Log_file,
				 LOG_ERROR (INVALID INSERT ARG) "\nplace = %ld > capacity = %ld!\n",
				 place, lst->capacity);
		return INVALID_INS_ARG;
	}

	if (lst->tail == 0)
	{
		fprintf (Log_file,
				 LOG_ERROR (INVALID INSERT) "\ninsert after 0 elem!\n");
		return INVALID_INS_ARG;
	}
	else if (place == -1 || place == lst->tail)
	{
		return ListPushBack (lst, value);
	}
	else if (place < 0)
	{
		fprintf (Log_file,
				 LOG_ERROR (INVALID INSERT ARG) "\nplace = %ld < 0!\n",
				 place);
		return INVALID_INS_ARG;
	}
	else if (lst->prev[place] == -1)
	{
		fprintf (Log_file,
				 LOG_ERROR (INVALID INSERT ARG) "\nno elem at place = %ld\n",
				 place);
	}

	long dest = lst->free;
	if (dest < 1)
	{
		ListResize (lst, lst->capacity);
	}

	lst->free		= lst->next[lst->free];
	lst->data[dest]	= value;
	lst->next[dest]	= lst->next[place];
	lst->next[place] = dest;
	lst->prev[dest]	= place;
	lst->size++;

	LIST_OK();

	return dest;
}

long ListPushBack (List *lst, type_t value)
{
	LIST_OK();
	long dest = lst->free;
	if (dest < 1)
	{
		fprintf (Log_file, LOG_ERROR (NO FREE SPACE) "\n");
		return -1;
	}

	if (lst->tail == 0)
	{
		lst->free		 = lst->next[lst->free];
		lst->tail		 = dest;
		lst->head		 = dest;
		lst->data[dest] = value;
		lst->next[dest] = 0;
		lst->prev[dest] = 0;
		lst->size++;

		return dest;
	}

	lst->free			= lst->next[lst->free];
	lst->data[dest]		= value;
	lst->next[dest]		= 0;
	lst->prev[dest]		= lst->tail;
	lst->next[lst->tail] = dest;
	lst->tail			= dest;
	lst->size++;

	LIST_OK();
	return dest;
}

long ListPushFront (List *lst, type_t value)
{
	LIST_OK();
	long dest = lst->free;
	if (dest < 1)
	{
		return -1;
	}

	if (lst->tail == 0)
	{
		lst->free		 = lst->next[lst->free];
		lst->tail		 = dest;
		lst->head		 = dest;
		lst->data[dest] = value;
		lst->next[dest] = 0;
		lst->prev[dest] = 0;
		lst->size++;

		return dest;
	}

	lst->free			= lst->next[lst->free];
	lst->data[dest]		= value;
	lst->next[dest]		= lst->head;
	lst->prev[dest]		= 0;
	lst->prev[lst->head] = dest;
	lst->head			= dest;
	lst->size++;

	LIST_OK();
	return dest;
}

type_t ListPopBack (List *lst, int *pop_err)
{
	LIST_OK();

	if (lst->tail == 0)
	{
		fprintf (Log_file,
				 LOG_ERROR (ZERO ELEMENT POP) "\n");
		if (pop_err) *pop_err = POP_FIND_ERR;
		return POP_FIND_ERR;
	}

	if (lst->tail == lst->head)
	{
		type_t tmp			 = lst->data[lst->tail];
		lst->next[lst->tail] = lst->free;
		lst->free			 = lst->tail;
		lst->prev[lst->tail] = -1;
		lst->data[lst->tail] =	0;
		lst->tail			 =	0;
		lst->head			 =	0;
		lst->size--;

		return tmp;
	}

	long prev = lst->prev[lst->tail];
	if (prev == -1)
	{
		fprintf (Log_file,
				 LOG_ERROR (NO ELEMENT FOUND: expected next = %ld</em>\n),
				 lst->tail);
		if (pop_err) *pop_err = POP_FIND_ERR;
		return POP_FIND_ERR;
	}

	type_t tmp			 = lst->data[lst->tail];
	lst->next[prev]		 =	0;
	lst->next[lst->tail] = lst->free;
	lst->free			 = lst->tail;
	lst->prev[lst->tail] = -1;
	lst->data[lst->tail] =	0;
	lst->tail			 = prev;
	lst->size--;

	LIST_OK();

	return tmp;
}

type_t ListPopFront (List *lst, int *pop_err)
{
	LIST_OK();

	if (lst->head == 0)
	{
		fprintf (Log_file,
				 LOG_ERROR (ZERO ELEMENT POP) "\n");
		if (pop_err) *pop_err = POP_FIND_ERR;
		return POP_FIND_ERR;
	}

	if (lst->tail == lst->head)
	{
	type_t tmp = lst->data[lst->tail];
	lst->next[lst->tail] = lst->free;
	lst->free			= lst->tail;
	lst->prev[lst->tail] = -1;
	lst->data[lst->tail] =	0;
	lst->tail			=	0;
	lst->head			=	0;
	lst->size--;

	return tmp;
	}

	long next = lst->next[lst->head];

	type_t tmp			 = lst->data[lst->head];
	lst->data[lst->head] =	0;
	lst->next[lst->head] = lst->free;
	lst->free			 = lst->head;
	lst->prev[lst->head] = -1;
	lst->head			 = next;
	lst->size--;

	LIST_OK();
	return tmp;
}

type_t ListPop (List *lst, long place, int *pop_err)
{
	LIST_OK();

	if (lst->tail == 0)
	{
		fprintf (Log_file,
				 LOG_ERROR (ZERO ELEMENT POP) "\n");
		if (pop_err) *pop_err = POP_FIND_ERR;
		return POP_FIND_ERR;
	}

	if (place == lst->head)
	{
		return ListPopFront (lst, pop_err);
	}

	if (place == lst->tail)
	{
		return ListPopBack (lst, pop_err);
	}

	long prev = lst->prev[place];
	if (prev == -1)
	{
		fprintf (Log_file,
				 LOG_ERROR (NO ELEMENT FOUND: expected next = %ld</em>\n),
				 lst->tail);
		if (pop_err) *pop_err = POP_FIND_ERR;
			return POP_FIND_ERR;
	}

	type_t tmp = lst->data[place];

	lst->next[lst->prev[place]] = lst->next[place];
	lst->prev[lst->next[place]] = lst->prev[place];
	lst->next[place]			= lst->free;
	lst->free					= place;
	lst->prev[place]			= -1;
	lst->data[place]			= 0;
	lst->size--;

	LIST_OK();
	return tmp;
}

int64_t ListOK (List *lst)
{
	int64_t err = 0;

	// Checking if all pointers are valid
	if (lst->data == NULL || lst->data == (type_t *)POISON)
	{
		err |= BAD_DATA_PTR;
	}

	if (lst->next == NULL || lst->next == (long *)POISON)
	{
		err |= BAD_NEXT_PTR;
	}

	if (lst->prev == NULL || lst->prev == (long *)POISON)
	{
		err |= BAD_PREV_PTR;
	}

	// Checking if prev linked to current and next
	// Also steps counter must be equal to size of the list
	if ((err & BAD_NEXT_PTR) || (err & BAD_PREV_PTR))
	{
		return err;
	}

	long steps = 2;
	long curr  = 0;
	for (curr = lst->next[lst->head]; lst->next[curr] != 0; curr = lst->next[curr], steps++)
	{
		if (lst->prev[lst->next[curr]] != curr)
		{
			err |= NEXT_NOT_LINKED;
			break;
		}
		if (lst->next[lst->prev[curr]] != curr)
		{
			err |= PREV_NOT_LINKED;
			break;
		}
		if (steps > lst->size)
		{
			err |= ENDLESS_LOOP;
			break;
		}
	}

	if (steps < lst->size && lst->next[curr] == 0)
	{
		err |= UNEXPECTED_END;
	}

	// For each free prev must be -1; number of free's equals capacity - size
	steps		  = 1;
	long free_num = lst->capacity - lst->size - 1;
	for (curr = lst->free; lst->next[curr] != 0; curr = lst->next[curr], steps++)
	{
		if (lst->prev[curr] != -1)
		{
			err |= INVALID_FREE_PREV;
			break;
		}
		if (steps > free_num)
		{
			err |= ENDLESS_FREE_LOOP;
			break;
		}
	}

	if (steps < free_num && lst->next[curr] == 0)
	{
		err |= UNEXPECTED_FREE_END;
	}

	return err;
}

int64_t ListResize (List *lst, long new_capacity)
{
	if (new_capacity <= 0) new_capacity = LIST_INIT_CAP;

	long buff_len = new_capacity * (long) sizeof (type_t);

	REALLOC (lst->data, type_t, buff_len);
	REALLOC (lst->next,	 long, buff_len);
	REALLOC (lst->prev,	 long, buff_len);

	for (long iter = lst->capacity; iter < new_capacity - 1; iter++)
	{
		lst->data[iter] = 0;
		lst->next[iter] = iter + 1;
		lst->prev[iter] = -1;
	}

	lst->next[new_capacity - 1] = 0;

	lst->capacity = new_capacity;

	return OK;
}

int ListDtor (List *lst)
{
	LIST_OK();
	LOG_PRINT ("<em style = \"color : #16c95e\">List destructed</em>\n");

	#ifdef LIST_LOGS
		CloseLogFile ();
	#endif

	KILL_PTR (lst->data, type_t);
	KILL_PTR (lst->next, long);
	KILL_PTR (lst->prev, long);

	return OK;
}

#ifdef LIST_LOGS
	#define LST_ERR(name, code)													\
		if (err & code)															\
		{																		\
			fprintf (Log_file, LOG_ERROR (name) "\n");							\
		}

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
								border-radius:6px;\
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
								border-radius:6px;\
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

	int64_t ListDump (List *lst, int64_t err, const char *called_from)
	{
	#ifdef LIST_LOGS
		if (!Log_file) return -1;

		Graph_file = fopen ("dotInput.dot", "w");
		if (!Graph_file)
		{
			printf ("OPENING GRAPH FILE FAILED\n");
			return OPEN_FILE_FAIL;
		}
		fprintf (Graph_file, "digraph\n{\nrankdir = \"LR\";\n");

		const char *err_string = err ? "<em style = \"color : red\">ERROR</em>" :
									 "<em style = \"color : #00FA9A\">ok</em>";
		fprintf (Log_file, "[%s] [%s] List &#60%s&#62 [&%p] %s ; called from %s\n",\
					 __DATE__, __TIME__, _type_name, lst, err_string, called_from);

		#include "lst_errs.h"

		#ifdef MAX_INFO
			if (1)
		#else
			if (err)
		#endif
		{
			fprintf (Log_file, "<pre>{\n\tcapacity = %ld;\n\tsize = %ld;",\
					 lst->capacity, (lst->tail - lst->head + lst->capacity) % lst->capacity);

			if (!lst->data)
			{
				fprintf (Log_file, "\t}\n}\n </pre>");
				return err;
			}

			fprintf (Log_file, "<table> <tr> <th>Position</th>");

			for (long data_iter = 0; data_iter < lst->capacity; data_iter++)
			{
				fprintf (Log_file, "<th>%4ld</th>", data_iter);
				fprintf (Graph_file, 	"NODE%ld[shape=record, style = \"rounded\","
										"label = \""
										"Data = %ld|"
										"{ Position = %ld|"
										"<f0> Next = %ld|"
										"<f1> Prev = %ld }\""
										"]\n",
										data_iter, lst->data[data_iter], data_iter,
										lst->next[data_iter], lst->prev[data_iter]);
			}

			fprintf (Log_file, "</tr> <tr> <td>Data</td>");

			for (long data_iter = 0; data_iter < lst->capacity; data_iter++)
			{
				fprintf (Log_file, "<td>%4ld</td>", lst->data[data_iter]);
				if (lst->prev[data_iter] >= 0)
				{
				fprintf (Graph_file, 	"NODE%ld->NODE%ld [style = invis, weight= 100];\n"
									 	"NODE%ld:<f0>->NODE%ld;\n"
									 	"NODE%ld:<f1>->NODE%d;\n",
										(data_iter - 1 + lst->capacity) % lst->capacity,
										data_iter, data_iter, lst->next[data_iter], data_iter, 0);
				}
			}

			fprintf (Log_file, "</tr> <tr> <td>Next</td>");

			for (long next_iter = 0; next_iter < lst->capacity; next_iter++)
			{
				fprintf (Log_file, "<td>%4ld</td>", lst->next[next_iter]);
			}

			fprintf (Log_file, "</tr> <tr> <td>Prev</td>");

			for (long prev_iter = 0; prev_iter < lst->capacity; prev_iter++)
			{
				fprintf (Log_file, "<td>%4ld</td>", lst->prev[prev_iter]);
			}

			fprintf (Log_file, "</tr></table>\n");

			fprintf (Log_file,	"<rect>Head\n%ld</rect>\n\n<rect>Tail\n%ld</rect>\n\n"
								"<rect>Free\n%ld</rect>\n\n"
								"\n}</pre>", lst->head, lst->tail, lst->free);

			fprintf (Graph_file, "\n}");
			fclose (Graph_file);

			char sys_command[43] = {};
			snprintf (sys_command, 43, 	"%s%d%s",
										"dot dotInput.dot -Tpng -o img/Img",
										GRAPH_NUM,
										".png");

			system (sys_command);

			fprintf (Log_file, "\n<img src = \"img/Img%d.png\" width = 1000px>\n", GRAPH_NUM);
			GRAPH_NUM++;
			fprintf (Log_file, HLINE (1000, 0) "\n");
		}
	#endif

	return 0;
	}
#endif
