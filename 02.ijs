lines =: (<;._2) 1!:1 < 3
opponent =: ('ABC'&i.)@{.@> lines
choice =: ('XYZ'&i.)@{:@> lines
smoutput +/ 1 + choice + 3 * 3 | 1 - opponent - choice
smoutput +/ (3 * choice) + 1 + 3 | opponent + choice - 1
