
/* c401.c: **********************************************************}
{* T�ma: Rekurzivn� implementace operac� nad BVS
**                                         Vytvo�il: Petr P�ikryl, listopad 1994
**                                         �pravy: Andrea N�mcov�, prosinec 1995
**                                                      Petr P�ikryl, duben 1996
**                                                   Petr P�ikryl, listopad 1997
**                                  P�evod do jazyka C: Martin Tu�ek, ��jen 2005
**                                         �pravy: Bohuslav K�ena, listopad 2009
**
** Implementujte rekurzivn�m zp�sobem operace nad bin�rn�m vyhled�vac�m
** stromem (BVS; v angli�tin� BST - Binary Search Tree).
**
** Kl��em uzlu stromu je jeden znak (obecn� j�m m��e b�t cokoliv, podle
** �eho se vyhled�v�). U�ite�n�m (vyhled�van�m) obsahem je zde integer.
** Uzly s men��m kl��em le�� vlevo, uzly s v�t��m kl��em le�� ve stromu
** vpravo. Vyu�ijte dynamick�ho p�id�lov�n� pam�ti.
** Rekurzivn�m zp�sobem implementujte n�sleduj�c� funkce:
**
**   BSTInit ...... inicializace vyhled�vac�ho stromu
**   BSTSearch .... vyhled�v�n� hodnoty uzlu zadan�ho kl��em
**   BSTInsert .... vkl�d�n� nov� hodnoty
**   BSTDelete .... zru�en� uzlu se zadan�m kl��em
**   BSTDispose ... zru�en� cel�ho stromu
**
** ADT BVS je reprezentov�n ko�enov�m ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje kl�� (typu char), podle
** kter�ho se ve stromu vyhled�v�, vlastn� obsah uzlu (pro jednoduchost
** typu int) a ukazatel na lev� a prav� podstrom (LPtr a RPtr). 
** P�esnou definici typ� naleznete v souboru c401.h.
**
** Pozor! Je t�eba spr�vn� rozli�ovat, kdy pou��t dereferen�n� oper�tor *
** (typicky p�i modifikaci) a kdy budeme pracovat pouze se samotn�m ukazatelem 
** (nap�. p�i vyhled�v�n�). V tomto p��kladu v�m napov� prototypy funkc�.
** Pokud pracujeme s ukazatelem na ukazatel, pou�ijeme dereferenci.
**/

#include "c401.h"
int solved;


/*
 * ~~~~~~~~~~~~~~~~~~~
 * Autor:	Karel Hala
 * Login:	xhalak00
 * Email:	xhalak00@stud.fit.vutbr.cz
 * ~~~~~~~~~~~~~~~~~~~
 **/

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede po��te�n� inicializaci stromu p�ed jeho prvn�m pou�it�m.
**
** Ov��it, zda byl ji� strom p�edan� p�es RootPtr inicializov�n, nelze,
** proto�e p�ed prvn� inicializac� m� ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Program�tor vyu��vaj�c� ADT BVS tedy mus� zajistit, aby inicializace
** byla vol�na pouze jednou, a to p�ed vlastn� prac� s BVS. Proveden�
** inicializace nad nepr�zdn�m stromem by toti� mohlo v�st ke ztr�t� p��stupu
** k dynamicky alokovan� pam�ti (tzv. "memory leak").
**	
** V�imn�te si, �e se v hlavi�ce objevuje typ ukazatel na ukazatel.	
** Proto je t�eba p�i p�i�azen� p�es RootPtr pou��t dereferen�n� oper�tor *.
** Ten bude pou�it i ve funkc�ch BSTDelete, BSTInsert a BSTDispose.
**/

	//nastavim binarni strom na NULL
	
	*RootPtr=NULL;
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
	
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhled� uzel v BVS s kl��em K.
**
** Pokud je takov� nalezen, vrac� funkce hodnotu TRUE a v prom�nn� Content se
** vrac� obsah p��slu�n�ho uzlu.�Pokud p��slu�n� uzel nen� nalezen, vrac� funkce
** hodnotu FALSE a obsah prom�nn� Content nen� definov�n (nic do n� proto
** nep�i�azujte).
**
** P�i vyhled�v�n� v bin�rn�m stromu bychom typicky pou�ili cyklus ukon�en�
** testem dosa�en� listu nebo nalezen� uzlu s kl��em K. V tomto p��pad� ale
** probl�m �e�te rekurzivn�m vol�n� t�to funkce, p�i�em� nedeklarujte ��dnou
** pomocnou funkci.
**/	

	//pokud vubec neco je ve stromu, jinak nema cenu ho prochazet
	if (RootPtr==NULL)
	{
		return FALSE;
	}
	else
	{
		//pokud najdeme uzel s hledanym klicem vracim jeho obsah
		if (RootPtr->Key==K)
		{
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
		
		//pokud je klic na aktualnim prvku vetsi, nez hledany, jdu do leva
		else if (RootPtr->Key>K)
		{
			//pri rekurzy nesmim zapomenout na vraceni hodnot
			return(BSTSearch(RootPtr->LPtr,K,Content));
		}
		
		//pokud je klic na aktualnim prvku mendi, nez hledany, jdu do prava
		else if (RootPtr->Key<K)
		{
			//pri rekurzy nesmim zapomenout na vraceni hodnot
			return(BSTSearch(RootPtr->RPtr,K,Content));
		}
	}
	return FALSE;
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
	
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vlo�� do stromu RootPtr hodnotu Content s kl��em K.
**
** Pokud ji� uzel se zadan�m kl��em ve stromu existuje, bude obsah uzlu
** s kl��em K nahrazen novou hodnotou. Pokud bude do stromu vlo�en nov�
** uzel, bude vlo�en v�dy jako list stromu.
**
** Funkci implementujte rekurzivn�. Nedeklarujte ��dnou pomocnou funkci.
**
** Rekurzivn� implementace je m�n� efektivn�, proto�e se p�i ka�d�m
** rekurzivn�m zano�en� ukl�d� na z�sobn�k obsah uzlu (zde integer).
** Nerekurzivn� varianta by v tomto p��pad� byla efektivn�j�� jak z hlediska
** rychlosti, tak z hlediska pam�ov�ch n�rok�. Zde jde ale o �koln�
** p��klad, na kter�m si chceme uk�zat eleganci rekurzivn�ho z�pisu.
**/

	//Pokud se nachazime na koncovem liste, nebo na hlavnim uzlu a nic tam proste neni
	//vytvorim si novy uzel a ulozim ho, tim take zkonci rekurze
	if ((*RootPtr)==NULL)
	{
		(*RootPtr)=malloc (sizeof(tBSTNodePtr)*sizeof(tBSTNodePtr)*sizeof(char)*(sizeof(int)));
		(*RootPtr)->Key=K;
		(*RootPtr)->BSTNodeCont=Content;
		(*RootPtr)->LPtr=NULL;
		(*RootPtr)->RPtr=NULL;
	}
	else 
	{
		//pokud je hledany klic vetsi, nez ten na ktrem se zrovna nachazime, jde do leva
		if ((*RootPtr)->Key>K)
		{	
			//volame rekurzivne do leva
			BSTInsert(&(*RootPtr)->LPtr,K,Content);
		}
		
		//pokud je hledany klic mensi, nez ten na ktrem se zrovna nachazime, jde do leva
		else if ((*RootPtr)->Key<K)
		{
			//volame rekurzivne do prava
			BSTInsert(&(*RootPtr)->RPtr,K,Content);
		}
		
		//pokud najdeme uzel se stejnym klicem, aktualize jeho obsahovou cast
		else if ((*RootPtr)->Key==K)
		{
			//aktualizujeme hodnotu
			(*RootPtr)->BSTNodeCont = Content;
		}
	}
	
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
	
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocn� funkce pro vyhled�n�, p�esun a uvoln�n� nejprav�j��ho uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kter�ho bude p�esunuta hodnota
** nejprav�j��ho uzlu v podstromu, kter� je ur�en ukazatelem RootPtr.
** P�edpokl�d� se, �e hodnota ukazatele RootPtr nebude NULL (zajist�te to
** testov�n�m p�ed vol�n� t�to funkce). Tuto funkci implementujte rekurzivn�. 
**
** Tato pomocn� funkce bude pou�ita d�le. Ne� ji za�nete implementovat,
** p�e�t�te si koment�� k funkci BSTDelete(). 
**/

	//funkce se provede pouze, pokud neco bude v uzlu, jinak to nema smysl
	if ((*RootPtr)!=NULL)
	{
		//Pokud se v pravu jeste neco nachazi, rekurzivne volam vetev do prava
		if ((*RootPtr)->RPtr!=NULL)
		{
			ReplaceByRightmost(PtrReplaced,&(*RootPtr)->RPtr);
		}
		
		//jinak jsme na nejpravejsim prvku
		else
		{
			//do meneneho listu ulozim hodnotu nejpravejsiho listu ajeho klic
			PtrReplaced->BSTNodeCont=(*RootPtr)->BSTNodeCont;
			PtrReplaced->Key=(*RootPtr)->Key;
		}
	}

	
		
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	
	
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zru�� uzel stromu, kter� obsahuje kl�� K.
**
** Pokud uzel se zadan�m kl��em neexistuje, ned�l� funkce nic. 
** Pokud m� ru�en� uzel jen jeden podstrom, pak jej zd�d� otec ru�en�ho uzlu.
** Pokud m� ru�en� uzel oba podstromy, pak je ru�en� uzel nahrazen nejprav�j��m
** uzlem lev�ho podstromu. Pozor! Nejprav�j�� uzel nemus� b�t listem.
**
** Tuto funkci implementujte rekurzivn� s vyu�it�m d��ve deklarovan�
** pomocn� funkce ReplaceByRightmost.
**/
	tBSTNodePtr item;
	item = NULL;
	if ((*RootPtr)!=NULL)
	{
		
		//pokud je hledany klic vetsi, nez ten na ktrem se zrovna nachazime, jde do leva
		if ((*RootPtr)->Key>K)
		{
			if ((*RootPtr)->LPtr!=NULL)
				BSTDelete (&(*RootPtr)->LPtr,K);
		}
		
		//pokud je hledany klic mensi, nez ten na ktrem se zrovna nachazime, jde do leva
		else if ((*RootPtr)->Key<K)
		{
			if ((*RootPtr)->RPtr!=NULL)
				BSTDelete (&(*RootPtr)->RPtr,K);
		}
		
		//pokud jsme na prvku, ktery chceme odstranit
		else if ((*RootPtr)->Key==K)
		{
			
			//pokud je v pravo prazdno a v levo nejaky uzel je to lehke, jen preukazu
			//ukazatele a odstranim prvek
			if ((*RootPtr)->RPtr ==NULL &&(*RootPtr)->LPtr !=NULL)
			{
				item = (*RootPtr);
				(*RootPtr)=(*RootPtr)->LPtr;
				free (item);
				item=NULL;
			}
			
			//pokud je v levo prazdno a v pravo nejaky uzel je to lehke, jen preukazu
			//ukazatele a odstranim prvek
			else if ((*RootPtr)->LPtr ==NULL && (*RootPtr)->RPtr !=NULL)
			{
				item = (*RootPtr);
				(*RootPtr)=(*RootPtr)->RPtr;
				free (item);
				item=NULL;
			}
			
			//pokud jsme na konci stromu, jen odstranim prvek, nehraji si s ukazately
			else if ((*RootPtr)->LPtr ==NULL && (*RootPtr)->RPtr ==NULL)
			{
				free ((*RootPtr));
				(*RootPtr)=NULL;
			}
			
			//pokud ma mazany prvek 2 potomky zamenim ho za nejpravejsi v leve vetvi
			else
			{
				tBSTNodePtr item_2;
				item_2=NULL;
				item_2=(*RootPtr);
				//nahraji hodnoty z leve vetve z nejpravejsiho listu do aktualniho
				ReplaceByRightmost (item_2,&(*RootPtr)->LPtr);
				
				//a odstranim nejpravejsi list v leve vetvi
				BSTDelete (&(*RootPtr)->LPtr,item_2->Key);
			}
		}
	}
	
	//solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	

} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zru�� cel� bin�rn� vyhled�vac� strom a korektn� uvoln� pam�.
**
** Po zru�en� se bude BVS nach�zet ve stejn�m stavu, jako se nach�zel po
** inicializaci. Tuto funkci implementujte rekurzivn� bez deklarov�n� pomocn�
** funkce.
**/
	if ((*RootPtr)!=NULL)
	{
		//rekurzivne prochazim jednotlive vetve nejdrive do leva
		if ((*RootPtr)->LPtr!=NULL)
			BSTDispose (&(*RootPtr)->LPtr);
			
		//potom take do prava
		if ((*RootPtr)->RPtr!=NULL)
			BSTDispose (&(*RootPtr)->RPtr);
			
		//a az jsem na poslenim listu odstanim ho
		if ((*RootPtr)->RPtr==NULL && (*RootPtr)->LPtr==NULL)
		{
			free(*RootPtr);
			*RootPtr=NULL;
		}
	}
	

	// solved = FALSE;		  /* V p��pad� �e�en� sma�te tento ��dek! */	

}

/* konec c401.c */


