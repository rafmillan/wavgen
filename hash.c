#include "hash.h"

static node *hashtab[HASHSIZE];

void inithashtab() {
    int i;
    for (i = 0; i < HASHSIZE; i++)
        hashtab[i] = NULL;
}

unsigned int hash(char *s) {
    unsigned int h = 0;
    for (; *s; s++)
        h = *s + h * 31;
    return h % HASHSIZE;
}

node *lookup(char *n) {
    unsigned int hi = hash(n);
    node *np = hashtab[hi];
    for (; np != NULL; np = np->next)
    {
        if (!strcmp(np->key, n))
            return np;
    }

    return NULL;
}

char *m_strdup(char *o) {
    int l = strlen(o) + 1;
    char *ns = (char *)malloc(l * sizeof(char));
    strcpy(ns, o);
    if (ns == NULL)
        return NULL;
    else
        return ns;
}

float get(char *key) {
    node *n = lookup(key);
    if (n == NULL)
        return -1.0;
    else
        return n->val;
}

int install(char *key, float val) {
    unsigned int hi;
    node *np;
    if ((np = lookup(key)) == NULL) {
        hi = hash(key);
        np = (node *)malloc(sizeof(node));
        if (np == NULL)
            return 0;
        np->key = m_strdup(key);
        if (np->key == NULL)
            return 0;
        np->next = hashtab[hi];
        hashtab[hi] = np;
    }
    np->val = val;

    return 1;
}

void displaytable() {
    int i;
    node *t;

    printf("TABLE:\n");
    for (i = 0; i < HASHSIZE; i++) {
        if (hashtab[i] == NULL)
            continue;
        else{
            t = hashtab[i];
            for (; t != NULL; t = t->next)
                printf("%s\t->\t%f\n", t->key, t->val);
        }
    }
    printf("\n");
}

void cleanup() {
    int i;
    node *np, *t;
    for (i = 0; i < HASHSIZE; i++) {
        if (hashtab[i] != NULL) {
            np = hashtab[i];
            while (np != NULL) {
                t = np->next;
                free(np->key);
                free(np);
                np = t;
            }
        }
    }
}

// int main() {
//     int i;
//     char *keys[] = {"A", "B", "C", "D", "E"};
//     float vals[] = {0.0, 1.1, 2.2, 3.3, 4.4};

//     inithashtab();
//     for (i = 0; i < 5; i++)
//         install(keys[i], vals[i]);

//     printf("Done\n");

//     displaytable();
//     install("F", 5.5);
    
//     displaytable();
//     install("AA", 0.5);

//     displaytable();

//     cleanup();
//     return 0;
// }
