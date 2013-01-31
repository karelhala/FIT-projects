#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>


#define BUFSIZE 1000

using namespace std;

//struktura obsahujuci prelozene adresy
typedef struct addresses {
	std::string first;
	std::string second;
} Taddres;

//struktura obsahujici prijatou zpravu
typedef struct ports{
	int first;
	int second;
	std::string address;
}Tports;


//globalni promnenna
int mainSocket;               // Soket
//pro sigint a sigterm

// funkce pro peklad adresy
std::string lookup_host (const char *host, int type)
{
	struct addrinfo hints, *res;
	int errcode;
	char addrstr[255];
	void *ptr=NULL;
	memset (&hints, 0, sizeof (hints));
	std::string pom="Err";
	//nastaveni struktury
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	errcode = getaddrinfo (host, NULL, &hints, &res);
	
	//pokud se nam nepovede preklad adresy
	if (errcode != 0)
	{
		char numstr[21];
		std::string pom="Err";
		sprintf(numstr, "%d", type);
		pom+=numstr;
		pom+=": Nenalezeno.";
		return pom;
	}
	while (res)
	{
		inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 255);
		//pokud chceme IPv6 a nacetli jsme IPv6
		if ((res->ai_family==AF_INET6)&&type==6)
		{
			//vyextrahujeme adresu
			ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
			inet_ntop (res->ai_family, ptr, addrstr, 255);
			//a vratime ji
			return string(addrstr);
		}
		//pokud chceme IPv4 a nacetli jsme IPv4
		else if ((res->ai_family==AF_INET)&&type==4)
		{
			//vyextrahujeme adresu
			ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
			inet_ntop (res->ai_family, ptr, addrstr, 255);
			//a vratime ji
			return string(addrstr);
		}
		res = res->ai_next;
	}
	//pokud se nam nepodari najit hledanou adresu vratime error ptokolu nenalezeno
	char numstr[21];
	sprintf(numstr, "%d", type);
	pom+=numstr;
	pom+=": Nenalezeno.";
	return pom;
}

//funkce osetrujici zombie procesy
void SigCatcher(int n)
{
  wait3(NULL,WNOHANG,NULL);
}

//funkce osetrujuci sigint
void killed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"Odchycen CTRL+C"<<endl;
	// a zavru hlavni socket
	close(mainSocket);
	exit (2);
}

//funkce osetrujici sigterm
void termed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"sigterm"<<endl;
	// a zavru hlavni socket
	close(mainSocket);
	exit (0);
}

//funkce na parsovani zprav
Tports pars_text(std::string incom)
{
	Tports navrat;
	std::string pom;
	navrat.address = "ERR";
	navrat.first = 0;
	navrat.second = 0;
	size_t found;
	//pokusim se najit prvni \n
	found=incom.find("\n");
	
	//pokud jsem ho nasel je to adresa
	if (found!=string::npos)
	{
		navrat.address = incom.substr(0,found);
		pom = incom.substr(found+1);
	}
	//pokusim se najit druhe \n pokud jsem ho nasel, je to prvni protokol
	found=pom.find("\n");
	if (found!=string::npos)
	{
		navrat.first = atoi (pom.substr(0,found).c_str());
		pom = pom.substr(found+1);
	}
	//pokusim se najit treti \n pokud jsem ho nasel, je to druhy protokol
	found=pom.find("\n");
	if (found!=string::npos)
	{
		navrat.second = atoi (pom.substr(0,found).c_str());
	}
	
	return navrat;
}

//server zvladne pouize jeden parametr -p
int proces_params(int argc, char**argv)
{
	if (argv[1][1]=='p'&&argc==3)
		return 0;
	else
		return 1;
}


int main(int argc, char *argv[])
{
	if (proces_params(argc,argv)==1)
	{
		cerr <<"chyba argumentu"<<endl;
		return 1;
	}
	std::string text;             // Prijimany text
	sockaddr_in sockName;         // "Jmeno" portu
	sockaddr_in clientInfo;       // Klient, ktery se pripojil 
	int port;                     // Cislo portu
	char buf[BUFSIZE];            // Prijimaci buffer
	int size;                     // Pocet prijatych a odeslanych bytu
	socklen_t addrlen;            // Velikost adresy vzdaleneho pocitace
	Tports version;
	Taddres addr;
	pid_t server = 0;

	//funkce na odchytani jednotlivych signalu
	signal(SIGCHLD,SigCatcher);
	signal(SIGTERM,termed);
	signal(SIGINT,killed);
	

	port = atoi(argv[argc-1]);
	// Vytvorime soket
	if ((mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Nelze vytvorit soket" << endl;
		return -1;
	}
	// Zaplnime strukturu sockaddr_in
	// 1) Rodina protokolu
	sockName.sin_family = AF_INET;
	// 2) Cislo portu, na kterem cekame
	sockName.sin_port = htons(port);
	// 3) Nastaveni IP adresy lokalni sitove karty, pres kterou je mozno se
	//    pripojit. Nastavime moznost pripojit se odkudkoliv. 
	sockName.sin_addr.s_addr = INADDR_ANY;
	// priradime soketu jmeno
	if (bind(mainSocket, (sockaddr *)&sockName, sizeof(sockName)) == -1)
	{
		cerr << "Problem s pojmenovanim soketu." << endl;
		return -1;
	}
	//naslouchani na zpravy
	if (listen(mainSocket, 10) == -1)
	{
		cerr << "Problem s nastavenim odposlouchani na portu" << endl;
		return -1;
	}
	addrlen = sizeof(clientInfo);
	while (1){

		//vyberu clienta, co chce preklad zpravy
		int client = accept(mainSocket, (sockaddr*)&clientInfo, &addrlen);

		//////////////////////
		//tady forknout
		//////////////////////
		server = fork();
		if (server>0)
		{
			//rodicovsky proces, tak zavru aktualniho clienta
			close (client);
			continue;
		}
		
		//potomek
		else if (server == 0)
		{
			//pokud nastal chyba s prijetim clienta
			if (client == -1)
			{
				cerr << "Err: Problem s prijetim spojeni" <<endl;
				return -1;
			}
						
			// Prijmu data. Ke komunikaci s klientem pouzivam soket "client"
			text = "";

			if ((size = recv(client, buf, BUFSIZE-1, 0)) == -1)
			{
				cerr << "Err: Problem s prijetim dat" << endl;
				return -1;
			}
			text += buf;
			
			//rozparsuji zpravu
			version = pars_text (text);
			text = "";
			
			//pokud client poslal platny protokol, prelozim adresu
			if (version.first==4||version.first==6)
			{
				addr.first = lookup_host (version.address.c_str(),version.first);
				text += addr.first;
				text += "\n";
			}
			//pokud chce, take druhy protokol, prelozim adresu a ulozim co jsem nacetl
			if (version.second==4||version.second==6)
			{
				addr.second = lookup_host (version.address.c_str(),version.second);
				text += addr.second;
				text += "\n";
			}
			//pokud, ale chce neplatny (jiny, nez 4,6) poslu mu chybu protokolu
			if (version.first!=4&&version.first!=6)
			{
				text = "Err: chyba protokolu!";
			}	
			
			//poslani dat clientovy
			if ((size = send(client, text.c_str(), text.size()+1, 0)) == -1)
			{
				cerr << "Err: Problem s odeslenim dat" << endl;
				return -1;
			}
			// Uzavru spojeni s klientem
			close(mainSocket);
			close(client);
			//konec forku
			exit (0);
		}
		//pokud se nepovedlo forknuti
		else if (server < 0)
		{
			cerr <<"Err: Problem s forknutim";
			return -1;
		}
	}
	//zavru hlavni socket
	close(mainSocket);
	return 0;
}
