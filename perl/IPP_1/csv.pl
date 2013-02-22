#!/usr/bin/perl
use Text::CSV;
use strict;
use warnings;
#CSV:xhalak00
use POSIX qw(locale_h);
######################################################
######################################################
########   IPP: projekt c1                    ########
########   varianta: CSV to XML               ########
########   Autor: Karel Hala                  ########
########   email: xhalak00@stud.fit.vutbr.cz  ########
########   login: xhalak00                    ########
########   ID: 138666                         ########
######################################################
######################################################

#Subrutina na tisknuti napovedy. Konci script s navratovym kodem 0
sub napoveda{
	print "\n\t\tIPP: projekt c1 CSV to XML\
		AUTOR: Karel Hala
		LOGIN: xhalak00\n
		-----------------------------\
		--help vypise tuhle napovedu\
		-----------------------------\n\
		--input=soubor : vstupni csv je v souboru \"soubor\"\
		--output=soubor : vystupni XML je v souboru \"soubor\" \n\
		-----------------------------\
		-n : negenerovat XML hlavicku\
		-r=root-element : obalovaci element celeho dokumentu \"root-element\"\
		-s=separator : oddelovac nastavit na \"separ\"\
		-h : prvni radek CSV dokumentu je hlavicka\
		-----------------------------\n\
		-l=line-element : obal jednotlivych radku nastavit na \"line-element\"\
		-i : lze kombinovat pouze s parametrem -l a nastavi vlozeni za line-element \"index\"\
		--start=n lze kombinovat pouze s -l a -i. Nastavi cislo indexu na \"n\"\n\
		-----------------------------\
		-e, --error-recovery : zotaveni z chybneho poctu sloupcu\
		--missing-value=val : pouze s -e pokud nejaky sloupec chyby doplni se za \"val\"\
		--all-columns : pouze s -e vypise vsechny sloupce\
		-----------------------------\ \n\n";
	exit 0;
}

#Subrutina na nacteni parametru
#jednotlive parametry parsuji pomoci funkce grep
#vysledky ukladam do hashovaci tabulky
#postupne pocitam pocet nactenych parametru a pokud se nejaky opakuje, nebo
#nachazi nejaky, ktery je neznamy vytisknu chybu parametru
sub get_params{
	my @arguments = @_;
    my $pocet = 0;
	my %uber = (
			'out' => 0,				#je vystup do souboru zapnut/vypnut
			'out_file' => "",		#cesta k vystupnimu souboru
			'in' => 0,				#je vstupni soubor zapnut/vypnut
			'in_file' => "",		#cesta ke vstupnimu souboru
			'head' => 1,			#mame gnerevoat hlavicku? prepinac -n 
			'root' => 0,			#mame vysledek obalovat do root?
			'root_el' => "",		#jmeno root elementu
			'-h' => 0,				#je prvni radek hlavicka CSV
			'separ' => ',',			#oddelovac v CSV dokumentu
			'line' => 'row',		#line element je nastaven na row
			'index' => 0,			#mame generovat index?
			'start' => 1,			#startovaci hodnota pro "index" je 1
			'error' => 0,			#mame ignorovat ruznou delku radku?
			'missing_val' => '',	#pokud bude chybet hodnota, nahradime za hodnotu nactenou z prikazove radky
			'columns' => 0,			#mame tisknout i prebytecne sloupce?
			'delim' => "\t",		#oddelovac, ciste z estetickeho hlediska
			'delim_2' => "",		#dalsi oddelovac, opet, ciste z estetickeho hlediska
			'-l'=>0					#je zapnut prepinac line element?
	);
	
	#pokud byl zadan pouze jeden parametr a ten je --help vytisknu napovedu
	if ((scalar @arguments)==1 && (lc($arguments[0]) eq "--help"))
	{

			napoveda();

	}else{
		my @pomocna;
		
		#parametr -r (root-element)
		if ( grep {?(-r=.*)?} @arguments ) {
			$uber{'root'} = 1;							#nastavim ze byl zapnut
			@pomocna = (grep {?(-r=.*)?} @arguments);
			$pomocna[0] =~ s/-r=//;						#vyparsuji jeho hodnotu
			$uber{'root_el'}=$pomocna[0];
			
			$pocet++;
			}
			
		#separator
		if ( grep {?(-s=.*)?} @arguments ) {

			@pomocna = (grep {?(-s=.*)?} @arguments);
			$pomocna[0] =~ s/-s=//;						#vyparsuji hodnotu oddelovace
			$uber{'separ'}=$pomocna[0];
			$pocet++;
			}
			
		#line-element
		if ( grep {?(-l=.*)?} @arguments ) {
			$uber{'-l'} = 1;							#aktivuji ho ze jej budu tisknout
			@pomocna = (grep {?(-l=.*)?} @arguments);
			$pomocna[0] =~ s/-l=//;						#vyparsuji hodnotu line-elementu
			$uber{'line'}=$pomocna[0];
			
			#index lze generovat jen v pripade ze byl zadan -l
			if ( grep {$_ eq '-i'} @arguments ) {
				$uber{'index'}=1;						#aktivuji ze budu tisknout index
				$pocet++;
				
				#nastaveni pocatecni hodnoty pro index
				if ( grep {?(--start=[0-9]+)?} @arguments ) {
					
					@pomocna = (grep {?(--start=[0-9]+)?} @arguments);
					$pomocna[0] =~ s/--start=//;		#muzou byt pouze kladna cisla vcetne 0
					$uber{'start'}=$pomocna[0];
					
					$pocet++;
				}
			}
			$pocet++;
			}
			
		#vstupni soubor
		if ( grep {?(--input=.*)?} @arguments ) {
			
			$uber{'in'}=1;								#aktivuji nacitani ze souboru
			@pomocna = (grep {?(--input=.*)?} @arguments);
			$pomocna[0] =~ s/--input=//;				#vyparsuji jmeno souboru
			$uber{'in_file'}=$pomocna[0];

			$pocet++;
			}
			
		#vystupni soubor
		if ( grep {?(--output=.*)?} @arguments ) {
			
			$uber{'out'}=1;								#aktivuji vypis do souboru
			@pomocna = (grep {?(--output=.*)?} @arguments);
			$pomocna[0] =~ s/--output=//;				#vyparsuji jmeno souboru
			$uber{'out_file'}=$pomocna[0];

			$pocet++;
			}
			
		#prepinac pro generovani hlavicky
		if ( grep {$_ eq '-n'} @arguments ) {

			$uber{'head'}=0;							#pokud je zapnut, vypnu generovani hlavicky
			
			$pocet++;
			}
			
		#prepinac pro urceni prvniho radku jako hlavicky CSV dokumentu
		if ( grep {$_ eq '-h'} @arguments ) {

			$uber{'-h'}=1;
			
			$pocet++;
			}
			
		#error recovery prepinac
		if ( (grep {$_ eq '-e'} @arguments) || ((grep {$_ eq '--error-recovery'} @arguments))) {
			$uber{'error'} = 1;							#aktivuji ze script nema kontrolovat pocty sloupcu
			
			#pokud bude nektery sloupec chybet nahradit za tuhle hodnotu
			if ( grep {?(--missing-value=.*)?} @arguments ) {

				@pomocna = (grep {?(--missing-value=.*)?} @arguments);
				$pomocna[0] =~ s/--missing-value=//;	#vyparsuji chybejici hodnotu
				$uber{'missing_val'}=$pomocna[0];
				
				$pocet++;
			}
			
			#prepinac pro zapnuti tisknuti prebytecnych sloupcu
			if ( grep {$_ eq '--all-columns'} @arguments ) {
				$uber{'columns'}=1;
				$pocet++;
			}
			$pocet++;
			}
			
		#ruzne chyby
		
		#pokud bude jmeno vstupniho souboru prazdne konec scriptu ERR: 2
		if ((grep {$_ eq '--input='} @arguments))
		{
			print STDERR "ERR: chyba vstupniho souboru\n";
			exit 2;
		}
		
		#pokud bude jmeno vystupniho souboru prazdne konec scriptu ERR: 2
		if ((grep {$_ eq '--output='} @arguments))
		{
			print STDERR "ERR: chyba vystupniho souboru\n";
			exit 3;
		}
		
		#pokud bude root element prazdny konec scriptu ERR: 30
		if ((grep {$_ eq '-r='} @arguments))
		{
			print STDERR "ERR: nevalidni XML\n";
			exit 30;
		}
		
		#pokud bude line element prazdny konec scriptu ERR: 30
		if ((grep {$_ eq '-l='} @arguments))
		{
			print STDERR "ERR: nevalidni XML\n";
			exit 30;
		}
		
		#pokud bude value prazdne nastavim jej na prazdnou hodnotu
		if ((grep {$_ eq '--missing-value='} @arguments))
		{
			$uber{'missing_val'}="";
			$pocet++;
		}
		
		#pokud se neshoduje pocet nalezenych a pocet celkovych parametru koncim s chybou 1
		if (scalar @arguments != $pocet )
		{
			print STDERR "ERR: chyba argumentu\n";
			exit 1;
		}
	}
	
	return %uber;
}

#subrutina pro kontrolu a opravu alementu
sub invalid_tag{
	my $string=$_[0];
	utf8::decode($string);
	
	#nejdrive nahradim nevalidni znaky za pomlcku
	$string =~ s/[^:_\-\w\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{10000}-\x{EFFFF}\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}}\.0-9\x{B7}\x{0300}\x{036F}\x{203F}-\x{2040}]/-/g;
	
	#pote overim zda je tento elemnt stale validni
	if (!($string =~ m/^[:a-zA-Z\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{10000}-\x{EFFFF}][:a-zA-Z0-9\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{10000}-\x{EFFFF}\.\-\x{B7}\x{0300}-\x{036F}\x{203F}-\x{2040}]*(\x{20}[:a-zA-Z0-9\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{10000}-\x{EFFFF}\.\-\x{B7}\x{0300}-\x{036F}\x{203F}-\x{2040}])*/i))
	{
		print STDERR "ERR: invalidni XML\n";
		exit 31;
	}
	utf8::encode($string);
	return $string;
}

#uprav string, aby v nem nebyli nejake nevalidni znaky
sub validate{
	
	#rozdelit string po znacich
	my @string = split ('',$_[0]);
	
	#ty potom jednotlive skontrolovat
	for (my $pos=0;$pos<(scalar @string);$pos++)
	{
		#uvzovky nahradit za: &quot
		if ($string[$pos] eq "\""){
			$string[$pos] = "&quot;";
		}

		#mensitko za &lt;
		elsif ($string[$pos] eq "<"){
			$string[$pos] = "&lt;";}
		
		#ampersand za &amp;
		elsif ($string[$pos] eq "&"){
			$string[$pos] = "&amp;";}
			
		#vetsitko za &gt;
		elsif ($string[$pos] eq ">"){
			$string[$pos]="&gt;";}
	}
	return (join ('',@string));
}



#subrutina na postupne nacitani a tisknuti souboru CSV
#hlavni jsou zde 2 cykly, ktere postupne prochazi radky a jendotlive data
#pro nacitani radku je zde knihovna CSV a v ni getline()
#jako hlavni je zde hash tabulka uber, ktera obsahuje vsechny data z prikazove radky
sub work {
	
	#hash tabulka, zmenena v subrutine ger_params
	my %uber=@_;
	
	#dvourozmerne pole obsahujici jednotlive radky a bunky
	my @fields;
	#pole obsahujici prvni radek, pokud budeme tisknout hlavicku
	my @head;
	#promenna do ktere postupne ukladam data, ktera se budou na konci tisknout
	my $saver="";
	#citac sloupcu
	my $col=0;

	#knihovna CSV
	my $csv = Text::CSV->new ({ 
			sep_char            => $uber{'separ'},
			eol                 => "\015\012",
			binary              => 1,});
			
	#oddelovac TAB nastavit allow_whitespace a jako oddelovat urcit \t
	if ($uber{'separ'} eq "TAB")
	{
		$csv->sep_char ("\t");
	}

	#nacteni vstupniho souboru
	if ($uber{'in'}==1)
	{
		#otevreme vstupni soubor, jmeno je v $uber{'in_file'}
		open my $source_handle, "<", $uber{'in_file'}
			or 
			print STDERR "ERR: chyba vstupniho souboru\n" and 
			die 2;
			
		#postupne nacitani jednotlivych radku
		while (my $row = $csv->getline ($source_handle)) {
			push @fields,[@$row]; 			#vlozeni do dvourozmerneho pole
		}

		close($source_handle);
	}
	else
	{
		#otevreni standardniho vstupu
		open(my $stdin, "<-");
		
		#postupne nacitani jednotlivych radku
		while (my $row = $csv->getline ($stdin)) {
		   push @fields,[@$row];			#vlozeni do dvourozmerneho pole
			 }
		close($stdin);
	}
	
	#pokud pri cteni CSV souboru nacte getline chybu, script končí s chybou 4
	if (defined $csv->error_input () ){
		print STDERR "ERR: chyba CSV\n";
		exit 4;
	}
	
	#pokud se nam nic nepovedlo nacist je pravdepodone chybny format CSV
	if ((scalar @fields)==0){
		print STDERR "ERR: nevalidni CSV\n";
		exit 30;
	}

	#prvni radek bude hlavicka
	#vyrizneme prvni radek z dvourozmerneho pole a overime zda obsahuje validni znaky
	if ($uber{'-h'}==1)
	{
		@head = @{$fields[0]};
		$col = scalar(@head);					#pocet sloupcu se bude ridit podle hlavicky
		
		#overeni, zda je hlavicka ve validnim formatu
		for (my $h_pos=0;$h_pos<$col;$h_pos++){
			$head[$h_pos]=invalid_tag($head[$h_pos])}
	}
	# generovani hlavicky
	if ($uber{'head'}==1)
	{
		$saver = "<" . "?xml version=\"1.0\" encoding=\"UTF-8\"?". ">\n";
	}
	
	#budeme generovat root obal xmlka?
	if ($uber{'root'}==1)
	{
		$uber{'root_el'} = invalid_tag($uber{'root_el'});
		$saver = $saver . "<"."$uber{'root_el'}".">"."\n";
		$uber{'delim'}="\t\t";
		$uber{'delim_2'}="\t";
	}

	
	#hlavni cyklus pro prochazeni jednotlivych radku a sloupcu
	#je zde hlavni x, ktere nese cislo radku, pokud je aktivovan -h prvni radek je hlavicka
	#tak zaciname od druheho radku
	for (my $x=$uber{'-h'};$x<(scalar @fields);$x++)
	{
		#nacteme aktualni radek do pole pro lepsi oriantaci
		my @line = @{$fields[$x]};
		
		#pokud nebyla hlavicka CSV, prvni radek urcuje pocet sloupcu
		if ($col==0)
		{
			$col = (scalar @line);
		}
		
		#skontrolovat, jestli neni chybnej pocet sloupcu na radku
		if ($uber{'error'}==0)
		{

			if ((scalar @line)!=$col){
				print STDERR "ERR: chybny pocet sloupcu\n";
				exit 32;
			}
		}
		#pokud je prapinac -e, a chybejici sloupce nahradit za nic, ulozime tuhle hodnotu do
		#nadbytecnych sloupcu
		elsif ($uber{'error'}==1){
			if ((scalar @line)<$col){
				my $max=(scalar @line);
				for (my $index=$max;$index<$col;$index++)
				{
					$line[$index]=$uber{'missing_val'};
				}
			}
		}
		
		#budeme tisknout obal kazde bunky.
		#<line-elemnt [index=cislo]></line-elemnt [index=cislo]>
		$uber{'line'} = invalid_tag($uber{'line'});
		$saver = $saver . $uber{'delim_2'} . "<" . "$uber{'line'}";
		
		#[index=cislo]
		if ($uber{'index'}==1){
			my $number_of_index = $uber{'start'}+$x-$uber{'-h'};
			$saver = $saver . " index=\"" . "$number_of_index" . "\">\n";
		}
		
		else{
			$saver = $saver. ">\n";
		}
		#obal bunky dotisknut
		
		#cyklus na tisknuti jednotlivych dat v poli
		for (my $position=0;$position<(scalar @line);$position++)
		{
			#nejdrive nahradime nevalidni XML znaky
			$line[$position] = validate($line[$position]);
			
			#pokud je na radku vic sloupcu, ale neni to povoleno pokracovat v hlavnim cyklu
			if ($uber{'error'}==1 and $uber{'columns'}==0)
			{
				if ($position>=$col){
					last;
				}
			}			
			
			#pokud mame hlavicku, tak tiskneme nazvy elementu podle hlavicky
			if ($uber{'-h'}==1)
			{
				#tisk XML:
				
				#tisknu zde i mezery a podobne, pro lepsi prehlednost
				#pokud mame tisknout prebytecne sloupce, tak je vytiskneme uz ted
				if ($uber{'columns'}==1 and ($position+1) > $col)
				{
					#<colX>data</colX>
					my $print_more = $position+1;
					$saver = $saver . $uber{'delim'} . "<col" . $print_more . ">".$line[$position] ."</col" . $print_more . ">" ."\n";
				}
				
				#jinak se musi vytisknout jeste nazvy jednotlivych bunek
				else{
					#nejdrive opravit data v hlavicce
					$head[$position]=invalid_tag($head[$position]);
					
					#<head[position]>data</head[position]>
					$saver = $saver . $uber{'delim'} . "<" . $head[$position] . ">" . $line[$position] ."</" . $head[$position] . ">" . "\n";
				}
			}
			
			#vytiskneme jako nazvy bunek col a jejich cislo, osetreno i, kdyz budeme tisknout prbytecne sloupce
			else
			{
				if ($uber{'columns'}==1){
					#<colX>data</colX>
					my $print_more = $position+1;
					$saver = $saver . $uber{'delim'} . "<" . "col" . $print_more . ">" . $line[$position] . "</" . "col" . $print_more . ">" . "\n";
				}
				else{
					my $print_more = $position+1;
					#<colX>data</colX>
					$saver = $saver . $uber{'delim'}."<"."col" . $print_more . ">". $line[$position] . "</" . "col" . $print_more . ">" . "\n";
				}
			}
		}
		#konec cyklu, ted uz jneom dotisknout konecny obal radku a dalsi veci
		$saver = $saver. $uber{'delim_2'} . "</" . "$uber{'line'}>\n";
		
	}
	#konec tisknuti, ted uz jenom vytisknout konecny tag root, kdyz je potreba
	
	#vytiskneme konecny tag rootu
	if ($uber{'root'}==1)
	{
		$saver= $saver. "</"."$uber{'root_el'}".">"."\n";
	}

	#samostatne tisteni
	if ($uber{'out'}==1)
	{
		#pokud do souboru
		open my $out_handle, ">", "$uber{'out_file'}"
			or 
			print STDERR "ERR: chybny vystupni soubor\n" and exit 3;
		print $out_handle $saver;
		close $out_handle;
	}
	else
	{
		#pokud na STDOUT
		print $saver;
	}
	return 0;
}
sub main{
	#ziskani parametru
	my %hash = get_params(@ARGV);
	
	#volani hlavni funkce
	return work(%hash);
}

#nastaveni local
my $old_locale = setlocale(LC_CTYPE);
setlocale(LC_CTYPE, "cs CZ.UTF-8");

#volani obalovaci funkce
my $err = main();
if ($err!=0){exit $err}
exit 0;


