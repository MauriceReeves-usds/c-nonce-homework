//
// Created by Maurice on 6/8/22.
//

#ifndef NONCE_HOMEWORK_NONCE_H
#define NONCE_HOMEWORK_NONCE_H

#define     MAX_NONCE_SIZE              39
#define     MAX_DATE_SIZE               25
#define     MAX_TIME_SIZE               25
#define     RECORD_STRING_MIN_SIZE      100

/* Contains our nonce dates */
typedef struct Nonce_Date {
    char date[MAX_DATE_SIZE];
    char time[MAX_TIME_SIZE];
} Nonce_Date;

/* The record for each nonce */
typedef struct Record {
    char nonce[MAX_NONCE_SIZE];
    char date[MAX_DATE_SIZE];
    char time[MAX_TIME_SIZE];
    int count;
    struct Record* next;
} Record;

/* Writes out the record to target_string */
void record_to_string(char* target_string, const Record* r);

/* Given a record, print it to stdout */
void print_record(const Record* r);

/* Look through the list of records and find one that matches the passed in nonce */
Record* find_record(const Record* r, const char* nonce);

/* walk through the list of records, and try to find anything with a count higher than 1 */
void find_duplicates();

/* find the count of duplicates in the linked list */
long find_duplicates_count(const Record* r);

/* Initialize the next record */
Record* init_record(Record* r, const char* nonce, const char* date, const char* time);

/* free all the records in the linked list of records */
void free_records(Record* r);

/* create a list of the records in the file */
void create_record_list(const char* file_path);

#endif //NONCE_HOMEWORK_NONCE_H
