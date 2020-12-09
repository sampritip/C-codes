

# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <time.h>
# include <string.h>
# include <ctype.h>

# define POS_INF INT_MAX
# define NEG_INF INT_MIN

/*****************************
    Code for SkipListEntry
*****************************/
typedef struct skip_list_entry_t *SkipListEntry;

/***********************************************************************
                                            up
                                            ^
                                            |
            +-------------------------------------------------+
    left <- | left | key | value | offset | up | down | right | -> right
            +-------------------------------------------------+
                                                  |
                                                  v
                                                 down
*************************************************************************/
struct skip_list_entry_t {
	int key;
	int value;
	int offset;       // To print the skiplist "nicely". No exactly a part of skip list
	SkipListEntry up, down, left, right;
};

SkipListEntry newSkipListEntry(int key, int value) {
	SkipListEntry e = (SkipListEntry) calloc(1, sizeof (struct skip_list_entry_t));
    e->key   = key  ;
	e->value = value;
    e->up    = NULL ;
	e->down  = NULL ;
	e->left  = NULL ;
	e->right = NULL ;
	return e;
}


/***********************************************
    Code for random double number generation
***********************************************/
static int _rand_init = 0; // a flag indicating the initialization of seed

double nextDouble() {
    if (_rand_init == 0) { // if the seed has not been set
        srand(time(0));    // set the seed of the random number initializer to the current time
        _rand_init = 1;    // set the flag 1 which confirm the seed will not be set again
    }
    return (double) rand() / RAND_MAX;
}


/*************************
    Code for SkipList
**************************/
typedef struct skiplist_t *SkipList;

struct skiplist_t {
    SkipListEntry head;    // First element of the top level
    SkipListEntry tail;    // Last element of the top level
    int height;            // Height
};

/************************************************************
                         NULL        NULL
                           ^           ^
                           |           |
     head -->>  NULL <-- -INF <----> +INF --> NULL  <<-- tail
                           |           |
                           v           v
                         NULL        NULL
 FIG.  An Empty Skip List during after initialization
************************************************************/
SkipList newSkipList() {
     SkipList skipList = (SkipList) calloc(1, sizeof (struct skiplist_t));
     skipList->head = newSkipListEntry(NEG_INF, 0);
     skipList->tail = newSkipListEntry(POS_INF, 0);
     skipList->head->right = skipList->tail;
     skipList->tail->left  = skipList->head;
     skipList->height = 0;
     return skipList;
}

/********************************************************************************
    This function finds the largest SkipListEntry with key less than or equal
    to the given key on the lowest level of the skip list.
    In other words it search(sl, key) returns x such that
    1. x is a SkipListEntry
    2. x->key <= key
    3. x is in the lowest level of the skip list.
    E.g.: key = 17; (k, v) indicates k is a key and v is the corresponding value.

        -oo <----------------------------------------> (18, 88) <-> +oo
         ||                                               ||         ||
        -oo <--------------> (13, 83) ---------------> (18, 88) <-> +oo
         ||                     ||                        ||         ||
        -oo <-> (10, 81) <-> (13, 83) <-> (16, 86) <-> (18, 88) <-> +oo
                                            ^
                                            |
                                            x
		x->right->key = 18 > key
 *******************************************************************************/
SkipListEntry search(SkipList skipList, int key){
    SkipListEntry x = skipList->head; // start at head
    while (1) { // repeat forever
        /***************************************************
            Search right until you find a larger entry
	    ***************************************************/
	    while (x->right->key != POS_INF && x->right->key <= key) {
            /** For printing the path; not a part of skip list implementation */
            if (x->key == NEG_INF) {
                printf("Going right at -oo to find %d\n", key);
            } else {
                printf("Going right at %d to find %d\n", x->key, key);
	        }
	        /** Path printing ends here */
            x = x->right;
        }
        /***********************************************
                Go down one level if possible
	    ***********************************************/
        if (x->down != NULL) { // we are not at the lowest level
            /** For printing the path; not a part of skip list implementation */
            if (x->key == NEG_INF) {
                printf("Going down at -oo to find %d\n", key);
            } else {
                printf("Going down at %d to find %d\n", x->key, key);
            }
            x = x->down;
            /** Path printing ends here */
        } else {	   	// we reached the lowest level
            break;
        }
    }
    /** For printing the path; not a part of skip list implementation */
    if (x->key == NEG_INF) {
        printf("Stopped at (-oo, %d) after finding %d\n", x->value, key);
    } else {
        printf("Stopped at (%d, %d) after finding %d\n", x->key, x->value, key);
    }
    /** Path printing ends here */
    return x;         // x->key <= key
}

/** Returns the value associated with a key; returns NEG_INF if the key is not found. */
int get(SkipList skipList, int key) {
    SkipListEntry p = search(skipList, key);
    if (p && p->key == key) {
        return p->value;
    }
    return NEG_INF;
}

/** insert a key-value pair in the map, replacing previous one if it exists. */
int insert(SkipList skipList, int key, int value) {
    SkipListEntry p, q;
    p = search(skipList, key);
    /****************************
        Check if key is found
	*****************************/
	if (key == p->key) {
        int oldValue = p->value;
        p->value = value;
    	return oldValue;
    }
    /****************************************
        Insert new entry q = (key, value)
	*****************************************/
    q = newSkipListEntry(key, value);
    q->left = p;
    q->right = p->right;
    p->right->left = q;
    p->right = q;
    int level = 0;                   // Current level = 0

    while (nextDouble() < 0.5) {
        /***********************************************
            Check if height exceed current height.
            If so, make a new empty level.
        ************************************************/
        if (level >= skipList->height) {
            SkipListEntry p1, p2;
            skipList->height++;
            p1 = newSkipListEntry(NEG_INF, 0);
            p2 = newSkipListEntry(POS_INF, 0);
            p1->right = p2;
            p2->left  = p1;
            p1->down = skipList->head;
            p2->down = skipList->tail;
            skipList->head->up = p1;
            skipList->tail->up = p2;
            skipList->head = p1;
            skipList->tail = p2;
        }
        /*********************
            Scan backwards
        **********************/
        while (p->up == NULL) {
            p = p->left;
        }
        p = p->up;

        /*************************************************************************
            Add one more (k, NIL) to the column; NIL is represented by 0.
            Only the lowest level stores the values.
	    *************************************************************************/
	    SkipListEntry e = newSkipListEntry(key, 0);  // Don't need the value...
	    /*****************************************
            Initialize links of e
        *****************************************/
        e->left = p;
        e->right = p->right;
        e->down = q;
        /*****************************************
            Change the neighboring links
        *****************************************/
        p->right->left = e;
        p->right = e;
        q->up = e;
        q = e;		// Set q up for the next iteration
        level++;	// Current level increased by 1
     }
     return NULL;   // No old value
}

int removeFromSkipList(SkipList skipList, int key) {
    SkipListEntry p = search(skipList, key);
    if (p->key != key) {
        return 0;     // Not found, don't remove; I usually use DataDefaultValue here
    }
    /****************************************************************
        We are at the lowest level.
        Travel up the tower and link the left and right neighbors.
    ****************************************************************/
   while (p) {
      p->left->right = p->right;
      p->right->left = p->left;
      SkipListEntry q = p->up;
      free(p);
      p = q;
   }
   return key;
}

/** I wrote this function during debugging. This is of no use anymore. You can
    use this function to print the list. You may learn something here. This is
    not exactly a part of skip list.*/
void printSkipListForDebug(SkipList skipList) {
    SkipListEntry e1 = skipList->head;
    while (e1) {
        SkipListEntry e2 = e1;
        while (e2) {
            if (e2->key == POS_INF) {
                printf("+oo", e2->key, e2->value);
            } else if (e2->key == NEG_INF) {
                printf("-oo", e2->key, e2->value);
            } else {
                printf("(%d, %d, %d)", e2->key, e2->value, e2->offset);
            }
            e2 = e2->right;
            if (e2) {
                printf(" <-> ");
            }
        }
        printf("\n");
        e1 = e1->down;
    }
}

/** This function prints the skip list nicely. This is not exactly a part
    of skip list. */
# define MAXLEVEL 100
# define BUFFER_SIZE 10000
void printSkipList(SkipList skipList) {
    char s[MAXLEVEL][BUFFER_SIZE];
    char lastRow[BUFFER_SIZE]= "";
    char buffer[BUFFER_SIZE];
    int length = strlen(lastRow);
    SkipListEntry e1 = skipList->head;
    while (e1->down) {
        e1 = e1->down;
    }
    while (e1) {
        length = strlen(lastRow);
        e1->offset = length + 2;
        if (e1->key == NEG_INF) {
            sprintf(buffer, "-oo");
        } else if (e1->key == POS_INF) {
            sprintf(buffer, " <-> +oo [LEVEL: %d]", 1);
        } else {
            sprintf(buffer, " <-> (%d, %d)", e1->key, e1->value);
        }
        strcat(lastRow, buffer);
        e1 = e1->right;
    }
    int level = skipList->height + 1;
    for (e1 = skipList->head; e1->down; e1 = e1->down) {
        for (SkipListEntry e2 = e1; e2; e2 = e2->right) {
            if (e2->key == NEG_INF) {
                length = printf("-oo");
                continue;
            }
            SkipListEntry e3;
            for (e3 = e2; e3->down; e3 = e3->down)
                ;
            int offset = e3->offset;
            length += printf(" <");
            while (length <= offset) {
                length += printf("-");
            }
            if (e2->key == POS_INF) {
                printf("> +oo [LEVEL: %d]\n", level);
            } else {
                length += printf("> (%d, %d)", e2->key, e2->value);
            }
        }
        level--;
    }
    printf("%s\n", lastRow);
}

/** The remaining code contains driver and main() and exactly not related to skip lists.
    However, you may get something new to learn here. */
void help() {
    printf("INSERT KEY VALUE   Inserts the integer value VALUE in the skip list with the\n"
           "                   key KEY. If KEY is already present in the list, it modifies\n"
           "                   corresponding value to VALUE.\n"
           "DELETE KEY         Deletes the key KEY from the skip list.\n"
           "SEARCH KEY         Searches for the key KEY in the skip list.\n"
           "PRINT              Prints the skip list.\n"
           "HELP               Shows this help.\n"
           "EXIT               Exits the program.\n"
    );
}

int compareIgnoreCase(char *pa, char *pb) {
    for (;; pa++, pb++) {
        int difference = toupper((unsigned char)*pa) - toupper((unsigned char)*pb);
        if (difference != 0 || !*pa) {
            return difference;
        }
    }
}

void driver() {
    help();
    fflush(stdout);
    SkipList list = newSkipList();
    char command[80];
    while (1) {
        printf("[skip-list@list ~]$ ");
        scanf("%s", command);
        if (compareIgnoreCase(command, "exit") == 0) {
            break;
        } else if (compareIgnoreCase(command, "insert") == 0) {
            int key = 0, value = 0;
            scanf("%d%d", &key, &value);
            printf("Inserting the value %d with the key %d in skip list...\n", value, key);
            insert(list, key, value);
            printf("Modified Skip list:\n");
            printSkipList(list);
        } else if (compareIgnoreCase(command, "delete") == 0) {
            int key = 0;
            scanf("%d", &key);
            printf("Deleting the key %d from skip list...\n", key);
            int value = removeFromSkipList(list, key);
            printf("The value %d is deleted.\n", value);
            printf("Modified Skip list:\n");
            printSkipList(list);
        } else if (compareIgnoreCase(command, "search") == 0) {
            int key = 0;
            scanf("%d", &key);
            int value = get(list, key);
            if (value != NEG_INF) {
                printf("The value corresponding to the key %d is %d.\n", key, value);
            } else {
                printf("The key %d is not found,\n", key);
            }
        } else if (compareIgnoreCase(command, "print") == 0) {
            printSkipList(list);
        } else if (compareIgnoreCase(command, "help") == 0) {
            help();
        }
    }
}

int main(int argc, char *argv[]) {
    driver();
    return 0;
}
