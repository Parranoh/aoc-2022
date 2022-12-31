input =: ".@>@:(#~ ~:&(<'->'))@:(<;._1)@:(' '&,)each <;._2 (1!:1) < 3

'minx miny maxx maxy' =: (<./ , >./) > ,each/ input
minx =: minx - 2
sx =: 500 - minx
w =: 3 + maxx - minx
h =: >: maxy
map =: (h , w) $ 0
range =: monad define
    'ax ay bx by' =. , y
    if. ax = bx do.
        f =. ay <. by
        t =. >: ay >. by
        (ax - minx) ,~"0 f + i. t - f
    else.
        f =. (ax <. bx) - minx
        t =. >: (ax >. bx) - minx
        ay ,"0 f + i. t - f
    end.
)
monad define''
    for_p. input do.
        map =: 1 (<"1 > ,&.>/ 2 <@range\ > p)} map
    end.
)
sand =: dyad define
    'h w' =. $ y
    sy =. 0
    while. (sy < <: h) *. (0 <: x) *. (x < w) do.
        if. 0 = (< (>: sy) , x) { y do.
            sy =. >: sy
        elseif. 0 = (< (>: sy) , (<: x)) { y do.
            x =. <: x
            sy =. >: sy
        elseif. 0 = (< (>: sy) , (>: x)) { y do.
            x =. >: x
            sy =. >: sy
        else.
            y =. 2 (< sy , x)} y
            break.
        end.
    end.
    y
)

smoutput (+/^:_) 2 = sx sand^:_ map
smoutput (+/^:_) 2 = (sx + h) sand^:_ ((0 $~ h , h) ,. map ,. (0 $~ h , h)) , (0 $~ w + +: h) ,: (1 $~ w + +: h)
