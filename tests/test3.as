; test 3 - error cases (should produce no output files)
MAIN:   add     r1, r2
        bogus_op r3
        bne     nowhere
END:    stop