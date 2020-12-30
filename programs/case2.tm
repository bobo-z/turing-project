;This program is to check if the input string is in the language of L2={1^m x 1^n = 1^mn}
;Input: a string of 1's, x's and ='s. e.g."111x11=111111"

;There are two steps:first step to check if the string is in the correct form, second to check the number of '1' is correct
;Step1: Check if string is in the form of "1^* x 1^* = 1*x" e.g."11x111=111"
;   divide the string by 'x' and '=', there are three parts:before symbol 'x'(be_x),after symbol 'x'(af_x) and after symbol '='(af_e)
;   the transition functions are below, it's easy to check.if it's not in the correct form, reject immediately.
;   while checking,copy the 1st,2nd and 3rd part to 2nd,3rd and 4th tape.

;Step2: Check if the number of '1' is correct.
;m,n and p are the number of '1' on three tapes. Check if m*n==p
;there are: m*n=p <=> (m-1)*n=p-n <=>...<=>0*n=0
;1) if there are '1' on 2nd tape, scan the 3rd tape.else -> reject
;2) while scanning the 3rd tape, empty the same number of '1' on 4th tape.if scanning is not finish and the 4th tape is already empty->reject
;3) finish scanning the 3rd tape, empty one '1' on the 2nd tape
;4) if the 2nd and 4th tape are both empty->accept
;5) else continue 1)

;to scan the 3rd tape from left to right, it's in state ck1
;else it's in state ck2

#Q = {0,be_x,af_x,af_e,ck1,ck2,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

#S = {1,x,=}

#G = {1,x,=,_,t,r,u,e,f,a,l,s}

#q0 = 0

#B = _

#F = {halt_accept,halt_reject}

#N = 4

; State 0: start state
0 1___ 1___ **** be_x
0 x___ x___ **** reject
0 =___ =___ **** reject
0 ____ ____ **** accept

; State be_x: while Before symbol'x',copy string to 2nd tape
be_x 1___ _1__ rr** be_x
be_x x___ ____ r*** af_x
be_x =___ =___ **** reject
be_x ____ ____ **** reject

; State af_x: while After symbol'x', copy string to 3rd tape
af_x 1___ __1_ r*r* af_x
af_x =___ ____ r*** af_e
af_x x___ x___ **** reject
af_x ____ ____ **** reject

; State af_e: while After symbol equal'=', copy string to 4th tape
af_e 1___ ___1 r**r af_e
af_e ____ ____ *lll ck1
af_e x___ x___ **** reject
af_e =___ =___ **** reject

; State ck1: From right to left scan 3rd tape
ck1 _111 _11_ **ll ck1
ck1 _11_ ____ **** reject
ck1 _1_1 ___1 *lr* ck2
ck1 _1__ ____ *l** ck2
ck1 __11 ____ **** reject
ck1 __1_ ____ **** reject
ck1 ___1 ____ **** reject
ck1 ____ ____ **** accept

; State ck2: From left to right scan 3rd tape
ck2 _111 _11_ **rl ck2
ck2 _11_ ____ **** reject
ck2 _1_1 ___1 *ll* ck1
ck2 _1__ ____ *l** ck1
ck2 __11 ____ **** reject
ck2 __1_ ____ **** reject
ck2 ___1 ____ **** reject
ck2 ____ ____ **** accept

; State accept*: write 'true' on 1st tape
accept ____ t___ r*** accept2
accept2 ____ r___ r*** accept3
accept3 ____ u___ r*** accept4
accept4 ____ e___ **** halt_accept

; State reject*: write 'false' on 1st tape
reject 1___ ____ r*** reject
reject x___ ____ r*** reject
reject =___ ____ r*** reject
reject ____ f___ r*** reject2
reject2 ____ a___ r*** reject3
reject3 ____ l___ r*** reject4
reject4 ____ s___ r*** reject5
reject5 ____ e___ **** halt_reject