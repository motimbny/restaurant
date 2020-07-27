#include "headd.h"

//function 1 to update list of dishes to kitchen
int CreateProduct(kitchen *kitch)
{
	int size, add;
	char tempname[100];
	FILE *in;
	dish *temp, *checkd;
	in = fopen("manot.txt", "rt");
	if (in == NULL)
	{
		printf("Updating kitchen failed \n");
		errormessage("\n Couldnt open file,please check");
	}
	if (!checkfile(in))
	{
		printf("Updating kitchen failed \n");
		errormessage("\n file is empty,please check");
	}
	while (fscanf(in, "%s", tempname) > 0)
	{
		checkd = checkname(kitch, tempname);
		if (checkd == NULL)
		{
			temp = (dish*)malloc(sizeof(dish));
			if (temp == NULL)
			{
				freeallocation(kitch);
				printf("Updating kitchen failed \n");
				errormessage("\n ERROR! allocation failed");
			}
			temp->dishname = (char*)malloc((strlen(tempname) + 1) * sizeof(char));
			if (temp->dishname == NULL)
			{
				free(temp);
				freeallocation(kitch);
				printf("Updating kitchen failed \n");
				errormessage("\n ERROR! allocation failed");
			}
			strcpy(temp->dishname, tempname);
			fscanf(in, "%d", &temp->qunat);
			if (temp->qunat <= 0)
			{
				free(temp->dishname);
				free(temp);
				freeallocation(kitch);
				printf("Updating kitchen failed \n");
				errormessage("\n ERROR! quantity is not vaild for dish.plese check");
			}
			fscanf(in, "%f", &temp->price);
			if (temp->price <= 0)
			{
				free(temp->dishname);
				free(temp);
				freeallocation(kitch);
				printf("Updating kitchen failed \n");
				errormessage("\n ERROR! price is not vaild for dish.plese check");
			}
			fscanf(in, " %c", &temp->prem);
			if (kitch->head == NULL)
			{
				temp->next = NULL;
				kitch->tail = temp;
			}
			else
				temp->next = kitch->head;
			kitch->head = temp;
			kitch->size++;
		}
		else //if dish is allready exsist
		{
			fscanf(in, "%d", &add);
			checkd->qunat += add;
			fscanf(in, "%f", &checkd->price);
			fscanf(in, " %c", &checkd->prem);
		}
	}
	fclose(in);//closing file
	printf("\n Updating kitchen was successful, ready for you chef !!! \n");
}

//function 2
void AddItems(char *name, int qua, kitchen *kit)
{
	dish *temp = checkname(kit, name);
	if (temp == NULL)
	{
		printf("\n The kitchen didnt find a meal that match the dish name. please check!");
		return;
	}
	if (qua <= 0)
	{
		printf("\n Can not add negative quantity or zero to kitchen, please check!");
		return;
	}
	temp->qunat += qua;
	printf("\n Added %d more to %s in the kitchen! good luck chef", qua, name);
}

//function 3
void OrderItem(char *diname, int qua, int tablnum, table *ta, kitchen *kit)
{
	dish *temp = checkname(kit, diname);
	torders *ordcheck, *toadd;
	if (temp == NULL)
	{
		printf("\n The kitchen didnt find a dish that match table request, please check!");
		return;
	}
	else if (tablnum <= 0 || tablnum>tmax)
	{
		printf("\n There is a problem locating the table %d, please check!", tablnum);
		return;
	}
	else if (qua<0)
	{
		printf("\n Can't order negative dishes to table, please check!");
		return;
	}
	else if (qua>temp->qunat)
	{
		printf("\n Can't order %d dishes to table,kitchen is at crisis, please check!", qua);
		return;
	}
	else //if we got here, everything is good with input and need to do the function
	{
		ordcheck = checkdup(ta[tablnum - 1].head, diname);
		if (ta[tablnum - 1].head != NULL && ordcheck != NULL) //if there is allready a bill to the table
		{
			ordcheck->qunat += qua;
			temp->qunat -= qua;
			ordcheck->prem = temp->prem;
			ordcheck->tprice += (temp->price)*qua;
			ta[tablnum - 1].bill += (temp->price)*qua;
			ta[tablnum - 1].totaldish += qua;
			printf("\n Added to Table %d , %d dish's of %s ,bon a pete", tablnum, qua, diname);
		}
		else //if table doesnt have a bill or dish from that kind
		{
			toadd = (torders*)malloc(sizeof(torders));
			if (toadd == NULL)
			{
				freeallocation(kit);
				freeallocation2(ta);
				errormessage("\n ERROR! allocation failed");
			}
			toadd->dishname = (char*)malloc((strlen(diname) + 1) * sizeof(char));
			if (toadd->dishname == NULL)
			{
				freeallocation(kit);
				freeallocation2(ta);
				errormessage("\n ERROR! allocation failed");
			}
			strcpy(toadd->dishname, diname);
			toadd->qunat = 0;
			toadd->tprice = 0;
			toadd->prem = temp->prem;
			toadd->qunat += qua;
			temp->qunat -= qua;
			toadd->tprice += qua * (temp->price);
			toadd->next = NULL;
			ta[tablnum - 1].bill += (temp->price)*qua;
			ta[tablnum - 1].totaldish += qua;
			if (ta[tablnum - 1].head == NULL)
			{
				ta[tablnum - 1].head = toadd;
			}
			else
			{
				toadd->next = ta[tablnum - 1].head;
				ta[tablnum - 1].head = toadd;
			}
			printf("\n Added to Table %d , %d dish's of %s ,bon a pete", tablnum, qua, diname);
		}
	}

}
//function 4
void RemoveItem(int tabnum, char *dishname, int qua, table *ta, kitchen *kit)
{
	dish *temp = checkname(kit, dishname);
	torders *ordcheck, *prev = NULL;
	ordcheck = checkdup(ta[tabnum - 1].head, dishname); //if this dish is at the table
	if (ordcheck != ta[tabnum].head)
		prev = getprev(ordcheck, dishname);
	if (tabnum<0 || tabnum>tmax)
	{
		printf("\n There was a problem locating table %d, please contact manager", tabnum);
		return;
	}
	else if (temp == NULL)
	{
		printf("\n We are unable to remove %s beacuase we dont have it in the resturant,learn the menu!", dishname);
		return;
	}
	else if (ordcheck == NULL)
	{
		printf("\n Table %d didnt order %s, please check with them!", tabnum, dishname);
		return;
	}
	else if (qua < 0)
	{
		printf("\n We can not remove negative quantitiy. please check");
		return;
	}
	else if (qua > ordcheck->qunat)
	{
		printf("\n We can not remove %d dish's from table %d, they ordered only %d dish's. please check", qua, tabnum, ordcheck->qunat);
		return;
	}
	else
	{
		if (ordcheck->qunat == qua) //if they cancel the whole dish order
		{
			ta[tabnum - 1].bill -= ((temp->price)*qua);
			ta[tabnum - 1].totaldish -= qua;
			if (ordcheck->next == NULL && prev != NULL) //if the dish is the last in the cross section
			{
				prev->next = NULL;
				free(ordcheck);
			}
			else if (ta[tabnum - 1].head == ordcheck) //if dish is the head of cross section
			{
				ta[tabnum - 1].head = ordcheck->next;
				free(ordcheck);
			}
			else //if dish is somewhere in the middile of the list
			{
				prev->next = ordcheck->next;
				free(ordcheck);
			}
		}
		else
		{
			ordcheck->qunat -= qua;
			ordcheck->tprice -= ((temp->price)*qua);
			ta[tabnum - 1].bill -= ((temp->price)*qua);
			ta[tabnum - 1].totaldish -= qua;
		}
	}
	printf("\n We removed %d of dish %s from table %d.", qua, dishname, tabnum);
}

//function 5
void RemoveTable(table *tab, int tabnum)
{
	int check = precheck(tab[tabnum - 1].head); //check is to see if the table was vip
	if (tabnum<0 || tabnum>tmax)
	{
		printf("\n There was a problem locating table %d, please contact manager", tabnum);
		return;
	}
	if (tab[tabnum - 1].head == NULL)
	{
		printf("\n Table %d is not open and dosent have any bill's that are not paid", tabnum);
		return;
	}
	else
	{
		printf("\n Table %d .This was a %s table.", tabnum, check > 0 ? "Premium" : "Regular");
		printf("\The total table bill is : %.2f", check > 0 ? ((tab[tabnum - 1].bill)*(1.2)) : tab[tabnum - 1].bill);
		printf("\n The Detailed bill: ");
		printbill(tab[tabnum - 1].head);
		closetable(&tab[tabnum - 1]);
	}
}
//function to check if file is empty
int checkfile(FILE *f)
{
	int size;
	fseek(f, 0, SEEK_END); //checking if file is empty
	size = ftell(f);
	if (size == 0)
		return 0;
	fseek(f, 0, 0); //moving pointer back to file start
	return 1;

}
//function to print menu
void Printbbb()
{
	printf("\n\nWelcome to BBB Restaurant");
	printf("\nHere is menu of resturant functions:");
	printf("\n By choosing 1 : Update the stock in kitchen");
	printf("\n By choosing 2 : Add Item in to kitchen stock");
	printf("\n By choosing 3 : Make order of dish to a table");
	printf("\n By choosing 4 : Cancel a dish from a table");
	printf("\n By choosing 5 : Closing table and getting bill");
	printf("\nTo countiue, please press '0' and then enter, otherwise input will be wrong: ");
}
//function to close table
void closetable(table *tab)
{
	torders *tmp;
	tab->bill = 0;
	tab->totaldish = 0;
	if (tab->head != NULL)
	{
		tmp = tab->head;
		tab->head = tab->head->next;
		free(tmp->dishname);
		free(tmp);
	}
	printf("\n Table is closed");
}
//function to print table bill
void printbill(torders *tab)
{
	torders *print = tab;
	while (print != NULL)
	{
		printf("\n Dish name : %s     Amount: %d       Total price for dish: %.2f", print->dishname, print->qunat, print->tprice);
		print = print->next;
	}
}
//function to check if table is perimuim
int precheck(torders *tab)
{
	torders *temp = tab;
	while (temp != NULL)
	{
		if (temp->prem == 'Y')
			return 1;
		else
			temp = temp->next;
	}
	return 0;
}


//function to find prev of dish table cross section
Ptorders getprev(torders *ta, char * str)
{
	torders *temp = ta;
	if (temp->next == NULL)
		return NULL;
	if (strcmp(temp->dishname, str) == 0)
		return temp;
	while (strcmp(temp->next->dishname, str) != 0)
		temp = temp->next;
	return temp;
}
//function to check if the name of dish is in the table
Ptorders checkdup(torders *t, char*str)
{
	torders *temp = t;
	if (temp == NULL)
		return temp;
	while (temp != NULL)
	{
		if (strcmp(temp->dishname, str) == 0)
			return temp;
		else
			temp = temp->next;
	}
	return temp;
}


//function to check if the name of dish is in the kitchen
Pdish checkname(kitchen* kit, char * str)
{
	dish *temp = kit->head;
	int check = 0;
	while (temp != NULL && check != 1)
	{
		if (strcmp(str, temp->dishname) == 0)
			check = 1;
		else
			temp = temp->next;
	}
	return temp;
}


//function to let user know that there was an error
void errormessage(char *str)
{
	printf("%s");
	exit(1);
}

//function to free dynamic memory if failed
void freeallocation(kitchen *kitch)
{
	dish *temp;
	while (kitch->head != NULL)
	{
		temp = kitch->head;
		kitch->head = temp->next;
		free(temp->dishname);
		free(temp);
	}
}
//free allocation of table bills
void freeallocation2(table *ta[])
{
	torders *temp;
	int i;
	for (i = 0; i < tmax; i++)
	{
		while (ta[i]->head != NULL)
		{
			temp = ta[i]->head;
			ta[i]->head = temp->next;
			free(temp->dishname);
			free(temp);
		}
	}
}

