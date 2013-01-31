#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define BUFFER 1000*sizeof(char)


//struktura na uchovani dat
typedef struct returnning {
	int delim;
	int port;	//cislo portu
	int size_of_object;	//na jakem indexu se nachazi oddelovac
	int id_server;	//vypis informaci o serveru
	int modi;	//posledni modifikace objektu
	int typ;	//typ objektu
	int err;	//cislo chyby
	int end;	//konecney index adresy
	char address[BUFFER];	//adresa serveru
	char add[BUFFER];		//uchovani adresy, kam se budeme pripojovat
	char content[BUFFER];	//co hledame
	char field[5];
	char headder[BUFFER];	//celkova hlavicka
} Treturn;


//vytahnuti parametru
Treturn params (int argums,char **param)
{
	Treturn navrat;
	int ch;
	navrat.size_of_object = 0;
	navrat.err = 200;
	navrat.id_server = 0;
	navrat.modi = 0;
	navrat.typ = 0;
	int pom=0;
	while ((ch = getopt(argums, param, ":lsmth")) != -1) {
		switch (ch) {
		case 'l':
			//ulozit pouze prvni vyskyt parametru l, dalsi zahodit, ale nevypsat chybu
			if (navrat.size_of_object ==0){
				navrat.size_of_object =1;
				navrat.field[pom]='l';
				pom++;
			}
			else
			{
				break;
			}
			break;
		case 's':
			//ulozit pouze prvni vyskyt parametru s, dalsi zahodit, ale nevypsat chybu
		if (navrat.id_server == 0){
				navrat.id_server = 1;
				navrat.field[pom]='s';
				pom++;
			}
			else
			{
				break;
			}
			break;
		case 'm':
			//ulozit pouze prvni vyskyt parametru m, dalsi zahodit, ale nevypsat chybu
		if (navrat.modi == 0){
				navrat.modi = 1;
				navrat.field[pom]='m';
				pom++;
			}
			else
			{
				break;
			}
			break;
		case 't':		
			//ulozit pouze prvni vyskyt parametru t, dalsi zahodit, ale nevypsat chybu
			if (navrat.typ == 0){
				navrat.typ = 1;
				navrat.field[pom]='t';
				pom++;
			}
			else
			{
				break;
			}	
			break;
		case 'h':
			//vypis napovedy
			if (strlen(navrat.field)==0 && (argums == 2)&&(strlen(param[1])==2))
			{
				navrat.err = 42;
				return navrat;
			}
			else
			{
				navrat.err = 666;
				return navrat;
			}
		default:
			//pokud cokoliv jineho tak je chyba parametru
			navrat.err = 666;
			return navrat;
		 break;
		}
	}
	if ((optind+1)==argums)
	{
		//pokud nam zbylo jeste neco, tak je to adresa, tak ji orezeme
		for (unsigned int x=0;x<strlen(param[optind]);x++)
		{
			if (isspace(param[optind][x]))
				continue;
			navrat.address[x]=param[optind][x];
			navrat.end = strlen(param[optind]);
		}
	}
	else
	{
		//pokud nam nic, ale nezbylo je to chyba
		navrat.err = 666;
		return navrat;
	}
	return navrat;
}
int tiskni (char string[BUFFER],char what[20])
{
	//projdeme hlavicku prvek po prvku
	for (int x=0;x<strlen(string);x++)
	{
		//pokud narazime na to co chceme tisknout
		if ((string[x]==what[0])&&string[x+1]==what[1]&&string[x+strlen(what)-1]==what[strlen(what)-1])
		{
			//vytiskneme to
			while (string[x]!='\n')
			{
				putchar (string[x]);
				x++;
			}
			putchar('\n');
			return 0;
		}
	}
	return 1;
}

//orezani adresy a ziskani iformaci jako je port a co mame hledat
Treturn adresa (char string[BUFFER],Treturn old)
{
	Treturn navrat=old;
	char where[BUFFER];
	char cont[BUFFER];
	cont[0]='/';
	navrat.content[0]='/';
	navrat.delim=0;
	int pom=0;
	navrat.port=80;
	int index;
	char *test;
	test="http://\0";
	//overime zda prvnich 7 znaku je http://
	for (int i=0;i<7;i++)
	{
		if (string[i]!=test[i])
		{
			navrat.err=666;
			return  navrat;
		}
	}
	//projdeme adresu znak po znaku, prvnich 7 znaku preskakujeme, jsou to jenom http://
	for (index=7;index<strlen(string);index++)
	{
		//pokud narazim na : -> ukladam port
		if (string[index]==':')
		{
			index++;
			while (string[index]!='/')
			{
				pom=pom*10+(string[index]-48);
				index++;
				navrat.port=pom;
			}
			if (pom==0)
			{
				navrat.err = 666;
				return navrat;
			}
		}
		
		//pokud narazim na / ukladam index, pro pozdejsi rozdeleni adresy
		if (string[index]=='/')
			break;
	}
	
	// rozdelim adresu
	for (int x=7;x<index;x++)
	{
		if (string[x]==':')
			break;
		where[x-7]=string[x];
	}
	for (int x=index;x<navrat.end;x++)
		cont[x-index]=string[x];
	strcpy(navrat.add,where);
	strcpy(navrat.content,cont);
	
	//vratim co a kde hledam
	return navrat;
}

//vykuchani adresy pri redirectu z hodnoty location: 
Treturn redirect (Treturn old,char string [BUFFER])
{
	Treturn navrat = old;
	char pom[BUFFER];
	char old_add[BUFFER];
	int size;
	//cyjkiti cele hlavicky
	for (int x=0;x<strlen(string);x++)
	{
		
		//pokud najdu retezec Location: tak orezu co se nachazi za nim a to prohlasim za adresu
		if (string[x]=='L'&& string[x+1]=='o'&& string[x+2]=='c'&& string[x+3]=='a'&& string[x+4]=='t'&& string[x+5]=='i'&& string[x+6]=='o'&& string[x+7]=='n')
		{
			//vyrez hlavicky
			for (size=10;size<strlen(string);size++) //10 kvůli Location: <- 10 písmen
			{
				if (string[x+size]=='\n')
					break;
				pom[size-10]=string[x+size]; //10 kvůli Location: <- 10 písmen
			}
			//pokud server odpovi pouze, co relativni adresou, poslu funkci na orezani adresy starou adresu
			if (pom[0]!='h'&&pom[1]!='t'){
				strcpy(old_add,old.add);
				if (pom[0]!=':')
				{
					pom[strlen(old.add)]=':';
					pom[strlen(old.add)+1]=old.port;
				}
				strncat (old_add,pom,strlen(pom));
				navrat = (pom,navrat);
			}else{
				//jinak normalne poslu to co jsem orezal
				navrat.end = size -10; //10 kvůli Location: <- 10 písmen
				navrat = adresa(pom,navrat);
			}
		}
	}
	return navrat;
}

//tisk samostatne hlavick
int header(Treturn ruler,int redir)
{
	int s, n;
	ruler.err = 0;
	struct sockaddr_in sin; struct hostent *hptr;
	char msg[BUFFER]="HEAD ";
	strncat(msg,ruler.content,strlen(ruler.content));
	strcat(msg," HTTP/1.1\r\nHost: ");
	strncat(msg,ruler.add,strlen(ruler.add));
	strcat(msg,"\r\nConnection: close\r\n\r\n");
	int x=0;
	char recieved [BUFFER];

	//vytvoreni socketu
	if ( (s = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
		perror("error on socket\n");  /* socket error */
		return -1; /*  write error */
	}
	
	//nastaveni protokolu, zjisteni IP adresy a preklad
	sin.sin_family = PF_INET;              /*set protocol family to Internet */
	sin.sin_port = htons(ruler.port);  /* set port no. */
	if ( (hptr =  gethostbyname(ruler.add) ) == NULL){
		fprintf(stderr, "gethostname error: %s\n",ruler.add);
		return -1; /*  write error */
	}
	
	//pripojeni k serveru
	memcpy( &sin.sin_addr, hptr->h_addr, hptr->h_length);
	if (connect (s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ){
		perror("error on connect\n"); 
		return -1; /*  write error */
	}
	
	//zaslni dotazu
	if ( write(s, msg, strlen(msg)) < 0 ) {  /* send message to server */
		perror("error on write\n");    
		return -1; /*  write error */
	}
	
	//cteni odpovedi od serveru po znaku
	while ((n = read(s, &msg, sizeof(char))) > 0) 
    {
		if (msg[0]=='\r')
		{
			read(s, &msg, sizeof(char));
			if (msg[0]=='\n')
			{
				strncat(recieved, msg, 1);
				continue;
			}
			else
			{
				strcat(recieved,"\r");
				strncat(recieved, msg, 1);
			}
		}
		// ///////////////////////////
		//citac poctu prectenych znaku
		x++;
		// ///////////////////////////
		strncat(recieved, msg, 1);
		if ((x+1%BUFFER)==0)
		{
			fprintf(stderr, "Hlavicka je prilis velka\n");
			return -1;
		}
		
		//odchyceni cisla chyby
		if (strcmp("HTTP/1.1 ",recieved)==0)
		{
			for (int x=0;x<3;x++)
			{
				n = read(s, msg, sizeof(char));
				strncat(recieved, msg, 1);
				ruler.err = ruler.err*10 + (msg[0]-48);
				
			}
		}
		
    }
    
    //uzavru stream k serveru
    if (close(s) < 0) { 
			perror("error on close");   /* close error */
			return -1; /*  write error */
	}
    //pokud dostaneme odpoved cisla chyby 301, nebo 302 jdeme na redirect
    if (ruler.err==301||ruler.err==302)
    {
		ruler = redirect (ruler,recieved);
		redir++;
		//je povoleno pouze 5 redirectu;
		if (ruler.err != 666)
		{
			if (redir<5)
			{
				return header(ruler,redir);
			}
			else
			{
				fprintf(stderr, "prilis mnoho presmerovani\n");
				return 666;
			}
		}else{
			fprintf(stderr, "chyba v adrese\n");
			return 666;
		}
	}
	
	//pokud prijde odpoved ze serveru jinatsi nez 200 coz je EOK vytisknu cislo chyby na STDERR 
	else if (ruler.err != 200)
	{
		
		fprintf(stderr, "chyba c.: %d\n",ruler.err);
		return ruler.err;
	}
	else
	{
		
		//zajisteni postupneho tisteni hlavicky podle parametru
		for (int x=0;x<strlen(ruler.field);x++)
		{
			if (ruler.field[x]=='l')
			{
				if ((tiskni (recieved,"Content-Length:"))==1)
				{
					printf("Content-Length: N/A\n");
				}
			}
			else if (ruler.field[x]=='s')
			{
				if ((tiskni (recieved,"Server:"))==1)
				{
					printf("Server: N/A\n");
				}
			}
			else if ( ruler.field[x]=='m')
			{
				if ((tiskni (recieved,"Last-Modofied:"))==1) 
				{
					printf("Last-Modified: N/A\n");
				}
			}
			else if ( ruler.field[x]=='t')
			{
				if ((tiskni (recieved,"Content-Type:"))==1)
				{
					printf("Content-Type: N/A\n");
				}
			}
		}
		
		//jinak vytiskneme celou hlavicku
		if (strlen(ruler.field)==0)
		{
			for (int x=0;x<strlen(recieved)-1;x++)
			{
				putchar(recieved[x]);
			}
		}
	}
	return 0;
}


int main (int argc, char **argv) {
	
	
	Treturn navrat;
	
	//upraveni argumentu
	navrat = params(argc,argv);
	char content[BUFFER],add[BUFFER];
	char string[BUFFER];
	unsigned int index;
	
	//pokud jsem dostal z argumentu 666 je chyba v argumentech
	if (navrat.err == 666)
	{
		fprintf(stderr, "Oh! chyba v argumentu. Chcete vypsat napovedu? zadejte prepinac -h.\n");
		return -1;
	}
	
	//pokud jsem dostal z argumentu 42 jdeme tisknout hlavicku
	else if (navrat.err == 42)
	{
		printf("------------------------------------------------------------------------\n\
	Napoveda pro program webinfo do predmetu IPK\n\
------------------------------------------------------------------------\n\
	prepinac -l: vypise velikost objektu\n\
	prepinac -s: vypise informace o serveru\n\
	prepinac -m: vypise iformace o posledni modifikaci objektu\n\
	prepinac -t: vypise typ obsahu objektu\n\
------------------------------------------------------------------------\n\
prepinace jsou kombinovatelne, az na prepinac -h, ten musi byt samostatny\n\
------------------------------------------------------------------------\n\
	samostatny prepinac -h: vypise tuto napovedu\n\
------------------------------------------------------------------------\n");
		return 0;
	}
	
	//jinak je vse v poradku a jdeme upravit adresu
	else
		navrat = adresa(navrat.address,navrat);
		
	//pokud dostaneme z upravy adresy 666 je v adrese chyba
	if (navrat.err != 666)
		return header(navrat,0);
	else
	{
		fprintf(stderr, "chyba v adrese, na zacatku musi byt http://\n");
	}
}
