noop =: 1 0
addx =: 2&,
input =: ".;._2 (1!:1) < 3

X =: 1
h =: s =: ''
monad define''
    for_tV. input do.
        't V' =. tV
        for. i. t do.
            h =: h , X
            s =: s , '.#' {~ 1 >: | X - 40 | # s
        end.
        X =: X + V
    end.
)

smoutput +/ (>: * {&h) (#~ 40&| = 19"0) i. # h
smoutput 6 40 $ s
