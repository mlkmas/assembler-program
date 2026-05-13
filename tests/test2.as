; test 2 - tests that externs and .ext file work
       .extern  EXTFUNC
       .entry   START
START:  mov     r1, r2
        jsr     EXTFUNC
        prn     #100
        stop
DATA1:  .data    1, 2, 3