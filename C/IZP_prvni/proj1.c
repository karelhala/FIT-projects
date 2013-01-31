/*
 * Soubor:  hlavni
 * Datum:   2010/11/2
 * Autor:   Hala Karel, xhalak@fit.vutbr.cz
 * Projekt: Jednoduchá komprese textu, projekt è. 1 pro pøedmìt IZP
 * Popis:   Program provádí dekompresy pro N znakù a kompresy pro 3 znaky
 */

// práce se vstupem/výstupem
#include <stdio.h>

// obecné funkce jazyka C
#include <stdlib.h>

// kvùli funkci strcmp
#include <string.h>

// testování znakù - isalpha, isdigit, atd.
#include <ctype.h>

// typ bool, konstanty true, false
#include <stdbool.h>

/** Kódy chyb programu */
enum tecodes
{
  EOK = 0,     /**< Bez chyby */
  ECLWRONG,    /**< Chybný pøíkazový øádek. */
  EUNKNOWN,    /**< Neznámá chyba */
};

/** Stavové kódy programu */
enum tstates
{
  CHELP,       /**< Napoveda */
  
};

/** Chybová hlá¹ení odpovídající chybovým kódùm. */
const char *ECODEMSG[] =
{
  /* EOK */
  "Vse v poradku.\n",
  /* ECLWRONG */
	"Prilis mnoho parametru!\n",

// práce se vstupem/výstupem
#include <stdio.h>

// obecné funkce jazyka C
#include <stdlib.h>

// kvùli funkci strcmp
#include <string.h>

// testování znakù - isalpha, isdigit, atd.
#include <ctype.h>

// pro testování programu
// #include <assert.h>

// limity èíselných typù
// #include <limits.h>
  "Nastala nepredvidana chyba!\n",
};

const char *HELPMSG =
  "Program Jednoducha komprese - vzorova implementace.\n"
  "Autor: Karel Hala (c) 2010\n"
  "Program provadi...\n"
  "Pouziti: proj1 -h		vypise napovedu\n"
  "         proj1 -c N		provede kompresy\n"
  "         proj1 -d N		provede dekompresy\n";


/**
 * Struktura obsahující hodnoty parametrù pøíkazové øádky.
 */
typedef struct params
{
  unsigned int N;   /**< Hodnota N z pøíkazové øádky. */
  int ecode;        /**< Chybový kód programu, odpovídá výètu tecodes. */
  int state;        /**< Stavový kód programu, odpovídá výètu tstates. */
} TParams;

void printECode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  { ecode = EUNKNOWN; }

  	fprintf(stderr, "%s", ECODEMSG[ecode]);
}


//dekomprimace
int dekom(char argv)
{
	unsigned int c,N;
   short int k,x;
	if((argv>='0')&&(argv<='9'))
		N=(argv);
	else
		N='1';
	char buf[N];
	//naèítá znaky dokud není ukonèení øádku
	while ((c=getchar())!=EOF)
	{	
		if (c==10)	//pokud bude konec øádku pokraèuj v cyklu
			continue;
		if (c>='0' && c<='9')
		{
			for (k=0;k<(N-48);k++)
				(buf[k])=getchar();

			for (x=0;x<(c-48);x++)
			{
				for (k=0;k<(N-48);k++)
				putchar(buf[k]);
			}
		}

		else
			putchar(c);
	}
	return EXIT_SUCCESS;
}
//komprimace
int komprimace(char argv)
{
int c;
unsigned int N;
		N=(argv);
N=N-48;
char pole[9],buff[9],pom;
int poc=0;
int cit='1';
//naèti 3 znaky
for (int opakovani=0; opakovani<N; opakovani++)
	pole[opakovani]=getchar();
//naèítej znay dokud není EOF
while ((c=getchar())!=EOF)
{
//pokud naète ukonèení øádku
if (c==10)
{
//pokud byl èítaè vìt¹í 2
	if (cit>='2')
	{
		putchar (cit);
		for (int opakovani=0; opakovani<N; opakovani++)
			putchar(pole[opakovani]);
	}
	else
	{
		for (int opakovani=0; opakovani<N; opakovani++)
			putchar(pole[opakovani]);
	if (N!=1)
	{
		for (int opakovani=0; opakovani<=N-2; opakovani++)
			putchar(buff[opakovani]);
	}
	}
	continue;
}

buff[poc]=c;
int zm=0;
int posun=1;
//pokud je buffer naplnìnen
if (poc==N-1)
{
	//porovnání bufferù
	for (int opakovani=0; opakovani<N; opakovani++)
					{
						if (pole[opakovani]==buff[opakovani])
						zm++;
						else
						zm=0;
					}
					//opakování dokud se buffery neli¹í
					while (zm==N)
					{
						zm=0;
						if (cit=='9')
						{
							putchar(cit);
							cit='0';
							for (int opakovani=0; opakovani<N; opakovani++)
								putchar(pole[opakovani]);
						}
						++cit;
						for (int opakovani=0; opakovani<N; opakovani++)
						{
							pole[opakovani]=buff[opakovani];
							buff[opakovani]=getchar();
					//pokud pøi porovnávání narazím na konec øádku
							if (buff[opakovani]==10)
							{
								if (cit>='2')
								{
									putchar(cit);
									cit='1';
									for (int opakovani=0; opakovani<N; opakovani++)
										putchar(pole[opakovani]);
								}
								continue;
							}
						}
						for (int opakovani=0; opakovani<N; opakovani++)
						{
							if (pole[opakovani]==buff[opakovani])
								zm++;
							else
								zm=0;
						}
						
					}
				//pokud pøi porovnávání narazil na stejný øetìzec vypí¹e ho
					if (cit>='2')
					{
						putchar(cit);
						cit='1';
						for (int opakovani=0; opakovani<N; opakovani++)
						{
							putchar(pole[opakovani]);
							pole[opakovani]=buff[opakovani];
						}
				//pokud pøi výpisu narazil na konec øádku vypí¹e poslední znaky a zkonèí
						for (int opakovani=0; opakovani<N-1; opakovani++)
						{
							buff[opakovani]=getchar();
							if(N==1)
								return EXIT_SUCCESS;
							else if (buff[opakovani]==10)
							{
								if(N==1)
									return EXIT_SUCCESS;
								for (int opakovani=0; opakovani<N; opakovani++)
									putchar(pole[opakovani]);
								putchar(buff[0]);
							}
						}
					}
				//pokud nejsou ¾ádné opakující se znaky posune buffery o jeden znak
					else
					{
					putchar (pole[0]);
					pom=buff[0];
					for (int opakovani=0;opakovani<N-1;opakovani++)
					{
						buff[opakovani]=buff[posun];
						pole[opakovani]=pole[posun];
						++posun;
					}
					pole[N-1]=pom;
					}
					
}
else
	poc++;
}																																									
return EXIT_SUCCESS;
}

TParams getParams(int argc, char *argv[])
{
  TParams result =
  { // inicializace struktury
    .N = 0,
    .ecode = EOK,
  };


  	if (argc == 2 && strcmp("-h", argv[1]) == 0)
  	{ // tisk nápovìdy
    result.state = CHELP;
 	}
	else if (argc == 3 && strcmp("-d", argv[1]) == 0 && argv[2][1]<1) 
	{
		if  (argv[2][0]>='0' && argv[2][0]<='9')
		{
			dekom(argv[2][0]);
			result.state = EOK;
		}
	}
  else if (argc == 3 && strcmp("-c", argv[1]) == 0 && argv[2][1]<1) 
	{
		if( (argv[2][0])>='0' && (argv[2][0])<='9' )
			komprimace(argv[2][0]);
			result.state = EOK;
	}
  else
  { // pøíli¹ mnoho parametrù
    result.ecode = ECLWRONG;
  }

  return result;
}


/**
 * Hlavní program.
 */

int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);
  if (params.ecode != EOK)
  { // nìco nestandardního
    printECode(params.ecode);
    return EXIT_FAILURE;
  }
	if (params.state == EOK)
  	{
		putchar('\n');
		return EXIT_SUCCESS;
		
	}
  if (params.state == CHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}

/* konec main.c */
