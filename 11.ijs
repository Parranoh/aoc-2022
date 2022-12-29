parse =: monad define"1
    'h s o c t f' =. y
    s =. ". (# '  Starting items: '            ) }. s
    o =.    (# '  Operation: new = '           ) }. o
    c =. ". (# '  Test: divisible by '         ) }. c
    t =. ". (# '    If true: throw to monkey ' ) }. t
    f =. ". (# '    If false: throw to monkey ') }. f
    s ; o ; c ; t ; f
)
'is ms' =: |: split"1 parse;._1 a: , <;._2 (1!:1) < 3

inspect =: adverb define"1 0
    old =. y
    u ". x
)
monkeys =: adverb define
    cs =. (# ms) $ 0
    for. i. x do.
        for_i. i. # ms do.
            l =. > i { y
            'o c t f' =. i { ms
            cs =. cs i}~ (# l) + i { cs
            y =. y i}~ a:
            'ts fs' =. l (#~ ; (#~ -.)) 0 = c | l =. o u inspect l
            y =. y t}~ < ts ,~ > t { y
            y =. y f}~ < fs ,~ > f { y
        end.
    end.
    */ 2 {. \:~ cs
)
lcm =: *./ > 1 {"1 ms

smoutput 20 <.@(%&3) monkeys is
smoutput 10000 lcm&| monkeys is
