map =: ("."0;._2) 1!:1 < 3

a =: > |.!.__@:(>./\)
smoutput +/^:_ (a +. a&.:|: +. a&.:|. +. a&.:(|.@:|:)) map

b1 =: dyad define"_ 0
    'i j' =. > y
    'h t' =. split j {"1 i }. x
    1 + (<: # t) <. h (<: i. 1:) t
)
b =: adverb : 'b1&.:(u`a:)'
'h w' =: $ map
smoutput >./^:_ map (]b * |.b * |:b * (|.@:|:)b) (i. h) <@,"0/ i. w
