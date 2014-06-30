

set term postscript eps enhanced color dl 2.0  "Helvetica" 20 


outfile = 'tat4_0.eps'
srcfile = 'tat4_0.dat' 
xloc=4.3
yloc=-2

set output outfile  

set ylabel "Surface Tension (mN/m)"
set key left 


title_f(a,b) = sprintf('f(x) = %5.2fx + %5.2f', a,b )
ka(a, ae) = sprintf('K_a: %5.2f +\- %5.2f (mN/m)' , a, ae )

set fit errorvariables 

set border lw 3.0 



##########
set xlabel "ln(A)"

f(x) = a *x + b 
fit f(x) srcfile u  (log($1)):($2)  via a, b 

set title outfile
set label 1 ka(a, a_err) at xloc, yloc

plot srcfile  u (log($1)):($2):($3)  w yerrorbars lw 5 title srcfile , f(x) t title_f(a,b)   w l lw 5 

