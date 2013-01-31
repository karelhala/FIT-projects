
/* c402.c: ********************************************************************}
{* T�ma: Nerekurzivn� implementace operac� nad BVS 
**                                     Implementace: Petr P�ikryl, prosinec 1994
**                                           �pravy: Petr P�ikryl, listopad 1997
**                                                     Petr P�ikryl, kv�ten 1998
**			  	                        P�evod do jazyka C: Martin Tu�ek, srpen 2005
**                                         �pravy: Bohuslav K�ena, listopad 2009
**
** S vyu�it�m dynamick�ho p�id�lov�n� pam�ti, implementujte NEREKURZIVN�
** n�sleduj�c� operace nad bin�rn�m vyhled�vac�m stromem (p�edpona BT znamen�
** Binary Tree a je u identifik�tor� uvedena kv�li mo�n� kolizi s ostatn�mi
** p��klady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivn� vlo�en� nov�ho uzlu do stromu
**     BTPreorder ...... nerekurzivn� pr�chod typu pre-order
**     BTInorder ....... nerekurzivn� pr�chod typu in-order
**     BTPostorder ..... nerekurzivn� pr�chod typu post-order
**     BTDisposeTree ... zru� v�echny uzly stromu
**
** U v�ech funkc�, kter� vyu��vaj� n�kter� z pr�chod� stromem, implementujte
** pomocnou funkci pro nalezen� nejlev�j��ho uzlu v podstromu.
**
** P�esn� definice typ� naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na n�j je typu tBTNodePtr. Jeden uzel obsahuje polo�ku int Cont,
** kter� sou�asn� slou�� jako u�ite�n� obsah i jako vyhled�vac� kl�� 
** a ukazatele na lev� a prav� podstrom (LPtr a RPtr).
**
** P��klad slou�� zejm�na k procvi�en� nerekurzivn�ch z�pis� algoritm�
** nad stromy. Ne� za�nete tento p��klad �e�it, prostudujte si d�kladn�
** principy p�evodu rekurzivn�ch algoritm� na nerekurzivn�. Programov�n�
** je p�edev��m in�en�rsk� discipl�na, kde op�tn� objevov�n� Ameriky nem�
** m�sto. Pokud se V�m zd�, �e by n�co �lo zapsat optim�ln�ji, promyslete
** si v�echny detaily Va�eho �e�en�. Pov�imn�te si typick�ho um�st�n� akc�
** pro r�zn� typy pr�chod�. Zamyslete se nad modifikac� �e�en�ch algoritm�
** nap��klad pro v�po�et po�tu uzl� stromu, po�tu list� stromu, v��ky stromu
** nebo pro vytvo�en� zrcadlov�ho obrazu stromu (pouze pop�ehazov�n� ukazatel�
** bez vytv��en� nov�ch uzl� a ru�en� star�ch).
**
** P�i pr�chodech stromem pou�ijte ke zpracov�n� uzlu funkci BTWorkOut().
** Pro zjednodu�en� pr�ce m�te p�edem p�ipraveny z�sobn�ky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro pr�ci
** s pomocn�mi z�sobn�ky neupravujte 
**
** Pozor! Je t�eba spr�vn� rozli�ovat, kdy pou��t dereferen�n� oper�tor *
** (typicky p�i modifikaci) a kdy budeme pracovat pouze se samotn�m ukazatelem 
** (nap�. p�i vyhled�v�n�). V tomto p��kladu v�m napov� prototypy funkc�.
** Pokud pracujeme s ukazatelem na ukazatel, pou�ijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocn� funkce, kterou budete volat p�i pr�chodech stromem pro zpracov�n�
** uzlu ur�en�ho ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla vol�na s NULL argumentem!\n");
  else 
    printf("V�pis hodnoty dan�ho uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro z�sobn�k hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace z�sobn�ku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vlo�� hodnotu na vrchol z�sobn�ku.
**/
{ 
                 /* P�i implementaci v poli m��e doj�t k p�ete�en� z�sobn�ku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: Do�lo k p�ete�en� z�sobn�ku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstran� prvek z vrcholu z�sobn�ku a sou�asn� vr�t� jeho hodnotu.
**/
{
                            /* Operace nad pr�zdn�m z�sobn�kem zp�sob� chybu. */
	if (S->top==0)  {
		printf("Chyba: Do�lo k podte�en� z�sobn�ku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li z�sobn�k pr�zdn�, vr�t� hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro z�sobn�k hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace z�sobn�ku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** Vlo�� hodnotu na vrchol z�sobn�ku.
**/
                 /* P�i implementaci v poli m��e doj�t k p�ete�en� z�sobn�ku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: Do�lo k p�ete�en� z�sobn�ku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstran� prvek z vrcholu z�sobn�ku a sou�asn� vr�t� jeho hodnotu.
**/
                            /* Operace nad pr�zdn�m z�sobn�kem zp�sob� chybu. */
	if (S->top==0) {
		printf("Chyba: Do�lo k podte�en� z�sobn�ku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li z�sobn�k pr�zdn�, vr�t� hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** N�sleduje j�dro dom�c� �lohy - funkce, kter� m�te implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci bin�rn�ho vyhled�vac�ho stromu.
**
** Inicializaci sm� program�tor volat pouze p�ed prvn�m pou�it�m bin�rn�ho
** stromu, proto�e neuvol�uje uzly nepr�zdn�ho stromu (a ani to d�lat nem��e,
** proto�e p�ed inicializac� jsou hodnoty nedefinovan�, tedy libovoln�).
** Ke zru�en� bin�rn�ho stromu slou�� procedura BTDisposeTree.
**	
** V�imn�te si, �e zde se poprv� v hlavi�ce objevuje typ ukazatel na ukazatel,	
** proto je t�eba p�i pr�ci s RootPtr pou��t dereferen�n� oper�tor *.
**/
	
	
	//nastavim prvni prvek na NULL
	
	(*RootPtr)=NULL;
	 //solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vlo�� do stromu nov� uzel s hodnotou Content.
**
** Z pohledu vkl�d�n� ch�pejte vytv��en� strom jako bin�rn� vyhled�vac� strom,
** kde uzly s hodnotou men�� ne� m� otec le�� v lev�m podstromu a uzly v�t��
** le�� vpravo. Pokud vkl�dan� uzel ji� existuje, neprov�d� se nic (dan� hodnota
** se ve stromu m��e vyskytnout nejv��e jednou). Pokud se vytv��� nov� uzel,
** vznik� v�dy jako list stromu. Funkci implementujte nerekurzivn�.
**/

		//vytvorim si novy list
		tBTNodePtr item=NULL,item_2=NULL;
		item_2=malloc(sizeof(tBTNodePtr)*sizeof(tBTNodePtr)*sizeof(tBTNodePtr)*sizeof(int));
		item_2->Cont=Content;
		item_2->LPtr=NULL;
		item_2->RPtr=NULL;
		
		//pokud jsme na zacatku okamzite ho ulozim
		if ((*RootPtr)==NULL)
			(*RootPtr)=item_2;
			
		//jinak projdu cely strom a ulozim ho na konec
		else
		{
			//pomocna pro pruchod stromu
			item=(*RootPtr);
			
			//dokud jsme nenrazili na konec stromu
			while (item!=NULL)
			{
				
				//pokud se tam jiz tato hodnota nachazi, uvolnim novy list a koncim funkci
				if (item->Cont == Content)
				{
					free(item_2);
					return ;
				}
				
				//pokud je hodnota vetsi nez hledany prvek jdu do leva
				else if (item->Cont > Content)
				{
					
					//pokud se v levo nic nenachazi, ulozim tam nove vytvoreny list a koncim funkci
					if (item->LPtr==NULL)
					{
						item->LPtr=item_2;
						return;
					}
					//pokud, ale v levo neco je, nactu levy uzel a pokracuji v cyklu
					item=item->LPtr;
				}
				//pokud je hodnota mensi nez hledany prvek jdu do prava
				else if (item->Cont < Content)
				{
					
					//pokud se v pravo nic nenachazi, ulozim tam nove vytvoreny list a koncim funkci
					if (item->RPtr==NULL)
					{
						item->RPtr=item_2;
						return;
					}
					
					//pokud, ale v pravo neco je, nactu pravy uzel a pokracuji v cyklu
					item=item->RPtr;
				}
			}
		}
		
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po lev� v�tvi podstromu, dokud nenaraz� na jeho nejlev�j�� uzel.
**
** P�i pr�chodu Preorder nav�t�ven� uzly zpracujeme vol�n�m funkce BTWorkOut()
** a ukazatele na n� is ulo��me do z�sobn�ku.
**/

	//s pouzitim zasobniku prochazim strom
	tBTNodePtr item;
	item=ptr;
	
	//dokud nejsme na konci
	while (item!=NULL)
	{
		//tisknu uzel, na kterem se nachazim
		BTWorkOut(item);
		
		//ulozim uzel pro pozdejsi pruchod do prava na zasobnik
		SPushP(Stack,item);
		
		//a jdu do leva
		item=item->LPtr;
	}
	
	
	 //solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Pr�chod stromem typu preorder implementovan� nerekurzivn� s vyu�it�m funkce
** Leftmost_Preorder a z�sobn�ku ukazatel�. Zpracov�n� jednoho uzlu stromu
** realizujte jako vol�n� funkce BTWorkOut(). 
**/

	//pokud ve strome vubec neco je
	if (RootPtr!=NULL)
	{
		
		//inicializace zasobniku, pomocneho stromu a prvni pruchod do leva
		//mam radsi dynamicke zasobniky
		tStackP *Stack=NULL;
		Stack=malloc (sizeof (tStackP));
		tBTNodePtr item=RootPtr;
		SInitP(Stack);
		Leftmost_Preorder(item,Stack);
		
		//dokud enni zasobnik prazdny
		while (!SEmptyP(Stack))
		{
			
			//do promenne ulozim ukazatel na uzel
			item=STopPopP(Stack);
			
			//a projfu uzel tentokrat do prava
			Leftmost_Preorder(item->RPtr,Stack);
		}
		free (Stack);
	}
	 //solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po lev� v�tvi podstromu, dokud nenaraz� na jeho nejlev�j�� uzel.
**
** P�i pr�chodu Inorder ukl�d�me ukazatele na v�echny nav�t�ven� uzly do
** z�sobn�ku. 
**/
	//s pouzitim zasobniku prochazim strom
	tBTNodePtr item;
	item=ptr;
	
	//dokud nejsme na konci
	while (item!=NULL)
	{
		//dam stavajici uzel na zasobnik pro pozdejsi pruchod do prava
		SPushP(Stack,item);
		
		//a jdu do leva
		item=item->LPtr;
	}
	
	
	
	// solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
	
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Pr�chod stromem typu inorder implementovan� nerekurzivn� s vyu�it�m funkce
** Leftmost_Inorder a z�sobn�ku ukazatel�. Zpracov�n� jednoho uzlu stromu
** realizujte jako vol�n� funkce BTWorkOut(). 
**/

	//pokud vubec neco je ve strome
	if (RootPtr!=NULL)
	{
		
		//pocatecni inicializace a vytvoreni zasobniku
		//mam radsi dynamicke zasobniky
		tStackP *Stack=NULL;
		Stack=malloc (sizeof (tStackP));
		tBTNodePtr item=RootPtr;
		SInitP(Stack);
		
		//prvni pruchod do leva
		Leftmost_Inorder(item,Stack);
		while (!SEmptyP(Stack))
		{
			
			//vytahnu si ze zasobniku prvni uzel
			item=STopPopP(Stack);
			//vytisknu ho
			BTWorkOut(item);
			
			//a prochazim jeho pravy list do leva
			Leftmost_Inorder(item->RPtr,Stack);
		}
		free (Stack);
	}
	
	// solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po lev� v�tvi podstromu, dokud nenaraz� na jeho nejlev�j�� uzel.
**
** P�i pr�chodu Postorder ukl�d�me ukazatele na nav�t�ven� uzly do z�sobn�ku
** a sou�asn� do z�sobn�ku bool hodnot ukl�d�me informaci, zda byl uzel
** nav�t�ven poprv� a �e se tedy je�t� nem� zpracov�vat. 
**/

	//s pouzitim dvou zasobniku prochazim strom
	tBTNodePtr item;
	item=ptr;
	
	//dkud nejsme na konci stromu
	while (item!=NULL)
	{
		
		//na zasobnik ulozim stavajici uzel
		SPushP(StackP,item);
		
		//a ulozim ze jsem sel do leva
		SPushB(StackB,true);
		item=item->LPtr;
	}
	
	
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Pr�chod stromem typu postorder implementovan� nerekurzivn� s vyu�it�m funkce
** Leftmost_Postorder, z�sobn�ku ukazatel� a z�sobn�ku hotdnot typu bool.
** Zpracov�n� jednoho uzlu stromu realizujte jako vol�n� funkce BTWorkOut(). 
**/
	if (RootPtr!=NULL)
	{
		
		//ininicializace promennych a zasobniku
		bool left;
		
		//mam radsi dynamicke zasobniky
		tStackP *StackP=NULL;
		StackP=malloc (sizeof (tStackP));
		tStackB *StackB=NULL;
		StackB=malloc (sizeof (tStackB));
		tBTNodePtr item;
		item = RootPtr;
		SInitP(StackP);
		SInitB(StackB);
		
		//prvni pruchod do leva
		Leftmost_Postorder(item,StackP,StackB);
		while (!SEmptyP(StackP))
		{
			//vytahnu si prvni uzel ze saboniku
			item=STopPopP(StackP);
			//ale on tam musi zustat
			SPushP(StackP,item);
			
			//zjistim, jestli jsem posledne sel do leva
			left=STopPopB(StackB);
			
			//pokud ano
			if (left)
			{
				//pujdu do prava
				SPushB(StackB,false);
				Leftmost_Postorder(item->RPtr,StackP,StackB);
			}
			
			//pokud jsem sel do prava
			else
			{
				//vytahnu uzel
				item=STopPopP(StackP);
				//a vytisknu ho
				BTWorkOut(item);
			}
		}
		free(StackP);
		free(StackB);
	}
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zru�� v�echny uzly stromu a korektn� uvoln� jimi zabranou pam�.
**
** Funkci implementujte nerekurzivn� s vyu�it�m z�sobn�ku ukazatel�.
**/
	//pokud ve stromu vubec neco je, tak ho smazu
	if ((*RootPtr)!=NULL)
	{
		
		//mama radsi dynamicke zasobniky
		tStackP *StackP=NULL;
		StackP=malloc (sizeof (tStackP));
		tBTNodePtr item_2;
		SInitP(StackP);
		
		//cyklus repeat
		do
		{
			//pokud jsme na konci vetve
			if ((*RootPtr)==NULL)
			{	
				//a neni prazdny zasobnik, tak si vytahneme uzel a pokracujeme v cyklu
				if (!SEmptyP(StackP))
					(*RootPtr)=STopPopP(StackP);
			}
			
			//pokud jsme nekde uprostred vetve
			else
			{
				//pokud ve stavajicim uzlu v pravo neco je, ulozim uzel na zasobnik
				if ((*RootPtr)->RPtr!=NULL)
					SPushP(StackP,(*RootPtr)->RPtr);
				
				//posunu se do leva a prvek, na kterem jsem doposud byl odsranim
				item_2=(*RootPtr);
				(*RootPtr)=(*RootPtr)->LPtr;
				free(item_2);
				item_2=NULL;
			}
		}
		while (!((*RootPtr)==NULL && SEmptyP(StackP)));
		//cyklus do while, pracuje podobne jako repeat v pascalu, s tim rozdilem ze
		//ukoncovaci podminku musime znegovat

		free(StackP);
	}
	
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
}

/* konec c402.c */

