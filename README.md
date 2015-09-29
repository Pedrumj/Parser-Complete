# Parser Complete
This project is a complete Parser + Lexical Analyzer. The program receives as input a text file with source code inside. The lexical analyzer will convert the file into tokens recognizable by the parser. The parser will then convert the tokens to a parse tree. 

# Lexical Analyzer
The following Regular expressions are defined for the lexical analyzer:
<pre>
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
</pre>

They can  be found in the <b>RegDefs.c</b> file. The lexical analyzer then converts these regular expressions into parse trees. The parse trees are then converted to NFAs (Nondeterministic Finite Automata).

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

#Usage
Inside the Input folder there is a file with the name Input.txt. The user can input source code that matches the grammar inside this file. After running the program the parse tree will be generated. The Main() function can be found inside <b>Compiler_C.c</b>.

Examples of syntax the program will understand are:
<pre>
while(1=1){
   f=3;
}
if(1=1){
   j=34-23;
}

temp = 34*34;
if(3>=34){
   a = 3;
}else{ f = 3;}
</pre>

#Example
Considering the input below:
<pre>
if(1=1){
   j=34-23;
}
</pre>

The output parse tree would be:
<pre>
0
\1
\\2
\\\16
\\\32
\\\5
\\\\7
\\\\\10
\\\\\\8
\\\\\\\9
\\\\\\\\20
\\\\23
\\\\7
\\\\\10
\\\\\\8
\\\\\\\9
\\\\\\\\20
\\\33
\\\34
\\\1
\\\\4
\\\\\6
\\\\\\19
\\\\\\23
\\\\\7
\\\\\\10
\\\\\\\10
\\\\\\\\8
\\\\\\\\\9
\\\\\\\\\\20
\\\\\\\29
\\\\\\\8
\\\\\\\\9
\\\\\\\\\20
\\\\36
\\\35
</pre>

#Source Files:
<pre>
<b>General:</b>
Class_Generic.h: Generic functions for defining structs. 
Create_Object.h: Generic function for initializing structs.
LinkedList.h: Generic linked list data structure.
ParseTree.h: Generic parse tree data structure.
Stack.h: Generic stack data structure.
String.h: String operations.

<b>Lexical Analyzer:</b>
LexAnalyzer.h: Reads the input file and returns a list of lexems
LexDefs.h: Contains a list of regular definition that are used for determining lexems
RegDefs.h: Contains a list of regular expressions. Not all the regular expressions are lexems. The file also expands the regular expressions where another regular expression is used in side it
LexTree.h: Receives as input a regular expression and generates the associated parse tree
NFA.h: Receives as input a parse tree and generates the associated nondeterministic finite autamata (NFA).

<b>Parser:</b>
Automation.h: Generates the grammar automaton
Closure.h: Generates the closure based on the input production and dot position.
First.h: Generates the FIRST set for each production
Follow.h: Generates the FOLLOW set for each nonterminal
Grammar.h: Processes the grammar matrix
ParseTable.h: Generates the parse table.
SLRParser.h: Generates a parse tree based on the input grammar and input list of tokens
</pre>
# License
[The MIT License (MIT)](http://opensource.org/licenses/MIT)
