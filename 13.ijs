replace =: (1&{)`(''''"1)`(''''"1)@.(('[]' -: }:) - ('[]' -: }.))"1@:(_1&|. ,. ] ,. 1&|.)
parse =: ,@:".@:(#~ ~:&' ')@:,@:(('(,(<'"0)`('),(<'"0)`('))'"0)`]@.('[,]'&i.))
input =: parse@:replace each (#~ ~:&a:) <;._2 (1!:1) < 3

order =: 1: = o =: dyad define
    if. 0 = # $ x do.
        if. 0 = # $ y do.
            * y - x
        else.
            (, < x) o y
        end.
    else.
        if. 0 = # $ y do.
            x o , < y
        else.
            if. (0 = # x) *. (0 = # y) do.
                0
            elseif. (0 = # x) do.
                1
            elseif. (0 = # y) do.
                _1
            else.
                'a as' =. split x
                'b bs' =. split y
                h =. (> a) o > b
                if. 0 = h do.
                    as o bs
                else.
                    h
                end.
            end.
        end.
    end.
)
pairs =: (2 ,~ -: # input) $ input

smoutput +/ (> order&.:>/"1 pairs) # >: i. # pairs
smoutput */ 1 2 + +/ input order&>/ parse@:replace each '[[2]]' ; '[[6]]'
