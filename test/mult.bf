// cell[3] <- cell[0] * cell[1]

(3)>(5)<                 // cell0 <- (3), cell[1] <- (5)

>>[-]>[-]>[-]<<<<       // clear cell #2 and #3 and #4
[->
    [->+>+<<]           // add cell #1 to #2
    >>
        [-<<+>>]        // move cell #3 back to #1
        >+<             // copy cell #0 to #4
    <<
<]
>>>>[-<<<<+>>>>]<<<<    // move cell #4 back to #0

>>?                    // print cell[2]'s ascii