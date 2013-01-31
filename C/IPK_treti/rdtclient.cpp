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
#include <sys/time.h>
#include <signal.h>


using namespace std;
#define MAXLINE 512
#define MAXSIZE 512
#define WINDOWSIZE 10
int udt;

/*
 * Globalni promenne. Promin IZP ja musel!
 * */
in_addr_t remote_addr = 0x7f000001;
in_port_t remote_port = 0;
in_port_t local_port = 0;
struct itimerval tout_val;
vector<std::string> sent;
vector<std::string> ACK;
std::string last="";

//Vytvoreni XML hlavicky.
std::string my_xml(int packet_number){
	std::ostringstream oss;
	std::ostringstream sour;
	std::ostringstream destin;
	oss << packet_number;
	std::string packet = "";
	packet.append("<rdt-segment id='xhalak00'><header>");
	packet.append("<packet_number>");
	packet.append(oss.str());
	packet.append("</packet_number>");
	packet.append("<last>");
	packet.append("</last>");
	packet.append("</header><data>"); 
	packet.append("</data></rdt-segment>");
	return packet;
}

//Pridame zakodovana data do packetu
std::string add_data(std::string data,std::string packet)
{
	size_t found;
	found=packet.find("</data>");
	if (found!=string::npos)
	{
		packet.insert(found,data);
	}
	return packet;
}

//vytazeni dat z packetu
std::string give_data (std::string packet)
{

	size_t found_begin;
	size_t found_end;
	found_begin=packet.find("<data>");
	std::string pom = packet.substr(found_begin+6);
	found_end=pom.find("</data>");

	return pom.substr(0,found_end);
}

//takhle dostaneme cislo packetu
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

//posledni oznackujeme nastavime jeho cislo na 0 a pridme cislo posledniho packetu
std::string mark_last(std::string packet)
{	
	size_t found,found_end;
	std::string pom;
	pom = give_number(packet);
	found_end=packet.find("<last>");
	packet.insert(found_end+6,pom);
	found=packet.find("<packet_number>");
	packet.replace(found+15,pom.length(),"0");	
	return packet;
}

//funkce na cteni dat z STDIN
vector<std::string> read_data(int count,int packet_number)
{
	vector<std::string> my_vect;
	std::string xml="";
	std::string encoded;
	std::string my_data;
	size_t read_bytes;
	char chr[1];
	
	//nacitam, dokud nenactu nulty bajt
	while (read_bytes!=0){		
		xml= my_xml(packet_number);
		while (1) {
			
			//nacitam dokud muzu
			read_bytes=fread(chr,1,1,stdin);
			if ((encoded.length()+xml.length()+2) > (MAXLINE-8)) {
				my_data+=chr[0];
				break;}
				
				//kdyz jsem nacetl konec souboru, oznackuju si ho
			else if (read_bytes==0){
				xml=mark_last(xml);
				break;
			}
			my_data+=chr[0];
			encoded = base64_encode(reinterpret_cast<const unsigned char*>(my_data.c_str()), my_data.length());	
		}
		
		//zakoduju co jsem nacetl
		encoded = base64_encode(reinterpret_cast<const unsigned char*>(my_data.c_str()), my_data.length());		
		xml = add_data(encoded,xml);
		packet_number++;
		if (read_bytes==0)
		{
			last=xml;
			break;
		}
		my_vect.push_back(xml);
		my_data="";
		encoded="";
		sent.push_back(xml);
		count--;
		if (count==0)
			break;
	}
	return my_vect;
}
void send_this (vector<std::string> my_vect)
{
	std::string xml;
	for (int x = 0;x<my_vect.size();x++){
		xml=my_vect[x];

		if (!udt_send(udt, remote_addr, remote_port, (void*) xml.c_str(), xml.length())) {
			perror("udt: ");	// some error
		}
	}
}
void check_ACK()
{
	vector<std::string> my_vect;
	
	for (int x=0;x<sent.size();x++)
	{		
		int pom=0;
		for (int i=0;i<ACK.size();i++)
		{
			if (give_number(ACK[i])==give_number(sent[x])){
				pom=1;
			}
		}
		if (pom==0){
			my_vect.push_back(sent[x]);
		}
		if (my_vect.size()==WINDOWSIZE)
			send_this(my_vect);
	}
	if (my_vect.size()!=0)
		send_this(my_vect);
}
void alarm_wakeup (int i)
{
	signal(SIGALRM,alarm_wakeup);
	check_ACK();
	tout_val.it_interval.tv_sec = 0;
	tout_val.it_interval.tv_usec = 0;
	tout_val.it_value.tv_sec = 2;
	tout_val.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &tout_val,0);
}
int main(int argc, char **argv ) {
	
	int packet_number = 1;
	std::string xml;
	std::string encoded;
	std::string my_data;
	char chr[1];
	int delim=1;
	//cout << xml<<endl;
	//std::string decoded = base64_decode(give_data(xml));
	//cout <<"tohle jsou data:" <<endl<<decoded<<endl;
	char ch;
	char buf[80];
	char sendline[MAXLINE];
	char recvline[MAXLINE];
	int nomore=1;
	
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
	tout_val.it_interval.tv_sec = 0;
	tout_val.it_interval.tv_usec = 0;
	tout_val.it_value.tv_sec = 2;
	tout_val.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &tout_val,0);
	signal(SIGALRM,alarm_wakeup); /* set the Alarm signal capture */
	inet_ntop(AF_INET, &remote_addr, buf, 80);
	signal(SIGALRM,alarm_wakeup); 
	// It is important to init UDT!
	udt = udt_init(local_port);

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);	// make stdin reading non-clocking

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(udt, &readfds);
	FD_SET(STDIN_FILENO, &readfds);
	while (select(udt+1, &readfds, NULL, NULL, NULL)) {
		if (FD_ISSET(STDIN_FILENO, &readfds)&&nomore!=0) { // we have read a new line to send
		vector<std::string> field_of_packets;
			
			field_of_packets=read_data(WINDOWSIZE,packet_number);
			packet_number+=field_of_packets.size();
			xml = sent[sent.size()-1];
			send_this(field_of_packets);
			if (last!="")
				nomore=0;

		}
		
		//prijimani zprav
		if (FD_ISSET(udt, &readfds)&&delim!=0) {	// We have a new message to print
			int n = udt_recv(udt, recvline, MAXLINE, NULL, NULL);
			recvline[n] = 0;
			std::string str;
			str.assign (recvline);
			std::string decoded = base64_decode(give_data(str));
			ACK.push_back(str);
			
			//poslani posledniho packetu
			if (sent.size()==ACK.size())
			{
				if (!udt_send(udt, remote_addr, remote_port, (void*) last.c_str(), last.length())) {
					perror("udt: ");	// some error
				}
				exit (0);
			}
		}
		// and again!
		FD_ZERO(&readfds);
		FD_SET(udt, &readfds);
		FD_SET(STDIN_FILENO, &readfds);
	}
	return EXIT_SUCCESS;
}
