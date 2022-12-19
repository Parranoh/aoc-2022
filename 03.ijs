lines =: (<;._2) 1!:1 < 3
find =: ({.~ -:&#) (e. # [) (}.~ -:&#)
misplaced =: {.@find@> lines
priority =: ' abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'&i.
smoutput +/ priority misplaced
smoutput +/ priority {."1 > ((e. # [)&.>/"1) ((3 %~ # lines) , 3) $ lines
