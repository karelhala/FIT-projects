	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2010
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* Chyba p�i pr�ci se seznamem.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L)	{
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
    L->First = NULL;	//prvni nastavim na NULL
    L->Last=NULL;	//posledni nastavim na NULL
	if (L->First!=NULL||L->Last!=NULL)
		DLError();	//pokud jeden z nich neni NULL, nepovedla se nam inicializace
	L->Act = NULL;	//nastavim aktivni prvek na NULL
}

void DLDisposeList (tDLList *L)	{
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	tDLElemPtr item;	//jako prvek, ktery budu pouzovat pozdeji pouziju promennou item
	while(L->First) //cykl�m dokud ukazatel na prvni prvek nebude NULL
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
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act=L->First;	//aktivni prvek ukazu na prvni
}

void DLLast (tDLList *L)	{
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act=L->Last;		//aktivni prvek ukazu na posledni
}

void DLCopyFirst (tDLList *L, int *val)	{
/*
** Vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
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
** Vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
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
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
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
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
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
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act!=NULL){
		tDLElemPtr item,item_2;
		item=L->Act;	//pokud je seznam aktivn� nactu do promenne ukazatel na aktivni prcek
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
** Vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
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
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;		//pokud je seznam aktivni
		item=L->Act;			//ulozim do aktivniho prvku nove nactena data
		item->data=val;
	}
}

void DLSucc (tDLList *L)	{
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;
		item=L->Act;			//pokud je seznam aktivni posunu jeho aktivitu o jednu do prava
		L->Act=item->rptr;
	}	
}


void DLPred (tDLList *L)	{
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act!=NULL){
		tDLElemPtr item;
		item=L->Act;			//pokud je seznam aktivni posunu jeho aktivitu o jednu do leva
		L->Act=item->lptr;
	}
}

int DLActive (tDLList *L) {		
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
	
	return (L->Act==NULL) ? FALSE : TRUE;		//pokud je seznam aktivni vratim hodnotu TRUE, pokud ne, vratim hodnotu FALSE
}

/* Konec c206.c*/
