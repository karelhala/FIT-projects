	/*
 	* Soubor:  	proj4
 	* Datum:   	2010/11/11
	* Autor:   	Karel Hala, xhalak00d@fit.vutbr.cz
	* Projekt: 	èeské øazení
	* Popis: 	prgram provádí:
	*				vytiskne zpravny sloupec do souboru
	*				provede funkci before/after
	*				seradi tisknuty sloupec pomoci bubble sort, tudiz je program pri tisknu serazeneho sloupce pomalejsi
	*				bohuzel jsem nestihl implementovat to, ze budou jednotlive sloupce oddeleny vice nez jednim oddelovacem
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>


//datová struktura seznamu
typedef struct item TItem;
struct item {
    char *uloz; //zde se uchovávají jednotlivé prvky seznamu

    TItem *next; //ukazatel na nový prvek seznamu
};
//dtová struktura TList (samostatný seznam)
typedef struct {
    TItem *first; //první prvek
    TItem *last; 	//poslední prvek
} TList;

//datová struktura jednotlivých øetìzcù
typedef struct str{
	char *sloupec; //data podle kterych se bude ridit funkce before a after
	char *vystup;	//data ktera se budou tisknout (ukladat)
}Tstr;


// Kódy chyb programu
enum tecodes
{
  EOK = 0,     //Bez chyby
  ECLWRONG,    //Chybný pøíkazový øádek.
  EUNKNOWN,    //Neznámá chyba
};

//kod parametru
enum tstates
{
  	CHELP,			//NÁPOVÌDA
	BEFORE,			//tisknutí sloupce pøed øetìzcem
	AFTER,			//tisknutí sloupce za øetìzcem
	PRINT,			//tisknutí celého sloupce
	BEFORE_SORT,	//before + øazení
	AFTER_SORT,		//after + øazení
	PRINT_SORT,		//tisk + øazení
};
//kod pro porovnvani dvou retezcu
enum Tporovnej_vysl
{
	rovno, 	//pokud se budou rovnat
	mensi,	//pokud budou mensi
	vetsi,	//pokud budou vetsi
};
/** Chybová hlá¹ení odpovídající chybovým kódùm. */
const char *ECODEMSG[] =
{
	// vsechno v poradku
	"EOK",
  	/* ECLWRONG */
	"Chybne parametry prikazoveho radku\n"
	"	     pro napovedu napis -h\n",
	//neocekavana chyba
	"Neocekavana chyba\n",
};
//nápovìda
const char *HELPMSG = 
	"\nProgram èeské øazení\n"
	"Autor: Karel Hala (c) 2010\n"
	"Pou¾ití: proj4 -h				nápovìda\n"
	"         proj4 [parametry] /vstup/ /výstup/	spu¹tìní programu\n\n"
	"         popis polo¾ky [parametry]\n\n"
	"         proj4 --before sloupec polo¾ka\n"
	"         od polo¾ky vypí¹e sloupec (smìrem dolù)\n\n"
	"         proj4 --after sloupec polo¾ka\n"
	"         do polo¾ky vypí¹e sloupec (smìr nahoru)\n\n"
	"         proj4 --sort 				seøadí postupnì polo¾ky\n"
	"         proj4 --print sloupec 			vytiskne sloupec (povinný)\n\n";
//konec nápovìdy


//hodnoty parametrù pøíkazové øádky.
 
typedef struct params
{
  int ecode;        				//Chybový kod programu, odpovídá výètu tecodes
  int state;        				// Stavový kód programu, odpovídá výètu tstates
} TParams;

void printECode(int ecode)
{
	 fprintf(stderr, "%s", ECODEMSG[ecode]);
}

//získní parametrù
TParams getParams(int argc, char *argv[],char **hledany,Tstr *strings)
{
	//pomocné do kterých si ukládám zda je pøesnì 1 parametr deného druhu v pøíkazové øádce
	int sort=0,bef=0,aft=0,print=0;

	TParams result =
	{ // inicializace struktury
		.ecode = EOK,
		.state = CHELP,
	};

	if (argc == 2 && strcmp("-h", argv[1]) == 0)
	{ // tisk nápovìdy
		result.state = CHELP;
	}
	//pokud bude mene nez 9 parametru provedu zkontrolovani jednotlivych parametru
	else if (argc <= 9){
		for (int param=1;param<argc;param++){
			//pokud bude v prikazove radce --sort zvysim jeho pocet o jedna
			if (strcmp("--sort", argv[param]) == 0)
				sort +=1;

			//pokud bude v prkazove radce --before zvysim jeho pocet o jedna
			else if	(strcmp("--before", argv[param]) == 0){
				strings->sloupec = argv[param+1];	//ze zadani plyne, ze pouze na miste nasledujiciho parametru je sloupec ve kterem budeme hledat
				*hledany = argv [param+2];				//ze zadani plyne, ze pouze na 2. miste po --before je retezec znaku se kterymi budu porovnavat
				bef +=1;
			}

			//pokud bude v prkazove radce --after zvysim jeho pocet o jedna
			else if (strcmp("--after", argv[param]) == 0){
				strings->sloupec = argv[param+1];	//ze zadani plyne, ze pouze na miste nasledujiciho parametru je sloupec ve kterem budeme hledat
				*hledany = argv[param+2];				//ze zadani plyne, ze pouze na 2. miste po --before je retezec znaku se kterymi budu porovnavat
				aft+=1;
			}

			//pokud bude v prkazove radce --print zvysim jeho pocet o jedna
			else if (strcmp("--print", argv[param]) == 0){
				print +=1;
				strings->vystup = argv[param+1];		//ze zadani plyne, ze na 1. miste po parametru --print bude nazev sloupce, ktery budeme tisknout
			}
		}

		//pokud budou promenne before a print nastaveny na 1, ale after bude na 0 zkontroluji zda neni v prikazove radce --sort a podle toho vratim vysledek
		if (bef==1 && print==1 && aft==0){
			if (sort==1&&argc==9)
				result.state = BEFORE_SORT;	//pokud bude zapnut sort vratim hodnotu pro before sort
			else if (argc==8)
				result.state = BEFORE;			//jinak vratim pouze hodnotu pro before
			else
				result.state = CHELP;	//pokud bude neco zadane spatne vratim hodnotu pro help
		}

		//pokud budou promenne after a print  nastaveny na 1, ale before bude na 0 zkontroluji zda neni v prikazove radce --sort a podle toho vratim vysledek
		else if (bef==0 && print==1 && aft==1){
			if (sort==1&&argc==9)
				result.state = AFTER_SORT;		//pokud bude zapnut sort vratim hodnotu pro after sort
			else if (argc==8)
				result.state = AFTER;			//jinak vratim pouze hodnotu pro after
			else
				result.state = CHELP;	//pokud bude neco zadane spatne vratim hodnotu pro help
		}

		//pokud bude v prikazove radce pouze print provedu nasledujici:
		else if (bef==0 && print==1 && aft==0){
			if (sort==1&&argc==6){							//pokud bude jeste potreba razeni
				strings->sloupec = strings->vystup;		//aby obe promenne ukazovaly na stejny sloupec
				result.state = PRINT_SORT;					//vratim hodnotu pro print sort
				}

			else if (argc==5){
				if (strings->sloupec==NULL)
					strings->sloupec = strings->vystup;		//obe promenne nyni ukazuji na stejny sloupec
				result.state = PRINT;							//vratim hodnotu pro print
			}
			else
				result.state = CHELP;		//pokud bude neco zadane spatne vratim hodnotu pro help
		}
		else
			result.state = CHELP;			//pokud bude neco zadane spatne vratim hodnotu pro help
	}
  else
  { // pøíli¹ mnoho parametrù
    result.ecode = ECLWRONG;
  }

  return result;
}

//funkce na porovnani ceskych retezcu
int porovnej (char *prvni, char *druhy)
{
	//primarni tabulka
	static const unsigned char prvni_tabulka[256] = {
		[(unsigned char)'A'] = 1,	[(unsigned char)'a'] = 1,
		[(unsigned char)'Á'] = 1,	[(unsigned char)'á'] = 1,
		[(unsigned char)'B'] = 2,	[(unsigned char)'b'] = 2,
		[(unsigned char)'C'] = 3,	[(unsigned char)'c'] = 3,
		[(unsigned char)'È'] = 4,	[(unsigned char)'è'] = 4,
		[(unsigned char)'D'] = 5,	[(unsigned char)'d'] = 5,
		[(unsigned char)'Ï'] = 5,	[(unsigned char)'ï'] = 5,
		[(unsigned char)'E'] = 6,	[(unsigned char)'e'] = 6,
		[(unsigned char)'É'] = 6,	[(unsigned char)'é'] = 6,
		[(unsigned char)'Ì'] = 6,	[(unsigned char)'ì'] = 6,
		[(unsigned char)'F'] = 7,	[(unsigned char)'f'] = 7,
		[(unsigned char)'G'] = 8,	[(unsigned char)'g'] = 8,
		[(unsigned char)'H'] = 9,	[(unsigned char)'h'] = 9,
		[(unsigned char)'I'] = 10,	[(unsigned char)'i'] = 10,
		[(unsigned char)'Í'] = 10,	[(unsigned char)'í'] = 10,
		[(unsigned char)'J'] = 11,	[(unsigned char)'j'] = 11,
		[(unsigned char)'K'] = 12,	[(unsigned char)'k'] = 12,
		[(unsigned char)'L'] = 13,	[(unsigned char)'l'] = 13,
		// Èíslo 14 je pro ch, nebo Ch, nebo CH
		[(unsigned char)'M'] = 15,	[(unsigned char)'m'] = 15,
		[(unsigned char)'N'] = 16,	[(unsigned char)'n'] = 16,
		[(unsigned char)'Ò'] = 16,	[(unsigned char)'ò'] = 16,
		[(unsigned char)'O'] = 17,	[(unsigned char)'o'] = 17,
		[(unsigned char)'Ó'] = 17,	[(unsigned char)'ó'] = 17,
		[(unsigned char)'P'] = 18,	[(unsigned char)'p'] = 18,
		[(unsigned char)'Q'] = 19,	[(unsigned char)'q'] = 19,
		[(unsigned char)'R'] = 20,	[(unsigned char)'r'] = 20,
		[(unsigned char)'Ø'] = 21,	[(unsigned char)'ø'] = 21,
		[(unsigned char)'S'] = 22,	[(unsigned char)'s'] = 22,
		[(unsigned char)'©'] = 23,	[(unsigned char)'¹'] = 23,
		[(unsigned char)'T'] = 24,	[(unsigned char)'t'] = 24,
		[(unsigned char)'«'] = 24,	[(unsigned char)'»'] = 24,
		[(unsigned char)'U'] = 25,	[(unsigned char)'u'] = 25,
		[(unsigned char)'Ú'] = 25,	[(unsigned char)'ú'] = 25,
		[(unsigned char)'Ù'] = 25,	[(unsigned char)'ù'] = 25,
		[(unsigned char)'V'] = 26,	[(unsigned char)'v'] = 26,
		[(unsigned char)'W'] = 27,	[(unsigned char)'w'] = 27,
		[(unsigned char)'X'] = 28,	[(unsigned char)'x'] = 28,
		[(unsigned char)'Y'] = 29,	[(unsigned char)'y'] = 29,
		[(unsigned char)'Ý'] = 29,	[(unsigned char)'ý'] = 29,
		[(unsigned char)'Z'] = 30,	[(unsigned char)'z'] = 30,
		[(unsigned char)'®'] = 31,	[(unsigned char)'¾'] = 31,
		[(unsigned char)'0'] = 32,	[(unsigned char)'1'] = 33,
		[(unsigned char)'2'] = 34,	[(unsigned char)'3'] = 35,
		[(unsigned char)'4'] = 36,	[(unsigned char)'5'] = 37,
		[(unsigned char)'6'] = 38,	[(unsigned char)'7'] = 39,
		[(unsigned char)'8'] = 40,	[(unsigned char)'9'] = 41,
		[(unsigned char)'.'] = 42,	[(unsigned char)','] = 43,
		[(unsigned char)';'] = 44,	[(unsigned char)'?'] = 45,
		[(unsigned char)'!'] = 46,	[(unsigned char)':'] = 47,
		[(unsigned char)'"'] = 48,	[(unsigned char)'-'] = 49,
		[(unsigned char)'|'] = 50,	[(unsigned char)'/'] = 51,
		[(unsigned char)'\\'] = 52,[(unsigned char)'('] = 53,
		[(unsigned char)')'] = 54,	[(unsigned char)'['] = 55,
		[(unsigned char)']'] = 56,	[(unsigned char)'<'] = 57,
		[(unsigned char)'>'] = 58,	[(unsigned char)'{'] = 59,
		[(unsigned char)'}'] = 60,	[(unsigned char)'&'] = 61,
		[(unsigned char)'§'] = 62,	[(unsigned char)'%'] = 63,
		[(unsigned char)'$'] = 64, [(unsigned char)'='] = 65,
		[(unsigned char)'+'] = 66, [(unsigned char)'×'] = 67,
		[(unsigned char)'*'] = 68, [(unsigned char)'#'] = 69,
		[(unsigned char)'~'] = 70,
	};
	//sekundarni tabulka
	static const unsigned char druha_tabulka[256] = {
		[(unsigned char)'A'] = 1,	[(unsigned char)'a'] = 1,
		[(unsigned char)'Á'] = 2,	[(unsigned char)'á'] = 2,
		[(unsigned char)'B'] = 3,	[(unsigned char)'b'] = 3,
		[(unsigned char)'C'] = 4,	[(unsigned char)'c'] = 4,
		[(unsigned char)'È'] = 5,	[(unsigned char)'è'] = 5,
		[(unsigned char)'D'] = 6,	[(unsigned char)'d'] = 6,
		[(unsigned char)'Ï'] = 7,	[(unsigned char)'ï'] = 7,
		[(unsigned char)'E'] = 8,	[(unsigned char)'e'] = 8,
		[(unsigned char)'É'] = 9,	[(unsigned char)'é'] = 9,
		[(unsigned char)'Ì'] = 10,	[(unsigned char)'ì'] = 10,
		[(unsigned char)'F'] = 11,	[(unsigned char)'f'] = 11,
		[(unsigned char)'G'] = 12,	[(unsigned char)'g'] = 12,
		[(unsigned char)'H'] = 13,	[(unsigned char)'h'] = 13,
		// Èíslo 14 je pro ch, nebo Ch, nebo CH
		[(unsigned char)'I'] = 15,	[(unsigned char)'i'] = 15,
		[(unsigned char)'Í'] = 16,	[(unsigned char)'í'] = 16,
		[(unsigned char)'J'] = 17,	[(unsigned char)'j'] = 17,
		[(unsigned char)'K'] = 18,	[(unsigned char)'k'] = 18,
		[(unsigned char)'L'] = 19,	[(unsigned char)'l'] = 19,
		[(unsigned char)'M'] = 20,	[(unsigned char)'m'] = 20,
		[(unsigned char)'N'] = 21,	[(unsigned char)'n'] = 21,
		[(unsigned char)'Ò'] = 22,	[(unsigned char)'ò'] = 22,
		[(unsigned char)'O'] = 23,	[(unsigned char)'o'] = 23,
		[(unsigned char)'Ó'] = 24,	[(unsigned char)'ó'] = 24,
		[(unsigned char)'P'] = 25,	[(unsigned char)'p'] = 25,
		[(unsigned char)'Q'] = 26,	[(unsigned char)'q'] = 26,
		[(unsigned char)'R'] = 27,	[(unsigned char)'r'] = 27,
		[(unsigned char)'Ø'] = 28,	[(unsigned char)'ø'] = 28,
		[(unsigned char)'S'] = 29,	[(unsigned char)'s'] = 29,
		[(unsigned char)'©'] = 30,	[(unsigned char)'¹'] = 30,
		[(unsigned char)'T'] = 31,	[(unsigned char)'t'] = 31,
		[(unsigned char)'«'] = 32,	[(unsigned char)'»'] = 32,
		[(unsigned char)'U'] = 33,	[(unsigned char)'u'] = 33,
		[(unsigned char)'Ú'] = 34,	[(unsigned char)'ú'] = 34,
		[(unsigned char)'Ù'] = 35,	[(unsigned char)'ù'] = 35,
		[(unsigned char)'V'] = 36,	[(unsigned char)'v'] = 36,
		[(unsigned char)'W'] = 37,	[(unsigned char)'w'] = 37,
		[(unsigned char)'X'] = 38,	[(unsigned char)'x'] = 38,
		[(unsigned char)'Y'] = 39,	[(unsigned char)'y'] = 39,
		[(unsigned char)'Ý'] = 40,	[(unsigned char)'ý'] = 40,
		[(unsigned char)'Z'] = 41,	[(unsigned char)'z'] = 41,
		[(unsigned char)'®'] = 42,	[(unsigned char)'¾'] = 42,
		[(unsigned char)'0'] = 43,	[(unsigned char)'1'] = 44,
		[(unsigned char)'2'] = 45,	[(unsigned char)'3'] = 46,
		[(unsigned char)'4'] = 47,	[(unsigned char)'5'] = 48,
		[(unsigned char)'6'] = 49,	[(unsigned char)'7'] = 50,
		[(unsigned char)'8'] = 51,	[(unsigned char)'9'] = 52,
		[(unsigned char)'.'] = 53,	[(unsigned char)','] = 54,
		[(unsigned char)';'] = 55,	[(unsigned char)'?'] = 56,
		[(unsigned char)'!'] = 57,	[(unsigned char)':'] = 58,
		[(unsigned char)'"'] = 59,	[(unsigned char)'-'] = 60,
		[(unsigned char)'|'] = 61,	[(unsigned char)'/'] = 62,
		[(unsigned char)'\\'] = 63,[(unsigned char)'('] = 64,
		[(unsigned char)')'] = 65,	[(unsigned char)'['] = 66,
		[(unsigned char)']'] = 67,	[(unsigned char)'<'] = 68,
		[(unsigned char)'>'] = 69,	[(unsigned char)'{'] = 70,
		[(unsigned char)'}'] = 71,	[(unsigned char)'&'] = 72,
		[(unsigned char)'§'] = 73,	[(unsigned char)'%'] = 74,
		[(unsigned char)'$'] = 75, [(unsigned char)'='] = 76,
		[(unsigned char)'+'] = 77, [(unsigned char)'×'] = 78,
		[(unsigned char)'*'] = 79, [(unsigned char)'#'] = 80,
		[(unsigned char)'~'] = 81,
	};
	
	int a=0;	//promenna diky ktere se budu pohybovat v poli
	int znak_jedna=0;	//pomocna do ktere budu ukladat jednotlive hodnoty znaku
	int znak_dva=0;	//pomocna do ktere budu ukladat jednotlive hodnoty znaku
	static const unsigned char *tabulka[] = { prvni_tabulka, druha_tabulka};	//spojeni primarni a sekundarni tabulky
	static const unsigned char CH = 14;	//staticka constanta pro znak Ch
	//pokud bude treba provedu 2 pruchody retezci
	for (int x=0;x<=1;x++){
		a=0;
			//dokud nebude aspon jeden znak \0 prochazim retezce
		while (prvni[a]!='\0'||druhy[a]!='\0')
		{
			//pokud bude jeden z nich \0 "vyskakuju" z funkce
			if (prvni[a]=='\0'||druhy[a]=='\0')
				break;
			//nactu hodnotu znaku do promenne znak_jedna
			znak_jedna = tabulka[x] [(unsigned char) prvni[a] ];	
			//pokud bude znak_jedna roven C vyzkousim zda nasledujici znak neni h a pokud ano, nastavim znak_jedna na konstatntu ch
			if (znak_jedna==4 && (prvni[a+1]=='h'||prvni[a+1]=='H'))
			{
				znak_jedna = CH;
			}

			//nactu hodnotu znaku do promenne znak_dva
			znak_dva = tabulka[x] [(unsigned char) druhy[a] ];
			//pokud bude znak_dva roven C vyzkousim zda nasledujici znak neni h a pokud ano, nastavim znak_dva na constantu ch
			if (znak_dva==4 && (druhy[a+1]=='h'||druhy[a+1]=='H'))
			{
				znak_dva = CH;
			}
			//pokud bude znak ch, tak zvysim a jedna, abych nenacital opet h
			if (znak_jedna==14||znak_dva==14)
				a++;
			//pokud bude prave nacteny znak_jedna vetsi, nez prave nacteny znak_dva vratim porovnani vetsi
			if (znak_jedna>znak_dva)
				return vetsi;
			//pokud bude prave nacteny znak_jedna mensi, nez prave nacteny znak_dva vratim porovnani mensi
			else if (znak_jedna<znak_dva)
				return mensi;
			a++;
		}
	}
	//pokud narazim na konec jednoho, nebo druheho retezce vratim danou hodnotu
	if (druhy[a]=='\0'&&prvni[a]!='\0')
		return vetsi;	//pokud bude druhy retezec uz ukoncen, ale prvni stale bude obsahovat nejake data vratim porovnani vetsi

	else if (druhy[a]!='\0'&&prvni[a]=='\0')
		return mensi;	//pokud bude jiz prvni retezec ukoncen, ale druhy stale bude obsahovat nejake data vratim porovnani mensi

	//jinak vratim hodnotu ze se rovnaji jednotlive retezce
	return rovno;
}
//nacitani hlavicky
void hlavicka (FILE *fr,Tstr strings,int *pocet_sloupcu,int *pocet_mezer)
{

	char c;
	int znak=0;	//pomocna diky ktere zjistuji zda posledni nacteny zak byl oddelovac
	int poc=0;	//pocet oddelovacu, ukldam si cisla tabulek shodych s hledanymy pro pouziti pri "preskakovani" prazdnych znaku
	char *nacteny=NULL;	//dynamicka promenna do ktere se ukladaji jednotlive retezce znaku
	//nacitam znaky dokud neni ukonceni radku
	while ( (c=fgetc(fr) )!='\n')
	{
		//pokud byl predchozi znak oddelovac vytvorim v pameti misto pro retezec znaku
		if (znak==0)
			nacteny=(char*) calloc(0, sizeof(char) );
		//porovnam jmeno hledaneho sloupce ve kterem budou polozky pro vypis a pokud bude promenna rovna "vyzkousim" zda neni sloupec pro vypis roven nazvu nactenemu sloupci
		if (porovnej(nacteny,strings.sloupec)==rovno){

			if (porovnej(nacteny,strings.vystup)==rovno){
				if (*pocet_mezer==0)
					*pocet_mezer=poc;		//priradim pocet doposud nactenych sloupcu k poctu mezer pred tisknutym sloupcem
			}

			free(nacteny);					//uvolnim dynamickou promennou
			if (*pocet_sloupcu==0)
				*pocet_sloupcu = poc;	//priradim pocet doposud nactenych sloupcu k poctu mezer pred sloupcem, ve kterem budu hledat retezce pro funkce --before/after
			poc++;
			znak=0;							//byla nactena mezera, tak nastavim promennou znak opet na nulu
		}
		//porovnam jmeno hledaneho sloupce ve kterem budu hledat retezce pro funkce --before/after 
		else if (porovnej(nacteny,strings.vystup)==rovno){

			if (znak!=0)
			free(nacteny);
			if (*pocet_mezer==0)
				*pocet_mezer=poc;			//priradim  doposud nactenych sloupcu k poctu mezer pred sloupcem, ve kterem budu hledat retezce pro funkce --before/after

			poc++;
			znak=0;
		}
		//pokud narazim na oddelovac nastavim promennou znak opet na nulu
		else if (c==' '||c=='\t'){
			poc++;
			free (nacteny);
			znak=0;
		}
		//reallocuji misto pro nacitany retezec
		else{
			nacteny = (char *) realloc (nacteny,sizeof(char)*(znak+2));
			nacteny[znak++] = c;		//nacteny znak ulozim na ove vytvorene misto v pameti a zvysim zak o jedna, abych mohl na posledni misto ulozit ukoceni retezce \0
			nacteny[znak]='\0';		//na misto posledniho znaku umistnim \0, jako koec retezce
		}
	}
	if (porovnej(nacteny,strings.sloupec)==rovno&&*pocet_sloupcu==0)	/*porovnam jmeno hledaneho sloupce ve kterem budu hledat retezce pro funkce --before
																												* nebo --after priradim pocet doposud nactenych sloupcu k poctu mezer pred sloupcem  */
		*pocet_sloupcu=poc;

	if (porovnej(nacteny,strings.vystup)==rovno && *pocet_mezer==0)	//porovnam jmeno hledaneho sloupce ve kterem budou polozky pro vypis
		*pocet_mezer=poc;
	free (nacteny);
}
//zavøení souboru
int zavri_soubor (FILE **f)
{
		if (fclose(*f)==EOF){
			printf("nepodarilo se zavrit soubor \n");
			return 1;
		}

	return EOK;		//pokud se povedlo zavreni souboru vratim nulu (EOK)
}

//otevøení souboru pro cteni
int otevri_soubor (char *jmeno,FILE **fr)
{

		if ((*fr = fopen(jmeno,"r"))==NULL) {
			printf("nepodarilo se otevrit soubor %s\n",jmeno );
			return 1;
		}

	return EOK;	//pokud se povedlo otevreni souboru pro cteni vratim nulu (EOK)
}
//otevreni souboru pro zapis
int otevri_soubor_pro_zapis (char *jmeno,FILE **fw)
{
	if ((*fw=fopen(jmeno,"w"))==NULL){
		printf("nepodarilo se otevrit soubor %s ",jmeno );
	return 1;
	}
	return EOK;		//pokud se povedlo otevreni souboru pro zapis vratim nulu (EOK)
}

// Inicializace seznamu
void list_init(TList *list)
{

    list->first = NULL;	//na zacatku programu nastavim prvni prvek na NULL
    list->last = NULL;	//na zacatku programu nastavim posledni prvek na NULL
}

// vlozi polozku na konec seznamu (spravne chovani fronty)
void list_insert_last(TList *list, TItem *item)
{

	item->next = NULL; //nasledujici prvek nastavim na prazdnou hodnotu

	if (list->first==NULL) {	//pokud inicializuji list poprve prvni a zaroven posledni prvek sezamu bude ukazovat na prave nactenou hodnotu
		list->first = item;		//prvni polozka seznamu
        list->last = item;		//posledni polozka seznamu
    } else {			//pokud budu mit v seznamu vice, nez 1 hodnotu ukladam jiz normalne data na konec seznamu
        list->last->next = item;	
        list->last = item;
    }
}

// Odstraneni polozky ze zacatku seznamu
void list_delete_first(TList *list)
{

    TItem *item;
    if (list->first)	//odstranuji polozky v opacnem poradi nez jsem je ukladal
    {
        item = list->first;
        list->first = item->next;
        free(item);
    }
}

// Uvolneni seznamu
void list_free(TList *list)
{
	//pro uvolneni seznamu pouzivam dalsi funkci list_delete_first
    while (list->first)
        list_delete_first(list);
}

// vrati !=0, pokud se dalsi polozky prochazet nemaji
typedef int (*FuncItem)(TItem *);

// Pruchod seznamem
int list_iterate(TList *list, FuncItem f)
{
    int code;
    for (TItem *item = list->first; item != NULL; item = item->next)
        if ((code = f(item)) != 0)
            break;
    return code;
}

// Alokuj novou polozku a prirad ji jeji obsah
TItem *item_new(char *uloz,int pocet)
{
	TItem *item = malloc(sizeof(TItem));
	item->uloz=(char *) malloc((pocet+1)*sizeof(char));	//vytvorim si v pameti misto pro novou polozku
	if (item != NULL)
	{
		strncpy(item->uloz, uloz, pocet+1);	//zkopituje presne dany pocet (pocet+1) znaku do item->uloz 
	}
	return item;
}

// Precti retezec ze souboru f az do oddelovace mezera, nebo \t bohuzel jsem jiz nemel cas implementovat jakekoliv mnozstvi oddelovacu, ale provadel bych to pres issapce(c)
// Vsechny znaky az do oddelovace ulozime do dst.
//zapis øetìzce do pamìti
int fread_str(FILE *f, char **dst,char *posl)
{
	char c;
	int i = 0;
	int length;
	char *nacteny; //dynamicka promenna
	nacteny = (char *) calloc (0,sizeof (char));
	while ((c = fgetc(f)) != '\n')	//nacitam jednotlive znaky dokud neni ukonceni radku
		if (c == '\t'||c==' '||c==-1)	//-1 zde mam kvuli konci souboru
			break;
		else
		{
			nacteny = (char *) realloc (nacteny,sizeof(char)*(i+2));
			nacteny[i++] = c;
			nacteny[i] = '\0';
		}
	//pocet nactenych znaku ulozim do length
	length = i;
	*dst = nacteny;	//dynamickou promennou ulozim do dst
	*posl = c;			//posledni nacteny znak ulozim do promenne posl
	return length;
}

// Preskocime vsechny znaky v souboru f az za prvni znak delim
void fskip_to(FILE *f,char delim,char del)
{
	int c;
	// cti, dokud neni EOF a zaroven dokud neni delim
	while ((c = fgetc(f)) != EOF){
		if (c == delim||c == del)
		break;
	}
}


// Tisk polozky seznamu do souboru
int tiskni_polozku(TList *list,FILE **fw)
{
	//prochazim seznam dokud nenarazim na konec seznamu a tisknu ho do souboru
	for (TItem *item = list->first; item != NULL; item = item->next)
		fprintf(*fw,"%s\n", item->uloz);
	return 0;
}

//uvolni naalokovanou pamet pro item->uloz
int uvolni_pamet (TItem *item)
{
	free (item->uloz);
	return 0;
}

//funkce pro serazeni bubble sort
int serad (TList *list,FILE **fw)
{
	char *x;	//pomocna do ktere budu ukladat prubezne vyssi hodnotu
	TItem *nacteny, *hranice, *vymena; 	//pomocne diky kterym budu prochazet seznam a menit jednotlive polozky
	bool zmena;		//promenna, ktera se nastavi pouze pokud nastane zmena v seznamu, pokud nenastane nic netisknu
	//int vysl_porovnani;	//pomocna pro porovnani
	hranice = list->last;
	do{
		zmena =false;

		for (nacteny=list->first;nacteny<hranice;nacteny= nacteny->next){
					//vysl_porovnani = porovnej(nacteny->uloz,nacteny->next->uloz);
					if (porovnej(nacteny->uloz,nacteny->next->uloz) == vetsi){ //pokud nasledujici retezec je vetsi, nez nacteny retezec
						x=nacteny->next->uloz;	//do pomocné ulo¾ím vìt¹í prvek
						nacteny->next->uloz = nacteny->uloz;	//vetsi prvek nahradim mensim
						nacteny->uloz = x;		//mensi prvek nahradim vetsim
						zmena = true;		//nastavim zmenu, abych mohl opet cyklit
						vymena = nacteny->next;	//nastavim aktualni vymenu na nacteny prvek->next
					}
		}

		hranice = vymena;		//hranici nastavim na posledni vymenu

	}while (zmena);	//dokud bude platit zmena

	//ukladam zmeneny spojovy seznam do souboru
	for (hranice = list->first;hranice!=NULL;hranice=hranice->next)
		fprintf(*fw,"%s\n",hranice->uloz);
	
	return 0;
}

//funkce main
int main(int argc, char *argv[])
{
	Tstr strings;	//pouziji zde znovu datovou strukturu jednotlivych retezcu

	char *hledany=NULL;	//promenna, ktera se nastavi pokud budeme pouzvat funkce beofre a after

	char posledni_znak;	//posledni nacteny znak



	//promenne, trosku vysvetlim co ktera znamena, protozejich je trosku vic :)
	//	cislo_sloupc: pocet mezer pred sloupcem, ve kterem budu hledat polozky funkci before a after
	//pocet_mezer: pocet mezer pred sloupcem, ktery budu ukladat a tisknout
	//length: pocet ulozenych cisel
	//pocet_sloupcu pouzivam pozdeji pro preskakovani bilich znaku
	//porovnani: vysledek funkce porovnej
	//cteni_souboru: vrati 1 pokud se spatne precetl, nebo ulozil soubor a EOK pokud vse probehlo v poradku
	int cislo_sloupce=0,pocet_mezer=0,length=0,pocet_sloupcu=1,porovnani=0,cteni_souboru;



	/*2 promenne pro praci s polem, mohl bych pouzit pouze jednu, ale to by se stalo neprehlednym,
	 takhle aspoò lze videt kdy nacitam soubor pro cteni (fr) a kdy pro zapis (fw) */
	FILE *fr,*fw;

	//funce pro ziskani parametru
	TParams params = getParams(argc, argv,&hledany,&strings);

	//pokud paramety vrati jinou hodnotu nez EOK vytiskne se prirazena chyba
	if (params.ecode != EOK)
  { 
    printECode(params.ecode);
    return EXIT_FAILURE;
  }
	//pokud bude uzivatel chtit tisknout napovedu
  if (params.state == CHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }
	//vytvoreni a inicializace seznamu
	TList list;
	list_init(&list);
	//jednotlive polozky seznamu budou ulozeny v item
	TItem *item;
		//pokusim se otevrit soubor
		cteni_souboru=otevri_soubor(argv[argc-2],&fr);	//proc argc-? ze zadani plyne ze nazev ukladaneho souboru bude na poslednim miste parametru

		//pokud jsme precetli zpravne soubor
		if (cteni_souboru==EOK){

			//nacteme hlavicku a ulozime jednotlive pocty mezer mezi sloupci
			hlavicka(fr,strings,&cislo_sloupce,&pocet_mezer);

			//cteme v souboru dokud nenastane konec soubor
			while (!feof(fr))
	    	{
	            // nacteme data
					//pokud pocet mezer, mezi sloupci je stejny, nebo mensi provedu cteni v souboru
				if (pocet_mezer>cislo_sloupce||pocet_mezer==cislo_sloupce)
				{
					//preskakuju bile znaky
					for (pocet_sloupcu=1;pocet_sloupcu<=cislo_sloupce;pocet_sloupcu++)
						fskip_to(fr,' ','\t');

					//nactu do promenne sloupec hodnotu, ktera se bude porovnavat funkcii before a after
					length=fread_str(fr, &strings.sloupec,&posledni_znak);

					//pokud bude pocet mezer mezi sloupcy roven, nenacitam znovu jednotlive prvky, ale jednoduse je priradim
					if (pocet_mezer==cislo_sloupce)
						strings.vystup=strings.sloupec;

					//jinak nactu hodnotu, kterou budu tisknout
					else{
						for (int x=pocet_sloupcu;x<pocet_mezer;x++)
							fskip_to(fr,' ','\t');
						length=fread_str(fr, &strings.vystup,&posledni_znak);
					}
	
					//pokud posledni znak byl ruzny od \n preskocim na konec radku pomoc funkce fskip
					if (posledni_znak!='\n')
						fskip_to(fr,'\n','\n');
	
				//jinak pokud pocet mezer, mezi sloupci je vetsi provedu cteni v souboru
				}else{

					//preskakuju bile znaky
					for (pocet_sloupcu=1;pocet_sloupcu<=pocet_mezer;pocet_sloupcu++)
						fskip_to(fr,' ','\t');

					//jinak nactu hodnoty, kterou budu tisknout
					length=fread_str(fr, &strings.vystup,&posledni_znak);

					//preskakuju bile znaky
					for (int x=pocet_sloupcu;x<cislo_sloupce;x++)
						fskip_to(fr,' ','\t');

					//nactu do promenne sloupec hodnotu, ktera se bude porovnavat funkcii before a after
					fread_str(fr, &strings.sloupec,&posledni_znak);

					//pokud posledni znak byl ruzny od \n preskocim na konec radku pomoc funkce fskip
					if (posledni_znak!='\n')
						fskip_to(fr,'\n','\n');
				}
	
				//pokud byli parametry ruzne od print provedu:
				switch (params.state){
	 				//porovnavam jednotlive retezce a pokud budou shodne, mensi znovu nacitam nove hodnoty
					case AFTER : porovnani = porovnej(strings.sloupec,hledany);
										if (porovnani == mensi || porovnani == rovno){
											free(strings.sloupec);	//uvolnim nacteny sloupec a znovu pokracuji v nacitani
											continue;
										}
										break;

					//porovnavam jednotlive retezce a pokud budou shodne,nebo vetsi nacitam nove hodnoty
					case BEFORE : porovnani = porovnej(strings.sloupec,hledany);
										if (porovnani == vetsi||porovnani == rovno){
											free(strings.sloupec);	//uvolnim nacteny sloupec a znovu pokracuji v nacitani
											continue;
										}
										break;

					//porovnavam jednotlive retezce a pokud budou shodne, mensi znovu nacitam nove hodnoty
					case AFTER_SORT : porovnani = porovnej(strings.sloupec,hledany);
										if (porovnani == mensi || porovnani == rovno){
											free(strings.sloupec);	//uvolnim nacteny sloupec a znovu pokracuji v nacitani
											continue;
										}
										break;

					//porovnavam jednotlive retezce a pokud budou shodne,nebo vetsi nacitam nove hodnoty
					case BEFORE_SORT : porovnani = porovnej(strings.sloupec,hledany);
										if (porovnani == vetsi||porovnani == rovno){
											free(strings.sloupec);	//uvolnim nacteny sloupec a znovu pokracuji v nacitani
											continue;
										 }
									break;
				}
	
				
				// vlozime novou polozku do seznamu
				if (posledni_znak!=-1){
					if ((item = item_new(strings.vystup,length)) == NULL)
						{
							zavri_soubor(&fr);
							list_free(&list);
							return 1;
						}
	 	         	  list_insert_last(&list, item);
				}
				//uvolnime nacteny sloupec pro vypis
				if (strings.vystup!=NULL)
					free (strings.vystup);
	
			}
		}
		//pokud probehlo uspesne otevreni souboru zavru ho
		if (cteni_souboru==EOK)
			zavri_soubor(&fr);
		//pokud probehlo uspesne zavreni souboru otevru novy soubor pro zapis
		if (cteni_souboru==EOK){
			cteni_souboru=otevri_soubor_pro_zapis(argv[argc-1],&fw);	//proc argc-1? ze zadani plyne ze nazev ukladaneho souboru bude na poslednim miste parametru

			//pokud chce uzivatel tisknout zavolam funkci serad
			if (params.state==AFTER_SORT||params.state==BEFORE_SORT||params.state==PRINT_SORT)
				serad(&list,&fw);

			//jinak je logicke ze zavolal pouze samostatnou funkci print tak zavolam funkci pro vypis
			else
				tiskni_polozku(&list,&fw);
		}
		//pokud probehlo otevreni souboru pro zapis v poradku zavru jej
		if (cteni_souboru==EOK)
			zavri_soubor(&fw);

	//funkce pro uvolneni jednotlivych polozek z pameti
	list_iterate(&list,uvolni_pamet);

	//funkce pro uvolneni seznamu
	list_free(&list);
	return 0;

}
//konec funkce main
