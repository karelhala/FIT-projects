
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Pøikryl, prosinec 1994
**                                           Úpravy: Petr Pøikryl, listopad 1997
**                                                     Petr Pøikryl, kvìten 1998
**			  	                        Pøevod do jazyka C: Martin Tuèek, srpen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**
** S vyu¾itím dynamického pøidìlování pamìti, implementujte NEREKURZIVNÌ
** následující operace nad binárním vyhledávacím stromem (pøedpona BT znamená
** Binary Tree a je u identifikátorù uvedena kvùli mo¾né kolizi s ostatními
** pøíklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vlo¾ení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní prùchod typu pre-order
**     BTInorder ....... nerekurzivní prùchod typu in-order
**     BTPostorder ..... nerekurzivní prùchod typu post-order
**     BTDisposeTree ... zru¹ v¹echny uzly stromu
**
** U v¹ech funkcí, které vyu¾ívají nìkterý z prùchodù stromem, implementujte
** pomocnou funkci pro nalezení nejlevìj¹ího uzlu v podstromu.
**
** Pøesné definice typù naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nìj je typu tBTNodePtr. Jeden uzel obsahuje polo¾ku int Cont,
** která souèasnì slou¾í jako u¾iteèný obsah i jako vyhledávací klíè 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Pøíklad slou¾í zejména k procvièení nerekurzivních zápisù algoritmù
** nad stromy. Ne¾ zaènete tento pøíklad øe¹it, prostudujte si dùkladnì
** principy pøevodu rekurzivních algoritmù na nerekurzivní. Programování
** je pøedev¹ím in¾enýrská disciplína, kde opìtné objevování Ameriky nemá
** místo. Pokud se Vám zdá, ¾e by nìco ¹lo zapsat optimálnìji, promyslete
** si v¹echny detaily Va¹eho øe¹ení. Pov¹imnìte si typického umístìní akcí
** pro rùzné typy prùchodù. Zamyslete se nad modifikací øe¹ených algoritmù
** napøíklad pro výpoèet poètu uzlù stromu, poètu listù stromu, vý¹ky stromu
** nebo pro vytvoøení zrcadlového obrazu stromu (pouze popøehazování ukazatelù
** bez vytváøení nových uzlù a ru¹ení starých).
**
** Pøi prùchodech stromem pou¾ijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodu¹ení práce máte pøedem pøipraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
**
** Pozor! Je tøeba správnì rozli¹ovat, kdy pou¾ít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, pou¾ijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat pøi prùchodech stromem pro zpracování
** uzlu urèeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else 
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
{ 
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: Do¹lo k pøeteèení zásobníku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Do¹lo k podteèení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: Do¹lo k pøeteèení zásobníku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Do¹lo k podteèení zásobníku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze pøed prvním pou¾itím binárního
** stromu, proto¾e neuvolòuje uzly neprázdného stromu (a ani to dìlat nemù¾e,
** proto¾e pøed inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zru¹ení binárního stromu slou¾í procedura BTDisposeTree.
**	
** V¹imnìte si, ¾e zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,	
** proto je tøeba pøi práci s RootPtr pou¾ít dereferenèní operátor *.
**/
	
	
	//nastavim prvni prvek na NULL
	
	(*RootPtr)=NULL;
	 //solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vlo¾í do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytváøený strom jako binární vyhledávací strom,
** kde uzly s hodnotou men¹í ne¾ má otec le¾í v levém podstromu a uzly vìt¹í
** le¾í vpravo. Pokud vkládaný uzel ji¾ existuje, neprovádí se nic (daná hodnota
** se ve stromu mù¾e vyskytnout nejvý¹e jednou). Pokud se vytváøí nový uzel,
** vzniká v¾dy jako list stromu. Funkci implementujte nerekurzivnì.
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
		
	//solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Preorder nav¹tívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na nì is ulo¾íme do zásobníku.
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
	
	
	 //solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Prùchod stromem typu preorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Preorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
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
	 //solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Inorder ukládáme ukazatele na v¹echny nav¹tívené uzly do
** zásobníku. 
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
	
	
	
	// solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
	
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Prùchod stromem typu inorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Inorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
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
	
	// solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Postorder ukládáme ukazatele na nav¹tívené uzly do zásobníku
** a souèasnì do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** nav¹tíven poprvé a ¾e se tedy je¹tì nemá zpracovávat. 
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
	
	
	//solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Prùchod stromem typu postorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Postorder, zásobníku ukazatelù a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
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
	//solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zru¹í v¹echny uzly stromu a korektnì uvolní jimi zabranou pamì».
**
** Funkci implementujte nerekurzivnì s vyu¾itím zásobníku ukazatelù.
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
	
	//solved = FALSE;		  /* V pøípadì øe¹ení sma¾te tento øádek! */	
}

/* konec c402.c */

