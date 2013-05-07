all: clean do

do: proj2.tex
	latex proj2.tex
	latex proj2.tex
	dvips -t a4 proj2.dvi
	ps2pdf proj2.ps

clean:
	rm -f proj2.dvi proj2.log proj2.aux proj2.ps proj2.pdf
