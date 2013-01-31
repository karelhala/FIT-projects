////////////////////////////////////////////////////
///////////autor:      Karel Hala  /////////////////
///////////login:       xhalak00   /////////////////
///////////email: xhalak00@stud.fit.vutbr.cz////////
////////////////////////////////////////////////////

/*
 * Program Radius authentication server
 * Zpracovava packety postupne a prohledava je
 * pokud se shoduje heslo s heslem od uzivatele
 * odesle access-accept
 * pokud se neshoduje
 * odesle acccet-rejected
 * 
 * */

#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "udt.h"
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <openssl/md5.h>

using namespace std;

#define MAXLINE 4096 //maximalni velikost radius packetu
#define MAXPACK 65535 //maximalni velikost UDP packetu
#define MAXPASSWDLEN 128 //maximalni velikost hesla
#define AUTHLEN 16 //velikost MD5

//dynamicke pomle uchovavjici nazvy zarizeni
int *udt_iface=NULL;

enum access
{
	DENIED = 0,
	GRANTED,
};

enum tecodes
{
  EOK = 0,     //Bez chyby
  EFILE,	   //chyba souboru
  ENFOUND,	   //chyba v conf file
  ECLWRONG,    //chybny prikazovy radek
  EUNKNOWN,    //Neznama chyba
};

/*
 * Struktura pro uchovani dat z packetu
 * code : kod packetu
 * id : id packetu
 * length : delka celeho packetu
 * auth : autentizacni MD5 hash
 * data : zbyle atributy
 * */
typedef struct radius_packet {
	uint8_t code;
	uint8_t id;
	uint8_t length[2];
	uint8_t auth[AUTHLEN];
	uint8_t data[1];
} Tradius_packet;


/*
 * Struktura na uchovani atributu z packetu
 * jmeno : login uzivatele
 * pwd : MD5 hesla, maximalni delka je 128
 * ident : NAS-identifikator
 * pwd_len : delka hesla
 * */
typedef struct radius_info{
	std::string name;
	int pwd[MAXPASSWDLEN];
	std::string ident;
	int pwd_len;
} Tradius_info;

static Tradius_packet *out_packet=NULL;

/*
 * Funkce na otevreni souboru
 * Vstupni argumenty:
 * file_name: cesta k souboru
 * navrat:
 * vektor obsahujici jednotlive radky ze souboru
 * */
vector<std::string> otevri_soubor(std::string file_name)
{
	std::string tmp; //promenna na ulozeni radku ze souboru
	vector<std::string> loaded; //vektor radku
	ifstream stream_soubor(file_name.c_str()); //otevreni souboru
	//chyba pri cteni souboru
	if (!stream_soubor)
	{
		cerr << "chyba pri cteni souboru"<<endl;
		return (vector<std::string>) EFILE;
	}
	
	//nacteni jednoho radku a ulozeni do vektoru
	while (!stream_soubor.eof())
	{
		getline(stream_soubor,tmp);
		loaded.push_back(tmp);
	}
	return loaded;
}

/*
 * Funkce na vyhledani polozek v configuracnim souboru
 * Vstupni argumenty:
 * loaded : vektor jednotlivych radku
 * looked_up : hledany prvek
 * Vystup:
 * zbytek radku na kterem nasel hledany prvek
 * */
std::string find_database(vector<std::string> loaded,std::string looked_up)
{
	int element; //radek na kterem se nachazi polozka
	
	//cyklus na prochazeni radku
	for (int i=0;i<loaded.size();i++)
	{
		//pokud najdeme vyskut hledaneho elementu ulozime si jeho cislo radku
		if (loaded[i].substr(0,looked_up.length())==looked_up){
			element=i;
			break;
		}
	}
	//nacteme radek a kterem jsme nasli hledany elemnt
	std::string last_line = loaded[element];
	//vratime zbytek radku, bez elementu
	return last_line.substr(looked_up.length());
}

/*
 * Funkce na vyplneni hashovaci tabulky jmen a hesel
 * Vstupni argumenty:
 * file_users : vektor jednotlivych radku souboru databaze
 * Vystup:
 * hashovaci mapa pom[jmeno]=heslo;
 * */
map<string,string> fill_database (vector<std::string> file_users)
{
	map<string,string> database; //hash tabulka
	size_t found;
	//prochazeni po radku databazi
	for (int i=0;i<file_users.size()-1;i++)
	{
		// na radku hledame :
		found = file_users[i].find(":");
		if (found!=string::npos){
			//pokud jiz nenamene uzivatele ulozeneho
			if (database[file_users[i].substr(0,found)].compare("")==0){
				//pokud je alespon nejake heslo a uzivatel na danem radku
				if (file_users[i].substr(found+1).compare("")!=0&&file_users[i].substr(0,found+1).compare("")!=0){
					database[file_users[i].substr(0,found)]=file_users[i].substr(found+1);
				}
			}
		}
	}
	//navrat zashovane databaze
	return database;
}

/*
 * Vytvoreni MD5 hashe na hesle, ktere je v databazi u uzivatele
 * Vstupni argumenty:
 * compare : MD5 co nam dosla jako heslo
 * passwd : heslo co je v databazi
 * vector : autentizace z prichoziho packetu
 * secret : tajne heslo
 * pwd_len : delka prichoziho hesla
 * Vystup:
 * Pristup povolen/odmitnut (GRANTED/DENIED)
 * */
int make_passwd(int* compare,unsigned  char* passwd,const uint8_t* vector,char* secret,int pwd_len)
{
	MD5_CTX context,old; //promenne pro praci s MD5
	uint8_t vysl[AUTHLEN]; //zde bude ulozen vysledny MD5 hash
	uint8_t tmp[MAXPASSWDLEN]={0}; //zde bude ulozeno heslo pro praci
	int inlen = 0,max_pass_len;
	
	//pokud by presahovala delka hesla 128 oriznu toto heslo
	if (strlen((char*)passwd)>MAXPASSWDLEN)
		max_pass_len = MAXPASSWDLEN;
	else
		max_pass_len = strlen((char*)passwd);
	
	//prekopiruju heslo do pole
	memcpy (tmp,passwd,max_pass_len);
	
	//pocet deleni hesla na 16 znakove celky
	inlen= strlen((char*)passwd)/AUTHLEN;
	if ((strlen(secret)%AUTHLEN)>0)
		inlen++;

	//prace s MD5
	MD5_Init(&context);
	MD5_Update(&context,(const void*) secret,strlen(secret)); //nejdrive zahashujeme secret
	//ulozime si context pro pozdejsi pohodlnejsi praci s MD5
	old = context; 
	//zahashujeme authenticator z prichziho packetu
	MD5_Update(&context,(const void*) vector,AUTHLEN);	
	//funkce na postupne hashovani jednotlivych casti hesla
	for (int i=0;i<inlen;i++)
	{
		//Pokud hashujeme heslo vetsi nez 16 postupne zahashujeme jeho 16 znaku
		if (i>0){
			context=old;
			uint8_t pom[AUTHLEN];
			//postupne ulozime znaky do pole o velikosti 16 znaku
			for (int x=0;x<AUTHLEN;x++)
				pom[x]=tmp[((i-1)*AUTHLEN)+x];
			//toto pole pote zahashujeme
			MD5_Update(&context,(const void*) pom,AUTHLEN);
		}
		//vytvorime vysledny hash a zxorujeme jej s ulozenym heslem
		MD5_Final (vysl,&context);
		for (int x=i*AUTHLEN;x<((i*AUTHLEN)+AUTHLEN);x++)
			tmp[x]^=vysl[x-(i*AUTHLEN)];
	}
	
	//pokud se delky hesel neshoduju, rovnou zamitneme pristup
	if ((inlen*AUTHLEN)!=pwd_len) return DENIED;
	
	//pokud se delky shoduji zkontrolujeme zda se hashe shoduji	
	else
	{
		for (int i=0;i<pwd_len;i++)
			if (tmp[i]!=compare[i]) return DENIED;
		//pokud ano, povolime pristup
		return GRANTED;
	}
}

/*
 * Funkce na naplneni informaci o packetu
 * Vstupni argumenty:
 * my_packet : ukazatel na misto do pameti, kde je ulozen packet
 * Vystup:
 * struktura obsahujici data z packetu
 * */
Tradius_info print_packet(Tradius_packet *my_packet)
{
	std::string pom="";
	std::stringstream sstm;
	int val;
	char numstr[9];
	//informace z packetu ulozime sem
	Tradius_info radius;
	radius.name="";
	radius.ident="";
	
	int type;
	int length=0;
	int old_length=0;
	//preskocime prvnich 20 znaku, coz je Code,ID,delka a authenticator
	while (length+20< (my_packet->length[0]+ my_packet->length[1])){
		//ulozime typ atributu
		type = my_packet->data[length];
		//delku tohoto atributu
		old_length = length;	
		length += my_packet->data[length+1];
		//pokud je typ login, nebo NAS-identifier
		if (type == 1 || type == 32){
			//ulozime si hodnotu tohoto atributu
			for (int i=old_length+2;i<length;i++)
				pom+=my_packet->data[i];
			//ulozime si postupne tyto hodnoty do struktury
			if (type == 1)
				radius.name = pom;
			else
				radius.ident = pom;
			pom="";
		}
		//pokud mame typ heslo
		else if (type == 2)
		{
			radius.pwd_len = length-old_length-2;
			//postupne ho znak po znaku ulozime do struktury
			for (int i=old_length+2;i<length;i++)
			{
				radius.pwd[i-old_length-2]=my_packet->data[i];
			}
		}
		//NAS-IP-address
		else if (type == 4)
		{
			int k = old_length+2;
			//prevod integeru do stringu
			for (int i=old_length+2;i<length;i++){
				char numstr[9];
				sprintf(numstr,"%d",my_packet->data[i]);
				radius.ident +=numstr;
				radius.ident +=".";
			}
			//aby byla NAS-IP-address pekna pridavam na konec . posledni tecku odstranim takhle
			radius.ident = radius.ident.substr(0,radius.ident.length()-1);			
		}
		//vsechny ostatni typy ignorujeme
	}
	return radius;
}

/*
 * Funkce na vytvoreni dat v odchozi zprave
 * Vstupni argumenty:
 * res_code : kod odpovedi
 * my_message : zprava
 * Vystup:
 * ukazatel do pameti na data
 * */
uint8_t* create_data(int res_code,char* my_message)
{
	//promenna obsahujici vysledne vygenerovane data
	static uint8_t data[MAXLINE];
	data[0]=18; //zprava ma hodnotu CODE 18 podle RFC
	data[1]=strlen(my_message)+2; //velikost zpravy +2 (Code,delka)
	//zkopirovani zpravy
	for (int i=0;i<strlen(my_message);i++){
		data[i+2]=my_message[i];
	}
	return data;
}

/*
 * Funkce na vytvoreni odchoziho packetu
 * Vstupni argumenty:
 * old_packet :
 * res_code :
 * message :
 * secret :
 * Vystup:
 * Vyplneny packet, pripraveny pro odeslani
 * */
Tradius_packet* create_packet(Tradius_packet *old_packet,uint8_t res_code,std::string message,char* secret)
{
	// autentikator a data
	uint8_t pom[AUTHLEN],*my_data;
	MD5_CTX context; //promenna pro praci s MD5

	//ukazatel na strukturu packetu
	//static Tradius_packet *out_packet=NULL;
	//vytvorime si data, ktera budou v atributech
	my_data = create_data(res_code,(char*)message.c_str());
	
	//vyvtvorime si misto pro ulozeni celeho packetu, vcetne dat
	out_packet =(Tradius_packet*) malloc(sizeof(Tradius_packet)+my_data[1]+1);
	
	//prekopirovani a nastaveni hodnot v novem packetu
	out_packet->code = res_code;
	out_packet->id = old_packet->id;
	out_packet->length[0] = 0;
	out_packet->length[1] = 20+my_data[1];
	memcpy(out_packet->auth,old_packet->auth,AUTHLEN);
	
	//pekopirovani dat
	memcpy(out_packet->data,my_data,my_data[1]);

	//vytvoreni MD5 hashe
	MD5_Init(&context);
	//nejdrive zahashujeme code+id+delka+data
	MD5_Update(&context,(void*) out_packet,out_packet->length[1]);
	//pote pridama heslo
	MD5_Update(&context,(void*) secret,strlen(secret));
	MD5_Final(pom,&context);
	//nakopirovani MD5 hashe do packetu
	memcpy(out_packet->auth,pom,AUTHLEN);

	return out_packet;
}

/*
 * Funkce na vyparsovani nazvu jednotlivych zarizeni
 * Vstupni argumenty:
 * my_interfaces : string obsahujici jmena zarizeni, oddelena carkou
 * Vystup:
 * Vektor stringu obsahujici nazvy jednotlivych zarizeni
 * */
vector<std::string> pars_interfaces(std::string my_interfaces)
{
	//vektor obsahujici jmena zarizeni
	vector<std::string> founded;
	std::string pom;
	
	//postupne prochazi string, znak po znaku
	for (int i=0;i<my_interfaces.size();i++)
	{
		//pokud najdeme razdne misto preskakujeme
		if (isspace(my_interfaces[i]))
			continue;
		//pokud najdeme carku
		else if (my_interfaces.compare(i,1,",")==0){
			//pokud se pred touto carkou nic nenachazi pokracujeme v parsovani
			if (pom.compare("")==0)
				continue;
			//jinak ulozime nactene zarizeni
			founded.push_back(pom);
			pom.clear();
		}
		else
			//postupne ukladame znaky do jmena zarizeni
			pom +=my_interfaces[i];
	}
	//ulozeni posledniho zarizeni
	founded.push_back(pom);
	return founded;
}

/*
 * Funkce na vyparsovani standardniho vstupu
 * Vystup:
 * Vektor stringu obsahujici radky ze standardniho vstupu
 * */
vector<std::string> pars_std_in ()
{
	vector<std::string> whole_file;
	std::string pom;
	int c;
	//cteni do konce souboru
	while ((c=getchar())!=EOF)
	{
		//pokud narazim na novy radek pridam nacteny radek do vektoru
		if (c=='\n')
		{
			whole_file.push_back(pom);
			pom.erase();
		}
		else
			pom+=c;
	}
	
	//pridani posledniho radku do vektoru
	whole_file.push_back(pom);
	
	return whole_file;
}

//funkce osetrujuci sigint
void killed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"Odchycen CTRL+C"<<endl;
	if (udt_iface!=NULL)
		free (udt_iface);
	exit (0);
}

//funkce osetrujici sigterm
void termed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"sigterm"<<endl;
	if (udt_iface!=NULL)
		free (udt_iface);
	exit (0);
}

void segfault(int n)
{
	// vypisu co ukoncilo program
	cerr <<"segfault"<<endl;
	if (udt_iface!=NULL)
		free (udt_iface);
	exit (0);
}
void quited(int n)
{
	// vypisu co ukoncilo program
	cerr <<"quited"<<endl;
	if (udt_iface!=NULL)
		free (udt_iface);
	exit (0);
}

/*
 * Hlavni funkce main
 * Vola vsechny ostatni funkce.
 * */
int main(int argc, char **argv ) {
	
	vector<std::string> loaded,file_users,interfaces;
	std::string last_line,secret,heslo="",file_name,out_msg;
	
	struct sockaddr_in pom;
	int compared,udt,slen=sizeof(pom);
	
	
	char recvline[MAXLINE];
	
	map <string,string> database_names;
	Tradius_packet *my_packet=NULL;
	Tradius_packet *out_pack=NULL;
	Tradius_info radius;
	
	in_port_t local_port;
	
	//pokud mame nacitat ze souboru, otevreme jej a nacteme data do vektoru
	if (argc==3){
		//otevreni a nacteni souboru
		file_name = argv[2];
		loaded = otevri_soubor(file_name);
		if (loaded == (vector<std::string>)EFILE )
			return EFILE;
	}
	//pokud nezadame zadny argument nacitame ze strandardniho vstupu
	else if (argc==1){
		loaded = pars_std_in();
	}
	//jinak vypiseme napovedu
	else{
		cout << "pouziti:\n./radauth\t\t\t  ocekava konfiguracni soubor na \
stdin\n./raduth -c <configuracni_soubor> cesta ke konfiguracnimu \
souboru\n";
		return 1;
	}
	//nalezeni cesty k databazi a nacteni jednotlivych uzivatelu
	file_users = otevri_soubor(find_database(loaded,"userdb="));
	if (loaded == (vector<std::string>)EFILE )
			return EFILE;
			
	//volani funkce na vyplneni databaze
	database_names = fill_database(file_users);
	
	//ulozeni tajneho hesla
	secret=find_database(loaded,"secret=");
	
	//nacteni portu
	local_port = atoi(find_database(loaded,"port=").c_str());
	//nemuzeme bindovat na port 0, tohle take osetruje, pokud jsme nacetli spatny port
	if (local_port==0)
	{
		cerr << "chyba v portu\n";
		return 1;
	}
	
	//nacteni jednotlivych interfacu
	interfaces= pars_interfaces (find_database(loaded,"iface="));
	
	//dynamicka promenna pro uchovani poctu zarizeni
	udt_iface =(int*) malloc(sizeof(int)*interfaces.size());
	
	
	//odchyceni signalu
	signal(SIGTERM,termed);
	signal(SIGINT,killed);
	signal(SIGSEGV,segfault);
	signal(SIGQUIT,quited);
	
	//nabindovani jednotlivych zarizeni na dany port
	for (int i=0;i<interfaces.size();i++)
	{
		udt_iface[i] = udt_init(local_port,(char*)interfaces[i].c_str());
	}

	//pokud nenajdeme zadne zarizeni
	if (interfaces.size() == 0)
		return 1;

	//vybrani popisovace s nejvyssim cislem
	for (int i=0;i<interfaces.size();i++)
	{
		if (udt<udt_iface[i])
			udt=udt_iface[i];
	}
	
	//nastaveni popisovacu pro select
	fd_set readfds;
	FD_ZERO(&readfds);
	for (int i=0;i<interfaces.size();i++)
	{
		FD_SET(udt_iface[i], &readfds);
	}

	//hlavni smycka
	while (1) {
		//vybereme z popisovacu
		select(udt+1, &readfds, NULL, NULL, NULL);
		//projedeme si popisovace, jestli tam neco neni
		for (int i=0;i<interfaces.size();i++){
			//pokud cekaji nejaka data na zpracovani zpracuji je
			if (FD_ISSET(udt_iface[i], &readfds)) {	
				//precteni zpravy
				int n = udt_recv(udt_iface[i], recvline, MAXPACK, NULL, NULL,&pom);
				
				//pokud nactu vetsi packet nez 4096 znaku zahodim jej
				if (n > MAXLINE)
				{
					//nastavim popisovace a jdu opet na select
					FD_ZERO(&readfds);
					for (int i=0;i<interfaces.size();i++)
					{
						FD_SET(udt_iface[i], &readfds);
					}
					continue;
				}
					
				//hratky s ukazateli
				my_packet = (Tradius_packet*) recvline;
				//vytahani informaci z packetu
				radius=print_packet(my_packet);
				//pokud se nejaky povinny parametr nevyskytuje, nastavim compared na DENIED
				if (radius.ident.compare("")==0||radius.name.compare("")==0||radius.pwd_len==0)
					compared = DENIED;
				else
					//vytazeni daneho hesla k uzivateli
					heslo = database_names[radius.name];
				//pokud se nejake heslo naslo vytvorim hash k tomuto heslu
				if (heslo.compare("")!=0){
					compared = make_passwd(radius.pwd,(unsigned  char*)heslo.c_str(),my_packet->auth,(char*)secret.c_str(),radius.pwd_len);
					heslo.clear();
				}
				else{
					//nenasli jsme uzivatele
					cerr << "user was not found!";
					cerr << endl;
					compared = DENIED;
				}
				
				//pokud nam byl pristup povolen vytvorim packet se zpravou access-granted
				if (compared == GRANTED){
					cerr << "access granted";
					out_msg = "Access granted for "+radius.name;
					out_pack=create_packet(my_packet,2,out_msg,(char*)secret.c_str());
				}
				//pokud nam byl pristup povolen vytvorim packet se zpravou access-rejected
				else{
					cerr << "access denied";
					out_msg = "Access denied for "+radius.name;
					out_pack=create_packet(my_packet,3,out_msg,(char*)secret.c_str());
				}
				cerr <<endl;
				//odeslu vytvoreny packet
				if (sendto(udt_iface[i], out_pack, MAXLINE, 0, (struct sockaddr*)&pom, slen)==-1)
					printf("oh oh");
				free (out_pack);
			}
		}
		//nastavim popisovace a jdu opet na select
		FD_ZERO(&readfds);
		for (int i=0;i<interfaces.size();i++)
		{
			FD_SET(udt_iface[i], &readfds);
		}
	}
	return EOK;
}
