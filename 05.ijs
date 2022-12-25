lines =: (<;._2) 1!:1 < 3
ix =: lines i. a:
stacks =: (#~ ~:&' ')each <"1 (1 = 4 | i. # stacks) # stacks =: |: > (ix - 1) {. lines
instr =: ". > 0 1 0 1 0 1 #"1 ;: > (ix + 1) }. lines
crane =: adverb define
for_i. x
do.
    'nm fi ti' =. i
    'fi ti' =. (fi,ti) - 1
    'moved fs' =. nm split > fi { y
    y =. y fi}~ < fs
    ts =. (u moved) , > ti { y
    y =. y ti}~ < ts
end.
{.&> y
)
echo instr |.crane stacks
echo instr ]crane stacks
