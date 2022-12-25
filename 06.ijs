input =: (#~ ~:&LF) 1!:1 < 3
solve =: dyad : 'x + x i.~ +/"1 ,"2 =/"1~ |: (i. x) |."0 _ y'
echo 4 solve input
echo 14 solve input
