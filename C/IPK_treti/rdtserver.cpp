/*
 * Napsal Karel Hala
 * login: xhalak00
 * email: xhalak00@stud.fit.vutbr.cz
 * Neco to udela, ale ne moc dobre. Ale aspon neco. Prosim spoustet prvne server 
 * a potom clienta. Jinak to nefunguje moc dobre.
 * */

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
#include <vector>
#include "base64.h"
#include "udt.h"
#include <signal.h>


using namespace std;
#define MAXLINE 512
#define MAXSIZE 512

in_addr_t remote_addr = 0x7f000001;
in_port_t remote_port = 0;
in_port_t local_port = 0;
fd_set readfds;
int udt;

std::string give_data (std::string packet)
{

	size_t found_begin;
	size_t found_end;
	found_begin=packet.find("<data>");
	std::string pom = packet.substr(found_begin+6);
	found_end=pom.find("</data>");

	return pom.substr(0,found_end);
}

//funkce osetrujuci sigint
void killed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"Odchycen CTRL+C"<<endl;
	// a zavru hlavni socket
	exit (2);
}

//funkce osetrujici sigterm
void termed(int n)
{
	// vypisu co ukoncilo program
	cerr <<"sigterm"<<endl;
	// a zavru hlavni socket
	exit (0);
}

//vratim cislo
std::string give_number(std::string packet)
{
	size_t found,found_end;
	std::string pom;
	found=packet.find("<packet_number>");
	pom= packet.substr(found+15);
	found_end=pom.find("</packet_number>");
	pom=pom.substr(0,found_end);
	return pom;
}
std::string last_nubmer(std::string packet)
{
	size_t found,found_end;
	std::string pom;
	found=packet.find("<last>");
	pom= packet.substr(found+6);
	found_end=pom.find("</last>");
	pom=pom.substr(0,found_end);
	return pom;
}
void print_message(vector<std::string> my_vect)
{
	std::string decoded;
	std::string last_packet;
	vector<std::string> print_vect;
	int last_packet_number;
	for (int i=0;i<my_vect.size();i++)
	{
		/*decoded = base64_decode(give_data(my_vect[i]));
		fwrite(decoded.c_str(),decoded.length(),1,stdout);*/
		if (atoi(last_nubmer(my_vect[i]).c_str())>0)
			last_packet_number=atoi(last_nubmer(my_vect[i]).c_str())-1;
	}
	while (print_vect.size()<last_packet_number){
		for (int i=0;i<my_vect.size();i++)
		{
			if (atoi(give_number(my_vect[i]).c_str())-1==print_vect.size())
				print_vect.push_back(my_vect[i]);
			if (give_number(my_vect[i])=="0")
				last_packet = my_vect[i];
		}
	}
	print_vect.push_back(last_packet);
	for (int i=0;i<print_vect.size();i++)
	{
		decoded = base64_decode(give_data(print_vect[i]));
		fwrite(decoded.c_str(),decoded.length(),1,stdout);
	}
}

int main(int argc, char **argv ) {
	vector<std::string> field_of_packets;
	int packet_number = 0;
	std::string data= "Lorem ipsum dolor sit amaet,";
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
	
	
	
	//cout << xml<<endl;
	//std::string decoded = base64_decode(give_data(xml));
	//cout <<"tohle jsou data:" <<endl<<decoded<<endl;
	char ch;
	char buf[80];
	char sendline[MAXLINE];
	char recvline[MAXLINE];
	
	
	while ((ch = getopt(argc,argv,"s:d:h")) != -1) {
		switch(ch) {
		case 's':
				local_port = atol(optarg);
			break;
		case 'd':
				remote_port = atol(optarg);
			break;
		case 'h':
			fprintf(stdout, "usage: udtdemo -p port -P port [-R address]\n\n");
			fprintf(stdout, "  -s port    : local UDP socket binds to `port'\n" );
			fprintf(stdout, "  -d port    : UDP datagrams will be sent to the remote `port'\n" );
			exit(EXIT_SUCCESS);
		}
	}
	// Complain if something is missing or wrong.
	if (remote_addr == 0 || remote_port == 0 || local_port == 0) {
		fprintf(stderr, "Missing required arguments! Type '-h' for help.\n");
		exit(EXIT_FAILURE);
	}

	local_port, inet_ntop(AF_INET, &remote_addr, buf, 80);


	// It is important to init UDT!
	udt = udt_init(local_port);

	signal(SIGTERM,termed);
	signal(SIGINT,killed);
	
	FD_ZERO(&readfds);
	FD_SET(udt, &readfds);
	
	//prijem zprav
	while (select(udt+1, &readfds, NULL, NULL, NULL)) {
		if (FD_ISSET(udt, &readfds)) {	// We have a new message to print
			int n = udt_recv(udt, recvline, MAXLINE, NULL, NULL);
			recvline[n] = 0;
			std::string str;
			str.assign (recvline);			
			field_of_packets.push_back(str);
			std::string ACK_return;
			//poslani packetu nazpet, ze dosel
			if (!udt_send(udt, remote_addr, remote_port, (void*) str.c_str(), str.length())) {
				perror("udt: ");	// some error
			}
			if ("0"==give_number(str))
			{
				print_message(field_of_packets);		
				fclose (stdout);
			}
		}
		// and again!
		FD_ZERO(&readfds);
		FD_SET(udt, &readfds);
		
	}
	return EXIT_SUCCESS;
}
