
%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP GE_OP LE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN STRUCT IDENTIFIER INC_OP PTR_OP OTHERS

%polymorphic INT: int; STRING: std::string; FLOAT: float; NODE: abstract_astnode *; EXPNODE: ExpAst*; STMTNODE: StmtAst*; SEQ: Seq*; REFNODE: RefAst*; VECEXP: vector < ExpAst* >; FUN_DECLARATOR: pair < string, vector < pair <string, string> > >; PARAMETER_LIST: vector < pair <string, string> >; PARAMETER_DECLARATION: pair <string, string>; DECLARATOR_LIST: vector < string >

%type <SEQ> statement_list
%type <STMTNODE> statement
%type <STMTNODE> assignment_statement
%type <EXPNODE> expression
%type <EXPNODE> logical_and_expression
%type <EXPNODE> logical_or_expression
%type <EXPNODE> equality_expression
%type <EXPNODE> relational_expression
%type <EXPNODE> additive_expression
%type <EXPNODE> multiplicative_expression
%type <EXPNODE> unary_expression
%type <EXPNODE> postfix_expression
%type <EXPNODE> primary_expression

%type <VECEXP> expression_list
%type <STMTNODE> selection_statement
%type <STMTNODE> iteration_statement
%type <STRING> unary_operator
%type <STRING> INT_CONSTANT
%type <STRING> FLOAT_CONSTANT
%type <STRING> STRING_LITERAL
%type <STRING> IDENTIFIER
%type <STRING> type_specifier
%type <FUN_DECLARATOR> fun_declarator
%type <PARAMETER_LIST> parameter_list
%type <PARAMETER_DECLARATION> parameter_declaration
%type <STRING> declarator
%type <DECLARATOR_LIST> declarator_list

%%

translation_unit_final
		: translation_unit {print_info();}
		;

translation_unit 
        :  struct_specifier
 	| function_definition 
 	| translation_unit function_definition 
        | translation_unit struct_specifier
        ;

struct_specifier 
        : STRUCT IDENTIFIER {
        if(globalSymbolTable.find($2)!=globalSymbolTable.end()){

        cout << "ERROR at line: " << line_count << " : redefinition of function/struct\n"; err=true; ABORT();
        
        }

        addToGlobalSymbolTable($2, "struct", "", 0);offset=0;current_func=$2;current_func_rettype="";} '{' declaration_list '}' ';'	{globalSymbolTable[$2]->sz=offset;}
        ;

function_definition
	: type_specifier fun_declarator 
	{
		string func_name = "";
		int i;
		string st = ($2).first;
		for(i = 0 ; (i < st.length()) && (st[i] != '*') ; i++)
			func_name += st[i];
		current_func = func_name;												//global variables current_func and offset instead of 																			// inherited attributes
		string type = $1 + st.substr(i);
        current_func_rettype=type;
		
		addToGlobalSymbolTable(func_name, "function", type, 0);

		offset = 0;
		vector < pair <string, string> > params = ($2).second;

		for(i = 0 ; i < params.size() ; i++) {

		string param_name = (params[i]).first;
		string param_type = (params[i]).second;
		param_type = param_type.substr(0, param_type.size()-1);
		
		int sz;																// for filling sz field in local symtab
		if(param_type.compare("int")==0)sz=4;
		else if(param_type.compare("float")==0)sz=4;
		else if(param_type.find(',')!=string::npos || param_type.find('*')!=string::npos)sz=4;	//arrays passed by pointer reference, 
																								//asssuming pointer size to be 4 bytes 
																								//on 64-bit machine
		else { 
			map <string, GlobalSymbolTableEntry *> :: iterator it ;
			it = globalSymbolTable.find(param_type);
			if(it==globalSymbolTable.end())	
            {
                cout << "ERROR at line: " << line_count << " : no such function/struct exists\n"; err=true; ABORT();
            }
			sz = globalSymbolTable[param_type] -> sz;
		}
		addToLocalSymbolTable(func_name, param_name, true, param_type, sz, offset);
		offset+=sz;

		}
	}
	compound_statement 
	;

type_specifier                   // This is the information 
        : VOID 		    		 {$$="void";}
        | INT   			 {$$="int";}
		| FLOAT 			 {$$="float";}
        | STRUCT IDENTIFIER  {$$=$2;}
        ;

fun_declarator
	: IDENTIFIER '(' parameter_list ')'
	{ 
		pair < string, vector < pair <string, string> > > tmp;
		tmp.first = $1;
		tmp.second = $3;
		$$ = tmp;
	} 

	| IDENTIFIER '(' ')' 
	{ 
		pair < string, vector < pair <string, string> > > tmp;
		tmp.first = $1;
		$$ = tmp;
	}

    | '*' fun_declarator
    {
    	pair < string, vector < pair <string, string> > > tmp;
		tmp.first = ($2).first + "*";
		tmp.second = ($2).second;
		$$ = tmp;	
    }
	;


parameter_list
	: parameter_declaration 
	{ 
		vector < pair <string, string> > tmp;
		tmp.push_back($1);
		$$ = tmp;
	}
	| parameter_list ',' parameter_declaration 
	{ 
		vector < pair <string, string> > tmp = $1;
		tmp.push_back($3);
		$$ = tmp; 
	}
	;

parameter_declaration
	: type_specifier declarator 
	{
		string type = $1;
		int i;
		for(i = 0 ; $2[i] != ',' ; i++)
			type = type + "*";
		string var_name = "";
		i++;
		for(; $2[i] != ',' ; i++)
			var_name += $2[i];
		type = type + ($2).substr(i);
        if(type == "void") {
            cout << "ERROR at line: " << line_count << " : cannot have a parameter of void type\n"; 
            err=true; ABORT();
        }
		$$ = make_pair(var_name, type);
	}
        ;

declarator
	: IDENTIFIER 
	{ 
		$$ = "," + $1 + ",";
	}
	| declarator '[' primary_expression ']' 
	{	
		if($3->type.compare("int")==0 && $3->vali>=0)
		{string t = "" + to_string($3->vali);
		$$ = $1 + t + ",";}
		else {cout<< "ERROR at line: " << line_count << ": Bad array index\n" ;err=true; ABORT();}
	}
	| '*' declarator 
	{
		$$ = "*" + $2;
	}
        ;

primary_expression 
        : IDENTIFIER 
        {
            string t = getType($1);
            if(t != "-") {
                $$ = new IdentifierRef($1); 
                $$ -> type = t;$$->is_lvalue=true;
                $$->is_local=true;
            }
            else {
                cout << "ERROR at line: " << line_count << " : Unknown identifier\n";
                err=true; ABORT();
            }
        }
        | INT_CONSTANT { IntConst* tmp = new IntConst($1); tmp->type="int";tmp->vali=stoi($1); $$=tmp; }
        | FLOAT_CONSTANT { FloatConst * tmp = new FloatConst($1); tmp->type="float";tmp->valf=stof($1); $$=tmp; }
        | STRING_LITERAL { $$ = new StringConst($1); $$ -> type = "string";}
        | '(' expression ')' { $$ = $2; }
        ;

compound_statement
	: '{' '}' 
	| '{' statement_list '}' 	{ globalSymbolTable[current_func]->ast = $2;}
        | '{' declaration_list statement_list '}' {	globalSymbolTable[current_func]->ast = $3;}
	;

statement_list
	: statement	{ vector < StmtAst * > tmp; tmp.push_back($1); $$ = new Seq(tmp); }
        | statement_list statement	{ vector < StmtAst*> tmp = $1->statementList; tmp.push_back($2); $$ = new Seq(tmp);}
	;

statement
        : '{' statement_list '}' { $$ = $2; }
        | selection_statement 	{ $$ = $1; }
        | iteration_statement 	{ $$ = $1; }
		| assignment_statement	{ $$ = $1; }
        | RETURN expression ';'	{ 

        if(!checkParameterCompatibility(current_func_rettype, $2)){
            cout<<"ERROR at line: " << line_count << " : Return type of function incompatible\n"; err=true; ABORT();
        }

        if($2->is_local_addr || $2->type.find(',')!=string::npos){
        cout<<"ERROR at line: " << line_count << " : cannot return address of a local variable\n"; err=true; ABORT();
        }
        
        $$ = new Return($2); }
        ;

assignment_statement
	: ';' 	{ $$ = new Empty(); }
	|  expression ';'  { $$ = new Ass($1); } 
	;

expression
        :  logical_or_expression { $$ = $1; }
        |  unary_expression '=' expression 
        {   
            ExpAst* tmp = new BinaryExpAst($1, $3, "=");
            if(!($1->is_lvalue)){

                cout<<"ERROR at line: " << line_count << " :  lvalue required as left operand of assignment\n"; err=true; ABORT();
            }
            
            string t1 = $1 -> type;
            string t2 = $3 -> type;
            if(!typeCompatible(t1, t2)) {

                if(t1.find("*")!=string::npos && t1.find(",")==string::npos && t2=="int" && $3->vali==0);
                else {
                    cout<<"ERROR at line: " << line_count << " : Type incompatible\n"; 
                    err=true; 
                    ABORT();
                }
            }
            tmp -> type = $1 -> type;
            if($1->type.compare("int")==0 && $3->type.compare("float")==0)$3->to_int=true;
            if($1->type.compare("float")==0 && $3->type.compare("int")==0)$3->to_float=true;
            $$ = tmp;
        }
        ;

logical_or_expression
	: logical_and_expression { $$ = $1; }
        | logical_or_expression OR_OP logical_and_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "OR_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali||$3->vali;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali||$3->valf;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf||$3->vali;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf||$3->valf;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to ||\n"; err=true; ABORT();
            
        }
        else{}

        $$=tmp;
		}
	;
logical_and_expression
        : equality_expression { $$ = $1; }
        | logical_and_expression AND_OP equality_expression { 

        ExpAst* tmp = new BinaryExpAst($1, $3, "AND_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali&&$3->vali;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali&&$3->valf;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf&&$3->vali;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf&&$3->valf;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }
        else{}

        $$=tmp;
		}
	;

equality_expression
	: relational_expression { $$ = $1; }
        | equality_expression EQ_OP relational_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "EQ_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali==$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali==$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf==$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf==$3->valf;
         tmp->is_floatop=true;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) &&\n"; err=true; ABORT();

        }

        else{}

        $$=tmp;
		}	
	| equality_expression NE_OP relational_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "NE_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali!=$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali!=$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf!=$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf!=$3->valf;
         tmp->is_floatop=true;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) &&\n"; err=true; ABORT();

        }

        else{}

        $$=tmp;
		}
	;
relational_expression
	: additive_expression { $$ = $1; }
        | relational_expression '<' additive_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "<");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali<$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali<$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf<$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf<$3->valf;
         tmp->is_floatop=true;}

         else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) &&\n"; err=true; ABORT();

        }

        else{tmp->is_intop=true;}

        $$=tmp;
		}
	| relational_expression '>' additive_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, ">");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali>$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali>$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf>$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf>$3->valf;
         tmp->is_floatop=true;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) &&\n"; err=true; ABORT();

        }

        else{tmp->is_intop=true;}

        $$=tmp;
		}
	| relational_expression LE_OP additive_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "LE_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali<=$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali<=$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf<=$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf<=$3->valf;
         tmp->is_floatop=true;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) &&\n"; err=true; ABORT();

        }

        else{tmp->is_intop=true;}

        $$=tmp;
		}

        | relational_expression GE_OP additive_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "GE_OP");tmp->type="int";

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->vali>=$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->vali>=$3->valf;
         tmp->is_floatop=true; $1->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->vali=$1->valf>=$3->vali;
         tmp->is_floatop=true; $3->to_float=true;}

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->vali=$1->valf>=$3->valf;
         tmp->is_floatop=true;}

        else if(($1->type.find("*")==string::npos && $1->type.find(",")==string::npos) || ($3->type.find("*")==string::npos && $3->type.find(",")==string::npos)){

            cout<<"ERROR at line: " << line_count << " : Incompatible operand to &&\n"; err=true; ABORT();
            
        }

        else if(process($1->type)!=process($3->type)){

         cout<<"ERROR at line: " << line_count << " : Incomparable types (pointers must also be of same type) \n"; err=true; ABORT();

        }

        else{tmp->is_intop=true;}

        $$=tmp;
		}
	;

additive_expression 
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "+");

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->type="int";tmp->vali=$1->vali+$3->vali;
         tmp->is_intop=true;}

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->vali+$3->valf;
         tmp->is_floatop=true; $1->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->type="float";tmp->valf=$1->valf+$3->vali;
         tmp->is_floatop=true; $3->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->valf+$3->valf;
         tmp->is_floatop=true; }

        else if(($1->type.find("*")==string::npos || $1->type.find(",")==string::npos) && $3->type=="int")
        {tmp->type=process($1->type);tmp->is_intop=true;}

        else if(($3->type.find("*")==string::npos || $3->type.find(",")==string::npos) && $1->type=="int")
        {tmp->type=process($3->type);tmp->is_intop=true;}

        else{
            
            cout<<"ERROR at line: " << line_count << " : Incompatible operand to +\n"; err=true; ABORT();            
        
        }

        $$=tmp;
		}
	| additive_expression '-' multiplicative_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "-");

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->type="int";tmp->vali=$1->vali-$3->vali;
         tmp->is_intop=true; }

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->vali-$3->valf;
         tmp->is_floatop=true; $1->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->type="float";tmp->valf=$1->valf-$3->vali;
         tmp->is_floatop=true; $3->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->valf-$3->valf;
         tmp->is_floatop=true; }

        else if(($1->type.find("*")==string::npos || $1->type.find(",")==string::npos) && $3->type=="int")
        {tmp->type=process($1->type);tmp->is_intop=true;}

        else if(($3->type.find("*")==string::npos || $3->type.find(",")==string::npos) && $1->type=="int")
        {tmp->type=process($3->type);tmp->is_intop=true;}

        else{
            
            cout<<"ERROR at line: " << line_count << " : Incompatible operand to -\n"; err=true; ABORT();            
        
        }

       
        $$=tmp;
		}
	;

multiplicative_expression
	: unary_expression { $$ = $1; }
	| multiplicative_expression '*' unary_expression 
	{ 
        ExpAst* tmp = new BinaryExpAst($1, $3, "*");

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->type="int";tmp->vali=$1->vali*$3->vali;
         tmp->is_intop=true; }

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->vali*$3->valf;
         tmp->is_floatop=true; $1->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->type="float";tmp->valf=$1->valf*$3->vali;
         tmp->is_floatop=true; $3->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->valf*$3->valf;
         tmp->is_floatop=true; }

        else{

        cout<<"ERROR at line: " << line_count << " : Incompatible operand to *\n"; err=true; ABORT();  

        }

        $$=tmp;
		}
	| multiplicative_expression '/' unary_expression { 
        ExpAst* tmp = new BinaryExpAst($1, $3, "/");

        if($1->type.compare("int")==0 && $3->type.compare("int")==0)
        {tmp->type="int";tmp->vali=$1->vali/$3->vali;
         tmp->is_intop=true; }

        else if($1->type.compare("int")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->vali/$3->valf;
         tmp->is_floatop=true; $1->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("int")==0)
        {tmp->type="float";tmp->valf=$1->valf/$3->vali;
         tmp->is_floatop=true; $3->to_float=true; }

        else if($1->type.compare("float")==0 && $3->type.compare("float")==0)
        {tmp->type="float";tmp->valf=$1->valf/$3->valf;
         tmp->is_floatop=true; }

        else{

        cout<<"ERROR at line: " << line_count << " : Incompatible operand to /\n"; err=true; ABORT();  

        }

        $$=tmp;
		}
	;
unary_expression
	: postfix_expression { $$ = $1; }				
	| unary_operator unary_expression { 
        if(($1).compare("-")==0)
        {
            ExpAst* tmp = new UnaryExpAst($2, $1);

            if($2->type.compare("int")==0)
            {tmp->type="int";tmp->vali=-($2->vali);}

            else if($2->type.compare("float")==0)
            {tmp->type="float";tmp->valf=-($2->valf);}

            else{cout << "ERROR at line: " << line_count << " : invalid argument to unary '-' \n"; err=true; ABORT();}

        $$=tmp;
        }

        else if(($1).compare("!")==0)
        {
            ExpAst* tmp = new UnaryExpAst($2, $1);
            tmp->type="int";

            if($2->type.compare("int")==0)
            {tmp->vali=!($2->vali);}

            else if($2->type.compare("float")==0)
            {tmp->vali=!($2->valf);}

            else if($2->type.find("*")==string::npos && $2->type.find(",")==string::npos){

            cout << "ERROR at line: " << line_count << " : invalid argument to unary '!' \n"; err=true; ABORT();

            }

            else{}

        $$=tmp;
        }

        else if(($1).compare("&")==0)
        {   
            if(!$2->is_lvalue){
            cout << "ERROR at line: " << line_count << " : lvalue required as opernd to '&' \n"; err=true; ABORT();
            }
            
            ExpAst* tmp = new Pointer((RefAst*)$2);

            if($2->type.find(',') != std::string::npos)
            {tmp->type= "*" + $2->type;}

            else 
            {tmp->type=$2->type+"*";}

        ;    
        if($2->is_local)tmp->is_local_addr=true;
        $$=tmp;
        }

        else 
        {
            ExpAst* tmp = new DeRef((RefAst*)$2);
            string type= $2->type;
            string ans = "";
            int p1, p2;
            p1 = type.find(',');

            if(type[0]=='*'){tmp->type=type.substr(1);tmp->is_local=true;}
            else if(p1 == std::string::npos)
            {
                if(type.find('*')==std::string::npos)
                {cout << "ERROR at line: " << line_count << " : invalid argument of unary '*' \n"; err=true; ABORT();}
                tmp->type=type.substr(0,type.length()-1);
                if($2->is_local_addr)tmp->is_local=true;
            }
            else{
            ans += type.substr(0, p1);
            type = type.substr(p1 + 1);
            p2 = type.find(',');
            if(p2 == std::string::npos)
            tmp->type= ans;
            else
            tmp->type= ans + type.substr(p2);
            tmp->is_local=true;
            }

            if(tmp->type=="void"){
            cout << "ERROR at line: " << line_count << " : cannot dereference void * \n"; err=true; ABORT();
            }
            tmp->is_lvalue=true;
            $$=tmp;

        }

        }

    ;

postfix_expression
	: primary_expression  		{ $$ = (UnaryExpAst*) $1; } 	
        | IDENTIFIER '(' ')' 	
        {   
            string t = getTypeGlobal($1);
            int correct = checkParameters(vector < ExpAst*>(), $1);
            if(t != "-" && correct == 1) {
                $$ = new Funcall($1, vector < ExpAst*>());
                $$ -> type = t;
               
            }
            else {
                if(t == "-"){
                cout << "Function '" << $1 << "' not found\n"; err=true; ABORT();}
                if(correct == 0)
                    {cout<<"ERROR at line: " << line_count << " : Parameter mismatch\n"; err=true; ABORT();}
                
            }
        }					    // Cannot appear on the LHS of '='. Enforce this.

	    | IDENTIFIER '(' expression_list ')'    
        { 
            string t = getTypeGlobal($1);
            int correct = checkParameters($3, $1);
            if(t != "-" && correct == 1) {
                $$ = new Funcall($1, $3); 
                $$ -> type = t;
               
            }
            else {
                if(t == "-")
                    {cout<<"ERROR at line: " << line_count << " : No such function found\n"; err=true; ABORT();}
                if(correct == 0)
                    {cout<<"ERROR at line: " << line_count << " : Parameter mismatch\n";
                err=true; ABORT();}
            }
        }// Cannot appear on the LHS of '='  Enforce this.
        | postfix_expression '[' expression ']'	
        {
            if(!($1->is_lvalue)){
            cout << "ERROR at line: " << line_count << " lvalue required\n";
            err=true; ABORT();
            }
            if(!($3->type=="int")){
            cout << "ERROR at line: " << line_count << " array index should be an integer\n";
            err=true; ABORT();
            }
            string t = $1 -> type + ',';
            int first, second, i;
            for(i = 0 ; t[i] != ','; i++);
            first = i;
            i++;
            for(; i < t.length() && t[i] != ','; i++);
            second = i;
            if(first >= t.length() || second >= t.length() || second == first + 1) {
                if(t.find("*")==string::npos){
                cout << "ERROR at line: " << line_count << " Type mismatch\n";
                err=true; ABORT();
                }
                else{
                t=t.substr(0,t.length()-2);
                }
            }
            else{
            t = t.substr(0, first) + t.substr(second);
            t=t.substr(0,t.length()-1);
            }

            $$ = new ArrayRef((RefAst*)$1, $3);
            $$->type=t; 
            $$->is_lvalue=true;
            $$->is_local=true;
        }
        | postfix_expression '.' IDENTIFIER     
        {   
            if(!($1->is_lvalue)){
            cout << "ERROR at line: " << line_count << " lvalue required\n";
            err=true; ABORT();
            }
            string t = getTypeStruct($3, $1 -> type);
            if(t != "-") {
                IdentifierRef* member= new IdentifierRef($3);
                $$ = new Member((RefAst*)$1, member); 
                $$ -> type = t;
                $$->is_lvalue=true;
                $$->is_local=true;
            }
            else {
                cout << "ERROR at line: " << line_count << " Struct '" << $1 -> type << "' has no attribute named '" << $3 << "'\n";
                err=true; ABORT();
            }
        }
        | postfix_expression PTR_OP IDENTIFIER  
        { 
            if(!($1->is_lvalue)){
            cout << "ERROR at line: " << line_count << " lvalue required\n";
            err=true; ABORT();
            }
            string t = $1 -> type;
            int cnt = 0;
            for(int i = 0 ; i < ($1 -> type).length() ; i++) {
                if(($1 -> type)[i] == '*')
                    cnt++;
            }
            if(cnt != 1) {
                cout << "Type mismatch\n";
                err=true; ABORT();
            }
            string tmp = $1 -> type;
            tmp = tmp.substr(0, tmp.length() - 1);
            t = getTypeStruct($3, tmp);
            if(t != "-") {
                IdentifierRef* member= new IdentifierRef($3);
                $$ = new Member((RefAst*)$1, member); 
                $$ -> type = t;
                $$->is_lvalue=true;
                $$->is_local=true;
            }
            else {
                cout << "ERROR at line: " << line_count << " Struct '" << $1 -> type << "' has no attribute named '" << $3 << "'\n";
                err=true; ABORT();
            }
        }
	    | postfix_expression INC_OP { 
        if(!($1->is_lvalue)){
            cout << "ERROR at line: " << line_count << " lvalue required for INC_OP\n";
            err=true; ABORT();
        }

        if($1->type=="int"||$1->type=="float"||($1->type.find(",")==string::npos && $1->type.find("*")!=string::npos))
        {$$ = new UnaryExpAst($1, "INC_OP"); $$->type=$1->type;  } // Cannot appear on the LHS of '='   Enforce this

        else{
        cout << "ERROR at line: " << line_count << " Wrong argument to INC_OP\n";
        err=true; ABORT();
        }

        }
	;


        

expression_list
        : expression { vector < ExpAst* > tmp; tmp.push_back($1); $$ = tmp; }
        | expression_list ',' expression { vector <ExpAst*> tmp = $1; tmp.push_back($3); $$ = tmp; }
	;

unary_operator
        : '-'	{ $$ = "-"; }
		| '!'	{ $$ = "!"; }
        | '&'   { $$ = "&"; }
        | '*' 	{ $$ = "*"; }
	;

selection_statement
        : IF '(' expression ')' statement ELSE statement { $$ = new If($3, $5, $7); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = new While($3, $5); }
	| FOR '(' expression ';' expression ';' expression ')' statement { $$ = new For($3, $5, $7, $9); }
        ;
	

declaration_list
        : declaration  					
        | declaration_list declaration
	;

declaration
	: type_specifier declarator_list';' 
	{

		for(int i=0; i<($2).size();i++)
			{
				string type = $1,base_type = $1;
				string declarator = ($2)[i];
				int j; bool is_pointer = false; int product=1;
				for( j = 0 ; declarator[j] != ',' ; j++)
					{ type = type + "*"; is_pointer = true; }	
					string var_name = "";
				j++;
				for(; declarator[j] != ',' ; j++)
					var_name += declarator[j];

                if((globalSymbolTable[current_func] -> symtab).find(var_name) != (globalSymbolTable[current_func] -> symtab).end())
                {
                    cout << "ERROR at line: " << line_count << " : redefinition of local variable/parameter\n"; 
                    err=true; ABORT();
                } 
				type = type + declarator.substr(j);
				type = type.substr(0, type.size()-1);

                if(type == "void") {
                cout << "ERROR at line: " << line_count << " : cannot have a parameter of void type\n"; 
                err=true; ABORT();
                }

                if(type.compare(current_func)==0){
                cout << "ERROR at line: " << line_count << " : cannot use object of struct being declared in declaration\n"; 
                err=true; ABORT();
                }

				j++;
				while(j<declarator.length())					//calculating size of an if variable is array ,eg int a[5] has size 20
					{
						string index="";
						for(; declarator[j] != ',' ; j++)
						{
							index+=declarator[j];
							
						}
                        int num= stoi(index);
                        product*=num;
						j++;
					}
				int sz;
				if(is_pointer){
					sz=4*product;
				}
				else if(base_type.compare("int")==0){
					sz=4*product;
				}
				else if(base_type.compare("float")==0){
					sz=4*product;
				}

				else{	

						map <string, GlobalSymbolTableEntry *> :: iterator it ;
						it = globalSymbolTable.find(base_type);
						if(it==globalSymbolTable.end())	{cout<<"ERROR at line: " << line_count << " : No such function or struct found\n"; err=true; ABORT();}
						sz = (globalSymbolTable[base_type] -> sz)*product;
					}
				addToLocalSymbolTable(current_func, var_name, false, type, sz, offset);
				offset+=sz;





			}

	}
	;

declarator_list
	: declarator                        
	{
		vector < string > tmp;
		tmp.push_back($1);
		$$ = tmp;

	}
	| declarator_list ',' declarator 
	{
		vector < string > tmp = $1;
		tmp.push_back($3);
		$$ = tmp;

	}
 	;
