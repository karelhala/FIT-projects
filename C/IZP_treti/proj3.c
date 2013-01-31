	/*
 	* Soubor:  	proj3
 	* Datum:   	2010/11/11
	* Autor:   	Karel Hala, xhalak00d@fit.vutbr.cz
	* Projekt: 	maticové operace
	* Popis: 	prgram provádí:	sčítání dvou vektorů
	* 										skalární součin dvou vektorů
	*										nsobení dvou matic
	*										mat. vzorec A*B*A
	*										osmisměrku
	*										hlední bublin v matici			
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Kódy chyb programu
enum tecodes
{
  EOK = 0,     //Bez chyby
  ECLWRONG,    //Chybný příkazový řádek.
  EUNKNOWN,    //Neznámá chyba
};

/** Stavové kódy programu */
enum tstates
{
  	CHELP,		//NPOVĚDA
  	TEST,			//TEST
	BUBBLES,		//BUBLINY
	VADD,			//SOUČET
	VSCAL,		//SOUČIN VEKTORŮ
	MMULT,		//SOUČIN MATIC
	MEXPR,		//MATEMATICKÝ VÝRAZ A*B*C
	EIGHT,		//OSMISMĚRKA
	
};

/** Chybová hlášení odpovídající chybovým kódům. */
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
//nápověda
const char *HELPMSG = 
	"\nProgram maticové operace\n"
	"Autor: Karel Hala (c) 2010\n"
	"Pouziti: proj3 --test /soubor/			otestuje zadaný soubor\n"
	"         proj3 --bubbles /soubor/		vyhledá bubliny v souboru\n"
	"         proj3 --vadd /soubor/ /soubor/		sečte 2 vektory\n"
	"         proj3 --vscal	/soubor/ /soubor/ 	skalární součet\n"
	"         proj3 --mmult /soubor/ /soubor/	součin 2 matice\n"
	"         proj3 --mexpr /soubor/ /soubor/	matematický výraz\n"
	"         proj3 --eight /soubor/ /soubor/	osmisměrka\n\n";
//konec nápovědy


//hodnoty parametrů příkazové řádky.
 
typedef struct params
{
  int ecode;        				//Chybový kod programu, odpovídá výčtu tecodes
  int state;        				// Stavový kód programu, odpovídá výčtu tstates
} TParams;

void printECode(int ecode)
{
	 fprintf(stderr, "%s", ECODEMSG[ecode]);
}
//získní parametrů
TParams getParams(int argc, char *argv[])
{
  TParams result =
  { // inicializace struktury
    .ecode = EOK,
    .state = CHELP,
  };

  if (argc == 2 && strcmp("-h", argv[1]) == 0)
  { // tisk nápovědy
    result.state = CHELP;
  }

  else if (argc == 3){
		if (strcmp("--test", argv[1]) == 0)
		{ 
			result.state = TEST;
		}
	
  		else if (strcmp("--bubbles", argv[1]) == 0)
  		{ 
			result.state = BUBBLES;
		}
	}

	else if (argc == 4){	
		if (strcmp("--vadd", argv[1]) == 0)
		{ 
			result.state = VADD;
		}

		else if (strcmp("--vscal", argv[1]) == 0)
		{ 
			result.state = VSCAL;
		}

		else if (strcmp("--mmult", argv[1]) == 0)
		{ 
			result.state = MMULT;
		}

		else if (strcmp("--mexpr", argv[1]) == 0)
		{ 
			result.state = MEXPR;
		}

		else if (strcmp("--eight", argv[1]) == 0)
		{ 
			result.state = EIGHT;
		}	
	}
  else
  { // příliš mnoho parametrů
    result.ecode = ECLWRONG;
  }

  return result;
}

//funkce na počítání bublin, pro větí bubliny je ale příli pomalá
int bubliny(int **matrix,int rada,int prvek)
{
	//pokud je v matici nějaké číslo menší než nula vrátím se zpátky do hl. programu a neprovádím hledání bublin
	for (int radek=0;radek<rada;radek++){
		for (int sloupec=0;sloupec<prvek;sloupec++){
			if (matrix[radek][sloupec]<0){
				return -1;
			}
		}
	}
	//samostatné hlední bublin, pokud narazí hledáček na 0 zkontroluje směr v pravo a dolů a sníží nulu na -1 , pokud při hledání narazí na -1 tak sníží počet bublin
	int pocet=0;
	for (int radek=0;radek<rada;radek++){
		for (int sloupec=0;sloupec<prvek;sloupec++){
			if (matrix[radek][sloupec]==0){ //pokud je v načteném prvku 0 program začíná hledat ve svém okolí dalí nuly, pokud na žádné nenarazí pokračuje v hledání
				pocet++;
				if ((radek>0) && (sloupec>0) && (matrix[radek][sloupec-1]==-1|| matrix[radek-1][sloupec]==-1)) //směr hlední bublin dolů
					pocet--;
				for (int x=sloupec;x<prvek;x++){
					if (matrix[radek][x]==-1){ //pokud narazil na -1 sníží počet bublin o 1 a pokračuje v hledání do prava
						pocet--; 
						break;
					}
					else if (matrix[radek][x]!=0)
						break;
					
					matrix[radek][x]=-1;
				}

				//směr hlední bublin v ravo
				for (int x=radek+1;x<rada;x++){
					if (matrix[x][sloupec]==-1){  //pokud narazí hledáček na -1 vrací se na místo odkud vyrazil
						break;
					}
					else if (matrix[x][sloupec]!=0)
						break;
					
					matrix[x][sloupec]=-1;
				}
			}
		}
	}
	//nastavení matice do původního tvaru, pokud by uživatel chtěl odsud použít matici někde jinde
	//printf ("počet bublin je: %d \n",pocet);
	//návrat matice do původního stavu
	//pokud chcete vidět výslednou matici po návratu do jejího původího stavu odstraňte komentář u funkcí printf a putchar
	// printf ("v matici:\n2\n%d %d\n",matice.radek[0],matice.sloupec[0]);
	for (int radek=0;radek<rada;radek++){
		for (int sloupec=0;sloupec<prvek;sloupec++){
			if (matrix[radek][sloupec]==-1)
				matrix[radek][sloupec]=0;
			//printf ("%d ",matice.matice_pole[0][radek][sloupec]);
		}
		//putchar('\n');
	}
	return pocet;
}
//							 Prosím omluvte tohle monstrum, ale nic tak efektivního mě nenapadlo
//osmisměrka hledá ve všech 8 směrech velmi podobým způsobem, ale pokaždé jsou nutné různé kontrolní hodnoty, proto mě nenapadl jiný způsob jak 8 větvení, 
//pro každý směr jeden.
//Každý směr funguje na podobném pricipu, že pokud se načtené číslo shoduje s prvním číslem vektoru tak program vyzkouší všechny směry.
//A pokud je možné prohledat matici v daném směru tak nastavím pomocnou proměnnou, která je pro každý směr jiná. 
//Tato proměnná se potom používá jako zástup za řádek, nebo sloupec.
//Pokud se daný směr shoduje s vektorem vrátím hodnotu nula, kter se poté vypíe jako true,
//pokud nenajdu ani jeden shodný směr vrátím 1 což se v hlavní funkc vypíe jako false
int osmismerka(int *vektor,int prvek,int **matrix,int rada, int sloup)
{
	for (int radek=0;radek<rada;radek++){
		for (int sloupec=0;sloupec<sloup;sloupec++){
			//směr doleva
			if((radek>=prvek-1)&&(matrix[radek][sloupec]==vektor[0])){
				//pokud bude vektor pouze jednoprvkový a tento prvek se bude rovnat nařtenému prvku z matice program vypíše true a zkončí s procházením matice
				if (prvek==1){
					return 0;
				}
				//směr nahoru	
				for (int x=1;x<prvek;x++){

					int pom=radek-x;

					if(matrix[pom][sloupec]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr v levo
			if((sloupec>=prvek-1)&&(matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom=sloupec-x;

					if(matrix[radek][pom]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr dolů	
			if((rada-radek>=prvek-1)&&(matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom=radek+x;

					if(matrix[pom][sloupec]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr do prava	
			if((sloup-sloupec>=prvek-1)&&(matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom=sloupec+x;

					if(matrix[radek][pom]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr v levo a nahoru	
			if((radek>=prvek-1) && (sloupec>=prvek-1) && (matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom_jedna=radek-x;
					int pom_dva=sloupec-x;

					if (matrix[pom_jedna][pom_dva]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr do prava a nahorů	
			if((rada-radek>=prvek-1) && (sloupec>=prvek-1) && (matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom_jedna=radek+x;
					int pom_dva=sloupec-x;

					if (matrix[pom_jedna][pom_dva]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr doleva a dolů	
			if((radek>=prvek-1) && (sloup-sloupec>=prvek-1) && (matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom_jedna=radek-x;
					int pom_dva=sloupec+x;

					if (matrix[pom_jedna][pom_dva]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
			//směr do prava a dolů	
			if((rada-radek>=prvek-1) && (sloup-sloupec>=prvek-1) && (matrix[radek][sloupec]==vektor[0])){
				for (int x=1;x<prvek;x++){

					int pom_jedna=radek+x;
					int pom_dva=sloupec+x;

					if (matrix[pom_jedna][pom_dva]!=vektor[x])
						break;
					else if(x==prvek-1){
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

//funkce pro násobení matic
int nasobeni_matic(int radek,int sloupec,int **matrix_jedna,int **matrix_dva,int ***vysledek)
{

	int **vysl_matice;	//pomocná matice a její malloc
	vysl_matice=(int**) malloc(radek*sizeof(int));
	for (int a = 0;a<radek;a++)
	{
		 vysl_matice[a]=(int *) malloc(sloupec*sizeof(int));
			if (vysl_matice[a]==NULL)
				return 1;
	}
	//samotn sobení dvou matic
	for (int t=0;t<radek;t++){
		for (int x=0;x<sloupec;x++){
			int pom=0;
			for (int m=0;m<radek;m++)
				pom+=matrix_jedna[t][m]*matrix_dva[m][x]; //do této pomocné proměnné si uložím dočasný výsledek, ke kterému přičítám jednotlivé mezivýsledky
			 vysl_matice[t][x]=pom;
		}
	}
	//pomocnou matici přiřadím výsledné matici 
	*vysledek=vysl_matice;
	return 0;
}

//funkce na součet dvou vektorů
int soucet_vectoru(int **vysl,int *vektor[2],int prvek)
{
	int *vysl_vektor;	//pomocný vektor a jeho naplnění
	vysl_vektor =(int*) malloc(prvek*sizeof(int));

	for (int x=0;x<prvek;x++)
	{
		vysl_vektor[x]=vektor[0][x]+vektor[1][x];	//samotný součet vektorů
	}
	//pomocný vektor přiřadím výslednému vektoru
	*vysl=vysl_vektor;
	return 0;
}

//funkce na skalární součin
int skalarni_soucin(int **vysl,int *vektor[2],int prvek)
{
	int *vysl_vektor;//pomocný vektor a jeho naplnění
	vysl_vektor =(int*) malloc(prvek*sizeof(int));

	for (int x=0;x<prvek;x++)
	{
		vysl_vektor[x]=vektor[0][x]*vektor[1][x];	//samotný skalrní součin dvou vektorů
	}
	//pomocný vektor přiřadím výslednému vektoru
	*vysl=vysl_vektor;
	return 0;
}

//funkce na vytvoření matice a jejího naplnění 
int vytvor_matici(FILE *fr,int sloupec,int radek,int ***vys_matice)
{
	int qq,chyba;
	int **matrix; //vytvoření a naplnění matice provádím přes pomocnou matici
	matrix=(int**) malloc(radek*sizeof(int));
		if (matrix==NULL)
			return 1;
	for (int a = 0;a<radek;a++)
	{
		matrix[a]=(int *) malloc(sloupec*sizeof(int));
			if (matrix[a]==NULL)
				return 1;
	}
	//naplnení matice, pokud při plnění matice nastane chyba (nelegální znak, mlo prvků v matici..) program zkončí a vypíše v hlavní funkci false
	for (int x=0;x<radek;x++){
		for (int a=0;a<sloupec;a++){
			qq=fscanf(fr,"%d",&matrix[x][a]);
			if (qq!=1){
				*vys_matice = matrix;
				return 1;
			}			
		}
	}
		//po naplnění matice testuju zda uživatel nezkoušel naplnit příliš velkou matici (například měl vstupní parametry 3x3, ale pokouel se naplnit matici 3x4),
		//pokud se o to pokoušel tak vrátím 1 a v hlaví funkci vypíšu false
		qq=fscanf(fr,"%d",&chyba);
		if (qq!=-1){
		*vys_matice = matrix;
		return 1;
	}
	//pomocnou matici nakonec přiřadím výsledné
	*vys_matice = matrix;
	return 0;
}

//funkce na vytvoření dynamického vektrou matic a jeho naplnění
int matice_vektor (FILE *fr,int pocet_matic,int radek,int sloupec,int ***vysl)
{
	int qq;
	int **vektor_matic; //vytvoření a naplnění vektoru matic provádím přes pomocnou matici
	vektor_matic=(int**) malloc(pocet_matic*radek*sizeof(int));
		if (vektor_matic==NULL)
			return 1;
	for (int a = 0;a<(radek*pocet_matic);a++)
	{
		vektor_matic[a]=(int *) malloc(sloupec*sizeof(int));
			if (vektor_matic[a]==NULL)
				return 1;
	}

	for (int x=0;x<pocet_matic;x++)
	{
		for (int x=0;x<radek;x++){
			for (int a=0;a<sloupec;a++){
				qq=fscanf(fr,"%d",&vektor_matic[x][a]);
			}
		}
	}
	*vysl=vektor_matic;
	return 0;	
} 
//funkce na dynamické vytvoření vektoru a jeho naplnění
int vytvor_vektor (FILE *fr,int prvky,int **vektor) 
{
	int qq,test,*vector; //vytvoření a naplnění pomocného vektoru
	vector =(int*) malloc(prvky * sizeof(int));
	for (int x=0;x<prvky;x++)
	{
		qq=fscanf(fr,"%d",&(vector[x]));
		if (qq != 1){ 	//pokud načte program neleglní znak, nebo konec řádku tak vypíše false v hlavním programu
			*vektor=vector;
			return 1;
		}
	}
	//pokud uživatel chtěl načíst větší vektor (například vektor měl mít 3 prvky, ale v souboru bylo 5 prvků) program vypíe v hlavní funkci false a nepočítá s tímto vektorem
	qq=fscanf(fr,"%d",&test);
	if (qq!=-1){
		*vektor=vector;
		return 1;
	}
	*vektor=vector;
	return 0;
}
//zavření souboru
int zavri_soubor (FILE **fr)
{
		if (fclose(*fr)==EOF){
			printf("false\n");
			return 1;
		}

	return 0;
}
//otevření souboru
int otevri_soubor (char *jmeno,FILE **fr)
{

		if ((*fr = fopen(jmeno,"r"))==NULL) {
			printf("false\n" );
			return 1;
		}

	return 0;
}
//funkce main
int main(int argc, char *argv[])
{

	TParams params = getParams(argc, argv);
	if (params.ecode != EOK)
  { 
    printECode(params.ecode);
    return EXIT_FAILURE;
  }

  if (params.state == CHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }
	FILE *fr;
	int stav,chyba,pocet,nacitani[2],pocet_vektoru=0,matice_pocet=0;
	int *vektor[2],prvek[2],*vysl_vektor=NULL;
	int **matrix[2],sloupec[2],radek[2],**matice_vysl,**vyraz;
	int pocet_matic=0,**vektor_matic=NULL;
	//nastavení všech dyanamických proměných
	matrix[0]=NULL;
	matrix[1]=NULL;
	vektor[0]=NULL;
	vektor[1]=NULL;
	nacitani[0]=0;
	nacitani[1]=0;
	//konec nastavovní
	for (int c=2;c<argc;c++){
	   chyba = otevri_soubor(argv[c],&fr); //otevření souboru
		if (chyba ==0){	//pokud bylo otevření souboru úspěné provedu naplnění vektorů, nebo matic
			fscanf(fr,"%d",&stav);
			pocet = pocet_vektoru+matice_pocet;
				switch (stav){
					case 1:	//vytvoření a naplnění vektoru
							fscanf (fr,"%d",&prvek[pocet_vektoru]);
							if (prvek[pocet_vektoru]>0){	//pokud bude počet prvků vektoru 0 neprovedu jeho naplnění
								nacitani[pocet]=vytvor_vektor(fr,prvek[pocet_vektoru],&vektor[pocet_vektoru]);
								pocet_vektoru +=1;
							}	//vetor
							break;
					case 2://vytvoření a naplnění matice
							fscanf (fr,"%d",&radek[matice_pocet]);
							fscanf (fr,"%d",&sloupec[matice_pocet]);
							if ((sloupec[matice_pocet]>0)&&(radek[matice_pocet]>0)){ //pokud bude počet sloupců 0, nebo poet řdků 0 neprovedu naplnění matice
								nacitani[pocet]=vytvor_matici(fr, sloupec[matice_pocet], radek[matice_pocet], &matrix[matice_pocet]);
								matice_pocet +=1;									
							}	//matice
							break;
					case 3://vtvoření a naplnění vektoru matic
//pro vektor matic nepoužívám žádnou další funkci kromě testu, proto si můžu dovolit ignorovat načítání vektoru matic a něčeho dalšího
//ing. Martínek na foru psal že pokud neděláme 3D bubliny nemusíme načítat vůbec vektor matic
							if ((matrix[0]==NULL)&&(matrix[1]==NULL)){
								fscanf (fr,"%d",&pocet_matic);
								fscanf (fr,"%d",&radek[0]);
								fscanf (fr,"%d",&sloupec[0]);
								if ((sloupec[matice_pocet]>0)&&(radek[matice_pocet]>0))
									matice_vektor(fr,pocet_matic,radek[0],sloupec[0],&vektor_matic);
							}
							//vektor matic
							break;
					default:
							printf("false\n");
				}
	
			zavri_soubor(&fr);
			}
		}
	if ((nacitani[0]==0)&&(nacitani[1]==0)){
		//test
		if (params.state == TEST){
	
			for (int t=0;t<pocet_vektoru;t++){
			printf("1\n%d\n",prvek[t]);
				if (vektor[t]!=NULL){	//vypsání vektoru, pokud není prázdný
					for (int a=0;a<prvek[t];a++)
							printf("%d ",vektor[t][a]);
				putchar('\n');
				}
			}
			
			for (int t=0;t<matice_pocet;t++){
				if (matrix[t]!=NULL){	//vypsání matice, pokud není prázdná
				printf("2\n%d %d\n",radek[t],sloupec[t]);
					for (int x=0;x<radek[t];x++){
		
						for (int a=0;a<sloupec[t];a++)
							printf("%d ",matrix[t][x][a]);
		
						putchar('\n');
					}
				}
			}
			if (vektor_matic!=NULL){	//vypsání vektoru matic, pokud není przdný
				printf("3\n%d %d %d\n",radek[0],sloupec[0],pocet_matic);
				for (int a=0;a<pocet_matic;a++){
					for (int rada=0;rada<radek[0];rada++){
						for (int sloup=0;sloup<sloupec[0];sloup++)
							printf("%d ",vektor_matic[rada][sloup]);
						putchar('\n');
					}
					if (a<pocet_matic-1)
						putchar('\n');
				}
			}
	  	}
	
		//součet dvou vektorů
		else if (params.state == VADD){
			if ((pocet_vektoru!=2)||(prvek[0]<=0)||(prvek[1]<=0)||(prvek[0]!=prvek[1]))	//pokud je počet vektorů různý 2 a počet prvků vektrů není shodní
				printf("false\n");																			//a nebo jeden z vektorů je przndý vypíšu false
			else{
				soucet_vectoru(&vysl_vektor,vektor,prvek[1]);
				if (vysl_vektor!=NULL){	//pokud je výsledný vektor neprázný vypíšu ho a uvolním
					for (int x=0;x<prvek[0];x++)
					{
						printf("%d ",vysl_vektor[x]);
					}
					putchar('\n');
					if (vysl_vektor!=NULL)
						free(vysl_vektor);
				}	
			}
		}
	
		//skalární součin dvou vektorů 
		else if (params.state == VSCAL){
			if ((pocet_vektoru!=2)||(prvek[0]<=0)||(prvek[1]<=0)||(prvek[0]!=prvek[1])) 	//pokud je počet vektorů různý 2 a počet prvků vektrů není shodní
				printf("false\n");																			//a nebo jeden z vektorů je przndý vypíšu false
				
			else{
				skalarni_soucin(&vysl_vektor,vektor,prvek[1]);
				if (vysl_vektor!=NULL){	//pokud je výsledný vektor neprázný vypíšu ho a uvolním
					for (int x=0;x<prvek[0];x++)
					{
						printf("%d ",vysl_vektor[x]);
					}
					putchar('\n');

					if (vysl_vektor!=NULL)
						free(vysl_vektor);
				}
			}
		}
		
		//násobení matic
		else if (params.state == MMULT){
			if ((matice_pocet==2)&&(radek[1] == sloupec[0]&&matrix[0]!=NULL&&matrix[1]!=NULL)){ 	//pokud jsou načteny 2 matice a ty jsou nenulové provedeme násobení dvou matic
				nasobeni_matic(radek[0],sloupec[1],matrix[0],matrix[1],&matice_vysl);					//matice mus splǒvat pravidlo, že počet sloupců první matice 
																																//musí být shodný s potem řádků druhé matice
				if (matice_vysl!=NULL){ //pokud jevýsledek nenulový vytisknu ho
					for (int x=0;x<radek[0];x++){
						for (int a=0;a<sloupec[1];a++)
							printf("%d ",matice_vysl[x][a]);
						putchar('\n');
					}
				}
				if (matice_vysl!=NULL){	//pokud je výsledek nenulový uvolním ho
					for (int x=0;x< radek[0];x++)
						free(matice_vysl[x]);
					free(matice_vysl);
				}
			}
			else
				printf("false\n");
		}
	
		//matematický výraz počítám přes dvojté volání násobení matic vzorec (A*B)*A nejdříve vypočítm A*B a potom výsledek vynsobím opět s maticí A          
		else if(params.state == MEXPR)
		{
			if ((matice_pocet==2)&&(radek[1] == sloupec[0])&&(radek[0]==sloupec[1])){ 	//pokud bude chtít uživatel nsobit 2 matice,
				nasobeni_matic(radek[0],sloupec[1],matrix[0],matrix[1],&matice_vysl);	//a počet řádků bude shodný s počtem sloupců druhé matice můžeme provést tuto funkci
				nasobeni_matic(radek[1],sloupec[1],matice_vysl,matrix[0],&vyraz); 
				if (vyraz!=NULL){ 		//pokud vysledek bude neprázdný vytisknu ho
					for (int x=0;x<radek[0];x++){
						for (int a=0;a<sloupec[1];a++)
							printf("%d ",vyraz[x][a]);
						putchar('\n');
					}
				}
				if (matice_vysl!=NULL){	//pokud pomocn matice bude neprázdná uvolním ji
					for (int x=0;x< radek[0];x++)
						free(matice_vysl[x]);
					free(matice_vysl);
				}
				if (vyraz!=NULL){ //pokud vysledek bude neprázdný uvolním ho
					for (int x=0;x< radek[0];x++)
						free(vyraz[x]);
					free(vyraz);
				}
			}
			else
				printf("false\n");
		}

		//osmisměrka
		else if(params.state == EIGHT)
		{
			int osm;
			if ((vektor[1]!=NULL)||(matrix[1]!=NULL)) //pokud bude uživatel chtít hledat vektor ve vektoru, nebo matici v matici vypíši false
				printf ("false\n");
			else if (prvek[0]-1>=radek[0]||prvek[0]-1>=sloupec[0]||vektor[0]==NULL) //pokud bude vektor příli velký vypíši false
				printf ("false\n");
			else{
				osm=osmismerka(vektor[0],prvek[0],matrix[0],radek[0],sloupec[0]);
				if (osm==0)
					printf("true\n");
				else
					printf("false\n");
			}

		}

		//bubliny
		else if(params.state == BUBBLES)
		{
		int bubl=0;
			if ((matice_pocet==1)&&(matrix[0]!=NULL)&&(vektor[0]==NULL)){ 		//vstupní podmínky pro bubliny, musí být načtena alespoň jedna matice a ta nesmí být prázdná
				bubl=bubliny(matrix[0],radek[0],sloupec[0]);							//zároveň nesmí užvatel hledat bubliny ve vektoru
				if (bubl>=0)
					printf("počet bublin je: %d\n",bubl);
				else
					printf("false\n");
			}
			else
					printf("false\n");
		}
	}
	else
		printf("false\n"); //pokud při načítání nastala nějaká chyba neprovedu ani jednu funkci a vypíšu false
	//pokud byl naalokovaný vektor uvolním ho
	for (int x=0;x<pocet_vektoru;x++){
		if (vektor[x]!=NULL)
			free (vektor[x]);
		}

	//pokud byla naalokována nějaká matice uvolním ji
	for (int t=0;t<matice_pocet;t++){
		if (matrix[t]!=NULL){
			for (int x=0;x<radek[t];x++)
				free(matrix[t][x]);
			free(matrix[t]);
		}
	}

	//pokud byl naalokovaný vektor matic uvolním ho
	if (vektor_matic!=NULL){
			for (int x=0;x<radek[0]*pocet_matic;x++)
				free(vektor_matic[x]);
			free(vektor_matic);
	}
  return EXIT_SUCCESS;
}
//konec funkce main
