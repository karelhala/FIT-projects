/*
 * Soubor:  hlavni
 * Datum:   2010/11/2
 * Autor:   Hala Karel, xhalak@fit.vutbr.cz
 * Projekt: Jednoduch� komprese textu, projekt �. 1 pro p�edm�t IZP
 * Popis:   Program prov�d� dekompresy pro N znak� a kompresy pro 3 znaky
 */

// pr�ce se vstupem/v�stupem
#include <stdio.h>

// obecn� funkce jazyka C
#include <stdlib.h>

// kv�li funkci strcmp
#include <string.h>

// testov�n� znak� - isalpha, isdigit, atd.
#include <ctype.h>

// typ bool, konstanty true, false
#include <stdbool.h>

/** K�dy chyb programu */
enum tecodes
{
  EOK = 0,     /**< Bez chyby */
  ECLWRONG,    /**< Chybn� p��kazov� ��dek. */
  EUNKNOWN,    /**< Nezn�m� chyba */
};

/** Stavov� k�dy programu */
enum tstates
{
  CHELP,       /**< Napoveda */
  
};

/** Chybov� hl�en� odpov�daj�c� chybov�m k�d�m. */
const char *ECODEMSG[] =
{
  /* EOK */
  "Vse v poradku.\n",
  /* ECLWRONG */
	"Prilis mnoho parametru!\n",

// pr�ce se vstupem/v�stupem
#include <stdio.h>

// obecn� funkce jazyka C
#include <stdlib.h>

// kv�li funkci strcmp
#include <string.h>

// testov�n� znak� - isalpha, isdigit, atd.
#include <ctype.h>

// pro testov�n� programu
// #include <assert.h>

// limity ��seln�ch typ�
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
 * Struktura obsahuj�c� hodnoty parametr� p��kazov� ��dky.
 */
typedef struct params
{
  unsigned int N;   /**< Hodnota N z p��kazov� ��dky. */
  int ecode;        /**< Chybov� k�d programu, odpov�d� v��tu tecodes. */
  int state;        /**< Stavov� k�d programu, odpov�d� v��tu tstates. */
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
	//na��t� znaky dokud nen� ukon�en� ��dku
	while ((c=getchar())!=EOF)
	{	
		if (c==10)	//pokud bude konec ��dku pokra�uj v cyklu
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
//na�ti 3 znaky
for (int opakovani=0; opakovani<N; opakovani++)
	pole[opakovani]=getchar();
//na��tej znay dokud nen� EOF
while ((c=getchar())!=EOF)
{
//pokud na�te ukon�en� ��dku
if (c==10)
{
//pokud byl ��ta� v�t�� 2
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
//pokud je buffer napln�nen
if (poc==N-1)
{
	//porovn�n� buffer�
	for (int opakovani=0; opakovani<N; opakovani++)
					{
						if (pole[opakovani]==buff[opakovani])
						zm++;
						else
						zm=0;
					}
					//opakov�n� dokud se buffery neli��
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
					//pokud p�i porovn�v�n� naraz�m na konec ��dku
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
				//pokud p�i porovn�v�n� narazil na stejn� �et�zec vyp�e ho
					if (cit>='2')
					{
						putchar(cit);
						cit='1';
						for (int opakovani=0; opakovani<N; opakovani++)
						{
							putchar(pole[opakovani]);
							pole[opakovani]=buff[opakovani];
						}
				//pokud p�i v�pisu narazil na konec ��dku vyp�e posledn� znaky a zkon��
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
				//pokud nejsou ��dn� opakuj�c� se znaky posune buffery o jeden znak
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
  	{ // tisk n�pov�dy
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
  { // p��li� mnoho parametr�
    result.ecode = ECLWRONG;
  }

  return result;
}


/**
 * Hlavn� program.
 */

int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);
  if (params.ecode != EOK)
  { // n�co nestandardn�ho
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
