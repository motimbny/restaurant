#ifndef headd.h

#pragma warning(disable : 4996)
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#define tmax 15
#define MAX 256
typedef struct dish
{
	char *dishname;
	float price;
	int qunat;
	char prem;
	struct dish *next;
}dish, *Pdish;

typedef struct kitchen
{
	struct dish *head;
	struct dish *tail;
	int size;
}kitchen, *Pkitchen;

typedef struct table
{
	float bill;
	int totaldish;
	struct torders *head;
}table, *Ptable;

typedef struct torders
{
	char *dishname;
	float tprice;
	int qunat;
	char prem;
	struct torders *next;
}torders, *Ptorders;

int CreateProduct(kitchen *); //function 1
void AddItems(char *, int, kitchen *); //function 2
void OrderItem(char *, int, int, table *, kitchen *);//function 3
void RemoveItem(int, char *, int, table *, kitchen *);//function 4
void RemoveTable(table *, int);//function 5
void Printbbb();//function to print menu
void closetable(table *);//function to close table
void printbill(torders *);//function to print table bill
int precheck(torders *);//function to check if table is prem
Ptorders getprev(torders *, char *); //getting prev to delete
void errormessage(char*);//error message
void freeallocation(kitchen *); //free allocation of kitchen
void freeallocation2(table *ta);//free allocation of table bills
Pdish checkname(kitchen*, char *); //check if name is in the kitchen
int checkfile(FILE *);//function to check if file is empty
Ptorders checkdup(torders *, char*); //function to check if the name of dish is in the table
#endif // header.h




