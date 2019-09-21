// cell[3] <- cell[0] + cell[1]

(10)>(12)<          // cell0 <- (10), cell[1] <- (12)

>>[-]>[-]<<<        // clear cell #2 and #3
[->>+>+<<<]         // transfer cell #0 to #2 and #3
>
    >>[-<<<+>>>]<<  // transfer cell #3 to #0
    [->+>+<<]       // transfer cell #1 to #2 and #3
    >>[-<<+>>]<<    // transfer cell #3 to #1
<

>>?                // print cell[2]'s ascii