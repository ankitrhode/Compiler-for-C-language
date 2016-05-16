

// There may be some flexer directives here

// TODO : String literals and correcting float constants

%%
[ \t]                                    //skip
"//"[^\r\n]*							 //skip
[\n]										{line_count++;}
"void"										return Parser::VOID;
"return"									return Parser::RETURN;
"int"										return Parser::INT;
"float"										return Parser::FLOAT;
"struct"									return Parser::STRUCT;
"string"									return Parser::STRING_LITERAL;

(([1-9][0-9]*)|(0[bB][01]*)|(0[0-7]*)|(0[xX][0-9a-fA-F]*))				           {Parser::d_val__ = std::string (matched()); return Parser::INT_CONSTANT;}

(([0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)|([0-9]+\.?[0-9]*([eE][-+]?[0-9]+)?))		{Parser::d_val__ = std::string (matched());return Parser::FLOAT_CONSTANT;}
\"(\\[\\\"nt]|[^\"\\])*\"					{Parser::d_val__ = std::string (matched());return Parser::STRING_LITERAL;}

"if"										return Parser::IF;
"else"										return Parser::ELSE;
"while"										return Parser::WHILE;
"for"										return Parser::FOR;

[a-zA-Z_][a-zA-Z0-9_]*		                {Parser::d_val__ = std::string (matched());return Parser::IDENTIFIER;}

"++"										return Parser::INC_OP;

"||"										return Parser::OR_OP;
"&&"										return Parser::AND_OP;
"=="										return Parser::EQ_OP;	
"!="										return Parser::NE_OP;
"<"											return '<';
">"											return '>';
"<="										return Parser::LE_OP;
">="										return Parser::GE_OP;
"->"										return Parser::PTR_OP;

"="											return '=';
"["											return '[';
"]"											return ']';

"+"                                         return '+';
"-"											return '-';
"*"											return '*';
"/"											return '/';

","                                         return ',';
"("                                         return '(';
")"                                         return ')';

";"											return ';';
"{"											return '{';
"}"											return '}';
"!"											return '!';
"."  										return '.';
"&"											return '&';
.                                           return Parser::OTHERS;                                          

