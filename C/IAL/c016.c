
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Polo¾kami
**                      První implementace: Petr Pøikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masaøík, øíjen 2011
**
** Vytvoøete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Polo¾kami = Hash table)
** s explicitnì øetìzenými synonymy. Tabulka je implementována polem
** lineárních seznamù synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pøed prvním pou¾itím
**  HTInsert ..... vlo¾ení prvku
**  HTSearch ..... zji¹tìní pøítomnosti prvku v tabulce
**  HTDelete ..... zru¹ení prvku
**  HTRead ....... pøeètení hodnoty prvku
**  HTClearAll ... zru¹ení obsahu celé tabulky (inicializace tabulky
**                 poté, co ji¾ byla pou¾ita)
**
** Definici typù naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelù na polo¾ky, je¾ obsahují slo¾ky
** klíèe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal¹í synonymum 'ptrnext'. Pøi implementaci funkcí
** uva¾ujte maximální rozmìr pole HTSIZE.
**
**
** U v¹ech procedur vyu¾ívejte rozptylovou funkci hashCode.  Pov¹imnìte si
** zpùsobu pøedávání parametrù a zamyslete se nad tím, zda je mo¾né parametry
** pøedávat jiným zpùsobem (hodnotou/odkazem) a v pøípadì, ¾e jsou obì
** mo¾nosti funkènì pøípustné, jaké jsou výhody èi nevýhody toho èi onoho
** zpùsobu.
**
** V pøíkladech jsou pou¾ity polo¾ky, kde klíèem je øetìzec, ke kterému
** je pøidán obsah - reálné èíslo.
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
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíè a pøidìlit
** mu index v rozmezí 0..HTSize-1.  V ideálním pøípadì by mìlo dojít
** k rovnomìrnému rozptýlení tìchto klíèù po celé tabulce.  V rámci
** pokusù se mù¾ete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitnì zøetìzenými synonymy.  Tato procedura
** se volá pouze pøed prvním pou¾itím tabulky.
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

/* TRP s explicitnì zøetìzenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíèe key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek 
** Pokud prvek nalezen není, vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	
	//pokud bude tabulka neinicializovaná, nebo polo¾ka tabulky bude NULL vrati NULL
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vkládá do tabulky ptrht polo¾ku s klíèem key a s daty
** data.  Proto¾e jde o vyhledávací tabulku, nemù¾e být prvek se stejným
** klíèem ulo¾en v tabulce více ne¾ jedenkrát.  Pokud se vkládá prvek,
** jeho¾ klíè se ji¾ v tabulce nachází, aktualizujte jeho datovou èást.
**
** Vyu¾ijte døíve vytvoøenou funkci htSearch.  Pøi vkládání nového
** prvku do seznamu synonym pou¾ijte co nejefektivnìj¹í zpùsob,
** tedy proveïte.vlo¾ení prvku na zaèátek seznamu.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato funkce zji¹»uje hodnotu datové èásti polo¾ky zadané klíèem.
** Pokud je polo¾ka nalezena, vrací funkce ukazatel na polo¾ku
** Pokud polo¾ka nalezena nebyla, vrací se funkèní hodnota NULL
**
** Vyu¾ijte døíve vytvoøenou funkci HTSearch.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vyjme polo¾ku s klíèem key z tabulky
** ptrht.  Uvolnìnou polo¾ku korektnì zru¹te.  Pokud polo¾ka s uvedeným
** klíèem neexistuje, dìlejte, jako kdyby se nic nestalo (tj. nedìlejte
** nic).
**
** V tomto pøípadì NEVYU®ÍVEJTE døíve vytvoøenou funkci HTSearch.
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

/* TRP s explicitnì zøetìzenými synonymy.
** Tato procedura zru¹í v¹echny polo¾ky tabulky, korektnì uvolní prostor,
** který tyto polo¾ky zabíraly, a uvede tabulku do poèáteèního stavu.
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
