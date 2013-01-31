
/* c016.c: **********************************************************}
{* T�ma:  Tabulka s Rozpt�len�mi Polo�kami
**                      Prvn� implementace: Petr P�ikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      �pravy: Karel Masa��k, ��jen 2011
**
** Vytvo�ete abstraktn� datov� typ
** TRP (Tabulka s Rozpt�len�mi Polo�kami = Hash table)
** s explicitn� �et�zen�mi synonymy. Tabulka je implementov�na polem
** line�rn�ch seznam� synonym.
**
** Implementujte n�sleduj�c� procedury a funkce.
**
**  HTInit ....... inicializuje tabulku p�ed prvn�m pou�it�m
**  HTInsert ..... vlo�en� prvku
**  HTSearch ..... zji�t�n� p��tomnosti prvku v tabulce
**  HTDelete ..... zru�en� prvku
**  HTRead ....... p�e�ten� hodnoty prvku
**  HTClearAll ... zru�en� obsahu cel� tabulky (inicializace tabulky
**                 pot�, co ji� byla pou�ita)
**
** Definici typ� naleznete v souboru c016.h.
**
** Tabulka je reprezentov�na datovou strukturou typu tHTable,
** kter� se skl�d� z ukazatel� na polo�ky, je� obsahuj� slo�ky
** kl��e 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal�� synonymum 'ptrnext'. P�i implementaci funkc�
** uva�ujte maxim�ln� rozm�r pole HTSIZE.
**
**
** U v�ech procedur vyu��vejte rozptylovou funkci hashCode.  Pov�imn�te si
** zp�sobu p�ed�v�n� parametr� a zamyslete se nad t�m, zda je mo�n� parametry
** p�ed�vat jin�m zp�sobem (hodnotou/odkazem) a v p��pad�, �e jsou ob�
** mo�nosti funk�n� p��pustn�, jak� jsou v�hody �i nev�hody toho �i onoho
** zp�sobu.
**
** V p��kladech jsou pou�ity polo�ky, kde kl��em je �et�zec, ke kter�mu
** je p�id�n obsah - re�ln� ��slo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*
 * ~~~~~~~~~~~~~~~~~~~
 * Autor:	Karel Hala
 * Login:	xhalak00
 * Email:	xhalak00@stud.fit.vutbr.cz
 * ~~~~~~~~~~~~~~~~~~~
 **/

/*          -------
** Rozptylovac� funkce - jej�m �kolem je zpracovat zadan� kl�� a p�id�lit
** mu index v rozmez� 0..HTSize-1.  V ide�ln�m p��pad� by m�lo doj�t
** k rovnom�rn�mu rozpt�len� t�chto kl��� po cel� tabulce.  V r�mci
** pokus� se m��ete zamyslet nad kvalitou t�to funkce.  (Funkce nebyla
** volena s ohledem na maxim�ln� kvalitu v�sledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitn� z�et�zen�mi synonymy.  Tato procedura
** se vol� pouze p�ed prvn�m pou�it�m tabulky.
*/

void htInit ( tHTable* ptrht ) {
	
	//Pokud bude adresa tabulky ruzna 0x0 adrese, do jednotlivych policek nastavi NULL
	if( (*ptrht) == NULL ) return;
	else
	{
		for (int x=0;x<MAX_HTSIZE;x++)
		(*ptrht)[x] = NULL;
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Vyhled�n� prvku v TRP ptrht podle zadan�ho kl��e key.  Pokud je
** dan� prvek nalezen, vrac� se ukazatel na dan� prvek 
** Pokud prvek nalezen nen�, vrac� se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	
	//pokud bude tabulka neinicializovan�, nebo polo�ka tabulky bude NULL vrati NULL
	if( (*ptrht) == NULL ) return NULL;
	else if ((*ptrht)[hashCode(key)]==NULL)
		return NULL;
	else
	{
		tHTItem* item;
		item = (*ptrht)[hashCode(key)];
		
		//prochazi jednotlive synonyma v tabulce a pokud najde synonymum se spravnym klicem, vrati jeho hodnotu
		while (item!=NULL)
		{
			if (item->key==key)return item;
			else item=item->ptrnext;	
		}
		
		//pokud, ale nic takoveho nenajde vrati NULL
		return NULL;
	}
	return NULL;
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* 
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vkl�d� do tabulky ptrht polo�ku s kl��em key a s daty
** data.  Proto�e jde o vyhled�vac� tabulku, nem��e b�t prvek se stejn�m
** kl��em ulo�en v tabulce v�ce ne� jedenkr�t.  Pokud se vkl�d� prvek,
** jeho� kl�� se ji� v tabulce nach�z�, aktualizujte jeho datovou ��st.
**
** Vyu�ijte d��ve vytvo�enou funkci htSearch.  P�i vkl�d�n� nov�ho
** prvku do seznamu synonym pou�ijte co nejefektivn�j�� zp�sob,
** tedy prove�te.vlo�en� prvku na za��tek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	if( (*ptrht) == NULL ) return;
	else
	{
		tHTItem *item;
		item =NULL;
		int x=hashCode(key);
		
		//pokud nam prohledani tabulky vratilo NULL, znamena to ze jsme nasli 
		//bud prazdne policko, nebo je v tabulce jiz nejake synonymum
		
		if ((item=htSearch(ptrht,key))==NULL)
		{
			
			//pokud mame prazdne policko, vytvorime nove synonymum a zapiseme 
			//ho na prvni pozici v tabulce
			
			if ((*ptrht)[x] == NULL)
			{
				(*ptrht)[x]=malloc(sizeof(tHTable));
				(*ptrht)[x]->key=key;
				(*ptrht)[x]->data=data;
				(*ptrht)[x]->ptrnext=NULL;
			}
			else
			{
				
				//Pokud, ale jiz nejake synonymum v tabulce je, vytvorime nove 
				//synonymum a provazem jednotlive synonyma
				
				item = malloc(sizeof(tHTable));
				item->key=key;
				item->data=data;
				item->ptrnext=(*ptrht)[x];
				(*ptrht)[x]=item;
			}
		}
		else
		{
			//pokud nam search vratil neco jine, nez NULL znamena to ze se tam 
			//hodnota s timto klicem nachazi a aktualizujeme pouze jeji datovou cast
			
			if ((item=htSearch(ptrht,key))!=NULL)
			{
				tHTItem *item_2;
				item_2=(*ptrht)[x];
				while (item_2!=NULL)
				{
					if (item_2->key==key)
					{
						item_2->data=data;
						return;
					}
					item_2=item_2->ptrnext;
					
				}
			}
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato funkce zji��uje hodnotu datov� ��sti polo�ky zadan� kl��em.
** Pokud je polo�ka nalezena, vrac� funkce ukazatel na polo�ku
** Pokud polo�ka nalezena nebyla, vrac� se funk�n� hodnota NULL
**
** Vyu�ijte d��ve vytvo�enou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	tHTItem *item;
	
	//pokud je tabulka prazdna, nebo neinicializovana vracim NULL
	if( (*ptrht) == NULL ) return NULL;
	else if( (*ptrht)[hashCode(key)] == NULL ) return NULL;
	
	//jinak vratim datovou cast co nam vyhledala funkce search
	else if ((item=htSearch(ptrht,key))!=NULL)
		return &(item->data);
	return NULL;
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vyjme polo�ku s kl��em key z tabulky
** ptrht.  Uvoln�nou polo�ku korektn� zru�te.  Pokud polo�ka s uveden�m
** kl��em neexistuje, d�lejte, jako kdyby se nic nestalo (tj. ned�lejte
** nic).
**
** V tomto p��pad� NEVYU��VEJTE d��ve vytvo�enou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if( (*ptrht) == NULL )return;
	tHTItem *item=NULL,*last=NULL;
	
	
	if ((*ptrht)[hashCode(key)]!=NULL)
	{ 
		//pokud je tabulka inicializovana projdu jednotlive synonyma a pokud narazim
		//na to, ktere chci smazat provazu zpet synonyma a smazu to, ktere chci mazat
		item=(*ptrht)[hashCode(key)];
		while (item!=NULL)
		{
			//pokud najdeme tynonymum, ktere chceme mazat
			if (item->key==key)
			{
				//pokud nejsme na konci seznamu
				if (last !=NULL)
				{
					//ulozime si nasledujici prvek
					last->ptrnext=item->ptrnext;
				}
				
				//pokud prvek, ktery chceme mazat je prvni
				if (item==(*ptrht)[hashCode(key)])
				{
					
					//provazu zpet seznam
					(*ptrht)[hashCode(key)] = item->ptrnext;
				}
				
				//odstranim prvek (konecne)
				free (item);
				item=NULL;
				return;
			}
			else
			{
				
				//pokud jsme ale jeste nenasli hledany prvek posouvame se o
				//jedno synonymum dal
				last = item;
				item=item->ptrnext;
			}
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura zru�� v�echny polo�ky tabulky, korektn� uvoln� prostor,
** kter� tyto polo�ky zab�raly, a uvede tabulku do po��te�n�ho stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	if( (*ptrht) == NULL ) return;
	else
		{
			
		//postupne maze jednotlive prvky v tabulce
		//cyklus pro projiti jednotlivych policek v tabulce
		for (int x=0;x<MAX_HTSIZE;x++)
		{
			
			//pokud je nejaky prvek v policku tabulky
			if ((*ptrht)[x]!=NULL)
			{
				tHTItem *item=NULL;
				
				//dokud nejsme na konci seznamu synonym
				while ((*ptrht)[x]!=NULL)
				{
					item=(*ptrht)[x];
					
					//posunu se na dalsi synonymum
					(*ptrht)[x]=(*ptrht)[x]->ptrnext;
					
					//a smazu prvek
					free (item);
					item=NULL;
				}
			}
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}
