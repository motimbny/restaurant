#include "headd.h"

int main()
{
	int flag, i, whe = 0, add = 0, tabn = 0;//flag if input not 0, whe is to know what function to operate
	FILE *ins;
	kitchen *p;
	dish *temp;
	table t[tmax];
	torders *tmp;
	char tdish[MAX];
	for (int i = 0; i < tmax; i++)
	{
		t[i].head = NULL;
		t[i].bill = 0;
		t[i].totaldish = 0;
	}
	p = (kitchen*)malloc(sizeof(kitchen));
	if (p == NULL)
	{
		printf("ERROR!!! allocation fail");
		exit(1);
	}
	p->head = p->tail = NULL;
	p->size = 0;
	ins = fopen("Instructions.txt", "r");
	if (ins == NULL)
	{
		printf("ERROR!!! couldnt open file");
		free(p);
		exit(1);
	}
	if (!checkfile(ins))  //if file is empty
	{
		printf("ERROR!!! file is empty");
		free(p);
		exit(1);
	}
	else
	{
		Printbbb();
		scanf("%d", &flag);
		while (flag != 0)
		{
			printf("Error! wrong input! Please enter 0 to continue: ");
			scanf("%d", &flag);
		}
		fscanf(ins, "%d", &whe); //if 1 is not first 
		if (whe == 1)
		{
			CreateProduct(p);
			while (!feof(ins))
			{
				Printbbb();
				scanf("%d", &flag);
				while (flag != 0)
				{
					printf("Error! wrong input! Please enter 0 to continue: ");
					scanf("%d", &flag);
				}
				fscanf(ins, "%d", &whe);
				printf("\n");
				if (whe == 1)
					CreateProduct(p);
				else if (whe == 2)
				{
					fscanf(ins, "%s", tdish);
					fscanf(ins, "%d", &add);
					AddItems(tdish, add, p);
				}
				else if (whe == 3)
				{
					fscanf(ins, "%d", &tabn);
					fscanf(ins, "%s", tdish);
					fscanf(ins, "%d", &add);
					OrderItem(tdish, add, tabn, &t, p);
				}
				else if (whe == 4)
				{
					fscanf(ins, "%d", &tabn);
					fscanf(ins, "%s", tdish);
					fscanf(ins, "%d", &add);
					RemoveItem(tabn, tdish, add, &t, p);
				}
				else if (whe == 5)
				{
					fscanf(ins, "%d", &tabn);
					RemoveTable(&t, tabn);
				}
			}
			printf("\n Resturant is closed! bye bye!");
			for (i = 0; i < tmax; i++) //free dynamic memory of arry
			{
			 if (t[i].head != NULL)
			    {
			      tmp = t[i].head;
			      t[i].head = t[i].head->next;
			      free(tmp->dishname);
			     free(tmp);
			    }
			}
			for (i = 0; i < p->size; i++) //free dynamic memory of kitchen list
			{
			temp = p->head;
			p->head = p->head->next;
			free(temp->dishname);
			free(temp);
			}
		}
		else
		{
			printf("\n There was a problem with file instruction's. make sure 1 is first");
		}
		free(p);
		fclose(ins); //closing file
	}
}
