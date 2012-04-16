#-*-Makefile-*- vim:syntax=make

PROJECT:= zig_swarm
TEXFILES:= ${PROJECT}.tex approtocol.tex title.tex

${PROJECT}.pdf: ${PROJECT}.dvi ${TEXFILES}
	dvipdf ${PROJECT}.dvi

${PROJECT}.aux: ${TEXFILES}
	latex $^

${PROJECT}.dvi: ${TEXFILES}
	latex $^

${PROJECT}.idx: ${TEXFILES}
	latex $^

gv: ${PROJECT}.pdf
	gv $^

kpdf: ${PROJECT}.pdf
	kpdf $^

ar: ${PROJECT}.pdf
	acroread $^

e: ${PROJECT}.pdf
	evince $^

clean:
	rm -f ./*.dvi ./*.pdf

clean_all: clean
	rm -f ./*.log ./*.aux ./*.out ./*.idx

