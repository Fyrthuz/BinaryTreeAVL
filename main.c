#include <stdio.h>
#include <stdlib.h>
#define INORDER 1
#define PREORDER 2
#define POSTORDER 3

struct tnodo{
    int data;
    int fe;
    int height;
    struct tnodo *left, *right, *parent;
};

typedef struct tnodo *arbol;


arbol raiz = NULL;
int born = 0;

void init (arbol *a,int data){
    *a = (arbol)malloc(sizeof(struct tnodo));
    (*a)->data = data;
    (*a)->fe = 0;
    (*a)->left = NULL;
    (*a)->right = NULL;
    (*a)->parent = NULL;
}



void calcularfenodo(arbol *a){
    if((*a)->left != NULL && (*a)->right != NULL){
        //cambiarlo por la resta de las alturas de los hijos
        (*a)->fe = (*a)->right->height - (*a)->left->height;
    }
    else if((*a)->right != NULL){
        (*a)->fe = (*a)->right->height;
    }
    else if((*a)->left != NULL){
        (*a)->fe = -(*a)->left->height;
    }
    else{
        (*a)->fe = 0;
    }
}

void recorrido(arbol *a){
    if(*a != NULL){
        recorrido(&(*a)->left);
        recorrido(&(*a)->right);
        calcularfenodo(a);
    }
}

int altura_nodos(arbol *a){
    if(*a != NULL){
        int x = altura_nodos(&(*a)->left);
        int y = altura_nodos(&(*a)->right);
        if(x > y){
            (*a)->height = x + 1;
            return x + 1;
        }else{
            (*a)->height = y + 1;
            return y + 1;
        }
    }
    return 0;
}


void rotacion_LL(arbol *a){
    printf("Rotacion LL\n");
    arbol aux = (*a)->left;

    (*a)->left = aux->right;    

    aux->right = *a;
    aux->parent = (*a)->parent;
    (*a)->parent = aux;

    if(aux->parent == NULL){
        raiz=aux;
    }
    *a = aux;

}

void rotacion_RR(arbol *a){
    
    printf("Rotacion RR\n");
    arbol aux = (*a)->right;

    (*a)->right = aux->left;

    aux->left = *a;

    aux->parent = (*a)->parent;

    (*a)->parent = aux;

    if(aux->parent == NULL){
        raiz=aux;
    }
    *a = aux;
    

}


void rotacion_RL(arbol *a){
    arbol *n1,*n2;
    printf("Rotacion RL\n");
    n1 = &(*a)->left;
    n2 = &(*n1)->right;
    (*a)->left = (*n2)->right;
    (*n2)->right = *n1;
    (*n1)->right = (*n2)->left;
    (*n2)->left = *a;
    (*n2)->parent = (*a)->parent;
    (*a)->parent = *n2;
    (*n1)->parent = *n2;
    if((*n2)->parent == NULL){
        raiz = *n2;
    }
    *a = *n2;
}

void rotacion_LR(arbol *a){
    arbol n1,n2;
    n1 = (*a)->left;
    n2 = n1->right;
    printf("Rotacion LR\n");
    n1->right = n2->left;
    n2->left = n1;
    (*a)->left = n2->right;
    n2->right = *a;
    n2->parent = (*a)->parent;
    n1->parent = n2;
    (*a)->parent = n2;
    if(n2->parent == NULL){
        raiz=n2;
    }
    *a = n2;
}


void do_rotations(arbol *a){
    if((*a)->left != NULL){
        do_rotations(&(*a)->left);
    }
    if((*a)->right != NULL){
        do_rotations(&(*a)->right);
    }
    if(*a != NULL){
        if((*a)->fe == -2 && (*a)->left != NULL){
            if((*a)->left->fe == -1){
                rotacion_LL(a);
            }
            else{
                rotacion_LR(a);
            }
        }
        else if((*a)->fe == 2 && (*a)->right != NULL){
            if((*a)->right->fe == 1){
                rotacion_RR(a);
            }
            else{
                rotacion_RL(a);
            }
        }
    }
    altura_nodos(&raiz);
    recorrido(&raiz);

}

void insert(arbol *a, int data, arbol parent){
    if(!born){
        init(a,data);
        (*a)->parent = parent;
        raiz = *a;
        born = 1;
        (*a)->height = 0;
    }else{
        if(data < (*a)->data){
            if((*a)->left == NULL){
                init(&(*a)->left,data);
                (*a)->left->parent = parent;
                (*a)->left->height = 0;
            }else{
                insert(&(*a)->left,data,*a);
            }
        }else{
            if((*a)->right == NULL){
                init(&(*a)->right,data);
                (*a)->right->parent = parent;
                (*a)->right->height = 0;
            }else{
                insert(&(*a)->right,data,*a);
            }
        }
    }

}


void add_element(arbol *a, int data){

    insert(a,data,*a);
    altura_nodos(a);
    recorrido(a);
    do_rotations(a);
    altura_nodos(a);
    recorrido(a);
}


void print_preorder(arbol a){
    if(a != NULL){
        printf("Nodo: ");
        printf("%d ",a->data);
        printf("%d \n",a->fe);
        print_preorder(a->left);
        print_preorder(a->right);
    }
}

void print_inorder(arbol a){
    if(a != NULL){
        print_inorder(a->left);
        printf("Nodo: ");
        printf("%d ",a->data);
        printf("%d \n",a->fe);
        print_inorder(a->right);
    }
}

void print_postorder(arbol a){
    if(a != NULL){
        print_postorder(a->left);
        print_postorder(a->right);
        printf("Nodo: ");
        printf("%d ",a->data);
        printf("%d \n",a->fe);
    }
}

void print_tree(arbol a, int option){
    printf("Tree:\n");
    switch(option){
        case INORDER:
            print_inorder(a);
            break;
        case PREORDER:
            print_preorder(a);
            break;
        case POSTORDER:
            print_postorder(a);
            break;
        default:
            printf("Invalid option\n");
            break;
    }
    printf("\n\n");
}

void delete(arbol *a,int num){
    if(*a != NULL){
        if((*a)->data == num){
            if((*a)->left == NULL && (*a)->right == NULL){
                free(*a);
                *a = NULL;
            }else if((*a)->left == NULL){
                arbol aux = *a;
                *a = (*a)->right;
                (*a)->parent = aux->parent;
                free(aux);
            }else if((*a)->right == NULL){
                arbol aux = *a;
                *a = (*a)->left;
                (*a)->parent = aux->parent;
                free(aux);
            }else{
                arbol aux = *a;
                *a = (*a)->right;
                while((*a)->left != NULL){
                    *a = (*a)->left;
                }
                (*a)->left = aux->left;
                (*a)->right = aux->right;
                (*a)->parent = aux->parent;
                free(aux);
            }
        }else{
            delete(&(*a)->left,num);
            delete(&(*a)->right,num);
        }
    }
}

void delete_nodo(arbol *a,int num){
    delete(a,num);
    altura_nodos(a);
    recorrido(a);
    do_rotations(a);
    altura_nodos(a);
    recorrido(a);
}

void free_memory(arbol *a){
    if(*a != NULL){
        free_memory(&(*a)->left);
        free_memory(&(*a)->right);
        free(*a);
        *a = NULL;
    }
}


int menu(){
    int option;
    printf("\n-------------------------MENU-------------------------\n");
    printf("1. Insertar\n");
    printf("2. Eliminar\n");
    printf("3. Imprimir\n");
    printf("4. Salir\n");
    printf("Ingrese una opcion: ");
    scanf("%d",&option);
    switch(option){
        case 1:
            printf("\nIngrese un numero: ");
            int num;
            scanf("%d",&num);
            add_element(&raiz,num);
            break;
        case 2:
            printf("\nIngrese un numero: ");
            int num2;
            scanf("%d",&num2);
            delete_nodo(&raiz,num2);
            break;
        case 3:
            printf("\n1. Inorden\n");
            printf("2. Preorden\n");
            printf("3. Postorden\n");
            printf("Ingrese una opcion: ");
            int option2;
            scanf("%d",&option2);
            print_tree(raiz,option2);
            break;
        case 4:
            return 0;
            break;
        default:
            printf("\nOpcion no valida\n");
            return 1;
    }
    printf("-------------------------------------------------------\n");
    return 1;

}

int main(){
    int state = 1;
    while(state){
        state = menu();
    }
    printf("\n\nFreeing memory\n");
    free_memory(&raiz);
    printf("Memory freed\n");
    return 0;
}