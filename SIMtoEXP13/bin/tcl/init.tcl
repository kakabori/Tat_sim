
###########################################################################
############### initialize to some defaults ###############################
###########################################################################

#set area 72.4
#set Nlip 288
#set binstep 0.2
set area 1
set Nlip 2
set binstep 1
set norm 1
set Zcentre 0
set Zdirection +1

set nid 16
set atomf 1
set partialc 1

# atom(0): Hydrogen H
set id(0) H
set b(0) -0.374E-4
set e(0) 1
set ffa1(0) 0.493
set ffa2(0) 0.323
set ffa3(0) 0.14
set ffa4(0) 0.041
set ffb1(0) 10.511
set ffb2(0) 26.126
set ffb3(0) 3.142
set ffb4(0) 57.8
set ffc(0) 0.003

# atom(1): Deuterium D
set id(1) D
set b(1) 0.667E-4
set e(1) 1
set ffa1(1) 0.493
set ffa2(1) 0.323
set ffa3(1) 0.14
set ffa4(1) 0.041
set ffb1(1) 10.511
set ffb2(1) 26.126
set ffb3(1) 3.142
set ffb4(1) 57.8
set ffc(1) 0.003

# atom(2): Carbon C
set id(2) C
set b(2) 0.665E-4
set e(2) 6
set ffa1(2) 2.31
set ffa2(2) 1.02
set ffa3(2) 1.589
set ffa4(2) 0.865
set ffb1(2) 20.844
set ffb2(2) 10.208
set ffb3(2) 0.569
set ffb4(2) 51.651
set ffc(2) 0.216

# atom(3): Nitrogen N
set id(3) N
set b(3) 0.940E-4
set e(3) 7
set ffa1(3) 12.213
set ffa2(3) 3.132
set ffa3(3) 2.013
set ffa4(3) 1.166
set ffb1(3) 0.006
set ffb2(3) 9.893
set ffb3(3) 28.997
set ffb4(3) 0.583
set ffc(3) -11.524

# atom(4): Oxygen O
set id(4) O
set b(4) 0.580E-4
set e(4) 8
set ffa1(4) 3.049
set ffa2(4) 2.287
set ffa3(4) 1.546
set ffa4(4) 0.867
set ffb1(4) 13.277
set ffb2(4) 5.701
set ffb3(4) 0.324
set ffb4(4) 32.909
set ffc(4) 0.251

# atom(5): Phosphorus P
set id(5) P
set b(5) 0.517E-4
set e(5) 15
set ffa1(5) 6.435
set ffa2(5) 4.179
set ffa3(5) 1.78
set ffa4(5) 1.491
set ffb1(5) 1.907
set ffb2(5) 27.157
set ffb3(5) 0.526
set ffb4(5) 68.164
set ffc(5) 1.115

###########################################################################
# united atoms
# methyl (CH3) T
set id(6) T
set b(6) [expr $b(2)+3*$b(0)]
set e(6) [expr $e(2)+3*$e(0)]
set ffa1(6) 3.279
set ffa2(6) 2.499
set ffa3(6) 2.0086
set ffa4(6) 0.988
set ffb1(6) 35
set ffb2(6) 16
set ffb3(6) 3
set ffb4(6) 65
set ffc(6) 0.2254

# methylen (CH2) M
set id(7) M
set b(7) [expr $b(2)+2*$b(0)]
set e(7) [expr $e(2)+2*$e(0)]
set ffa1(7) 2.956
set ffa2(7) 2.006
set ffa3(7) 1.8686
set ffa4(7) 0.947
set ffb1(7) 30
set ffb2(7) 15
set ffb3(7) 3
set ffb4(7) 60
set ffc(7) 0.2224

# methines (CH) B
set id(8) B
set b(8) [expr $b(2)+$b(0)]
set e(8) [expr $e(2)+$e(0)]
set ffa1(8) 2.633
set ffa2(8) 1.513
set ffa3(8) 1.7285
set ffa4(8) 0.906
set ffb1(8) 25
set ffb2(8) 14
set ffb3(8) 3
set ffb4(8) 55
set ffc(8) 0.2195

# water (H2O) W
set id(9) W
set b(9) [expr 2*$b(0)+$b(4)]
set e(9) [expr 2*$e(0)+$e(4)]
set ffc(9) $e(9)

# heavy water (D2O) V
set id(10) V
set b(10) [expr 2*$b(1)+$b(4)]
set e(10) [expr 2*$e(1)+$e(4)]
set ffc(10) $e(10)

# methines (CH) X
set id(11) X
set b(11) [expr $b(2)+$b(0)]
set e(11) [expr $e(2)+$e(0)]
set ffa1(11) 2.633
set ffa2(11) 1.513
set ffa3(11) 1.7285
set ffa4(11) 0.906
set ffb1(11) 25
set ffb2(11) 14
set ffb3(11) 3
set ffb4(11) 55
set ffc(11) 0.2195

###########################################################################
# Fourier transform parameters
set symmetrized 0
set xq0 0.0
set xq1 1.0
set xqstep 0.001
set xWat 0.333
set nq0 0.0
set nq1 0.4
set nqstep 0.0025
set nWat 6.38e-6
