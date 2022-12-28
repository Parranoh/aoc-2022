R =:  1 0&;
L =: _1 0&;
U =: 0  1&;
D =: 0 _1&;
input =: ".;._2 (1!:1) < 3

s =: 10 2 $ 0
a =: ,: 1 { s
b =: ,: 9 { s
monad define''
    for_i. input do.
        'd ns' =. i
        for. i. ns do.
            s =: (d + {. s) 0} s
            for_j. i. <: # s do.
                'h t' =. 2 {. j }. s
                if. +./ 1 < | h - t do.
                    s =: (t + * h - t) (>: j)} s
                end.
            end.
            a =: a , 1 { s
            b =: b , 9 { s
        end.
    end.
)

smoutput # ~. a
smoutput # ~. b
