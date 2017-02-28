#ifndef BST_H_DEFINED
#define BST_H_DEFINED

#include <stdio.h>

typedef struct  binarysearchtree *BST;
typedef char* Key;


BST	    BSTinit() ;
BST	    BSTread(FILE *) ;
int		BSTcount(BST) ;
int     BSTempty(BST) ;
Item    BSTmin(BST) ;
Item    BSTmax(BST) ;
void 	BSTinsert_leafI(BST,Item) ;
void 	BSTinsert_leafR(BST,Item) ;
void 	BSTinsert_root(BST,Item) ;
Item	BSTsearch(BST,Key) ;
void	BSTdelete(BST,Item) ;
Item 	BSTselect(BST,int) ;
void	BSTsortinorder(BST,void (*visit) (Item, FILE *), FILE *fp) ;
void	BSTsortpreorder(BST,void (*visit) (Item, FILE *), FILE *fp) ;
void	BSTsortpostorder(BST,void (*visit) (Item, FILE *), FILE *fp) ;
Item	BSTsucc(BST,Item) ;
Item	BSTpred(BST,Item) ;
int     BSTdepth(BST, int *, int *) ;
void BSTfree(BST) ;

#endif
