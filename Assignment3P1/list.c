#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node node;
struct Node
{
    char *command;
    struct Node *next;
};

int size__SS = 0;
node *head = NULL;
node *tail = NULL;

node *initNode(char *st)
{
    node *ob = (node *)malloc(sizeof(node *));
    ob->next = NULL;
    ob->command = st;
    return ob;
}

void insert(char *st)
{
    char *st1 = (char *)malloc(1000);
    strcpy(st1, st);
    if (size__SS == 0 || head == NULL)
    {
        // printf("Initialize Head\n");
        head = initNode(st1);
        tail = head;
        size__SS++;
    }
    else
    {
        node *nn = initNode(st1);
        tail->next = nn;
        tail = tail->next;
    }
    size__SS++;
    if (size__SS > 20)
    {
        delete_node();
    }
    // printf("Added %s to nodes.\n", st1);
}

int delete_node()
{
    if (size__SS < 20)
    {
        return size__SS;
    }
    else
    {
        node *add = head;
        head = head->next;
        free(add);
        size__SS--;
        return size__SS;
    }
}

void displayNodes(int c, FILE *fp)
{

    // printf("Display Called\n");
    node *tmp = head;
    int count = 0;
    while (tmp != NULL)
    {
        // printf("Display Called %d %d %d\n", count, size__SS, c);

        if (count >= size__SS - c)
        {
            if (fp == NULL)
            {
                printf("%s", tmp->command);
            }
            else
                fprintf(fp, "%s", tmp->command);
        }
        tmp = tmp->next;
        count++;
    }
}