#ifndef HASH_H
#define HASH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 150

typedef struct _node {
    char *key;
    float val;
    struct _node *next;
} node;

static node *hashtab[HASHSIZE];

void inithashtab();

unsigned int hash(char *s);

node *lookup(char *n);

char *m_strdup(char *o);

float get(char *key);

int install(char *key, float val);

void displaytable();

void cleanup();

#endif