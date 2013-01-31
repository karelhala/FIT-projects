#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


#define BUFSIZE 1000

using namespace std;
//struktura ve ktere drzim data z prikazove radky
typedef struct returnning {
	char which[2];			//pole na uchovani posloupnosti protokolu
	std::string address;	//prekladana adresa
	std::string server;		//server, ktery bude slouzit pro preklad
	int how_many;			//pocet protokolu v parametrech
	int port;				//port na kterem pracuji
	int err;				//cislo chyby
} Treturn;

//funkce na parsovani argumentu
Treturn params (int argums,char **param)
{
	Treturn navrat;
	int ch=0,maximum=0;
	navrat.how_many =0;
	size_t found;
	navrat.err=0;
	
	//prubezne cyklim v argumentech
	for (int x=2;x<argums;x++)
	{
		maximum =x;
		//pokud narazim na pomlcku
		if (param[x][0]=='-')
		{
			//priojedu cely parametr
			for (unsigned int index=1;index<strlen(param[x]);index++)
			{
				//pokud nactu jine cislo, nez 4, nebo 6 koncim cyklus a nastavuji chybu
				if (param[x][index]!='4'&&param[x][index]!='6')
				{
					navrat.err = 666;
					break;
				}
				//a ulozim cislo protokolu (podporovane jsou 4, nebo 6)
				if (ch<2)
				{
					navrat.which[ch]=param[x][index];
					ch++;
					navrat.how_many=ch;
				}
			}
		}
	}
	
	//pokud byli argumenty v poradku
	if (argums==(maximum+1)&&navrat.err==0&&navrat.how_many>0)
	{
		navrat.server = param[1];									//ulozim server
		navrat.address = param[maximum];							//prekladanou adresu
		found=navrat.server.find(':');								//pokusim se najit port
		if (found==string::npos){
			cerr <<"hybejici port\n";
			navrat.err = 666;
			return navrat;
		}
		
		navrat.port = atoi(navrat.server.substr (found+1).c_str());//ulozim port
		navrat.server = navrat.server.substr (0,found);				//ulozim server, ktery slouzi pro preklad
	}
	else
	{
		navrat.err = 666;
		return navrat;
	}
	return navrat;
}
// Funkce pro vypis stringu, ktery prijal client.
// tikne na STDERR, nebo na STDOUT
int vypis (std::string text)
{

	size_t found;
	size_t found_endl;
	//Pokud se vyskytla nejaka chyba pri prekladu adresy zde to overuji
	//po pripade tisknu na STDERR
	found=text.find("Err");
	if (found!=string::npos)
	{
		found_endl=text.substr(found+1).find("\n");
		cerr<<"E"<<text.substr(found+1,found_endl)<<endl;
		text.replace(found,found_endl+2,"");
	}
	//Pokud se v navratove zprave nachazi 2 chyby musim vytisknout take druhou chybu na STDERR
	found=text.find("Err");
	if (found!=string::npos)
	{
		found_endl=text.substr(found+1).find("\n");
		cerr<<"E"<<text.substr(found+1,found_endl)<<endl;
		text.replace(found,found_endl+2,"");
	}else
		cout <<text;	//pokud neco zbylo ve zprave tisknu jeji obsah na STDOUT
	return 0;
}
int main(int argc, char *argv[])
{
    std::string text;
    Treturn info; //struktura pro uchovani informaci o serveru, adrese, portu a protokolech

    info = params(argc,argv);
    if (info.err==666)
    {
		cerr << "chyba v argumentech";
		return 1;
	}
    text = info.address;
    text+= "\n";
    //text = "svetlatkovychplen.cz:6";
    hostent *host;              // Vzdaleny pocitac
    sockaddr_in serverSock;     // Vzdaleny "konec potrubi"
    int mySocket;               // Soket
    char buf[BUFSIZE];          // Prijimaci buffer
    int size;                   // Pocet prijatych a odeslanych bytu
    for (int x=0;x<info.how_many;x++)
    {
		text += info.which[x];
		text += "\n";
	}
    // Zjistime info o vzdalenem pocitaci
    if ((host = gethostbyname(info.server.c_str())) == NULL)
    {
        cerr << "Spatna adresa" << endl;
        return -1;
    }
    // Vytvorime soket
    if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        cerr << "Nelze vytvorit soket" << endl;
        return -1;
    }
    // Zaplnime strukturu sockaddr_in
    // 1) Rodina protokolu
    serverSock.sin_family = AF_INET;
    // 2) Cislo portu, ke kteremu se pripojime
    serverSock.sin_port = htons(info.port);
    // 3) Nastaveni IP adresy, ke ktere se pripojime
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
    // Pripojeni soketu
    if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
    {
        cerr << "Nelze navazat spojeni" << endl;
        return -1;
    }
    // Odeslani dat
    if ((size = send(mySocket, text.c_str(), text.size() + 1, 0)) == -1)
    {
        cerr << "Problem s odeslenim dat" << endl;
        return -1;
    }
    // Prijem dat
    text = "";
    while (((size = recv(mySocket, buf, BUFSIZE +1, 0)) != -1) && (size != 0))
    {
        text += buf;
    }
    // Uzavru spojení
    close(mySocket);

	vypis(text);
    return 0;
}
