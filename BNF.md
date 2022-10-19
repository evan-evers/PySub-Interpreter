The following BNF production rules describe the grammar for the PySub Language, which is a subset of the Python Programming Language:   

<program>  ::= <lines>
  
<lines>  ::= <statement> | <block> | <lines><nl><lines> 
  
<statement>  ::= <print> | <conditional> | <loop> | <input> | <assignment> | <comment> 
  
<block>  ::= <whitespace><statement> | <block><nl><block> 
  
<print> ::= “print”(<print-param>) 
  
<print-param> ::= <string> | <expression> | <print-param>, <print-param> 
  
<conditional> ::= “if” <expression> “:”<nl><whitespace><lines><nl>[“elif” <expression> “:”<nl><whitespace><lines><nl>][“else” “:”<nl><whitespace><lines><nl>] NOTE:  <whitespace> for indent must be exactly the same within the same block 
  
<loop> ::= “while” <expression> “:”<nl><whitespace><lines><nl>  NOTE: <whitespace> for indent must be exactly the same within the same block 
  
<input> ::= <identifier> = “int”(“input”(<string>)) 
  
<assignment> ::= <identifier> = <expression> 
  
<comment> ::= # <text> 
  
<expression> ::= <number> | <identifier> | <expression> <operator> <expression> (NOTE:  for the unary “not” operator, it is: <operator> <expression>) 
  
<nl>  ::= ‘\n’(i.e. new line, so that anything following would be on a separate line) 
  
<whitespace> ::= ‘ ‘|\t’|<whitespace><whitespace>  (i.e. blank or tab or combinations of blanks and tabs) 
  
<relational-op>  ::= “<” | “>” | “<=” | “>=” | “==” | “!=” 
  
<logical-op> ::= ”and” | ”or” | ”not” 
  
<arith-op>  ::= “+” | “-“ | “*” | “/” | “%” 
  
<operator> ::= <relational-op>|<logical-op>|<arith-op> 
  
<identifier> ::= <letter>|<letter><identifier> 
  
<string> ::= ‘”’ <text> ‘”’ | “’” <text> “’” 
  
<text> ::= <character> | <character> <text> 
  
<character> ::= <letter> | <digit> | <symbol> 
  
<number> ::= <digit> | <digit> <number> 
  
<letter> ::= “A” | “B” | “C” | “D” | “E” | “F” | “G” | “H” | “I” | “J” | “K” | “L” | “M” | “N” | “O” | “P” | “Q” | “R” | “S” | “T” | “U” | “V” | “W” | “X” | “Y” | “Z” | “a” | “b” | “c” | “d” | “e” | “f” | “g” | “h” | “i” | “j” | “k” | “l” | “m” | “n” | “o” | “p” | “q” | “r” | “s” | “t” | “u” | “u” | “v” | “w” | “x” | “y” | “z” 
  
<digit> ::= “0” | “1” | “2” | “3” | “4” | “5” | “6” | “7” | “8” | “9” 
  
<symbol> :: = “!” | “@” | “#” | “$” | “%” | “^” | “&” | “*” | “(“ | “)” | “_” | “+” | “~” | “-“ | “=” | “{“ | “}” | “[“ | “]” | “|” | “\” | “,” | “:” | “;” | “<” | “>” | “,” | “.” | “?” | “/” 

  
NOTE:  Square brackets [] indicate optional items 
