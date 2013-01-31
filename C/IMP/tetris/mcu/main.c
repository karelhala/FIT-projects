//////////////////////////////////////////////////
//////////   Jmeno: Karel Hala               /////
//////////   Login: xhalak00                 /////
//////////   Cast kodu prevzata z:           /////
//////////		keyboard_lcd 5%              /////
////////// pouze promenne a inicializace HW  /////
//////////        autor: Zdeněk Vašíček      /////
///////////            vasicek@fit.vutbr.cz  /////
//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

//promenne pro nactene znaky z klavesnice
char last_ch; //naposledy precteny znak
char char_cnt;

//struktura pro utvar
typedef struct {
	int shape[4][4];
} Tshape;

//struktura pro hraci pole
typedef struct {
	int shape[8][8];
	int count; //pocet ulozenych znaku z tvaru
	int last_line; //jsme na poslednim radku?
} Tmatrix;

//globalni promenne x, y a nahrane skore
int x=0,y=0,score=0;

//funkce pro posun utvaru pozaduje utvar a hraci pole
Tmatrix move_it(Tshape myshape,int pane[8][8])
{
	Tmatrix matrix;
	matrix.count = 0;
	matrix.last_line=0;
	int i=0,t=0;
	//vynulovani pomocne matice
	for (i=0;i<8;i++)
		for (t=0;t<8;t++)
			matrix.shape[i][t]=0;
	
	//2 cykli pro ukladani do hraciho pole
	for (i=0;i<4;i++)
	{
		for (t=0;t<4;t++)
			//pokud nalezneme pri prochazeni utvaru nejaky prvek
			if (myshape.shape[i][t]==1){
				//kontroluji zda muzeme vykreslit dany utvar
				if ((y+i<8 && y+i>=0) && (x+t<8 && x+t>=0)){
					if (pane[i+y][t+x]==0){
						matrix.shape[i+y][t+x]=myshape.shape[i][t];
						matrix.count++;
					}
					//pokud nacteme posledni radek
					if ((y+i)==7){
						matrix.last_line=1;
					}
				}
			}
	}
	return matrix;
}
//funkce pro tisknuti
int print_it(int matrix[8][8])
{
	
	int i=0,t=0,row=0,pow=1;
	//2 cykli pro jednotlive vypisovani
	for (i=0;i<8;i++)
	{
		//prevod radku matice na desitkove cislo
		for (t=7;t>=0;t--)
		{
			row+=matrix[i][t]*pow;
			pow*=2;
		}
		//tisk prvku na radku
		P4OUT=P2OUT=row;
		row=0;
		pow=1;
		//select radku
		switch (i)
		{
			case 0 : P6OUT = 0xfe; 
				break;
			case 1 : P6OUT = 0xfd; 
				break;
			case 2 : P6OUT = 0xfb; 
				break;
			case 3 : P6OUT = 0xf7; 
				break;
			case 4 : P6OUT = 0xef; 
				break;
			case 5 : P6OUT = 0xdf; 
				break;
			case 6 : P6OUT = 0xbf; 
				break;
			case 7 : P6OUT = 0x7f; 
				break;
		}
	}
	return 0;
}

//funkce pro vytvareni utvaru
Tshape create_shape(int index)
{
	Tshape myshape;
	int i,x;
	//nulovani matice
	for (i=0;i<4;i++)
	{
		for (x=0;x<4;x++)
			myshape.shape[i][x] = 0;
	}
	
	//jednotlive utvary
	if (index==0)
	{
		myshape.shape[1][0] = 1;
		myshape.shape[1][1] = 1;
		myshape.shape[1][2] = 1;
		myshape.shape[1][3] = 1;
	}
	else if (index == 1)
	{
		myshape.shape[1][1] = 1;
		myshape.shape[1][2] = 1;
		myshape.shape[2][2] = 1;
		myshape.shape[2][3] = 1;
	}
	else if (index==2)
	{
		myshape.shape[0][2] = 1;
		myshape.shape[1][2] = 1;
		myshape.shape[2][2] = 1;
		myshape.shape[2][1] = 1;
	}
	else if (index==3){
		myshape.shape[0][1] = 1;
		myshape.shape[1][1] = 1;
		myshape.shape[2][1] = 1;
		myshape.shape[2][2] = 1;
	}
	else if (index == 4)
	{
		myshape.shape[1][1] = 1;
		myshape.shape[1][2] = 1;
		myshape.shape[2][1] = 1;
		myshape.shape[2][2] = 1;
	}
	else if (index == 5)
	{
		myshape.shape[1][2] = 1;
		myshape.shape[2][1] = 1;
		myshape.shape[2][2] = 1;
		myshape.shape[2][3] = 1;
	}
	else if (index == 6)
	{
		myshape.shape[1][2] = 1;
		myshape.shape[1][3] = 1;
		myshape.shape[2][1] = 1;
		myshape.shape[2][2] = 1;
	}
	return myshape;
}

//funkce na ulozeni utvaru do vysledne matice
Tmatrix save_it(Tmatrix matrix,int pane[8][8])
{
	int i,t;
	Tmatrix pom_matrix;
	//nulovani pomocne matice
	for (i=0;i<8;i++)
	{
		for (t=0;t<8;t++)
			pom_matrix.shape[i][t] = 0;
	}
	
	//ukladani utvaru do vysledne matice
	for (i=0;i<8;i++)
	{
		for (t=0;t<8;t++)
			if (matrix.shape[i][t]==1)
				pom_matrix.shape[i][t] = matrix.shape[i][t];
			else
				pom_matrix.shape[i][t] = pane[i][t];
				
	}
	return pom_matrix;
}
//funkce na rotaci zpatky
Tshape rotate_back (Tshape myshape)
{
	int i,t;
	Tshape rotated;
	for (i=0;i<4;i++)
	{
		//rotujeme radky a sloupce
		for (t=0;t<4;t++)
		{
			rotated.shape[i][4-1-t]=myshape.shape[t][i];
		}
	}
	return rotated;
}
//funkce na rotaci
Tshape rotate (Tshape myshape)
{
	int i,t;
	Tshape rotated;
	for (i=0;i<4;i++)
	{
		//rotujeme radky a sloupce
		for (t=0;t<4;t++)
		{
			rotated.shape[4-1-i][t]=myshape.shape[t][i];
		}
	}
	return rotated;
}
//funkce na mazani radku
Tmatrix delete_rows(Tmatrix matrix)
{
	int i,j,count=0,k;
	for (i=0;i<8;i++){
		//pocitam pocet jednicek na radku
		for (j=0;j<8;j++)
		{
			if (matrix.shape[i][j]==1)
				count++;
		}
		//pokud nactu 8 prvku na radku
		if (count==8)
		{
			//projedu vysledne pole odshoda dolu
			for (k=i;k>0;k--)
			{
				//posunu cely radek dolu
				for (j=0;j<8;j++)
				{
					matrix.shape[k][j] = matrix.shape[k-1][j];
				}
			}
			score++;
			//printf("%d\n",score);
		}
		count = 0;
	}
	return matrix;
}


/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
  char ch;
  int random_num;
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch) 
  {
    last_ch = ch;
    if (ch != 0) 
    {
      char_cnt++;
      
    }
  }
  return 0;
}


	
/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
  LCD_init();
  LCD_clear();
}

//vypis score/game over na LCD
void print_score(char message[50])
{
	char pom_score[50];
	LCD_clear();
	LCD_append_string(message);
	sprintf(pom_score,"%d",score);
	LCD_append_string(pom_score);
	
}
//hlavni funkce starajici se o chod aplikace
int main(int argc, char **argv )
{
	unsigned int cnt = 0,old_score=1;
	char_cnt = 0;
	last_ch = 0;
	//iniciaizace HW
	initialize_hardware();
	keyboard_init();
	
	int random_number,i,j;
	Tshape myshape;
	Tmatrix matrix,pane,pom;
	int pom_matrix[8][8] = {0};
	char c=0;
	int latest_line;

	//srand(2);
	//vytvoreni noveho utvaru
	myshape = create_shape(rand()%8);
	//posun na zacatek
	matrix = move_it(myshape,pom_matrix);
	//ulozeni noveho utvaru
	pane = save_it(matrix,pom_matrix);


	set_led_d6(1);                       // rozsviceni D6
	set_led_d5(1);                       // rozsviceni D5

	//inicializace LED displeje
	P6DIR=0xff;
	P4DIR=0x0f;
	P2DIR=0xf0;
	P4OUT=0x00;
	P2OUT=0x00;
	P6OUT=0xff;
	while (1)
	{
		delay_ms(1);
		cnt++;
		//pri zmacknuti klavesy
		if (c!=last_ch){
			c=last_ch;
			//pokud byla zmacknuta 6
			if (c=='6'){
				//posunu se
				x++;
				//pokusim se ulozit na to misto utvar
				matrix = move_it(myshape,pom_matrix);
				//pokud nelze ulozit	
				if (matrix.count!=4){
					//vratim se
					x--;
					//a ulozim
					matrix = move_it(myshape,pom_matrix);				
				}
			}
			//pri zmacknuti 4  
			else if (c=='4'){
				//posunu se
				x--;
				//pokusim se ulozit
				matrix = move_it(myshape,pom_matrix);
				//pokud bych vypsal mene jak 4 znaky z utvaru
				if (matrix.count!=4){
					//posouvam se zpatky
					x++;
					matrix = move_it(myshape,pom_matrix);
				}
			}
			//pokud nactu 8
			else if (c=='8'){
				//posunu se
				y++;
				//pokusim se ulozit
				matrix = move_it(myshape,pom_matrix);
				if (matrix.count!=4){
					//pokud nelze posun, posunu se nazpet
					y--;
					matrix = move_it(myshape,pom_matrix);
					matrix.last_line = 1;
				}			
			}
			//rotace
			else if (c=='5'){
				//pouzivam prvne rotate_back kvuli "hezci" rotaci
				myshape = rotate_back(myshape);
				//pokusim se ulozit dany utvar
				matrix = move_it(myshape,pom_matrix);
				//pokud pri posunu nelze vypsat vsechny 4 prvky daneho utvaru
				if (matrix.count!=4){
					//posunu se zpatky
					myshape = rotate(myshape);
					matrix = move_it(myshape,pom_matrix);
				}		
			}
			else
			{
				continue;
			}
			//ulozim do vysledneho hraciho pole
			pane = save_it(matrix,pom_matrix);
			//pokud jsem nacetl posledni radek
			if (matrix.last_line==1)
			{
				//ulozim si hraci pole do pomocne matice
				for (i=0;i<8;i++)
				{
					for (j=0;j<8;j++)
					{
						pom_matrix[i][j] = pane.shape[i][j];
					}
				}
				matrix.last_line=0;
				//smazu radky
				pane=delete_rows(pane);

				x=0;
				y=0;
				//vygeneruji novy utvar
				myshape = create_shape(rand()%8);
				int i,t;
				//ulozim do pomocne matice hraci pole
				for (i=0;i<8;i++)
					for (t=0;t<8;t++)
						pom_matrix[i][t]=pane.shape[i][t];
				//posunu nove vygenerovany utvar na zacatek hraciho pole
				matrix = move_it(myshape,pom_matrix);
				//ulozim tento stav
				pane = save_it(matrix,pom_matrix);
				
			}
			c=last_ch;
		}
		//tisknuti skore
		if (old_score!=score && (cnt%100)==0){
			old_score=score;
			print_score("Score: ");
		}
		
		if (cnt > 1000)
		{
			cnt = 0;
			flip_led_d6();                   // negace portu na ktere je LED
		}
		
		print_it(pane.shape);
		keyboard_idle();                   // obsluha klavesnice
		terminal_idle();                   // obsluha terminalu
  } 
	
	return 0;
}
