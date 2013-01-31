
/* ******************************* c202.c *********************************** */
/*  Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì                                 */
/*  Úkol: c202 - Zásobník znakù v poli                                        */
/*  Referenèní implementace: Petr Pøikryl, 1994                               */
/*  Vytvoøil: Václav Topinka, záøí 2005                                       */
/*  Úpravy: Bohuslav Køena, záøí 2011                                         */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znakù ve statickém poli. ADT zásobník je
** reprezentován záznamem typu tStack. Statické pole 'arr' je indexováno
** do maximální hodnoty STACK_SIZE. Polo¾ka záznamu 'top' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota top == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Pøesnou definici typù neleznete
** v hlavièkovém souboru c202.h. 
**
** Implementujte následující funkce:
**
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplnìnost zásobníku
**    stackTop ..... pøeète hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vlo¾í prvku do zásobníku
**
** Své øe¹ení úèelnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c202.h"

int STACK_SIZE = MAX_STACK;
int solved;

void stackError ( int error_code ){
/*   ----------
** Vytiskne upozornìní na to, ¾e do¹lo k chybì pøi práci se zásobníkem.
**
** TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
*/
	static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
	if ( error_code <= 0 || error_code > MAX_SERR )
		error_code = 0;
	printf ( "%s\n", SERR_STRINGS[error_code] );
	err_flag = 1;
}

void stackInit ( tStack* s ) {
/*   ---------
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
** Hodnoty ve statickém poli neupravujte - po inicializaci zásobníku
** jsou nedefinované.
**
** V pøípadì, ¾e funkce dostane jako parametr s == NULL,
** volejte funkci stackError(SERR_INIT). U ostatních funkcí pro zjednodu¹ení
** pøedpokládejte, ¾e tato situace nenastane. 
*/

	if (s!=NULL)
		s->top=-1;	//Pokud pri vytvareni zasobniku neni zasobnik NULL, nastavim vrchol zasobniku na -1
	else
		stackError(SERR_INIT);	//jinak volam error
}

int stackEmpty ( const tStack* s ) {
/*  ----------
** Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
** Funkci implementujte jako jediný pøíkaz. Vyvarujte se zejména konstrukce
** typu "if ( true ) b=true else b=false".
*/
	return (s->top!=-1) ? 0 : s->top; //pokud vrsek zasobniku, neni nastaven na -1 vratim cislo vrcholu zasobniku
}

int stackFull ( const tStack* s ) {
/*  ---------
** Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
** Dejte si zde pozor na èastou programátorskou chybu "o jednièku"
** a dobøe se zamyslete, kdy je zásobník plný, jestli¾e mù¾e obsahovat
** nejvý¹e STACK_SIZE prkvù a první prvek je vlo¾en na pozici 0.
**
** Funkci implementujte jako jediný pøíkaz.
*/
	return ((s->top+1)<STACK_SIZE) ? 0 : 1; //pokud je vrchol zasobniku mensi nez velikost zasobniku, vratim nenulovou hodnotu
}

void stackTop ( const tStack* s, char* c ) {
/*   --------
** Vrací znak z vrcholu zásobníku prostøednictvím parametru c.
** Tato operace ale prvek z vrcholu zásobníku neodstraòuje.
** Volání operace Top pøi prázdném zásobníku je nekorektní
** a o¹etøete ho voláním funkce stackError(SERR_TOP). 
**
** Pro ovìøení, zda je zásobník prázdný, pou¾ijte døíve definovanou
** funkci stackEmpty.
*/
	if (stackEmpty(s)==-1) 	//Pokud je vrchol zasobniku nastaven na -1 (je prazdny) zavolam funkci pro vypis chyby
		stackError(SERR_TOP);
	else{
		*c=s->arr[s->top]; //do promenne c dal hodnotu z vrcholu zasobniku
	}
}


void stackPop ( tStack* s ) {
/*   --------
** Odstraní prvek z vrcholu zásobníku. Pro ovìøení, zda je zásobník prázdný,
** pou¾ijte døíve definovanou funkci stackEmpty.
**
** Vyvolání operace Pop pøi prázdném zásobníku je sice podezøelé a mù¾e
** signalizovat chybu v algoritmu, ve kterém je zásobník pou¾it, ale funkci
** pro o¹etøení chyby zde nevolejte (mù¾eme zásobník ponechat prázdný).
** Spí¹e ne¾ volání chyby by se zde hodilo vypsat varování, co¾ v¹ak pro
** jednoduchost nedìláme.
** 
*/
	if (stackEmpty(s)==0)
		s->top--; //pokud neni zasobnik prazdny, odstranim hodnotu na vrcholu zasobniku
}


void stackPush ( tStack* s, char c ) {
/*   ---------
** Vlo¾í znak na vrchol zásobníku. Pokus o vlo¾ení prvku do plného zásobníku
** je nekorektní a o¹etøete ho voláním procedury stackError(SERR_PUSH).
**
** Pro ovìøení, zda je zásobník plný, pou¾ijte døíve definovanou
** funkci stackFull.
*/
	if (stackFull(s)!=0)
		stackError(SERR_PUSH); //Pokud je zasobnik plny, zavolam funkci na vypis chyby
	else{
		s->top++;	//jinak zvysim pocet prvku v zasobniku
		s->arr[s->top]=c;	//a ulozim hodnotu na vrchol
	}
}

/* Konec c202.c */
