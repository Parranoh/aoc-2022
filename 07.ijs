input =: <;._2 (1!:1) < 3

NB. build tree
name =: , < ''
size =: , 0
parent =: , 0
cwd =: ''
cwdix =: 0
cd =: monad define
    d =. 5 }. y
    if. d -: , '/' do.
        cwd =: ''
        cwdix =: 0
    elseif. d -: '..' do.
        cwd =: }: cwd
        cwdix =: cwdix { parent
    else.
        cwd =: cwd , < d
        i =. name i. < cwd
        if. i = # name do.
            name =: name , < cwd
            size =: size , 0
            parent =: parent , cwdix
        end.
        cwdix =: i
    end.
)
ls =: ]
file =: monad define
    's f' =. <;._1 ' ' , y
    s =. ". s
    p =. < cwd , < f
    if. -. p e. name do.
        name =: name , p
        size =: size , s
        parent =: parent , cwdix
    end.
)
dir =: [: file '0' , 3 }. ]
monad define input
    for_c. input do.
        cmd =: > c
        cd`ls`file`dir@.((4 3 $ '$ c$ l9zzdir') I. 3 {. cmd) cmd
    end.
)

NB. compute sizes of dirs
adjacency =: (= {&parent)"0/~ i. # parent
descendant =: adjacency (] +. +./ .*.)^:_ = i. # parent
du =: (size = 0) # descendant +/ .* size

smoutput +/ (#~ <:&100000) du
smoutput <./ (#~ >:&(_40000000 + {. du)) du
