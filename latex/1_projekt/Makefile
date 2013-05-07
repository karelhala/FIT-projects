all: clean do

do: proj1.tex
	latex proj1.tex
	latex proj1.tex
	dvips -t a4 proj1.dvi
	ps2pdf proj1.ps

clean:
	rm -f proj1.dvi proj1.log proj1.aux proj1.ps proj1.pdf