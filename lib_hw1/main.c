#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"

//store the name of the list and the list itself
struct list_sub
{	
	char name[10];
	struct list element;
};
//the node of the list between the head and the tail
struct list_node
{
	int numinfo;
	struct list_elem pointer;
};

struct hash_sub
{
	char name[10];
	struct hash element;
};
struct hash_node
{
	int numinfo;
	struct hash_elem pointer;
};

struct bitmap_sub
{
	char name[10];
	struct bitmap *element;
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
bool list_less(const struct list_elem *a,const struct list_elem *b,void *aux);

unsigned hash_hash(const struct hash_elem *e,void *aux);
bool hash_less(const struct hash_elem *a,const struct hash_elem *b,void *aux);
void destructor(struct hash_elem *e,void *aux);
void square(struct hash_elem *e,void *aux);
void triple(struct hash_elem *e,void *aux);
void create_hash(struct hash_sub *hash, char *name);
int search_hash(struct hash_sub *hash, char *name);
struct hash_node *newnode_hash(struct hash_sub *hash, int num);
unsigned hash_int_2(int i);

void create_bitmap(struct bitmap_sub *bitmap, char *name,size_t size);
int search_bitmap(struct bitmap_sub *bitmap, char *name);
bool strtobool(char *name);
struct bitmap *bitmap_expand(struct bitmap *bitmap,int size);

int main(void)
{
	char tp, command[20], sub_com[10], name[10], name2[10];
	int num1, num2, num3;
	int i,j,index;
	int *tmp;

	struct list_sub list[10];
	struct list_elem *temp, *temp2;

	struct hash_sub hash[10];
	struct hash_node *newnode;
	struct hash_elem *temp0;
	struct hash_iterator hash_i;

	struct bitmap_sub bitmap[10];

	for(i=0;i<10;i++)
	{
		strcpy(list[i].name,"\0");
		strcpy(hash[i].name,"\0");
		strcpy(bitmap[i].name,"\0");
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
				create_hash(hash,name);
			if(!strcmp(sub_com,"bitmap"))
			{
				scanf("%d",&num1);
				create_bitmap(bitmap,name,(size_t)num1);
			}
		}
		else{
			scanf("%s",name); //name of structure

			if(!strcmp(command,"delete"))
			{
				index = search_list(list,name);
				if(index > -1 && index < 10)
				{
					for(temp = list_begin(&list[index].element); temp != list_end(&list[index].element); temp = list_remove(temp));
					strcpy(list[index].name,"\0");
				}
				else
				{
					index = search_hash(hash,name);
					if(index > -1 && index < 10)
					{
						hash_clear(&hash[index].element,NULL);
						strcpy(hash[index].name,"\0");
						//hash_destroy(&hash[i].element,destructor);
					}
					else
					{
						index = search_bitmap(bitmap,name);
						bitmap_destroy(bitmap[index].element);
						strcpy(bitmap[index].name,"\0");
					}
				}

			}	
			else if(!strcmp(command,"dumpdata"))
			{
				index = search_list(list,name);
				if(index > -1 && index < 10)
				{
					if(list_size(&(list[index].element)) == 0)
						continue;
					temp = list_head(&(list[index].element));
					while((temp = list_next(temp)) != list_end(&list[index].element))
						printf("%d ",list_entry(temp,struct list_node,pointer)->numinfo);
					printf("\n");
				}
				else
				{
					index = search_hash(hash,name);
					if(index > -1 && index < 10)
					{
						if(hash_size(&(hash[index].element)) == 0)
							continue;
						hash_first(&hash_i, &hash[index].element);
						while(hash_next(&hash_i))
							printf("%d ",hash_entry(hash_cur(&hash_i),struct hash_node,pointer)->numinfo);
						printf("\n");
					}
					else
					{
						index = search_bitmap(bitmap,name);
						for(i=0;i<(int)bitmap_size(bitmap[index].element);i++)
						{
							if(bitmap_test(bitmap[index].element,(size_t)i))
								printf("1");
							else
								printf("0");
						}
						printf("\n");
					}
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
				for(temp = list_begin(&(list[index].element)),i=0; i<num2; temp = list_next(temp))
					i++;
				index = search_list(list,name); //find the list with "name"
				//insert the (num2)th node to the (num3)th node of the list with "name2" to the list with "name" at (num1)th location and remove them from their list
				for(i=num2;i<num3;i++)
				{
					insert_list(&list[index],list_entry(temp,struct list_node,pointer)->numinfo,num1+i-num2);
					temp = list_remove(temp);
				}
			}
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
				insert_list(&list[index],num1,list_size(&(list[index].element)));	
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
			
			else if(!strcmp(command,"list_sort"))
			{
				index = search_list(list,name);
				list_sort(&(list[index].element),list_less,NULL);
			}
			else if(!strcmp(command,"list_insert_ordered"))
			{
				index = search_list(list,name);
				scanf("%d",&num1);
				insert_list(&list[index],num1,0);
				list_sort(&(list[index].element),list_less,NULL);
			}
			else if(!strcmp(command,"list_unique"))
			{
				num1 = search_list(list,name);
				scanf("%c",&tp);
				if(tp == ' ')
				{
					scanf("%s",name2);
					num2 = search_list(list,name2);

					num3 = list_size(&list[num1].element);
					tmp = (int *)malloc(sizeof(int)*num3);
					for(temp = list_begin(&list[num1].element), i=0; temp != list_end(&list[num1].element); temp = list_next(temp), i++)
						tmp[i] = list_entry(temp,struct list_node,pointer)->numinfo;
					
					list_unique(&(list[num1].element),NULL,list_less,NULL);
				
					temp = list_begin(&list[num1].element);
					for(i=0,j=0;i<num3;i++)
					{
						if(list_entry(temp,struct list_node,pointer)->numinfo != tmp[i])
						{
							insert_list(&list[num2],tmp[i],j);
							j++;
						}
						else
							temp = list_next(temp);
					}
				}
				else
					list_unique(&(list[num1].element),NULL,list_less,NULL);

			}
			else if(!strcmp(command,"list_max"))
			{
				index = search_list(list,name);
				printf("%d\n",list_entry(list_max(&(list[index].element),list_less,NULL),struct list_node,pointer)->numinfo);
			}
			else if(!strcmp(command,"list_min"))
			{
				index = search_list(list,name);
				printf("%d\n",list_entry(list_min(&(list[index].element),list_less,NULL),struct list_node,pointer)->numinfo);
			}


			else if(!strcmp(command,"hash_insert"))
			{
				index = search_hash(hash,name);
				scanf("%d", &num1);
				newnode = newnode_hash(&hash[index],num1);
				hash_insert(&(hash[index].element),&(newnode->pointer)); //insert new node in the list
			}
			else if(!strcmp(command,"hash_replace"))
			{
				index = search_hash(hash,name);
				scanf("%d",&num1);
				newnode = newnode_hash(&hash[index],num1);
				hash_replace(&(hash[index].element),&(newnode->pointer)); //insert new node in the list
			}
			else if(!strcmp(command,"hash_find"))
			{
				index = search_hash(hash,name);
				scanf("%d",&num1);
				newnode = newnode_hash(&hash[index],num1);
				temp0 = hash_find(&hash[index].element,&(newnode->pointer));
				if(temp0 != NULL)
					printf("%d\n",hash_entry(temp0,struct hash_node,pointer)->numinfo);
			}
			else if(!strcmp(command,"hash_delete"))
			{
				index = search_hash(hash,name);
				scanf("%d",&num1);
				newnode = newnode_hash(&hash[index],num1);
				hash_delete(&hash[index].element,&(newnode->pointer));
			}
			else if(!strcmp(command,"hash_clear"))
			{
				index = search_hash(hash,name);
				hash_clear(&hash[index].element,NULL);
			}
			else if(!strcmp(command,"hash_size"))
			{
				index = search_hash(hash,name);
				printf("%d\n",(int)hash_size(&hash[index].element));
			}
			else if(!strcmp(command,"hash_empty"))
			{
				index = search_hash(hash,name);
				if(hash_empty(&hash[index].element))
					printf("true\n");
				else
					printf("false\n");

			}
			else if(!strcmp(command,"hash_apply"))
			{
				index = search_hash(hash,name);
				scanf("%s",name2);
				if(!strcmp(name2,"square"))
					hash_apply(&hash[index].element,square);
				else
					hash_apply(&hash[index].element,triple);
			}

			
			else if(!strcmp(command,"bitmap_size"))
			{
				index = search_bitmap(bitmap,name);
				printf("%d\n",(int)bitmap_size(bitmap[index].element));
			}	
			else if(!strcmp(command,"bitmap_set"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %s",&num1,name2);
				bitmap_set(bitmap[index].element,(size_t)num1,strtobool(name2));
			}	
			else if(!strcmp(command,"bitmap_mark"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d",&num1);
				bitmap_mark(bitmap[index].element,(size_t)num1);
			}
			else if(!strcmp(command,"bitmap_reset"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d",&num1);
				bitmap_reset(bitmap[index].element,(size_t)num1);
			}
			else if(!strcmp(command,"bitmap_flip"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d",&num1);
				bitmap_flip(bitmap[index].element,(size_t)num1);
			}
			else if(!strcmp(command,"bitmap_test"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d",&num1);
				if(bitmap_test(bitmap[index].element,(size_t)num1))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"bitmap_set_all"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%s",name2);
				bitmap_set_all(bitmap[index].element,strtobool(name2));
			}
			else if(!strcmp(command,"bitmap_set_multiple"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d %s",&num1,&num2,name2);
				bitmap_set_multiple(bitmap[index].element,(size_t)num1,(size_t)num2,strtobool(name2));
			}
			else if(!strcmp(command,"bitmap_count"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d %s",&num1,&num2,name2);
				printf("%d\n",bitmap_count(bitmap[index].element,(size_t)num1,(size_t)num2,strtobool(name2)));
			}
			else if(!strcmp(command,"bitmap_contains"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d %s",&num1,&num2,name2);
				if(bitmap_contains(bitmap[index].element,(size_t)num1,(size_t)num2,strtobool(name2)))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"bitmap_any"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d",&num1,&num2);
				if(bitmap_any(bitmap[index].element,(size_t)num1,(size_t)num2))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"bitmap_none"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d",&num1,&num2);
				if(bitmap_none(bitmap[index].element,(size_t)num1,(size_t)num2))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"bitmap_all"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d",&num1,&num2);
				if(bitmap_all(bitmap[index].element,(size_t)num1,(size_t)num2))
					printf("true\n");
				else
					printf("false\n");
			}
			else if(!strcmp(command,"bitmap_scan"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d %s",&num1,&num2,name2);
				printf("%zu\n",bitmap_scan(bitmap[index].element,(size_t)num1,(size_t)num2,strtobool(name2)));
			}
			else if(!strcmp(command,"bitmap_scan_and_flip"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d %d %s",&num1,&num2,name2);
				printf("%zu\n",bitmap_scan_and_flip(bitmap[index].element,(size_t)num1,(size_t)num2,strtobool(name2)));
			}
			else if(!strcmp(command,"bitmap_dump"))
			{
				index = search_bitmap(bitmap,name);
				bitmap_dump(bitmap[index].element);
			}
			else if(!strcmp(command,"bitmap_expand"))
			{
				index = search_bitmap(bitmap,name);
				scanf("%d",&num1);
				bitmap[index].element = bitmap_expand(bitmap[index].element,num1);
			}

			else
			{
			   printf("There is no such command!\n");
			   exit(-1);
			}
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
bool list_less(const struct list_elem *a,const struct list_elem *b,void *aux)
{
	int num1 = list_entry(a,struct list_node,pointer)->numinfo;
	int num2 = list_entry(b,struct list_node,pointer)->numinfo;

	if(num1<num2)
		return true;
	else
		return false;
}


unsigned hash_hash(const struct hash_elem *e,void *aux)
{
	return hash_int(hash_entry(e,struct hash_node,pointer)->numinfo);
}
bool hash_less(const struct hash_elem *a,const struct hash_elem *b,void *aux)
{
	int num1 = hash_entry(a,struct hash_node,pointer)->numinfo;
	int num2 = hash_entry(b,struct hash_node,pointer)->numinfo;
	if(num1<num2)
		return true;
	else
		return false;
}
void destructor(struct hash_elem *e,void *aux)
{
	free(e);
}
void square(struct hash_elem *e,void *aux)
{
	int num;
	num = hash_entry(e,struct hash_node,pointer)->numinfo;
	hash_entry(e,struct hash_node,pointer)->numinfo = num*num;
}
void triple(struct hash_elem *e,void *aux)
{
	int num;
	num = hash_entry(e,struct hash_node,pointer)->numinfo;
	hash_entry(e,struct hash_node,pointer)->numinfo = num*num*num;
}
void create_hash(struct hash_sub *hash, char *name)
{
	int i=0;
	while(strlen(hash[i].name) != 0) //find the hashtable not allocated yet
		i++;
	//initializing the list
	strcpy(hash[i].name,name);
	hash_init(&hash[i].element,hash_hash,hash_less,NULL);
}
int search_hash(struct hash_sub *hash, char *name)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(!strcmp(hash[i].name,name))
			return i;
	}
}
struct hash_node *newnode_hash(struct hash_sub *hash, int num)
{
	struct hash_node *h_node;
	struct hash_elem *temp = (struct hash_elem *)malloc(sizeof(struct hash_elem));

	h_node = hash_entry(temp,struct hash_node,pointer);
	h_node->numinfo = num;
	return h_node;
}
unsigned hash_int_2(int i)
{
	return (i*i*i)%SIZE_MAX;
}

void create_bitmap(struct bitmap_sub *bitmap, char *name, size_t size)
{
	int i=0;
	while(strlen(bitmap[i].name) != 0) //find the hashtable not allocated yet
		i++;
	//initializing the list
	strcpy(bitmap[i].name,name);
	bitmap[i].element = bitmap_create(size);
}
int search_bitmap(struct bitmap_sub *bitmap, char *name)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(!strcmp(bitmap[i].name,name))
			return i;
	}
}
bool strtobool(char *name)
{
	if(!strcmp(name,"true"))
		return true;
	else
		return false;
}
struct bitmap *bitmap_expand(struct bitmap *bitmap,int size)
{
	int i;
	size_t len = bitmap_size(bitmap) + (size_t)size;
	struct bitmap *temp = bitmap_create(len);
	for(i=0;i<(int)bitmap_size(bitmap);i++)
		bitmap_set(temp,(size_t)i,bitmap_test(bitmap,(size_t)i));
	return temp;
}
