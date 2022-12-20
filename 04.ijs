input =: (".;._1)@('-'&,)@>@(<;._1)@(','&,)@> (<;._2) 1!:1 < 3
al =: {.@:{."2
au =: {:@:{."2
bl =: {.@:{:"2
bu =: {:@:{:"2
smoutput +/ (((al <: bl) *. (bu <: au)) +. ((bl <: al) *. (au <: bu))) input
smoutput +/ -. ((au < bl) +. (bu < al)) input
