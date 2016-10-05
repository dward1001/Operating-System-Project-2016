#include <stdio.h>
#include <time.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"

//store the name of the list and the list itself
struct list_sub
{	
	int flag;
	char name[10];
	struct list element;
};
//the node of the list between the head and the tail
struct list_node
{
	int numinfo;
	struct list_elem pointer;
};

/*
   List
   - list_insert, list_splice, list_push, list_push_front, list_push_back
   - list_remove, list_pop_front, list_pop_back, list_front, list_back
   - list_size, list_empty
   - list_reverse, list_sort, list_insert_ordered, list_unique
   - list_max, list_min
   Hashtable
   - hash_insert, hash_replace, hash_find, hash_delete
   - hash_clear, hash_size, hash_empty, hash_apply
   Bitmap
   - bitmap_size, bitmap_set, bitmap_mark, bitmap_reset, bitmap_flip, bitmap_test
   - bitmap_set_all, bitmap_set_multiple, bitmap_count, bitmap_contains
   - bitmap_any, bitmap_none, bitmap_all
   - bitmap_scan, bitmap_scan_and_flip, bitmap_dump
 */

void create_list(struct list_sub *list, char *name);
int search_list(struct list_sub *list, char *name);
void insert_list(struct list_sub *list, int num, int order);
void list_swap(struct list_elem *a,struct list_elem *b);
void list_shuffle(struct list *list);

int main(void)
{
	char command[20], sub_com[10], name[10], name2[10];
	int num1, num2, num3;
	int i,j,index;

	struct list_sub list[10];
	struct list_elem *temp, *temp2;

	for(i=0;i<10;i++)
	{
		list[i].flag = 0;
		strcpy(list[i].name,"\0");
	}

	while(1)
	{		
		scanf("%s",command);

		if(!strcmp(command,"quit"))
			return 0;

		else if(!strcmp(command,"create"))
		{
			scanf("%s",sub_com); //name of type
			scanf("%s",name); //name of structure
			if(!strcmp(sub_com,"list"))
				create_list(list,name);
			if(!strcmp(sub_com,"hashtable"))
			{
				//
			}
			if(!strcmp(sub_com,"bitmap"))
			{
				//
			}
		}
		else{
			scanf("%s",name); //name of structure

			if(!strcmp(command,"delete"))
			{
				index = search_list(list,name);
				if(index < 10)
				{
					for(temp = list_begin(&list[index].element); temp != list_end(&list[index].element); temp = list_remove(temp));
					strcpy(list[index].name,"\0");
				}
				else
				{
				}

			}	
			else if(!strcmp(command,"dumpdata"))
			{
				index = search_list(list,name);
				if(index < 10)
				{
					if(list[index].flag == 0)
						continue;
					temp = list_head(&(list[index].element));
					while((temp = list_next(temp)) != list_end(&list[index].element))
						printf("%d ",list_entry(temp,struct list_node,pointer)->numinfo);
					printf("\n");
				}
				else
				{
				}
			}	

			//list_insert list0 0 1
			//inserting the node with "num2" at (num1)th location in the list with "name"
			else if(!strcmp(command,"list_insert"))
			{
				index = search_list(list,name); //find the list with "name"
				scanf("%d %d", &num1, &num2); //put num2 at (num1)th location
				insert_list(&list[index],num2,num1);

				temp = list_head(&(list[index].element));
				temp = list_next(temp);
			}
			//list_splice list0 2 list1 1 4
			//remove the (num2)th node to (num3)th node of the list with "name2" and insert them to the list with "name" at (num1)th location 
			else if(!strcmp(command,"list_splice"))
			{
				scanf("%d %s %d %d",&num1,name2,&num2,&num3);
				index = search_list(list,name2); //find the list with "name2"
				//temp = list_head(&(list[index].element)); 
				for(temp = list_begin(&(list[index].element)),i=0; i<num2; temp = list_next(temp))
					i++;
				//for(i=0;i<=num2;i++)
				//	temp = list_next(temp); //find the (num2)th node in the list with "name2"
				index = search_list(list,name); //find the list with "name"
				//insert the (num2)th node to the (num3)th node of the list with "name2" to the list with "name" at (num1)th location and remove them from their list
				for(i=num2;i<num3;i++)
				{
					insert_list(&list[index],list_entry(temp,struct list_node,pointer)->numinfo,num1+i-num2);
					temp = list_remove(temp);
				}
			}
			/*
			   else if(!strcmp(command,"list_push"))
			   {

			   }
			 */
			else if(!strcmp(command,"list_push_front"))
			{
				scanf("%d",&num1);
				index = search_list(list,name);
				insert_list(&list[index],num1,0);	
			}
			else if(!strcmp(command,"list_push_back"))
			{
				scanf("%d",&num1);
				index = search_list(list,name);
				insert_list(&list[index],num1,list[index].flag);	
			}
			else if(!strcmp(command,"list_remove"))
			{
				scanf("%d",&num1);
				index = search_list(list,name);
				for(temp = list_begin(&(list[index].element)),i=0; i<num1; temp = list_next(temp))
					i++;
				list_remove(temp);
			}
			else if(!strcmp(command,"list_pop_front"))
			{
				scanf("%d",&num1);
				index = search_list(list,name);
				list_pop_front(&(list[index].element));
			}
			else if(!strcmp(command,"list_pop_back"))
			{
				scanf("%d",&num1);
				index = search_list(list,name);
				list_pop_back(&(list[index].element));
			}
			else if(!strcmp(command,"list_front"))
			{
				index = search_list(list,name);
				printf("%d\n",list_entry(list_front(&list[index].element),struct list_node,pointer)->numinfo);
			}
			else if(!strcmp(command,"list_back"))
			{
				index = search_list(list,name);
				printf("%d\n",list_entry(list_back(&list[index].element),struct list_node,pointer)->numinfo);
			}
			else if(!strcmp(command,"list_size"))
			{
				index = search_list(list,name);
				printf("%d\n",(int)list_size(&list[index].element));	
			}
			else if(!strcmp(command,"list_empty"))
			{
				index = search_list(list,name);
				if(list_empty(&list[index].element))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"list_reverse"))
			{
				index = search_list(list,name);
				list_reverse(&(list[index].element));
			}
			else if(!strcmp(command,"list_swap"))
			{
				scanf("%d %d",&num1,&num2);
				index = search_list(list,name);
				for(temp = list_begin(&(list[index].element)),i=0; i<num1; temp = list_next(temp))
					i++;
				for(temp2 = list_begin(&(list[index].element)),i=0; i<num2; temp2 = list_next(temp2))
					i++;
				list_swap(&(list_entry(temp,struct list_node,pointer)->pointer),&(list_entry(temp2,struct list_node,pointer)->pointer));
			}
			else if(!strcmp(command,"list_shuffle"))
			{
				index = search_list(list,name);
				list_shuffle(&(list[index].element));	
			}
			/*
			   else if(!strcmp(command,"list_sort"))
			   {
			   }
			   else if(!strcmp(command,"list_insert_ordered"))
			   {
			   }
			   else if(!strcmp(command,"list_unique"))
			   {
			   }
			   else if(!strcmp(command,"list_max"))
			   {

			   }
			   else if(!strcmp(command,"list_min"))
			   {
			   }
			 */

			else if(!strcmp(command,"hash_insert"))
			{
			}
			else if(!strcmp(command,"hash_replace"))
			{
			}
			else if(!strcmp(command,"hash_find"))
			{
			}
			else if(!strcmp(command,"hash_delete"))
			{
			}
			else if(!strcmp(command,"hash_clear"))
			{
			}
			else if(!strcmp(command,"hash_size"))
			{
			}
			else if(!strcmp(command,"hash_empty"))
			{
			}
			else if(!strcmp(command,"hash_apply"))
			{
			}

			/*
			   else if(!strcmp(command,"bitmap_size"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_set"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_mark"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_reset"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_flip"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_test"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_set_all"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_set_multiple"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_count"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_contains"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_any"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_none"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_all"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_scan"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_scan_and_flip"))
			   {
			   }	
			   else if(!strcmp(command,"bitmap_dump"))
			   {
			   }
			   else if(!strcmp(command,"bitmap_expand"))
			   {
			   }

			   else
			   {
			   printf("There is no such command!\n");
			   exit(-1);
			   }*/
		}
	}
	return 0;
}

void create_list(struct list_sub *list,char *name)
{
	int i=0;
	while(strlen(list[i].name) != 0) //find the list not allocated yet
		i++;
	//initializing the list
	strcpy(list[i].name,name);
	list_init(&list[i].element);
}
//search the list with "name"
int search_list(struct list_sub *list, char *name)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(!strcmp(list[i].name,name))
			return i;
	}
}
//insert the new node having num in the list at (order)th location
void insert_list(struct list_sub *list, int num, int order)
{
	struct list_node *l_node;
	struct list_elem *temp;
	struct list_elem *temp2 = (struct list_elem *)malloc(sizeof(struct list_elem));
	int i=0;

	for(temp = list_begin(&(list->element)); i<order; temp = list_next(temp))
		i++;
	l_node = list_entry(temp2,struct list_node,pointer);
	l_node->numinfo = num;	
	list_insert(temp,&(l_node->pointer)); //insert new node in the list
	list->flag++;
}
void list_swap(struct list_elem *a,struct list_elem *b)
{
	int temp;
	int num1 = list_entry(a,struct list_node,pointer)->numinfo;	
	int num2 = list_entry(b,struct list_node,pointer)->numinfo;	

	temp = num1;
	list_entry(a,struct list_node,pointer)->numinfo = num2;	
	list_entry(b,struct list_node,pointer)->numinfo = temp;	
}
void list_shuffle(struct list *list)
{
	int num1, num2, size, i, j;
	struct list_elem *temp, *temp2;

	size = (int)list_size(list);
	for(i=0;i<size/2;i++)
	{
		num1 = rand()%size;
		num2 = rand()%size;
		for(temp = list_begin(list),j=0; j<num1; temp = list_next(temp))
			j++;
		for(temp2 = list_begin(list),j=0; j<num2; temp2 = list_next(temp2))
			j++;
		list_swap(&(list_entry(temp,struct list_node,pointer)->pointer),&(list_entry(temp2,struct list_node,pointer)->pointer));
	}	
}
