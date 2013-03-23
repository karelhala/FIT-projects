#!/usr/local/bin/env python3
import re
#CSV:xhalak00
import math
import sys
import codecs

######################################################
######################################################
########   IPP: projekt c2                    ########
########   varianta: SYN		              ########
########   Autor: Karel Hala                  ########
########   email: xhalak00@stud.fit.vutbr.cz  ########
########   login: xhalak00                    ########
########   ID: 138666                         ########
######################################################
######################################################

re.DOTALL

#Funkce pro tisk napovedy
def print_help():
	print ("\n\t\tIPP: projekt c2 SYN\n\
		AUTOR: Karel Hala\n\
		LOGIN: xhalak00\n\
		-----------------------------\n\
		--help vypise tuhle napovedu\n\
		-----------------------------\n\
		--input=soubor 	: vstupni \"soubor\"\n\
		--output=soubor : vystupni \"soubor\" \n\
		--dormat=soubor : vystupni formatovaci \"soubor\" \n\
		-----------------------------\n\
		--br 			:za kazdy radek dat <br /> \n\
		-----------------------------\n\
	")
#Funkce pro zpracovani parametru
#jednotlive parametry parsuji pomoci re.match
#vysledky ukladam do hashovaci tabulky
#postupne pocitam pocet nactenych parametru a pokud se nejaky opakuje, nebo
#nachazi nejaky, ktery je neznamy vytisknu chybu parametru
def pars_arguments(argums):
	del argums[0]
	counter=0
	result=""
	table = {'input':'','input_on':0,'output':'','output_on':0,'format':'','br':0}
	for arguments in argums:
		#nejdrive overim vyskyt --help pro napovedu
		if (len(argums)==1 and arguments=="--help"):
			print_help()
			exit (0)
		else:
			#pote nactu cestu ke vstupnimu souboru, pokud je potreba
			if (re.match('--input=.*',arguments)):
				result = re.match('--input=(.*)',arguments)
				table['input']=result.group(1)
				table['input_on']=1
				counter+=1
			#dale nactu formatovaci soubor
			if (re.match('--format=.*',arguments)):
				result = re.match('--format=(.*)',arguments)
				table['format']=result.group(1)
				counter+=1
			#Nebo soubor pro vystup
			if (re.match('--output=[^ ]+',arguments)):
				result = re.match('--output=(.*)',arguments)
				table['output']=result.group(1)
				table['output_on']=1
				counter+=1
			#Pokud mam tisknout za kazdym radkem <br /> kontroluje to tenhle prepinac
			if (re.match('--br',arguments)):
				result = re.match('--br',arguments)
				table['br']=1
				counter+=1
	#Zkontroluji, zda jsou parametry v poradku
	if (len(argums)!=counter):
		print ("Argums Error",file=sys.stderr)
		exit (1)
	return table

#funkce pro kontrolu regularnich vyrazu
#Funkce pracuje v cyklu prevazne kvuli vicenasobnym %
#Puvodne jsem vyhledaval pomoci regularnich vyrazu, ale ty mi nedokazali osetrit vicenasobne %
#script se timto mirne spomalil, ale ne prilis
def my_regulars(string):
	counter=0
	outer=""
	
	#nejdrive nahradim znaky co by mohli delat problemy
	string=re.sub(r'\[','\\[',string)
	string=re.sub(r'\]','\\]',string)
	string=re.sub(r'\^','\\^',string)
	string=re.sub('\{','\\{',string)
	string=re.sub('\}','\\}',string)
	string=re.sub('\$','\\\$',string)
	string = re.sub('\?','\?',string)
	
	#pokud se za ! bude nachazet * + ! | ( ) nastala chyba regularniho vyrazu
	if (re.search('!(?=[*+!|()])',string)!=None):
		print ("ERR: chyba v regularnim vyrazu", file=sys.stderr)
		exit (4)
	
	#postupne nahradit ! ruznymy hodnotami
	string=re.sub('!%s',r'\S',string)
	string=re.sub('!%a',r'[^.]',string)
	string=re.sub('!%d',r'[^0-9]',string)
	string=re.sub('!%l',r'[^a-z]',string)
	string=re.sub('!%L',r'[^A-Z]',string)
	string=re.sub('!%w',r'[^a-zA-Z]',string)
	string=re.sub('!%W',r'[^\w]',string)
	string=re.sub('!%t',r'[^\t]',string)
	string=re.sub('!%n',r'[^\n]',string)
	string = re.sub('!(.)',r'[^\1]',string)
	
	#postupne projedu jednotlive prvky a zamenim je
	for index in range (len(string)):
		pom=""
		#pokud nactu \v textu nahradim ho za \\ tohle je hlavne kvuli \t a \n
		if (string[index]=='\\'and (string[index+1]=='n' or string[index+1]=='t')):
			outer+=r'\\'
		#pokud nactu % zvysim citac
		elif (string[index] == "%"):
			counter+=1
		#pokud nactu cokoliv jineho nez % nebo \
		else:
			#nejdrive zkonroluji pocet %
			#pokud jsem nacetl lichy pocet, jedna se o %specialni_znak
			if ((counter%2)!=0):
				#ulozim soucasny pocet %
				outer+='%'*(math.trunc(counter/2))
				#specialni znaky postupne nahradim
				if (string[index] =="d" or string[index] =="t"
				or string[index] =="." or string[index] =="|"or string[index] =="+"or string[index] =="*"
				or string[index] ==")"or string[index] =="("):
					outer+="\\"
					outer+=string[index]
				#lower
				elif (string[index] =="l"):
					outer+="[a-z]"
				#jakykoliv znak+\r\n a \n
				elif (string[index] =="a"):
					outer+="(.|\r\n|\n)"
				#upper
				elif (string[index] =="L"):
					outer+="[A-Z]"
				#vsechny pismena
				elif (string[index] =="w"):
					outer+="[a-zA-Z]"
				#a-z a 0-9
				elif (string[index] =="W"):
					outer+="\w"
				#vsechny prazdne znaky
				elif (string[index] =="s"):
					outer+="\r\n|\n|\t|\f|\v| "
				#odradkovani
				elif (string[index]=="n"):
					outer+="(\r\n|\n)"
				#%! prevede na !
				elif (string[index]=="!"):
					outer+="!"
				else:
				#pokud se vyskytne cokoliv jineho vypisi chybu
					print ("Error v regularnim vyrazu", file=sys.stderr)
					exit (4)
			#pokud nactu sudej pocet %
			elif (counter>0):
				#ulozim je
				outer+='%'*(math.trunc(counter/2))
				#osetreni specialnich chyb
				#za ( nemuze byt . nebo |
				if (string[index]=="(" and index!=len(string)-1 and (string[index+1]=="." or string[index+1]=="|")):
					print ("Error v regularnim vyrazu", file=sys.stderr)
					exit (4)
				#za | nemuze byt + * . ) |
				elif (string[index]=="|" and index!=len(string)-1 and (string[index+1]=="+" or string[index+1]=="*" or string[index+1]=="." or string[index+1]==")" or string[index+1]=="|")):
					print ("Error v regularnim vyrazu", file=sys.stderr)
					exit (4)
				#nemuze byt +* za sebou
				elif (string[index]=="*" and index!= len(string)-1 and string[index+1]=="+"):
					print ("Error v regularnim vyrazu", file=sys.stderr)
					exit (4)
				#jinak pokud nactu neco jineho nez tectu, ulozim ji
				elif (string[index]!="."):
					outer+=string[index]
					
				#pokud nactu tectu, tak za ni nemuze byt + * . ) |
				elif (index!=len(string)-1 and (string[index+1]=="+" or string[index+1]=="*" or string[index+1]=="." or string[index+1]==")" or string[index+1]=="|")):
					print ("Error v regularnim vyrazu", file=sys.stderr)
					exit (4)
			#stejne osetreni jako pri sudem poctu %
			#za ( nemuze byt . nebo |
			elif (string[index]=="(" and index!=len(string)-1 and (string[index+1]=="." or string[index+1]=="|")):
				print ("Error v regularnim vyrazu", file=sys.stderr)
				exit (4)
			
			#za | nemuze byt + * . ) |
			elif (string[index]=="|" and index!=len(string)-1 and (string[index+1]=="+" or string[index+1]=="*" or string[index+1]=="." or string[index+1]==")" or string[index+1]=="|")):
				print ("Error v regularnim vyrazu", file=sys.stderr)
				exit (4)
			#nemuze byt +* za sebou
			elif (string[index]=="*" and index!= len(string)-1 and string[index+1]=="+"):
				print ("Error v regularnim vyrazu", file=sys.stderr)
				exit (4)
			#jinak pokud nactu neco jineho nez tectu, ulozim ji
			elif (string[index]!="."):
				outer+=string[index]
			#pokud nactu tectu, tak za ni nemuze byt + * . ) |
			elif (index!=len(string)-1 and (string[index+1]=="+" or string[index+1]=="*" or string[index+1]=="." or string[index+1]==")" or string[index+1]=="|")):
				print ("Error v regularnim vyrazu", file=sys.stderr)
				exit (4)
			
			counter = 0
	#pokud se bude na konci nachat par % ulozim je
	if (counter>0):
		outer+='%'*(math.trunc(counter/2))
		if ((counter%2)!=0):
			outer+="%"
	#pokudsim se zkompilovat regular upraveny
	try:
		re.compile(outer)
	#popripade vypisi chybu
	except re.error:
		print ("Error v regularnim vyrazu", file=sys.stderr)
		exit (4)
	return outer
#funkce na zmenu tagu
#postupne prochazim v cyklu jeden tag za druhym a nahrazuji za HTML tag
def change_tags(array):
	start=""
	end=""
	result=""
	returned=[]
	#cyklus na porstupne prochazeni polozek
	for i in range(len(array)):
		#nahrazeni size:[cislo]
		if ((re.match(r'size:([1-7])',array[i])) != None):
			result=re.match(r'size:([1-7])',array[i])
			start +="<font size="
			start +=result.group(1)+">"
			end ="</font>"+end
		
		#nahrazeni color:[hex_cislo]
		elif ((re.match(r'color:([0-9A-Fa-f]{6})',array[i])) != None):
			result = re.match(r'color:([0-9A-Fa-f]{6})',array[i])
			start +="<font color=#"
			start +=result.group(1)+">"
			end="</font>"+end
		
		#nahrazeni bold
		elif (array[i]=="bold"):
			start +="<b>"
			end="</b>"+end
		
		#nahrazeni italic
		elif (array[i]=="italic"):
			start +="<i>"
			end="</i>"+end
		
		#underline
		elif (array[i]=="underline"):
			start +="<u>"
			end="</u>"+end
		
		#teletype
		elif (array[i]=="teletype"):
			start +="<tt>"
			end="</tt>"+end
		
		#prazdny retezec nahradim za prazdny retezec
		elif (array[i]==""):
			start+=""
			end+=""
		
		#jinak vypisi chybu 4 (formatovaci soubor)
		else:
			exit (4)
	returned.append(start)
	returned.append(end)
	
	# avratime pole pocatcnich a koncovych tagu
	return returned

#funkce na rozdeleni jednotlivych tagu
def split_by_columns(string):
	pom=[]
	#postupne prochazim znak po znaku
	for i in range(len(string)):
		#pokud nactu mezeru, nebo \t nic neukladam
		if (string[i]==" " or string[i]=="\t"):
			continue
		
		#jinak pridam k tagu aktualni znak
		else:
			pom.append(string[i])
	string= "".join(pom)
	#rozdelim po carkachs
	pom=string.split(",")
	pom=change_tags(pom)
	return pom

#funkce na rozdeleni tagu od regularnich vyrazu
def split_tab(file):
	splited=[]
	list_of_lists=[[]]
	string=""
	#postupne prochazim pole radek po radku
	for i in range(len(file)):
		#rozdelim pres \t
		splited=file[i].partition("\t")
		
		#pokud nenajdu \t je to chyba 4
		if (splited[1]!="\t"):
			print ("Error v regularnim vyrazu", file=sys.stderr)
			exit (4)
		#prevedu regularni vyrazy
		string = my_regulars(splited[0])
		#pridam je do pole
		list_of_lists[i].append(string)
		
		#prevedu tagy
		string=split_by_columns(splited[2])
		#pridam je do pole
		list_of_lists[i].append(string)
		# a pridam jejich velikost, pocatecnich a koncovych
		list_of_lists[i].append(len(string[0]))
		list_of_lists[i].append(len(string[1]))
		if ((i+1)!=len(file)):
			list_of_lists.append([])
	return list_of_lists

#funkce pro nacteni formatovaciho souboru a jeho rozdeleni po radcich
def open_file(path):
	if (path!=""):
		
		#nacteme soubor
		try:
			inp = codecs.open( path, 'r','utf-8' ) 	
		except:
			print ("nemuzu otevrit soubor!",file=sys.stderr)
			exit (4)
			
		#ulozime jeho radky
		line = inp.readlines()
		try:
			inp.close()
		except:
			print ("nemuzu otevrit soubor!",file=sys.stderr)
			exit (4)
		arr = []
		pom=[]
		#read line into array 
		for l in range(len(line)):
			pom=line[l].split("\n")
			if (len(pom)>1):
				del pom[1]
			arr.append(pom[0])
	else:
		arr=[]
	return arr
	
#funkce pro nacteni vstupniho souboru
def input_file(path):
	try:
		inp = codecs.open( path, 'r','utf-8' )
	except:
		print ("nemuzu otevrit soubor!",file=sys.stderr)
		exit (2)
	file = inp.read()
	try:
		inp.close()
	except:
		print ("nemuzu zavrit soubor!",file=sys.stderr)
		exit (2)
	return file

#funkce pro nalezeni jednotlivych indexu v nactenem souboru
def find_iter(main_array,in_file):
	#postupne projdu hlavni pole
	for index in range(len(main_array)):
		#pokud je vubec co hledat
		if (main_array[index]!=[]):
			main_array[index].append([])
			main_array[index].append([])
			#cyklus pro nalezeni indexu daneho regularu a ulozeni do hlavniho pole
			for m in re.finditer(main_array[index][0], in_file):
				if (m.start()!=m.end()):
					main_array[index][4].append(m.start())
					main_array[index][5].append(m.end())
	return main_array

#funkce na tisk a posun ve stringu
#funkce pracuje s hlavnim polem
#postupne prochazi jednotlive prvky
#a pokud ma co vlozit, vlozi a projde znovu pole a posune jednotlive polozky
def shift_array(array,main_string):
	pom=""
	#pruchod polem po radcich
	for index in range(len(array)):
		#pokud mame vubec co hledat
		if (array[index]!=[]):
			#cyklus na projiti jednotlivych tagu
			for i in range(len(array[index][4])):
				
				#ulozime pocatecni tag
				pom=main_string[array[index][4][i]:]
				main_string=main_string[:array[index][4][i]]+array[index][1][0]
				main_string=main_string[:len(main_string)]+pom	
				#nyni projdeme znovu pole a popripade posuneme indexy, dalsich tagu
				for field in range(len(array)):
					for k in range(len(array[field][4])):
						#pokud by jsme vkladali pocatecni tag pred dalsi, neb na misto dalsich tagu
						#musime indexy posuout
						if (array[index][4][i]<array[field][4][k] or (array[index][4][i]==array[field][4][k] and index!=field)):
							array[field][4][k]+=array[index][2]
						if (array[index][4][i]<array[field][5][k]):
							array[field][5][k]+=array[index][2]	

				#ulozime ukoncovaci tag
				pom=main_string[array[index][5][i]:]
				main_string=main_string[:array[index][5][i]]+array[index][1][1]
				main_string=main_string[:len(main_string)]+pom
				#nyni projdeme znovu pole a popripade posuneme indexy, dalsich tagu
				for field in range(len(array)):
					for k in range(len(array[field][4])):
						#pokud by jsme vkladali koncovy tag pred dalsi, neb na misto dalsich tagu
						#musime indexy posuout
						if (array[index][5][i]<array[field][4][k] or (array[index][5][i]==array[field][4][k])):
							array[field][4][k]+=array[index][3]
						if (array[index][5][i]<array[field][5][k]):
							array[field][5][k]+=array[index][3]
							
	return main_string

#funkce pro nacteni vystupniho souboru
def out_print(path,string):
	try:
		out=codecs.open(path,'w','utf-8')
	except:
		print ("chyba pri otevirani",file=sys.stderr)
		exit (3)
	print (string,file=out)
	try:
		out.close()
	except:
		print ("chyba pri zavirani",file=sys.stderr)

#hlavni funkce
def main():
	
	#zpracovani argumentu
	argums = sys.argv
	table = pars_arguments(argums)
	
	#otevreni formatovaciho souboru
	format_file=open_file(table['format'])
	
	#otevreni vstupniho souboru
	if (table['input_on']):
		input=input_file(table['input'])
	else:
		input=sys.stdin.read()
	
	#rozdeleni regularu a tagu
	tabs= split_tab(format_file)
	
	#nalezeni poloh
	tabs = find_iter(tabs,input)
	
	#vytisknuti a posun
	input=shift_array(tabs,input)
	
	#pripadne tisknuti <br />
	if (table['br']==1):
		input=re.sub('\n','<br />\n',input)
	
	#tisknuti vysledneho souboru
	if (table['output_on']):
		out_print (table['output'],input)
	else:
		print (input)
main()
