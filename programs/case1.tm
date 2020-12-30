#Q = {0,1a,1b,2a,2b,cmp,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

#S = {a,b}

#G = {a,b,_,t,r,u,e,f,a,l,s}

#q0 = 0

#B = _

#F = {halt_accept,halt_reject}

#N = 3

; State 0: start state
0 a__ a__ *** 1a
0 b__ b__ *** reject
0 ___ ___ *** accept

; 1a
1a a__ _a_ rr* 1a
1a b__ _b_ rr* 1b
1a ___ ___ *** reject

; 1b
1b a__ __a r*r 2a
1b b__ _b_ rr* 1b
1b ___ ___ rr* reject

;2a 
2a a__ __a r*r 2a
2a b__ __b r*r 2b
2a ___ ___ *** reject

;2b 
2b a__ a__ *** reject
2b b__ __b r*r 2b
2b ___ ___ *ll cmp

;cmp
cmp _bb ___ *ll cmp
cmp _aa ___ *ll cmp
cmp _ba ___ *** reject
cmp _ab ___ *** reject
cmp _b_ ___ *** reject
cmp _a_ ___ *** reject
cmp __b ___ *** reject
cmp __a ___ *** reject
cmp ___ ___ *** accept

; State accept*: write 'true' on 1st tape
accept ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; State reject*: write 'false' on 1st tape
reject a__ ___ r** reject
reject b__ ___ r** reject
reject ___ f__ r** reject2
reject2 ___ a__ r** reject3
reject3 ___ l__ r** reject4
reject4 ___ s__ r** reject5
reject5 ___ e__ *** halt_reject
