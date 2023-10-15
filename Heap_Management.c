// Name : Om Gore
// Enroll no: BT20CSE042
// CPL Assignment 3
#include<stdio.h>
#include<stdlib.h>
#define MAX_MEM 1024

void allocate_first_fit_heap(int id,int size_to_alloc);
void deallocate_heap(int id);

typedef struct allocate_heap_node_tag
{
    int id;
    int size;
    int starting_point;
    int free;
    int end_point;
    struct allocate_heap_node_tag* prev;
    struct allocate_heap_node_tag* next;
}allocate_heap;

allocate_heap* allocate_head;

typedef struct free_heap_node_tag
{
    int size;
    int starting_point;
    int end_point;
    int free;
    struct free_heap_node_tag* prev;
    struct free_heap_node_tag* next;
}free_heap;

free_heap* free_head;

free_heap *make_free_heap_node(int starting,int size);
allocate_heap *make_allocated_node(int starting ,int size,int id);
void free_node_add(free_heap *temp_free_ptr);
allocate_heap* allocate_node_add(free_heap *lptr,int size_to_alloc,int id);


int main(){
    allocate_head=NULL;
    free_head=make_free_heap_node(0,MAX_MEM);
    int choose=1;
    while (choose)
    {
        printf("Choose from the option :\n1)Allocate Memory 2)Deallocate Memory 3)View Allocated 4)View Free 5)Exit\n");
        scanf("%d",&choose);
        if(choose==5){
            choose=0;
        }
        else if(choose==1){
            int id,size;
            printf("To Allocate:\n");
            printf("Enter Process ID: ");
            scanf("%d",&id);
            printf("Enter Memory Block Size: ");
            scanf("%d",&size);
            allocate_first_fit_heap(id,size);
        }
        else if(choose==2){
            int id;
            printf("To Deallocate:\n");
            printf("Enter Process ID: ");
            scanf("%d",&id);
            deallocate_heap(id);
        }
        else if(choose==3){
            allocate_heap *aptr;
            aptr=allocate_head;
            if(aptr==NULL){
                printf("Allocated Heap Memory is Empty.\n");
            }
            else{
                printf("Allocated Heap:\n");
                printf("ProcessID\tMemoryStart\tMemoryEnd\tSize\n");
                while(aptr!=NULL){
                    aptr->end_point=aptr->starting_point+aptr->size;
                    printf("%d\t\t%d\t\t%d\t\t%d\n",aptr->id,aptr->starting_point,aptr->end_point,aptr->size);
                    aptr = aptr->next;
                }
                printf("\n");
            }
        }
        else if(choose==4){
            free_heap *free_ptr;
            free_ptr=free_head;
            printf("Free Heap Memory:\n");
            printf("MemoryStart\tMemoryEnd\n");
            while(free_ptr!=NULL){
                free_ptr->end_point=free_ptr->starting_point+free_ptr->size;
                printf("%d\t\t%d\n",free_ptr->starting_point,free_ptr->end_point);
                free_ptr = free_ptr->next;
            }
            printf("\n");
        }
        else{
            printf("Invalid Option!! Try Again\n");
        }
    }
}
allocate_heap *make_allocated_node(int starting ,int size,int id){
    allocate_heap *temp;
    temp= (allocate_heap*)(malloc(sizeof(allocate_heap)));
    temp->starting_point = starting;
    temp->size=size;
    temp->end_point=temp->starting_point+temp->size;
    temp->id=id;
    temp->free=0;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}

free_heap *make_free_heap_node(int starting,int size){
    free_heap *temp;
    temp= (free_heap*)(malloc(sizeof(free_heap)));
    temp->starting_point = starting;
    temp->size = size;
    temp->end_point=temp->starting_point+temp->size;
    if(temp->end_point>MAX_MEM) temp->end_point=MAX_MEM;
    temp->free=0;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}
allocate_heap* allocate_node_add(free_heap *lptr,int size_to_alloc,int id){
    allocate_heap *allocate_ptr;
    allocate_ptr = make_allocated_node(lptr->starting_point,size_to_alloc,id);
    free(lptr);
    allocate_heap* lptr_allocated=allocate_head;
    if(lptr_allocated==NULL){
        allocate_head=allocate_ptr;
        return allocate_ptr;
    }
    
    if(lptr_allocated==allocate_head ){
        if(allocate_ptr->starting_point < lptr_allocated->starting_point){   
            lptr_allocated->prev=allocate_ptr;
            allocate_ptr->next=lptr_allocated;
            allocate_head=allocate_ptr;
            return allocate_ptr;
        }
    }
 
    int condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    while (lptr_allocated->next!=NULL && condition)
    {
        lptr_allocated=lptr_allocated->next;
        condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    }

    condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    if(lptr_allocated->next==NULL && condition){
        if (allocate_ptr!=NULL)
        {  
            allocate_ptr->next=NULL;
            allocate_ptr->prev=lptr_allocated;
        }
        lptr_allocated->next=allocate_ptr;
        return allocate_ptr;
    }
    
    lptr_allocated->prev->next=allocate_ptr;

    if(allocate_ptr!=NULL){
        allocate_ptr->next=lptr_allocated;
        allocate_ptr->prev=lptr_allocated->prev;
    }
    
    lptr_allocated->prev=allocate_ptr;

    return allocate_ptr;
}
void free_node_add(free_heap *temp_free_ptr){
    if(temp_free_ptr==NULL) return ;
    free_heap *lptr;
    lptr=free_head;
    int condition=lptr->starting_point<temp_free_ptr->starting_point;
    while (condition && lptr->next!=NULL && lptr)
    {
        lptr=lptr->next;
        condition=lptr->starting_point<temp_free_ptr->starting_point;
    }
    condition=lptr->starting_point<temp_free_ptr->starting_point;
    if(condition && lptr->next==NULL && lptr){
        temp_free_ptr->prev=lptr;
        if(lptr!=NULL){
            lptr->next=temp_free_ptr;
        }
        return ;
    }

    if(lptr!=NULL && lptr==free_head){
        temp_free_ptr->next=lptr;
        free_head=temp_free_ptr;
        if(lptr!=NULL){
            lptr->prev=temp_free_ptr;
        }
        return ;  
    } 

    if(lptr!=NULL){
        temp_free_ptr->prev=lptr->prev;
        if(lptr!=NULL){
            lptr->prev->next=temp_free_ptr;
            lptr->prev=temp_free_ptr;
        }
        temp_free_ptr->next=lptr;
    }
}
void allocate_first_fit_heap(int id,int size_to_alloc){
    allocate_heap *aptr;
    aptr=allocate_head;
    while (aptr!=NULL)
    {
        if(aptr->id==id){
            int input;
            printf("Same id allocation found\nChoose 1)To Overwrite 2)Cancel\n");
            scanf("%d",&input);
            if(input==1) deallocate_heap(id);
            else return;
            aptr->next=NULL;
        }
        aptr=aptr->next;
    } 
    free_heap *lptr;
    lptr = free_head;
    while (lptr!=NULL && !(lptr->size>size_to_alloc))
    {
        lptr = lptr->next;
    }
    if(lptr==NULL){
        printf("Error No more free space available.\n");
        return ;
    }
    int last_point = lptr->starting_point + size_to_alloc;

    if(lptr->next==NULL && last_point>MAX_MEM){
        printf("Error No more free space available.\n");
        return ;
    }

    free_heap *free_ptr;
    free_ptr = make_free_heap_node(lptr->starting_point+size_to_alloc,size_to_alloc);

    if(lptr!=free_head){
        lptr->prev->next = free_ptr;
        if(free_ptr!=NULL){
            free_ptr->next=lptr->next;
            free_ptr->prev=lptr->prev;
        }
        if(lptr->next!=NULL) {
            lptr->next->prev=free_ptr;
        }
        int temp_size=lptr->size-size_to_alloc;
        free_ptr->size=temp_size;
    }
    else{
        if(free_ptr!=NULL){
            free_ptr->next = free_head->next;
        }
        free_head=free_ptr;
        int temp_size=lptr->size-size_to_alloc;
        free_ptr->size=temp_size;
    }  
    
    allocate_heap *allocate_ptr=allocate_node_add(lptr,size_to_alloc,id);
    printf("Allocation Done from %d to %d.\n",allocate_ptr->starting_point,allocate_ptr->starting_point+allocate_ptr->size);
    
}

void deallocate_heap(int id){
    allocate_heap *nptr;
    nptr=allocate_head;
    if(nptr==NULL){
        printf("This id is not present.\n");
        return ;
    }
    free_heap *temp_free_ptr;
    while (nptr!=NULL && nptr->id!=id)
    {
        nptr=nptr->next;
        if(nptr==NULL || allocate_head==NULL){
            printf("This id is not present.\n");
            return ;
        }
    }

    temp_free_ptr = make_free_heap_node(nptr->starting_point,nptr->size);
    if(free_head==NULL){
        free_head = temp_free_ptr;
        return;
    }

    if (allocate_head!=nptr)
    {
        nptr->prev->next=nptr->next;
    }
    else{
        allocate_head=nptr->next;
    }

    if(nptr->next!=NULL){
        nptr->next->prev=nptr->prev;
    }

    free_node_add(temp_free_ptr);

    if(temp_free_ptr==NULL){
        return ;
    }

    if(temp_free_ptr!=NULL){
        if(temp_free_ptr->next!=NULL && temp_free_ptr->next->starting_point==temp_free_ptr->starting_point+temp_free_ptr->size){
            int s=temp_free_ptr->next->size;
            temp_free_ptr->next=temp_free_ptr->next->next;
            if(temp_free_ptr->next!=NULL){
                temp_free_ptr->next->prev=temp_free_ptr;
            }
            temp_free_ptr->size=s+temp_free_ptr->size;
        }
    }

    if(temp_free_ptr!=NULL){
        if(temp_free_ptr->prev!=NULL && temp_free_ptr->starting_point==temp_free_ptr->prev->starting_point+temp_free_ptr->prev->size){
            int s=temp_free_ptr->size;
            temp_free_ptr->prev->next=temp_free_ptr->next;
            if(temp_free_ptr->next!=NULL){
                temp_free_ptr->next->prev=temp_free_ptr->prev;
            }
            temp_free_ptr->prev->size=s+temp_free_ptr->prev->size;
            temp_free_ptr=temp_free_ptr->prev;
        }
    }
    return ;
}