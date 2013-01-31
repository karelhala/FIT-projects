
/* c201.c *********************************************************************}
{* Téma: Jednosmìrný lineární seznam
**
**                     Návrh a referenèní implementace: Petr Pøikryl, øíjen 1994
**                                          Úpravy: Andrea Nìmcová listopad 1996
**                                                   Petr Pøikryl, listopad 1997
**                                Pøepracované zadání: Petr Pøikryl, bøezen 1998
**                                  Pøepis do jazyka C: Martin Tuèek, øíjen 2004
**	                                          Úpravy: Bohuslav Køena, øíjen 2010
**
** Implementujte abstraktní datový typ jednosmìrný lineární seznam.
** U¾iteèným obsahem prvku seznamu je celé èíslo typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou typu tList.
** Definici konstant a typù naleznete v hlavièkovém souboru c201.h.
** 
** Va¹ím úkolem je implementovat následující operace, které spolu s vý¹e
** uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DisposeList ... zru¹ení v¹ech prvkù seznamu,
**      InsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zru¹í první prvek seznamu,
**      PostDelete .... ru¹í prvek za aktivním prvkem,
**      PostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal¹í prvek seznamu,
**      Active ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci funkcí nevolejte ¾ádnou z funkcí implementovaných v rámci
** tohoto pøíkladu, není-li u funkce explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error()	{
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* Chyba pøi práci se seznamem.\n");
    errflg = TRUE;                      /* globální promìnná -- pøíznak chyby */
}

void InitList (tList *L)	{
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
	//první nastavím na NULL, pokud se nepovede, vypisi chybu
	L->First = NULL;
	if (L->First!=NULL)
		Error();
	L->Act = NULL;

}

void DisposeList (tList *L)	{
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. V¹echny prvky seznamu L budou korektnì uvolnìny voláním
** operace free.
***/

	tElemPtr item;
	//Dokud prvni prvek nebude ukazovat na NULL odstranuju jeden prvek po druhym
	while(L->First)
	{
		item = L->First;
        L->First = item->ptr;
        free(item);
	}
	//Nastavim aktivni prvek na NULL
	L->Act=NULL;
}

void InsertFirst (tList *L, int val)	{
/*
** Vlo¾í prvek s hodnotou val na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/

	//Vlozim data a ukazi na ne, jako na prvni prvek
	tElemPtr item = malloc(sizeof(struct tElem));
	if (item==NULL)
		Error();
	item->data=val;
	item->ptr = L->First;
    L->First = item;

}

void First (tList *L)		{
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný pøíkaz, ani¾ byste testovali,
** zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val)	{
/*
** Vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if (L->First!=NULL){
		//do item ulozim ukazatel na prvni prvek a potom vypisi hodnotu
		tElemPtr item;
		item=L->First;
		*val = item->data;
	}
	else
		Error();
}

void DeleteFirst (tList *L)	{
/*
** Ru¹í první prvek seznamu L. Pokud byl ru¹ený prvek aktivní, aktivita seznamu
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje!
**/
	if (L->Act==L->First)
		L->Act=NULL;
	tElemPtr item;
    if (L->First)
    {
		//do item ulozim ukazatel na prvni prvek, potom nastavim ukazatel na dalsi prvek a prvni prvek smazu
        item = L->First;
        L->First = item->ptr;
        free(item);
    }
}	

void PostDelete (tList *L)		{
/* 
** Ru¹í prvek seznamu L za aktivním prvkem. Pokud není seznam L aktivní
** nebo pokud je aktivní poslední prvek seznamu L, nic se nedìje!
**/
	if (L->Act!=NULL){
		tElemPtr item,item_2;
		item=L->Act;
		if (item->ptr!=NULL){
			//do item_2 ulozim ukazatel na dalsi prvek po prvku, ktery chci smazt
			item_2=item->ptr->ptr;
			//uvolnim prvek po aktivnim prvku
			free(item->ptr);
			//Spojim seznam zase dohromady
			item->ptr=item_2;
		}
	}
}

void PostInsert (tList *L, int val)	{
/*
** Vlo¾í prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje!
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** zavolá funkci Error().
**/
	if (L->Act!=NULL)
	{
		tElemPtr item,item_2;
		item = malloc(sizeof(struct tElem));
		
		if (item==NULL)
			Error();
		//Pouziji podobnou posloupnost prikazu, jako pri vkladani prvniho prvku
		item->data=val;
		item_2=L->Act;
		item->ptr=item_2->ptr;
		item_2->ptr=item;
	}
}

void Copy (tList *L, int *val)		{
/*
** Vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/

	//nacte aktivni prvek a potom jeho data ulozi
	if (L->Act!=NULL){
		tElemPtr item;
		item=L->Act;
		*val = item->data;
	}
	else
		Error();
	
}

void Actualize (tList *L, int val)	{
/*
** Pøepí¹e data aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	if (L->Act!=NULL){
		tElemPtr item;
		item=L->Act;
		item->data=val;
	}	
}

void Succ (tList *L)	{
/*
** Posune aktivitu na následující prvek seznamu L.
** V¹imnìte si, ¾e touto operací se mù¾e aktivní seznam stát neaktivním.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	if (L->Act!=NULL){
		tElemPtr item;
		item=L->Act;
		//posunu ukazatel o jeden prvek dal
		L->Act=item->ptr;
	}	
}

int Active (tList *L) 	{		
/*
** Je-li seznam L aktivní, vrací True. V opaèném pøípadì vrací false.
** Tuto funkci implementujte jako jediný pøíkaz return. 
**/
	//Pokud je aktivni prvek nastaven na NULL vratim hodnotu False, jinak True
	return (L->Act==NULL) ? FALSE : TRUE;
}

/* Konec c201.c */
