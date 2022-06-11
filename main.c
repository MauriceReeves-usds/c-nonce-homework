#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "nonce.h"

/* our global head variable */
Record* head;
Record* duplicates;

/* Writes out the record to target_string */
void record_to_string(char* target_string, const Record* r) {
    assert(target_string != NULL);
    sprintf(target_string, "Nonce -> %s Date -> %s Time -> %s Count -> %d", r->nonce, r->date, r->time, r->count);
}

/* Given a record, print it to stdout */
void print_record(const Record* r) {
    assert(r != NULL);
    char string_repr[RECORD_STRING_MIN_SIZE] ;
    record_to_string(string_repr, r);
    printf("%s\n", string_repr);
}

/* Look through the list of records and find one that matches the passed in nonce */
Record* find_record(const Record* r, const char* nonce) {
    Record *ptr = r->next;
    while(ptr != NULL) {
        if (strcmp(ptr->nonce, nonce) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

/* walk through the list of records, and try to find anything with a count higher than 1 */
void find_duplicates() {
    int count = 0;
    Record *new_duplicates;
    duplicates = (Record*)malloc(sizeof(Record));
    /* move off the header */
    Record* ptr = head->next;
    while (ptr != NULL) {
        if (ptr->count > 1) {
            /* assign our array element */
            duplicates[count++] = *ptr;
            /* expand our array */
            new_duplicates = (Record*)realloc(duplicates, (count + 1) * sizeof(Record));
            /* if the allocation was successful, reset the pointer */
            if (new_duplicates != NULL) {
                duplicates = new_duplicates;
            }
        }
        ptr = ptr->next;
    }
}

/* find the count of duplicates in the linked list */
long find_duplicates_count(const Record* r) {
    Record* ptr = r->next;
    long count = 0;
    while (ptr != NULL) {
        if (ptr->count > 1) count++;
        ptr = ptr->next;
    }
    return count;
}

/* Initialize the next record */
Record* init_record(Record* r, const char* nonce, const char* date, const char* time) {
    if (r == NULL) {
        r = malloc(sizeof(Record));
        return r;
    }
    /* check for any duplicates and skip for now, after incrementing the counter */
    Record* duplicate = find_record(head, nonce);
    if (duplicate != NULL) {
        duplicate->count++;
        return r;
    }
    Record* next = malloc(sizeof(Record));
    strlcpy(next->nonce, nonce, MAX_NONCE_SIZE);
    strlcpy(next->date, date, MAX_DATE_SIZE);
    strlcpy(next->time, time, MAX_TIME_SIZE);
    next->count = 1;
    r->next = next;
    return next;
}

/* free all the records in the linked list of records */
void free_records(Record* r) {
    Record* ptr = r->next;
    /* free the memory */
    free(r);
    /* set the pointer to null */
    r = NULL;
    if (ptr != NULL) {
        free_records(ptr);
    }
}

/* create a list of the records in the file */
void create_record_list(const char* file_path) {
    /* some variables */
    FILE * file_pointer;
    char nonce[MAX_NONCE_SIZE], date[MAX_DATE_SIZE], time[MAX_TIME_SIZE];
    bool first = true;
    Record *next;
    /* initialize our head record */
    head = init_record(head, "", "", "");
    /* open our file for reading */
    file_pointer = fopen(file_path, "r");
    /* reading each line, create a new record and add it to its head */
    while (fscanf(file_pointer, "%36s %10s %15s[\n]", nonce, date, time) != EOF) {
        /*
         * add the first record to the head. init_record returns the last item in the linked list
         */
        if (first) {
            next = init_record(head, nonce, date, time);
            first = false;
            continue;
        }
        /* keep building our linked list of records */
        next = init_record(next, nonce, date, time);
    }
    /* close out */
    fclose(file_pointer);
}

/* our main method */
int main() {
    /* create a record list and add it to the head record */
    create_record_list("/Users/maurice/development/smeqa-homework/nonce.txt");

    /* print out all our records */
    Record* rec = head->next;
    while (rec != NULL) {
        print_record(rec);
        rec = rec->next;
    }

    /* search our records, just to show how it's done */
    rec = find_record(head, "a7cd3b49-5a17-4011-aa70-01a56d5455e7");
    printf("Found:\n");
    print_record(rec);

    /* go find some duplicates */
    long duplicates_count = find_duplicates_count(head);
    printf("Found %ld duplicates!\n", duplicates_count);
    if (duplicates_count > 0) {
        find_duplicates();
        for (long idx = 0; idx < duplicates_count; ++idx) {
            print_record(&duplicates[idx]);
        }
        /* clean up our duplicates if we found any */
        free(duplicates);
        /* set the pointer to null for great justice */
        duplicates = NULL;
    } else {
        printf("No duplicates detected!\n");
    }

    /* clean up */
    free_records(head);

    /* exit */
    return EXIT_SUCCESS;
}
