/*
 * Soubor:  proj2
 * Datum:   2010/11/11
 * Autor:   Karel Hala, xhalak00d@fit.vutbr.cz
 * Projekt: Iterační výpočty
 * Popis: 
 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

#include <math.h>

#define DOUBLE_DIG 20

//globalni promenne
struct matematicke_pomocne{
	double citatel,jmenovatel; 		//proměnné pro výsledky
	double vyp_x;							//proměnná pro výpočet logaritmu
	unsigned int pocet;					//pomocná proměnná pro výpočet logaritmu
	int jednicka;							//proměnná, která nabývá hodnoty 1 a -1
} T_math_vypocty;

// Kódy chyb
enum tecodes
{
  EOK = 0,     // Bez chyby
  ECLWRONG,    // Chybný příkazový řádek.
  EUNKNOWN,    // Neznámá chyba
};

// Stavové kódy programu
enum tstates
{
  	CHELP,      // Nápověda 
  	TANH,       // hyperbolicky tangens
	LOGAX,    	// logaritmus 
	ARITM,		// aritmetický průměr
	KVADR,		// kvadratický průměr
};
const char *chybne_x=
		"Zadali jste záporné x, což je chybné zadání\n";
const char *vstup =
	"\nZadali jste špatné číslo.\n"
	"Dávejte si pozor na to aby číslo mělo tvar:\nnapř: 2e3 , 2000 , 0.003 , 3e-3\n";

/** Chybová hlášení odpovídající chybovým kódům. */
const char *ECODEMSG[] =
{
	// vsechno v poradku
	"EOK",
  	// chybne zadane parametry programu
	"Chybné parametry příkazového řádku\n"
	"	     pro nápovědu napiš -h\n",
	//neocekavana chyba
	"Neočekávaná chyba\n",
};

const char *HELPMSG =
	"\nProgram iteračni výpočty\n"
	"Autor: Karel Hala (c) 2010\n"
	"Použití: 	proj2 -h		 vypíše nápovědu\n"
	"		proj2 --logax sigdig a	 logaritmus\n"
	"        	proj2 --tanh sigdig 	 hyperbolický tangens\n"
	"        	proj2 --wam 		 váženy aritmetický průměr\n"
	"       	 	proj2 --wqm 		 váženy kvadratický průměr\n\n";



//hodnoty parametrů příkazové řádky.
 
typedef struct params
{
  int ecode;        				// Chybový kod programu, odpovídá výčtu tecodes
  int state;        				// Stavový kód programu, odpovídá výčtu tstates
} TParams;

void printECode(int ecode)
{
	 fprintf(stderr, "%s", ECODEMSG[ecode]);
}
//získání parametrů
TParams getParams(int argc, char *argv[])
{
  TParams result =
  { // inicializace struktury
    .ecode = EOK,
    .state = CHELP,
  };
	if (argc==2)
	{
		if (strcmp("-h", argv[1]) == 0) //nápověda
			result.state = CHELP;

		else if (strcmp ("--wam", argv[1]) == 0) //aritmetický průměr
			result.state = ARITM;	

		else if (strcmp ("--wqm", argv[1]) == 0) //kvadratický průměr
			result.state = KVADR;	

		else
   		result.ecode = ECLWRONG;
	}

	else if (argc==3 && (strcmp ("--tanh", argv[1]) == 0)) //tangens
	{
		result.state = TANH;
	}

	else if (argc==4 && (strcmp ("--logax", argv[1]) == 0)) //logaritmus
	{	
		result.state = LOGAX;
	}

	else
  		result.ecode = ECLWRONG;

  return result;
}
//logaritmus
//výpočet jmenovatele a čitatele pro funkci logaritmu
double vypocet (double x, double sid)
{
	double y=(x-1)/(x+1);
	double yp=y-1;
	double tk=y;
	double pom;
	double k=1;
	
	while (fabs(y-yp)>sid)
	{
		tk=tk*(((x-1)*(x-1))/((x+1)*(x+1)));
		k=k+2;
		pom=(1/k)*tk;
		yp=y;
		y=pom+yp;
	}
	return 2*y;
}

//předávání hodnot pro funkci logaritmu
double logaritmus (double x,double a, double sid)
{
	double pom=10,m=10;
		for (int a=0;a<=sid;a++){
			pom *=m;
		}
		sid = 1/pom;
	double citatel, jmenovatel;
	if (x<1e-308) //pokud bude x menší než číslo, které se vleze do doublu, nastavím ho na nejmenší možné číslo
			x=1e-308;
	if ((a==INFINITY) && (x>0)){
		citatel=NAN;
		jmenovatel =NAN;
	}
	else{
		citatel = vypocet(x,sid);
		jmenovatel =vypocet(a,sid);
	}
	return citatel/jmenovatel;
}
//vedlejší výpočet pro funkci logaritmus, pokud je číslo X moc velké, převede jej na menší
int prevod_x(double x,double a)
{
	T_math_vypocty.pocet = 0;
	T_math_vypocty.jednicka = 1;
	if (x<1){
		x=1/x;
		T_math_vypocty.jednicka =-1;
	}
	while (x > a){
		T_math_vypocty.pocet+=1;
		x = x/a;
	}
	T_math_vypocty.vyp_x=x;
	return T_math_vypocty.vyp_x;
}

//konec výpočtu logaritmu


//tangens
//výpočet cosinu pro funkci tangens (jmenovatel)
double cosinus(double x, double sig)
{
 		//nastavení hodnot pro funkci cosinus
		double yp=-1;
		double cos_y = 1;
		double cos_i = 0;
		double cos_taylor = 1;
		while (fabs(cos_y-yp)>sig)
		{    
			cos_i = cos_i + 2;
			cos_taylor = cos_taylor*x*x/(cos_i*(cos_i-1));
			yp = cos_y;
			cos_y = cos_taylor + yp;    
		}
		return cos_y;
}
//výpočet sinu pro funkci tangens (čitatel)
double sinus(double x, double sig)
{
	//nastavení hodnot pro funkci sinus
	double yp=-1;
	double sin_y = x;
	double sin_i = 0;
	double sin_taylor = x;

	while (fabs(sin_y-yp)>sig)
	{
		sin_i = sin_i+2;
		sin_taylor = sin_taylor*x*x/(sin_i*(sin_i+1));
		yp = sin_y;
		sin_y = sin_taylor + yp;
	}
	return (sin_y);
}
//předávání hodnot funkci tangens
double tangens(double x,double pres)
{
	double pom=10,m=10;
		for (int a=0;a<=pres;a++){
			pom *=m;
		}
		pres = 1/pom;
	double vysl;
	double jmenovatel,citatel;

	jmenovatel =sinus(x,pres);
	citatel=cosinus(x,pres);
	//samostatný výpočet tangens
	vysl = jmenovatel/citatel;

	return vysl;
}
//konec výpočtu tangens


//průměry
//výpočet aritmetického průměru
double prumaritm(double x,double h)
{
	T_math_vypocty.jmenovatel = T_math_vypocty.jmenovatel+(x*h);
	T_math_vypocty.citatel= T_math_vypocty.citatel+h;
	return T_math_vypocty.jmenovatel/T_math_vypocty.citatel;
}
//výpočet kvadratického průměru
double prumkvadr(double x,double h)
{
	T_math_vypocty.jmenovatel = T_math_vypocty.jmenovatel+((x*x)*h);
	T_math_vypocty.citatel = T_math_vypocty.citatel+h;
	return sqrt(T_math_vypocty.jmenovatel/T_math_vypocty.citatel);
}
//konec výpočtů průměrů


//funkce main
int main(int argc, char *argv[])
{
	int qq,pp;
	char *chyba;
	double x,h,vysl,sig,a;
	
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
	//oetření aby uživatel nezadal nelegální znak jako přesnost
	if (argc>=3){
		sig = strtod(argv[2],&chyba);

		if (*chyba!='\0'){
			printf("zadali jste špatnou přesnost\n");
			return EXIT_FAILURE;
		}
		
	}
	//oetření aby uživatel nezadal nelegální znak jako základ pro logaritmus
	if (argc==4){
		a = strtod(argv[3],&chyba);

		if (a==1||a<=0){	//pokud uživatel zadá nesmyslný základ, jako třeba 1, nebo číslo měnší než 0
				printf ("Zadali jste nesmyslný základ\n");
			return 1;
			}
		if (*chyba!='\0'){
			printf("zadali jste špatný základ\n");
			return EXIT_FAILURE;
			}
	}
	//načítání hodnot pro funkce
	while ((qq=scanf("%lf",&x))!=EOF){
		if(qq==0){
			scanf("%*s");
			x=NAN;
		}
		if (x>1e308) //pokud bude x větší než číslo, které se vleze do doublu, nastavím ho na nekonečno
			x=1e308;
		if (x<-1e308)
			x=-1e308;
		
			
		//posílání hodnot pro funkci logaritmus
		if (params.state == LOGAX)
 		{
			if (x<0){			//ošetření, aby uživatel nezadal záporné X
				printf ("byla zadána záporná hodnota X, což je chybné zadání\n");
				continue;
			}
			//pro větí rychlost vypočítávám hodnoty X, když jsou nekonečno, nebo nula přímo
			if (x==INFINITY){
				printf("%.10e\n",INFINITY);
				continue;
			}
			
			if (x==0){
				printf("%.10e\n",-INFINITY);
				continue;
			}
			
			//pro větší rychlost převedu x na menší číslo
			if (a>1){
				prevod_x(x,a);
				vysl=logaritmus(T_math_vypocty.vyp_x,a,sig);
				printf ("%.10e\n",(T_math_vypocty.pocet+vysl)*T_math_vypocty.jednicka);
			}
			else{
				vysl=logaritmus(x,a,sig);
				printf ("%.10e\n",(vysl));
			}

		}
		//konec posílání hodnot pro funkci logaritmu

		//posílání hodnot pro funkci tangens
		else if (params.state == TANH)
 		{
			if (x<-DOUBLE_DIG)
				vysl=tangens(-DOUBLE_DIG,sig);
			else if (x>DOUBLE_DIG)
				vysl=tangens(DOUBLE_DIG,sig);
			else
				vysl=tangens(x,sig);

			printf("%.10e\n",vysl);
		}
		//konec posílání hodnot pro funkci tangens

		//posílání hodnot pro výpočet průměrů
		//aritmetický průměr
		else if (params.state == ARITM)
 		{
			if (x<0){
				printf ("%s",chybne_x);
				return 1;
			}
			//načtení váhy
			pp=scanf("%lf",&h);
			if(pp!=0){
				scanf("%*s");
				h=NAN;
			}
			if (h<=0){
				continue;
			}
			//pokud bude váha rovna nekonečnu nemůže být proveden výpočet
			if (h==INFINITY||h==(-INFINITY))
				h=NAN;
			vysl = (prumaritm(x,h));
			printf ("%.10e\n",vysl);
		}

		//kvadratický průměr
		else if (params.state == KVADR)
 		{
			if (x<0){
				printf ("%s",chybne_x);
				return 1;
			}
			//načtení váhy
			pp=scanf("%lf",&h);
			if(pp!=1){
				h=NAN;
				scanf("%*s");
			}

			if (h<=0){
				continue;
			}
			//pokud bude váha rovna nekonečnu nemůže být proveden výpočet
			if (h==INFINITY||h==(-INFINITY))
				h=NAN;
			vysl = (prumkvadr(x,h));
			printf ("%.10e\n",vysl);
		}
		//konec posílání hodnot pro výpočet průměrů
	}
  return EXIT_SUCCESS;
}
//konec funkce main
