# Parser Complete
This project is a complete Parser + Lexical Analyzer. The program receives as input a text file with source code inside. The lexical analyzer will convert the file into tokens recognizable by the parser. The parser will then convert the tokens to a parse tree. 

# Lexical Analyzer
The following Regular expressions are defined for the lexical analyzer:
delim                             [ \t\n]
ws                                {delim}+
letter                            [A-Za-z]
digit                             [0-9]
id                                {letter}({letter}|{digit})*
number                            {digit}+(\\.{digit}+)?(E[\\+\\-]?{digit}+)?
IF                                if
THEN                              then 
ELSE                              else
LT                                <
LE                                <=
EQ                                =
NE                                <>        
GT                                >
GE                                >=
WHILE                             while

They can  be found in the RegDefs.c file. The lexical analyzer then converts these regular expressions into parse trees. The parse trees are then converted to NFAs (Nondeterministic Finite Automata).

#Parser
The parser is an SLR parser. The following grammar is defined for the parser:
<pre>
S'->S
S->SF;|SW;|SA;|F;|W;|A;
F->&lt;IF>(Expr){S}|&lt;IF>(Expr){S}&lt;ELSE>{S}
W->&lt;WHILE>(Expr){S}
A->LR
L->&lt;ID>&lt;EQ>
Expr->R&lt;LT>R|R&lt;LE>R|R&lt;EQ>R|R&lt;NE>R|R&lt;GT>R|R&lt;GE>R
R->T
T->T+E|T-E|E
E->E*Q|E/Q|Q
Q->(R)|&lt;NUMBER>|&lt;ID>
</pre>

The following are the nonterminals along with their associated token ID:
<pre>
[S',     0]
[S,      1]
[F,      2]
[W,      3]
[A,      4]
[Expr,   5]
[L,      6]
[R,      7]
[E,      8]
[Q,      9]
[T,     10]
</pre>

and the following are the terminals along with their associated token ID:
<pre>
[WS     15]
[IF     16]
[THEN   17]
[ELSE   18]
[ID     19]
[NUMBER 20]
[LT     21]
[LE     22]
[EQ     23]
[NE     24]
[GT     25]
[GE     26]
[WHILE  27]
[+      28]
[-      29]
[*      30]
[/      31]
[(      32]
[)      33]
[{      34]
[}      35]
[;      36]
</pre>

11~14 are reserved for future nonterminals. 



<link>http://jtech-online.com/?page_id=492</link>
# License

[The MIT License (MIT)](http://opensource.org/licenses/MIT)
