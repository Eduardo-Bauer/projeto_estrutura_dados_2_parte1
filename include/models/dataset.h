#ifndef DATASET_H
#define DATASET_H

/* Estrutura interna fixa do dataset. */
 typedef struct estrutura_dataset{
    char event_time[25];
    char event_type[10];
    int product_id;
    long long category_id;
    char category_code[50];
    char brand[20];
    float price;
    int user_id;
    char user_session[50];
}ESTRUTURA_DATASET;

#endif