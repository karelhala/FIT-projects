	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2010
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* Chyba pøi práci se seznamem.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L)	{
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;	//prvni nastavim na NULL
    L->Last=NULL;	//posledni nastavim na NULL
	if (L->First!=NULL||L->Last!=NULL)
		DLError();	//pokud jeden z nich neni NULL, nepovedla se nam inicializace
	L->Act = NULL;	//nastavim aktivni prvek na NULL
}

void DLDisposeList (tDLList *L)	{
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
	tDLElemPtr item;	//jako prvek, ktery budu pouzovat pozdeji pouziju promennou item
	while(L->First) //cyklím dokud ukazatel na prvni prvek nebude NULL
	{
		item = L->First;	//do item ulozim ukazatel na prvni prvek
        L->First = item->rptr;	//ukazatel na prvni prvek nastavim na dalsi prvek
        free(item);			//uvolnim prvni prvek
	}
	L->Act=NULL;	//aktivni prvek nastavim na NULL
	L->Last=NULL;	//ukazatel na posledni prvek nastavim na NULL
}

void DLInsertFirst (tDLList *L, int val)	{
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr item=NULL,item_2=NULL; 
	item = malloc(sizeof(struct tDLElem));
	if (item==NULL)
		DLError();
	item->data=val;	//do item ulozim promennou
	if (L->First==NULL){	//pokud je ukazatel na prvni prvek NULL (je prazdny seznam)
		item->rptr=NULL;	//nastavim ukazatel do prava na NULL
		item->lptr=NULL;	//nastavim ukazatel do leva na NULL
		L->First=item;		//ukazatel na prvni prvek nastavim na nami nove vznikly item
		L->Last=item;		//ukazatel na posledni prvek nastavim na nami nove vznikly item
	}
	else{
		item->rptr=L->First;	//pokud nemame prazdny seznam nastavim ukazatel do prava na prvni prvek
		item_2=L->First;		//do item_2 ulozim ukazatel na prvni prvek, aby se nam nerozapdl seznam
		item->lptr=item_2->lptr;	//ukazatel do leva nastavim na stejny ukazatel, jako byl predtim
		item_2->lptr=item;	//ukazatel do leva nastavim na nove vznikly prvek
		L->First=item;		//ukazatel na prvni prvek dam na nove vznikly prvek
	}
}

void DLInsertLast(tDLList *L, int val)	{
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr item=NULL,item_2=NULL;
	item = malloc(sizeof(struct tDLElem));
	if (item==NULL)
		DLError();
	item->data=val;	//do item ulozim promennou
	if (L->Last==NULL){		//pokud je ukazatel na posledni prvek NULL (je prazdny seznam)
		item->rptr=NULL;	//nastavim ukazatel do prava na NULL
		item->lptr=NULL;	//nastavim ukazatel do leva na NULL
		L->Last=item;		//ukazatel na posledni prvek nastavim na nami nove vznikly item
		L->First=item;		//ukazatel na prvni prvek nastavim na nami nove vznikly item
	}
	else{
		item->lptr=L->Last;		//pokud nemame prazdny seznam nastavim ukazatel do leva na posledni prvek
		item_2=L->Last;			//do item_2 ulozim ukazatel na posledni prvek, aby se nam nerozapdl seznam
		item->rptr=item_2->rptr;	//ukazatel do prava nastavim na stejny ukazatel, jako byl predtim
		item_2->rptr=item;			//ukazatel do prava nastavim na nove vznikly prvek
		L->Last=item;				//ukazatel na posledni prvek dam na nove vznikly prvek
	}
}

void DLFirst (tDLList *L)	{
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act=L->First;	//aktivni prvek ukazu na prvni
}

void DLLast (tDLList *L)	{
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act=L->Last;		//aktivni prvek ukazu na posledni
}

void DLCopyFirst (tDLList *L, int *val)	{
/*
** Vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First!=NULL){
		tDLElemPtr item;
		item=L->First;	//do item ulozim ukazatel na prvni prvek
		*val = item->data;	//do promenne val ulozim data prvniho prvku
	}
	else
		DLError();
}

void DLCopyLast (tDLList *L, int *val)	{
/*
** Vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last!=NULL){
		tDLElemPtr item;
		item=L->Last;	//do item ulozim ukazatel na prvni prvek
		*val = item->data;	//do promenne val ulozim data prvniho prvku
	}
	else
		DLError();
}

void DLDeleteFirst (tDLList *L)	{
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	if (L->Act==L->First)
		L->Act = NULL;		//pokud je ukazatel na prvni prvek aktivni zrusim aktivitu
	if (L->First!=NULL){	
		tDLElemPtr item;
		item=L->First;	//pokud je seznam neprazdny ulozim ukazatel na prvni prvek do pomocne item
		if (L->First==L->Last){
			L->First=NULL;	//pokud je v seznamu jeden prvek nastavim oba ukazatele na NULL
			L->Last=NULL;
		}
		else{
			L->First=L->First->rptr;	//jinak nastavim ukazatel na prvni prvek na druhy prvek prvek
			L->First->lptr=NULL;	//a ukazatel do leva nastavim na NULL
		}
		free(item);			//uvolnim prvni prvek
	}
}	

void DLDeleteLast (tDLList *L)	{
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
	if (L->Act==L->Last)
		L->Act = NULL;		//pokud je ukazatel na posledni prvek aktivni zrusim aktivitu
	if (L->Last!=NULL){
		tDLElemPtr item;
		item=L->Last;		//pokud je seznam neprazdny ulozim ukazatel na posledni prvek do pomocne item
		if (L->First==L->Last){
			L->First=NULL;	//pokud je v seznamu jeden prvek nastavim oba ukazatele na NULL
			L->Last=NULL;
		}
		else{
			L->Last=L->Last->lptr;	//jinak nastavim ukazatel na posledni prvek na predposledni prvek
			L->Last->rptr=NULL;		//a ukazatel do prava nastavim na NULL
		}
		free(item);				//uvolnim poseldni prvek
	}
}

void DLPostDelete (tDLList *L)	{
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
	if(L->Act!=NULL && L->Act!=L->Last){
		tDLElemPtr item;	//pokud neni aktivni prvek posledni a seznam je vubec aktivni
		item=L->Act->rptr;	//do promenne item, ulozim prvek ktery budu mazat
		
		if(item==L->Last){
			L->Last=L->Act;	//pokud je prvek, ktery budu mazat posledni, nastavim posledni prvek na aktivni
			L->Last->rptr=NULL;	//a ukazatel do prava na NULL
		}
		else{
			item->lptr->rptr=item->rptr;	//pokud je prvek co budu mazat, nekde u prostred seznamu, prevazu cely seznam
			item->rptr->lptr=item->lptr;
		}
		free(item);	//uvolnim prvek
	}
}

void DLPreDelete (tDLList *L)	{
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
	if(L->Act!=NULL && L->Act!=L->First){
		tDLElemPtr item;	//pokud neni aktivni prvek pprvni a seznam je vubec aktivni
		item=L->Act->lptr;	//do promenne item, ulozim prvek ktery budu mazat
		if (item->lptr==NULL){
			L->First=L->Act;		//pokud je prvek, ktery budu mazat prvni, nastavim prvni prvek na aktivni
			L->First->lptr=NULL;	//a ukazatel do leva na NULL
		}
		else{
			item->lptr->rptr=item->rptr;	//pokud je prvek co budu mazat, nekde u prostred seznamu, prevazu cely seznam
			item->rptr->lptr=item->lptr;	
		}
		free(item);		//uvolnim prvek
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if(L->Act!=NULL){
		tDLElemPtr item,item_2;
		item=L->Act;	//promennou item nastavim na aktivni prvek
		item_2 = malloc(sizeof(struct tDLElem));
		item_2->data=val;
		if (L->Act==L->Last){	//pokud je aktivni prvek posledni
			item_2->lptr = L->Last;	//do prvku, ktery budu ukladat nastavim ukazatel do leva, na posledni
			item_2->rptr=NULL;		//do prvku, ktery budu ukladat nastavim ukazatel do prava na NULL
			item->rptr=item_2;		//ukazatel z aktivniho prvku do prava, nastavim na nove vznikly prvek
			L->Last=item_2;			//ukazatel na posledni prvek nastavim na novy prvek
		}else{
			item_2->lptr=item;	//pokud chci ulozit prvek nekam do seznamu a nejsem na konci do nove ukladaneho prvku nastavim ukazatel do leva na aktivni prvek
			item_2->rptr=item->rptr;	//ukazatel do prava nastavim na aktivni prvek a ukzatel do prava
			item->rptr->lptr=item_2;	//ukazatel do leva u aktivniho prvku nastavim na nove ukladany prvek
			item->rptr=item_2;			//ukazatel do prava nastavim na nove ukladany prvek
		}
		
	}
}

void DLPreInsert (tDLList *L, int val)		{
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if(L->Act!=NULL){
		tDLElemPtr item,item_2;
		item=L->Act;	//pokud je seznam aktivní nactu do promenne ukazatel na aktivni prcek
		item_2 = malloc(sizeof(struct tDLElem));
		item_2->data=val;	//ulozim nove nactena data
		if (L->Act==L->First){
			item_2->rptr=L->First;	//pokud byl aktivni prvek prvni ulozim nove nacteny prvek na prvni pozici
			item_2->lptr=NULL;		//spojim seznam dohromady a do ukazatele na prvni prvek ulozim novy prvek
			item->lptr=item_2;
			L->First=item_2;
		}else{
			item_2->rptr=item;			//pokud je nove ukladany prvek, nekde v seznamu
			item_2->lptr=item->lptr;	//spojim seznam dohromady
			item->lptr->rptr=item_2;	//a ulozim nove nacteny prvek
			item->lptr=item_2;
		}

	}
}

void DLCopy (tDLList *L, int *val)	{
/*
** Vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	if (L->Act==NULL)
		DLError ();		//pokud neni seznam aktivni zavolam funkci pro vypis chyby
	else{
		tDLElemPtr item;		//jinak vytisknu hodnotu aktivniho prvku
		item=L->Act;
		*val = item->data;
	}
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;		//pokud je seznam aktivni
		item=L->Act;			//ulozim do aktivniho prvku nove nactena data
		item->data=val;
	}
}

void DLSucc (tDLList *L)	{
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;
		item=L->Act;			//pokud je seznam aktivni posunu jeho aktivitu o jednu do prava
		L->Act=item->rptr;
	}	
}


void DLPred (tDLList *L)	{
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;
		item=L->Act;			//pokud je seznam aktivni posunu jeho aktivitu o jednu do leva
		L->Act=item->lptr;
	}
}

int DLActive (tDLList *L) {		
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/
	
	return (L->Act==NULL) ? FALSE : TRUE;		//pokud je seznam aktivni vratim hodnotu TRUE, pokud ne, vratim hodnotu FALSE
}

/* Konec c206.c*/
