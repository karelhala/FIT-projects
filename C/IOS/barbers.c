#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#define id 138666


/* 
	Program: Spici Holic
	Vytvoreno ciste z me hlavy.
	Kdo to zdrbal: Karel Hala (c) 2011
*/


//----------------------
//--------napoveda------
//----------------------
const char *HELPMSG = 
	"\nProgram spleeping barber\n"
	"Autor: Karel Hala (c) 2011\n\n"
	"PouĹžitĂ ./barbers -h				vypise tuto napovedu\n"
	"         ./barbers Q GenC GenB N soubor		vypise vystup do souboru\n"
	"         ./databaze Q GenC GenB N -		vypise vystup na stdout\n\n"
	"vysvetleni jednotlivych parametru:\n"
	"		Q		pocet zidly\n"
	"		GenC		rozsah ro generovani zakazniku\n"
	"		GenB		rozsah ro generovani doby obsluhy\n"
	"		N		pocet zakazniku\n\n";
//----------------------
//----konec napovedydy----
//----------------------

//navratove hodnoty
enum
{
	EOK,
	chyba,
};

//vyber parametru
enum
{
	HINT,
	DO_SOUBORU,
	NA_OBRAZOVKU,
	SPATNE,
};

//struktura pro sdilenou pamet
typedef struct pamet {
	int cislo_akce;			//citac akci
	int sedici;					//tady si uchovavam pocet sedicich zakazniku
	int zakaznik_cislo;		//cislo zakaznika
	int celkem_zakazniku;	//kolik bude maximalne obslouzeno zakazniku
	int celkem_zidli;			//kolik je celkem zidli v cekarne
	int obslouzeno;			//kolik ze jich bylo obslouzeno
}Tpamet;

/*struktura pro semafory*/
typedef struct semafory{
	sem_t vypis;				//semafor pro vypis na stdout, nebo do souboru
	sem_t zmena_pameti;		//semafor na zmenu promennych ve sdilene pameti
	sem_t usni;					//semafor na uspani holice
	sem_t pockej;				//semafor na pockani zakaznika nez vypise barber ready a finished
	sem_t kreslo;				//semafor na usednuti do kadernikova kresla
	sem_t kadernik_pockej;	//semafor na pockani kadernika dokud zakaznik nevypise ready
	sem_t spust;				//semafor na zacatek kadernika, aby nezacal vypisovat ready driv nez bude zakaznik pripraven se rozkoukat
	sem_t strihej;				//semafor na sdeleni kadernikovi ze zakaznik sedl do kresla a on muze zacit makat
}Tsemafory;

/*funkce na zpracovani parametru*/
int zpracovani_parametru (int argc, char *argv[])
{
	if (argc == 2 && strcmp("-h", argv[1]) == 0)
	{
		return HINT;	//pokud chceme vypsat napovedu
	}
	else if (argc == 6)
	{
		if (strcmp("-", argv[5]) == 0)
			return NA_OBRAZOVKU;		//pokud chceme vypsat vystup na stdout
		else
			return DO_SOUBORU;		//pokud chceme vypsat vystup do souboru
	}
	else
		return SPATNE;					//pokud nastane neocekavana chyba
}

/*funkce na otevreni souboru*/
int otevri_soubor_pro_zapis (char *jmeno,FILE **fw)
{
	if ((*fw=fopen(jmeno,"w"))==NULL){
		printf("nepodarilo se otevrit soubor %s ",jmeno );
	return 1;
	}
	return EOK;		//pokud se povedlo otevreni souboru pro zapis vratim nulu (EOK)
}

/*funkce na zavreni souboru*/
int zavri_soubor (FILE **f)
{
		if (fclose(*f)==EOF){
			printf("nepodarilo se zavrit soubor \n");
			return 1;
		}

	return EOK;		//pokud se povedlo zavreni souboru vratim nulu (EOK)
}

/*funkce holictvi v sobe obshauje vsechno co by mel delat kadernik

popis parametru: 
id_pameti je id sdilene pameti
id_sem je id sdilenych semaforu 
strihani je maximalni cislo na urceni doby strihani
kam je promenna ve ktere je urceno kam mam vypisovat
soubor je pro zapis do souboru

popis funkce:
holic zkontroluje jestli je nekdo v cekarne, pokud neni usne.
Pokud tam nekdo je vezme ho a ostiha ho.
na konci cyklu zkontrolue jestli jeste nekdo dojde, pokud ne ukonci se.
*/
int holicstvi(int id_pameti, int id_sem,int strihani,int kam,FILE** soubor)
{
	srandom(time(NULL));
	int cekej,celkem_zakazniku,zidle,sedi;
	Tsemafory* semafory;											//ukazatel na sdilene semafory
	semafory =(Tsemafory*) shmat(id_sem,0,0);				//ziskani hodnot ze sdilene pameti
	Tpamet* pamet;													//ukazatel na sdilenou pamet
	pamet =(Tpamet*) shmat(id_pameti,0,0);					//ziskani hodnot ze sdilene pameti

	//nekonecny cyklus
	for(;;){
		sem_wait(&semafory->vypis);							//uzavru semafor pro vypis
		sem_wait(&semafory->zmena_pameti);					//uzavru semafor pro zmenu hodnot ve sdilene pameti
		zidle=pamet->celkem_zidli;								//nactu celkem zidli do proemnne, abych nemusel znovu vytahovat tuto hodnotu
		celkem_zakazniku=pamet->celkem_zakazniku;			//nactu pocet zakazniku kolik jich dojde ten den

		//escape podminka
		if (celkem_zakazniku==pamet->obslouzeno)
		{
			sem_post(&semafory->zmena_pameti);
			sem_post(&semafory->vypis);
			break;
		}

		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: barber: checks\n",pamet->cislo_akce++);		//zapis akce do souboru
		else
			printf("%d: barber: checks\n",pamet->cislo_akce++);					//zapis akce na stdout

		sedi=pamet->sedici;				//kolik zakazniku sedi na zidli
		sem_post(&semafory->zmena_pameti);	//pustim semafor na zmenu pameti
		sem_post(&semafory->vypis);			//pustim semafor na vypis
		//pokdu nikdo nesedi usnu
		if (sedi==0)
		{

			if (zidle==0) 		//pokud nemam porizenou zadnou zidli ukoncim cyklus
				break;
			sem_wait(&semafory->usni);
		}
		//posloupnost prikazu pro praci
		sem_wait(&semafory->spust);		//pockam dokud mi zakaznik neda najevo ze muze
		sem_wait(&semafory->vypis);		
		sem_wait(&semafory->zmena_pameti);

		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: barber: ready\n",pamet->cislo_akce++);
		else
			printf("%d: barber: ready\n",pamet->cislo_akce++);

		//snizim pocet sedicich zakazniku
		pamet->sedici--;

		sem_post(&semafory->pockej);					//spustim zakaznika a dam mu najevo e muze prijit do kresla
		sem_wait(&semafory->strihej);					//pockam dokud zakaznik neusedne do kresla
		cekej=random() % (strihani+1);				//zjistim jak dlouho budu strihat
		usleep(cekej*1000);
		sem_wait(&semafory->vypis);
		sem_wait(&semafory->zmena_pameti);

		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: barber: finished\n",pamet->cislo_akce++);
		else
			printf("%d: barber: finished\n",pamet->cislo_akce++);

		sem_post(&semafory->kreslo);					//propustim zakaznika z kresla
		sem_wait(&semafory->kadernik_pockej);		//pockam dokud nevypise ze je obslouzen

		//escape podminka
		if (celkem_zakazniku==pamet->obslouzeno){
			sem_post(&semafory->zmena_pameti);
			sem_post(&semafory->vypis);
			break;
		}
		sem_post(&semafory->zmena_pameti);
		sem_post(&semafory->vypis);

	}

	zavri_soubor(soubor);		//zavru soubor v tomhle podprocesu
	_exit(0);
}

/*funce zakaznici je posloupnost prikazu pro jednotlive zakazniky

popis parametry:
id_pameti je id sdilene pameti
id_sem je id sdilenych semaforu
kam je promenna ve ktere je urceno kam mam vypisovat
soubor je pro zapis do souboru

popis funkce:
zakaznik se vytvori, prijde do cekarny a podiva se zda je misto k sezeni.
Pokud neni odejde. Ale pokud je, tak si usedne.
Zkontroluje jestli neni jediny sedici, pokud je tak probudi spiciho holice.
Po ostrihani odejde a zavre za sebou soubor (dvere).
*/

int zakaznici(int id_pameti, int id_sem,int kam,FILE** soubor)
{
	Tsemafory* semafory;										//ukazatel na sdilene semafory
	semafory =(Tsemafory*) shmat(id_sem,0,0);			//nacteni hodnot semaforu
	Tpamet* pamet;												//ukazatel na sdilenou pamet
	Tpamet pamet_funkce;										//lokalni pamet funkce, je zde ulozeno napriklad to kolikaty je tohle zakaznik
	pamet =(Tpamet*) shmat(id_pameti,0,0);				//nacteni hodnot sdilene pameti
	sem_wait(&semafory->vypis);
	sem_wait(&semafory->zmena_pameti);
	pamet_funkce=*pamet;
	pamet->zakaznik_cislo++;

	//posloupnost prikazu na vytvoreni a vstupu do cekarny
	if (kam==DO_SOUBORU)
		fprintf(*soubor,"%d: customer %d: created\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
	else
		printf("%d: customer %d: created\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);

	sem_post(&semafory->zmena_pameti);
	sem_post(&semafory->vypis);

	sem_wait(&semafory->vypis);
	sem_wait(&semafory->zmena_pameti);

	if (kam==DO_SOUBORU)
		fprintf(*soubor,"%d: customer %d: enters\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
	else
		printf("%d: customer %d: enters\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);

	sem_post(&semafory->zmena_pameti);
	sem_post(&semafory->vypis);

	//posloupnost prikazu pro opusteni, nebo usednuti a naslednem ostrihani
	sem_wait(&semafory->zmena_pameti);
	if (pamet_funkce.celkem_zidli>pamet->sedici){
		if (pamet->sedici==0)	//pokud nikdo nesedi v cekarne probudim ospalyho kadernika
			sem_post(&semafory->usni);
		pamet->sedici++;			//zvysim pocet sedicich zakazniku
		sem_post(&semafory->zmena_pameti);
		sem_post(&semafory->spust);
		sem_wait(&semafory->pockej);		//pockam dokud mi kadernik neda nejevo ze je ready
		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: customer %d: ready\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
		else
			printf("%d: customer %d: ready\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);

		sem_post(&semafory->zmena_pameti);
		sem_post(&semafory->vypis);

		//dam najevo kadernikovy ze muze zacit pracovat a usednu do kresla
		sem_post(&semafory->strihej);
		sem_wait(&semafory->kreslo);
		
		//vypisu ze jsem byl obslouzen a pustim kadernika zpet do prace
		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: customer %d: served\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
		else
			printf("%d: customer %d: served\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);

		pamet->obslouzeno++;			//zvysim pocet obslouzenych zakazniku
		sem_post(&semafory->kadernik_pockej);
	}

	//pokud neni misto v cekarne odejdu
	else{
		sem_post(&semafory->zmena_pameti);
		sem_wait(&semafory->vypis);
		sem_wait(&semafory->zmena_pameti);

		if (kam==DO_SOUBORU)
			fprintf(*soubor,"%d: customer %d: refused\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
		else
			printf("%d: customer %d: refused\n",pamet->cislo_akce++,pamet_funkce.zakaznik_cislo);
		pamet->obslouzeno++;		//zvysim pocet obslouzenych zakazniku
		sem_post(&semafory->zmena_pameti);
		sem_post(&semafory->vypis);
		zavri_soubor(soubor);
		_exit(0);
	}
	zavri_soubor(soubor);
	_exit (0);
}

//--------------------------
//----hlavni funkce main----
//--------------------------
int main(int argc, char *argv[])
{
	
	//v promenne pocet je pocet zakazniku
	int parametry,holic,pocet,zakaznik;
	FILE* jmeno_souboru;
	parametry=zpracovani_parametru(argc,argv);	//ziskani parametru

	if (parametry==HINT){
		printf ("%s",HELPMSG);
		return EXIT_SUCCESS;
	}

	else if (parametry==SPATNE)
	{
		printf ("spatne zadane parametry, prectete si napovedu ./barbers -h\n");
		return chyba;
	}

	else if (parametry==DO_SOUBORU){
		otevri_soubor_pro_zapis(argv[5],&jmeno_souboru);
		setbuf(jmeno_souboru,NULL);
	}

	srandom(time(NULL));
	struct shmid_ds shmbuffer;
	key_t semkey;							//unikatni klic ke sdilenym semaforum
	Tpamet* sdilena_pamet;				//ukazatel na sdilenou pamet
	Tsemafory* sdilene_semafory;		//ukazatel na sdilene semafory
	Tsemafory moje_semafory;			//strktura semaforu
	Tpamet pamet;							//struktura pameti
	int id_pameti,id_semaforu,dobre,id_main;	//id sdilene pameti, semaforu a pid hlavniho procesu
	int* pole_pid;							//pole jednotlivych pid
	int base=0; 							//zaklad bude 0 pro strtoul protoze budeme pocitat v 10 soustave
	char *endptr;							//co nam zbyde k prelozeni na cisla
	int zjisteni;							//promena na zjisteni jestli nejaky parametr neni neco jineho nez cislo
	errno=0;									//cislo chyby
	
	//prevedu parametr na pocet vytvoreni zakazniku 
	pocet = strtoul (argv[4],&endptr,base);
	if ((errno == ERANGE && (pocet == LONG_MAX || pocet == LONG_MIN)) || (errno != 0 && pocet == 0)||endptr == argv[4]||*endptr != '\0'){
		fprintf(stderr,"chyba v parametru\n");
		return chyba;
	}
	//zjistim jestli neni nejaky parametr neco spatne, pokud je ukoncim okamzite program a vypisu do stderr chybu
	zjisteni = strtoul (argv[1],&endptr,base);
	if ((errno == ERANGE && (zjisteni == LONG_MAX || zjisteni == LONG_MIN)) || (errno != 0 && zjisteni == 0)||endptr == argv[1]||*endptr != '\0'){
		fprintf(stderr,"chyba v parametru\n");
		return chyba;
	}

	zjisteni = strtoul (argv[2],&endptr,base);
	if ((errno == ERANGE && (zjisteni == LONG_MAX || zjisteni == LONG_MIN)) || (errno != 0 && zjisteni == 0)||endptr == argv[2]||*endptr != '\0'){
		fprintf(stderr,"chyba v parametru\n");
		return chyba;
	}

	zjisteni = strtoul (argv[3],&endptr,base);
	if ((errno == ERANGE && (zjisteni == LONG_MAX || zjisteni == LONG_MIN)) || (errno != 0 && zjisteni == 0)||endptr == argv[3]||*endptr != '\0'){
		fprintf(stderr,"chyba v parametru\n");
		return chyba;
	}

	pole_pid=malloc((pocet+1)*sizeof(int));

	//ziskam unikatni klic k semaforum
	semkey = ftok(argv[0],id);
	if (semkey==-1){
		printf("chyba pri vytvareni klice\n");
		return 1;}

	//inicializace jednotlivych semaforu
	if ((sem_init(&moje_semafory.vypis,1,1))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.zmena_pameti,1,1))==-1)
		printf ("chyba pri vytvareni semaforu");

	//tyto semafory jsou "cakci", cekaji dokud je jiny proces nespusti
	if ((sem_init(&moje_semafory.usni,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.pockej,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.kreslo,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.kadernik_pockej,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.spust,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");
	if ((sem_init(&moje_semafory.strihej,1,0))==-1)
		printf ("chyba pri vytvareni semaforu");

	//vytvoreni sdilene pameti pro semafory a nasledne ulozeni struktury semaforu do pameti
	id_semaforu = shmget (semkey,sizeof(Tsemafory),IPC_CREAT|0777);
	if (id_semaforu==-1){
		printf("chyba pri vytvareni sdilene pameti");
		return 1;
		}
	//ulozeni struktury semaforu do pameti
	sdilene_semafory = (Tsemafory*) shmat(id_semaforu,0,0);
	*sdilene_semafory = moje_semafory;

	//inicializace jednotlivych prvku struktury
	pamet.cislo_akce=1;
	pamet.sedici=0;
	pamet.zakaznik_cislo=1;
	pamet.celkem_zakazniku=pocet;
	pamet.celkem_zidli=atoi(argv[1]);
	pamet.obslouzeno = 0;
	
	//vytvoreni sdilene pameti
	id_pameti = shmget (IPC_PRIVATE,sizeof(Tpamet),IPC_CREAT|0777);
	if (id_pameti==-1){
		printf ("chyba pri vytvareni sdilene pameti \n");
		return 1;
		}
	//ulozeni struktury do pameti
	sdilena_pamet =(Tpamet*) shmat(id_pameti,0,0);
	*sdilena_pamet = pamet;

	id_main=getpid();		//zjisteni pid hlavni funkce

	//pokud pocet zakazniku je vetsi nez nula provedu rozdeleni funkci do jednotlivych vlaken
	if (pocet>0){
		holic =fork();

		if (holic == 0){
			holicstvi(id_pameti,id_semaforu,atoi(argv[3]),parametry,&jmeno_souboru);	//volani funkce holice
		}

		else if (holic > 0){
			for (dobre=0;dobre<pocet;dobre++){
	
				//urceni jak dlouho se bude cekat pred vytvorenim vlakna zakaznika
				int doba_cekani;
				doba_cekani = random() % (atoi(argv[2])+1);
				usleep (doba_cekani*1000);
	
				zakaznik = fork();
	
				if (zakaznik == 0){
					zakaznici(id_pameti,id_semaforu,parametry,&jmeno_souboru);	//volani funkce zakaznik
					break;
						}
				else if (zakaznik>0){
					pole_pid[dobre]=zakaznik;	//ulozim pid nove vytvoreneho procesu do pole
					pole_pid[dobre+1]=holic;	//na vrchol pole zpravne vytvorenych zakazniku ulozim pid holice
					continue;
				}
				else{
					printf ("chyba forku\n");
					break;							//pokud nastane chyba pri vytvareni zakazniku breknu cyklus
					}
			}
		}
		else{
			printf ("chyba forku\n");
			if (shmdt(sdilena_pamet)==0)	//pokud se povede odpojit od pameti odstranim ji
				shmctl (id_pameti, IPC_RMID, &shmbuffer);	

			else
				printf("chyba pri odstraneni sdilene pameti\n");	//pokud se nepovede odpojit od pameti vytisknu chybu

			if (shmdt(sdilene_semafory)==0)	//pokud se povede odpojit od pameti odstranim ji
				shmctl (id_semaforu, IPC_RMID, &shmbuffer);

			else{
				printf("chyba pri odstraneni sdilenych semaforu\n");	//pokud se nepovede odpojit od pameti vytisknu chybu
				return chyba;
			}

			return chyba;						//pokud nastane chyba ukoncim program, ale jeste uvolnim sdilenou pamet a semafory
		}


	}

	//hlavni program ceka na ukonceni vsech podprocesu co se vytvorili
	if (id_main==getpid())
		{
			for (int x=0;x<=dobre;x++)
			//wait(0);
			waitpid(pole_pid[x],NULL,0);
		}

	if (shmdt(sdilena_pamet)==0)			//pokud se povede odpojit od pameti odstranim ji
		shmctl (id_pameti, IPC_RMID, &shmbuffer);
	else
		printf("chyba pri odstraneni sdilene pameti\n");	//pokud se nepovede odpojit od pameti vytisknu chybu
	if (shmdt(sdilene_semafory)==0)		//pokud se povede odpojit od pameti odstranim ji
		shmctl (id_semaforu, IPC_RMID, &shmbuffer);
	else
		printf("chyba pri odstraneni sdilenych semaforu\n"); //pokud se nepovede odpojit od pameti vytisknu chybu

	if (parametry==DO_SOUBORU)
		zavri_soubor(&jmeno_souboru);		//pokud mam pracovat se souborem zavru ho

	sem_destroy(&moje_semafory.vypis);					//uvolneni semaforu
	sem_destroy(&moje_semafory.zmena_pameti); 		//uvolneni semaforu
	sem_destroy(&moje_semafory.usni);					//uvolneni semaforu
	sem_destroy(&moje_semafory.pockej);					//uvolneni semaforu
	sem_destroy(&moje_semafory.kreslo);					//uvolneni semaforu
	sem_destroy(&moje_semafory.spust);					//uvolneni semaforu
	sem_destroy(&moje_semafory.kadernik_pockej);		//uvolneni semaforu
	sem_destroy(&moje_semafory.strihej);				//uvolneni semaforu

	return EXIT_SUCCESS;

}
