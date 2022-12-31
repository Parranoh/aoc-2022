input =: ];._2 (1!:1) < 3
height =: a. i. ]`('a'"0)`('z'"0)@.(({&1 2 0)@('SE'&i.)) input
f =: conjunction : '>: (v y) + _ * -. x <: >: u x'
relax =: [: <./"1 ] ,."_1 (|.!.__)f(|.!._) ,."_1 (1&(|.!.__))f(1&(|.!._)) ,."_1 (|.!.__"1)f(|.!._"1) ,."0 (1&(|.!.__)"1)f(1&(|.!._)"1)
smoutput ('E' = , input) # , height relax^:_ ('S' = input) { _ 0
smoutput ('E' = , input) # , height relax^:_ ('a' = input) { _ 0
