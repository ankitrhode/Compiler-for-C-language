#include <bits/stdc++.h>
// #include "ast.h"
using namespace std;


int global_count = 0;
stack <int> label_stack;
int label_counter = 0;
vector <string> printstr;
int printed_strings=0;

bool isParameter = false;
bool is_local = false;
int func_ret_size;
bool remove_ret=false;
string curr_code_func;

LocalSymbolTableEntry :: LocalSymbolTableEntry(bool isParameter, string type, int sz, int offset) {
	LocalSymbolTableEntry :: isParameter = isParameter;
	LocalSymbolTableEntry :: type = type;
	LocalSymbolTableEntry :: sz = sz;
	LocalSymbolTableEntry :: offset = offset;
}

GlobalSymbolTableEntry :: GlobalSymbolTableEntry(string identity,string return_type, int sz) {
	GlobalSymbolTableEntry :: identity = identity;
	GlobalSymbolTableEntry :: return_type = return_type;
	GlobalSymbolTableEntry :: sz = sz;
}



string process(string type) {
	string ans = "";
	int p1, p2;
	if(type[0]=='*'){return type;}
	p1 = type.find(',');
	if(p1 == std::string::npos)
		return type;
	ans += type.substr(0, p1);
	type = type.substr(p1 + 1);
	p2 = type.find(',');
	if(p2 == std::string::npos)
		return ans + '*';
	return '*' + ans + type.substr(p2);
}

bool checkParameterCompatibility(string type1, ExpAst* rexp) {
	
	string type2=rexp->type;
	if(type1 == "int"  && type2 == "float" )
		{ rexp->to_int=true; return true; }
	if(type1 == "float"  && type2 == "int" )
		{ rexp->to_float=true; return true; }

	if(type1 == "void*" && type2.find('*') != std::string::npos)
		return true;
	if(type2 == "void*" && type1.find('*') != std::string::npos)
		return true;

	type1 = process(type1);
	type2 = process(type2);
	return type1 == type2;
}

map <string, GlobalSymbolTableEntry *> globalSymbolTable;

bool paramcheck(string id)
{

	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(curr_code_func);
	map < string, LocalSymbolTableEntry * > :: iterator it1 = ((it -> second) -> symtab).find(id);
	if((it1 -> second) -> isParameter)
		return 1;
	return 0;
}

int starting_offset_of_curr_function() {
	int ans = INT_MAX;
	map <string, LocalSymbolTableEntry*> :: iterator it = globalSymbolTable[curr_code_func] -> symtab.begin();
	while(it != globalSymbolTable[curr_code_func] -> symtab.end()) {
		ans = min(ans, it -> second -> offset);
		it++;
	}
	return ans;
}

int tot_param_sz() {
	int ret=0;
	map <string, LocalSymbolTableEntry*> :: iterator it = globalSymbolTable[curr_code_func] -> symtab.begin();
	while(it != globalSymbolTable[curr_code_func] -> symtab.end()) {
		if(it -> second -> isParameter)
			ret+=it->second->sz;
		it++;
	}
	return ret;
}


int offset_local(IdentifierRef* local)
{
	
		string var_name= local->identifier;
		return (globalSymbolTable[curr_code_func] -> symtab)[var_name]->offset;

}

int offset_struct(IdentifierRef* local, string structname)
{
	// cout <<structname<<"\n";
	string var_name= local->identifier;
		return (globalSymbolTable[structname] -> symtab)[var_name]->offset;

}

int get_sizestruct(string structname)
{
	return globalSymbolTable[structname]->sz;
}


bool isStructType(string s) {
	if(globalSymbolTable.find(s) == globalSymbolTable.end())
		return 0;
	return 1;
}

void modify_offset_of_locals(string func_name, string param_name, int k) {
	(globalSymbolTable[func_name] -> symtab)[param_name] -> offset -= k;
}

void modify_offset_in_local_symbol_table(string func_name, string param_name, int k) {
	(globalSymbolTable[func_name] -> symtab)[param_name] -> offset = k - (globalSymbolTable[func_name] -> symtab)[param_name] -> offset - (globalSymbolTable[func_name] -> symtab)[param_name] -> sz;
}

int checkParameters(vector <ExpAst*> tmp, string function_name) {
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(function_name);
	if(it == globalSymbolTable.end())
		return 0;
	int offset = 8;
	bool found = false;
	for(int i = tmp.size() - 1 ; i >= 0 ; i--) {
		found = false;
		ExpAst* curr = tmp[i];
		string type = curr -> type;
		map <string, LocalSymbolTableEntry*> :: iterator it1 = ((it -> second) -> symtab).begin();
		while(it1 != ((it -> second) -> symtab).end()) {
			if((it1 -> second) -> offset == offset && (it1 -> second) -> isParameter && checkParameterCompatibility((it1 -> second) -> type, curr)) {
				found = true;
				offset += (it1 -> second) -> sz;
				break;
			}
			it1++;
		}
		if(!found)
			return 0;
	}
	map <string, LocalSymbolTableEntry*> :: iterator it1 = ((it -> second) -> symtab).begin();
	while(it1 != ((it -> second) -> symtab).end()) {
		if((it1 -> second) -> offset == offset && (it1 -> second) -> isParameter)	return 0;
		it1++;
	}
	return 1;
}

int offset,index_val,parameter_offset;
string current_func,func_check,current_func_rettype;
bool err = false;

bool calculating_index;
void addToGlobalSymbolTable(string symbol_name, string identity, string return_type, int sz) {
	GlobalSymbolTableEntry* global_entry = new GlobalSymbolTableEntry(identity, return_type, sz);
	global_entry -> cnt = global_count++;
	globalSymbolTable[symbol_name] = global_entry;
	
}

string getType(string id) {
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(current_func);
	map < string, LocalSymbolTableEntry * > :: iterator it1 = ((it -> second) -> symtab).find(id);
	if(it1 != ((it -> second) -> symtab).end())
		return (it1 -> second) -> type;
	return "-";
}


bool isAStructParameter(string id, string struct_name) {
	
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(struct_name);
	map < string, LocalSymbolTableEntry * > :: iterator it1 = ((it -> second) -> symtab).find(id);
	if((it1 -> second) -> isParameter)
		return 1;
	return 0;
}

bool isAParameter(string id) {
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(current_func);
	map < string, LocalSymbolTableEntry * > :: iterator it1 = ((it -> second) -> symtab).find(id);
	if((it1 -> second) -> isParameter)
		return 1;
	return 0;
}

string getTypeGlobal(string id) {
	if((globalSymbolTable[current_func] -> symtab).find(id) != (globalSymbolTable[current_func] -> symtab).end())
		return "-";
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(id);
	if(it != globalSymbolTable.end())
		return it -> second -> return_type;
	return "-";
}

string getTypeStruct(string id, string structname) {
	// cout << structname<<endl;
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(structname);
	if(it==globalSymbolTable.end())cout<<"wtf1\n";
	map < string, LocalSymbolTableEntry * > :: iterator it1 = ((it -> second) -> symtab).find(id);
	if(it1 != ((it -> second) -> symtab).end()) {
		return (it1 -> second) -> type;
	}
	return "-";
}

bool typeCompatible(string t1, string t2) {
	if((t1.find(',') != std::string::npos))
		return false;
	if(t1 == process(t2))
		return true;
	if(t1 == "int" && t2 == "float")
		return true;
	if(t2 == "int" && t1 == "float")
		return true;
	if(t1 == "void*" && t2.find('*') != std::string::npos)
		return true;
	if(t2 == "void*" && t1.find('*') != std::string::npos)
		return true;
	return false;
}

bool type_check_parameter(string t1, string t2, int i){

	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.find(t2);
	map < string, LocalSymbolTableEntry * > :: iterator it1= ((it -> second) -> symtab).begin();
	for(;it1!=((it -> second) -> symtab).end();it1++)
		{
			if((it1->second)->offset==i){
				if(!((it1->second)->isParameter)){return false;}
				if(t1.compare(t2)!=0) return false;						//to be replaced with type compatibility for arguments
				parameter_offset+=(it1->second)->sz;
				return true;
			}


		}
		return false;
	
}

void print_info()
{
	map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.begin();
	cout<<"Global Symbol Table:\n";
	cout<<setw(20)<<"Symbol name"<<setw(20)<<"function/struct"<<setw(20)<<"Return_type"<<setw(20)<<"Size(for struct)"<<endl;
	while(it != globalSymbolTable.end()) 
		{	string t = it -> first;
			if(t.find('#') != string::npos)
				t = t.substr(0, t.length() - 1);
			cout << setw(20) << t << setw(20) << it->second->identity << setw(20) << it->second->return_type << setw(20) << it->second->sz << endl;it++;}
	it = globalSymbolTable.begin();cout<<endl;
	while(it != globalSymbolTable.end()) {
		string t = it -> first;
			if(t.find('#') != string::npos)
				t = t.substr(0, t.length() - 1);
		cout<<"Local Symbol Table for "<< t <<":\n";
		map <string, LocalSymbolTableEntry *> :: iterator it1 = it->second->symtab.begin();
		cout<<setw(20) << "Symbol" << setw(20) << "is_parameter" << setw(20) << "Type" << setw(20) << "offset" << setw(20) << "size" << endl;
		while(it1 != it->second->symtab.end())
			{
				string t = it1->second->type;
				int idx = t.find('#');
				if(idx != string::npos)
					t = t.substr(0, idx) + t.substr(idx + 1);
				cout<< setw(20) << it1->first << setw(20) << it1->second->isParameter << setw(20) << t << setw(20) << it1->second->offset << setw(20) << it1->second->sz <<endl;
				it1++;
			}
		cout<<endl;it++;
	}	

}


void addToLocalSymbolTable(string global_sym_name, string symbol_name, bool isParameter, string type, int sz, int offset) {
	LocalSymbolTableEntry * local_entry = new LocalSymbolTableEntry(isParameter, type, sz, offset);
	(globalSymbolTable[global_sym_name] -> symtab)[symbol_name] = local_entry;
}

abstract_astnode :: abstract_astnode() {}
ExpAst :: ExpAst() {}
StmtAst :: StmtAst() {}
RefAst :: RefAst() {}

Empty :: Empty() {}

void Empty :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(Empty)\n");
}
void Empty :: generate_code(){

}


///////////////////////////////////////////////

FloatConst :: FloatConst(string s) {
	FloatConst :: val = stof(s);
	FloatConst :: astnode_type = "float_const";
}

void FloatConst :: print(int counter) {
	cout << "(FloatConst " << FloatConst :: val << ")";
}
void FloatConst :: generate_code(){
	printf("li.s $f0 %f\naddi $sp $sp -4\ns.s $f0 0($sp)\n",FloatConst :: val);
}

///////////////////////////////////////////////

IntConst :: IntConst(string s) {
	IntConst :: val = stoi(s);
	IntConst :: astnode_type = "int_const";
}

void IntConst :: print(int counter) {
	cout << "(IntConst " << IntConst :: val << ")";
	// printf("(IntConst %d)", IntConst :: val);
}
void IntConst :: generate_code(){
	printf("li $s0 %d\naddi $sp $sp -4\nsw $s0 0($sp)\n",IntConst :: val);
}
///////////////////////////////////////////////

StringConst :: StringConst(string s) {
	StringConst :: val = s;
	StringConst :: astnode_type = "string_const";
}

void StringConst :: print(int counter) {
	cout << "(StringConst " << val << ")";
}
void StringConst :: generate_code(){
	
}

///////////////////////////////////////////////

If :: If(ExpAst * expressionAst, StmtAst * thenAst, StmtAst * elseAst) {
	If :: expressionAst = expressionAst;
	If :: thenAst = thenAst;
	If :: elseAst = elseAst;
	If :: astnode_type = "if";
}

void If :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(If ");
	(If :: expressionAst) -> print(counter + 1);
	printf("\n");
	(If :: thenAst) -> print(counter + 1);
	(If :: elseAst) -> print(counter + 1);
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf(")\n");
}
void If :: generate_code(){
	If :: expressionAst -> generate_code();
	ExpAst * cur = If::expressionAst;
	string t = expressionAst->type;

	printf("mtc1 $0 $f2\n");
	
	if(cur -> astnode_type == "identifier_ref" || cur -> astnode_type == "array_ref" || cur -> astnode_type == "member" || cur -> astnode_type == "arrow" || cur ->astnode_type == "de_ref" || cur->astnode_type == "funcall")
		{
			if(t == "float")
			{
				printf("l.s $f0 0($s2)\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}  //we already have retvalue on stack
				else
					{}
			}
				
			else
			{
				printf("lw $s0 0($s2)\nmtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}  //we already have space on stack for retval
				else
					{}
			}
		}

	else
	{

		if(If :: expressionAst -> type != "float")
			printf("lw $s0 0($sp)\nmtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
		else
			printf("l.s $f0 0($sp)\n");
		printf("addi $sp $sp 4\n");
	}
	int else_label = label_counter;
	printf("c.eq.s $f0 $f2\nbc1t L%d\n", label_counter++);		// goto ELSE
	If :: thenAst -> generate_code();
	printf("j L%d\n", label_stack.top());
	printf("L%d:\n", else_label);
	If :: elseAst -> generate_code();
	printf("j L%d\n", label_stack.top());
}

///////////////////////////////////////////////

While :: While(ExpAst * expressionAst, StmtAst * statementAst) {
	While :: expressionAst = expressionAst;
	While :: statementAst = statementAst;
	While :: astnode_type = "while";
}

void While :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(While ");
	(While :: expressionAst) -> print(counter + 1);
	printf("\n");
	(While :: statementAst) -> print(counter + 1);
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf(")\n");
}
void While :: generate_code(){
	int t = label_counter;
	printf("L%d:\n", t);
	label_counter++;
	(While :: expressionAst) -> generate_code();

	printf("mtc1 $0 $f2\n");
	ExpAst *cur =expressionAst;
	string type= expressionAst->type;
	if(cur -> astnode_type == "identifier_ref" || cur -> astnode_type == "array_ref" || cur -> astnode_type == "member" || cur -> astnode_type == "arrow" || cur ->astnode_type == "de_ref" || cur->astnode_type == "funcall")
		{
			if(type == "float")
			{
				printf("l.s $f0 0($s2)\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}  //we already have retvalue on stack
				else
					{}
			}
				
			else
			{
				printf("lw $s0 0($s2)\nmtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}  //we already have space on stack for retval
				else
					{}
			}
		}

	else
	{

		if(While :: expressionAst -> type != "float")
			printf("lw $s0 0($sp)\nmtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
		else
			printf("l.s $f0 0($sp)\n");
		printf("addi $sp $sp 4\n");
	}
	
	
	printf("c.eq.s $f0 $f2\nbc1t L%d\n", label_stack.top());
	
	label_stack.push(t);
	(While :: statementAst) -> generate_code();
	label_stack.pop();
	printf("j L%d\n", t);
}

///////////////////////////////////////////////

Return :: Return(ExpAst * expressionAst) {
	Return :: expressionAst = expressionAst;
	Return :: astnode_type = "return";
}

void Return :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(Return ");
	if(Return :: expressionAst != NULL)
		(Return :: expressionAst) -> print(counter + 1);
	printf(")\n");
}
void Return :: generate_code(){

		ExpAst * right = Return :: expressionAst;

		right -> generate_code();

		if(right -> astnode_type == "identifier_ref" || right -> astnode_type == "array_ref" || right -> astnode_type == "member" || right -> astnode_type == "arrow" || right -> astnode_type == "de_ref" || right -> astnode_type == "funcall")
			{	
				if(right->type=="float")
					printf("l.s $f1 0($s2)\n");
				else if ((right -> type.find("*")!=string :: npos && right->type.find(",")==string::npos) || right -> type == "int")
					printf("lw $s1 0($s2)\n");
				else if (right -> type.find(",")!=string :: npos)
					printf("addi $s1 $s2 0\n");
				else
					printf("move $s3 $s2\nlw $s1 0($s3)\n");
			}
		else
			{
				if(right ->type == "float")
					printf("l.s $f1 0($sp)\naddi $sp $sp 4\n");
				else if (right -> type.find("*")!=string :: npos || right -> type == "int")
					printf("lw $s1 0($sp)\naddi $sp $sp 4\n"); // assume that values needed for unary,binary operations are store at the top of the stack 
				
				else
					printf("lw $s3 0($sp)\nlw $s1 0($s3)\naddi $sp $sp 4\n");
			}
		
		if(curr_code_func=="main")
		{

			if(right -> type != "float") {
			printf("move $v1 $s1\n");
			}
			else  {
			printf("cvt.w.s $f1 $f1\nmfc1 $v1 $f1\n");
			}

		}

		else{

		printf("addi $s2 $fp %d\n",36+tot_param_sz());
		// string exp_type= right->type;
		string exp_type= globalSymbolTable[curr_code_func]->return_type;  // why doesnt work
		// cout << "exptype = " << exp_type << "\n";

		
		if(exp_type != "float" && right -> type != "float") {
			printf("sw $s1 0($s2)\n");
		}
		else if(exp_type != "float" && right -> type == "float") {
			printf("cvt.w.s $f1 $f1\nmfc1 $s1 $f1\n");
			printf("sw $s1 0($s2)\n");
		}
		else if(exp_type == "float" && right -> type != "float") {
			printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("s.s $f1 0($s2)\n");
		}
		else {
			printf("s.s $f1 0($s2)\n");
		}

		if(exp_type.find("*")==string :: npos && exp_type!="int" && exp_type!="float")
			{
				int sz=get_sizestruct(exp_type);
			
				for(int i=0;i<(sz-4)/4;i++)
					{
						printf("addi $s3 $s3 4\nlw $s1 0($s3)\naddi $s2 $s2 4\nsw $s1 0($s2)\n");

					}
				printf("addi $s2 $s2 %d\n",-(sz-4));		//doubt

			}
		}

		if(remove_ret)
			{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}

		map < string, LocalSymbolTableEntry * > :: iterator it1 = (globalSymbolTable[curr_code_func]->symtab).begin();
        int local_var_size=0;

        for(;it1!=(globalSymbolTable[curr_code_func]->symtab).end();it1++)
          {
            if( !it1->second->isParameter)
              local_var_size+= it1->second->sz;
          }

         printf("addi $sp $sp %d\nlw $fp 0($sp)\naddi $sp $sp 4\nlw $ra 0($sp)\naddi $sp $sp 4\njr $ra\n",(local_var_size));
	
}

///////////////////////////////////////////////

Seq :: Seq(vector < StmtAst* > statementList) {
	Seq :: statementList = statementList;
	Seq :: astnode_type = "seq";
}

void Seq :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(Block\n");
	for(int i = 0 ; i < (Seq :: statementList).size() ; i++) {
		((Seq :: statementList)[i]) -> print(counter + 1);
	}
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf(")\n");
}

void Seq :: generate_code(){

	for(int i=0;i < (Seq :: statementList).size() - 1 ; i++) {
		int curr_label = label_counter++;
		label_stack.push(curr_label);
		((Seq :: statementList)[i]) -> generate_code();
		printf("\n");
		label_stack.pop();
		printf("L%d:\n", curr_label);
	}
	if(label_stack.empty()) {
		int curr_label = label_counter++;
		label_stack.push(curr_label);
		((Seq :: statementList)[(Seq :: statementList).size() - 1]) -> generate_code();
		printf("\n");
		label_stack.pop();
		printf("L%d:\n", curr_label);
	}
	else {
		((Seq :: statementList)[(Seq :: statementList).size() - 1]) -> generate_code();
		printf("\n");
	}
}

///////////////////////////////////////////////

Ass :: Ass(ExpAst * expr) {
	Ass :: expressionAst = expr;
	Ass :: astnode_type = "ass";
}

void Ass :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(Ass ");
	(Ass :: expressionAst) -> print(counter + 1);
	printf(")\n");
}

void Ass :: generate_code() {
	if(Ass::expressionAst->astnode_type == "binary_exp_ast") {

		ExpAst * left = ((BinaryExpAst *)Ass :: expressionAst) -> leftExp;
		ExpAst * right = ((BinaryExpAst *)Ass :: expressionAst) -> rightExp;

		right -> generate_code();

		if(right -> astnode_type == "identifier_ref" || right -> astnode_type == "array_ref" || right -> astnode_type == "member" || right -> astnode_type == "arrow" || right -> astnode_type == "de_ref" || right -> astnode_type == "funcall")
			{	
				if(right->type=="float")
					printf("l.s $f1 0($s2)\n");
				else if ((right -> type.find("*")!=string :: npos && right->type.find(",")==string::npos) || right -> type == "int")
					printf("lw $s5 0($s2)\n");
				else if (right -> type.find(",")!=string :: npos)
					printf("addi $s5 $s2 0\n");
				else
					printf("move $s3 $s2\nlw $s5 0($s3)\n");
			}
		else
			{
				if(right ->type == "float")
					printf("l.s $f1 0($sp)\naddi $sp $sp 4\n");
				else if (right -> type.find("*")!=string :: npos || right -> type == "int")
					printf("lw $s5 0($sp)\naddi $sp $sp 4\n"); // assume that values needed for unary,binary operations are store at the top of the stack 
				
				else
					printf("lw $s3 0($sp)\nlw $s5 0($s3)\naddi $sp $sp 4\n");
			}
		left -> generate_code();
		
		if(left -> type != "float" && right -> type != "float") {
			printf("sw $s5 0($s2)\n");
		}
		else if(left -> type != "float" && right -> type == "float") {
			printf("cvt.w.s $f1 $f1\nmfc1 $s5 $f1\n");
			printf("sw $s5 0($s2)\n");
		}
		else if(left -> type == "float" && right -> type != "float") {
			printf("mtc1 $s5 $f1\ncvt.s.w $f1 $f1\n");
			printf("s.s $f1 0($s2)\n");
		}
		else {
			printf("s.s $f1 0($s2)\n");
		}

		if(left -> type.find("*")==string :: npos && left->type!="int" && left->type!="float")
			{
				int sz=get_sizestruct(left->type);
				for(int i=0;i<(sz-4)/4;i++)
					{
						printf("addi $s3 $s3 4\nlw $s5 0($s3)\naddi $s2 $s2 4\nsw $s5 0($s2)\n");

					}
				printf("addi $s2 $s2 %d\nmove $s5 $s2\n",-(sz-4));

			}
		if(remove_ret)
			{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}
	}

	else if(Ass::expressionAst -> astnode_type == "identifier_ref" || Ass::expressionAst -> astnode_type == "array_ref" || Ass::expressionAst -> astnode_type == "member" || Ass::expressionAst -> astnode_type == "arrow" || Ass::expressionAst -> astnode_type == "de_ref" || Ass::expressionAst -> astnode_type == "funcall")
			{	
				Ass::expressionAst->generate_code();
				if(remove_ret)
					{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;} 
			}
	else
		{
			Ass::expressionAst->generate_code();
			printf("addi $sp $sp 4\n");
		}
}


///////////////////////////////////////////////

For :: For(ExpAst * initialiserExp, ExpAst * guardExp, ExpAst * stepExp, StmtAst * statementAst ) {
	For :: initialiserExp = initialiserExp;
	For :: guardExp = guardExp;
	For :: stepExp = stepExp;
	For :: statementAst = statementAst;
	For :: astnode_type = "for";
}

void For :: print(int counter) {
	int i;
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf("(For ");
	(For :: initialiserExp) -> print(counter + 1);
	(For :: guardExp) -> print(counter + 1);
	(For :: stepExp) -> print(counter + 1);
	printf("\n");
	(For :: statementAst) -> print(counter + 1);
	for(i = 0 ; i < counter ; i++)
		printf("\t");
	printf(")\n");
}
void For :: generate_code(){
	 
	Ass * tmp1 = new Ass(For::initialiserExp);
	tmp1->generate_code();
		
	int t = label_counter;
	label_counter+=2;
	printf("L%d:\n", t);
	For :: guardExp -> generate_code();

	if(For :: guardExp -> type != "float")
		printf("lw $s0 0($sp)\nmtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
	else
		printf("l.s $f0 0($sp)\n");
	printf("addi $sp $sp 4\n");

	printf("c.eq.s $f0 $f2\nbc1t L%d\n", label_stack.top());
	
	label_stack.push(t+1);
	(For :: statementAst) -> generate_code();

	printf("L%d:\n",t+1);
	Ass * tmp2 = new Ass(For::stepExp);
	tmp2->generate_code();
	label_stack.pop();
	printf("j L%d\n", t);
}

///////////////////////////////////////////////

Funcall :: Funcall(string identifier, vector < ExpAst * > parameterExp) {
	Funcall :: identifier = identifier;
	Funcall :: parameterExp = parameterExp;
	Funcall :: astnode_type = "funcall";
}

void Funcall :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	cout << Funcall :: identifier;
	for(int i = 0 ; i < (Funcall :: parameterExp).size() ; i++)
		((Funcall :: parameterExp)[i]) -> print(counter + 1);
	printf(")");	
}
void Funcall :: generate_code(){

	if(Funcall :: identifier=="printf")
	{

		int param_count = Funcall :: parameterExp.size();
		for(int i=0;i<param_count;i++)
		{
			string param_type= Funcall :: parameterExp[i]->type;

			if(param_type=="string")
			{
				printf("li $v0 4\nla $a0 s%d\nsyscall\n",printed_strings++);		
			}

			else if (param_type=="int")
			{
				ExpAst * right = parameterExp[i];
				right->generate_code();

				if(right -> astnode_type == "identifier_ref" || right -> astnode_type == "array_ref" || right -> astnode_type == "member" || right -> astnode_type == "arrow" || right -> astnode_type == "de_ref" || right->astnode_type=="funcall")
				{
					printf("lw $a0 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;} 
				}
				else
				{
					printf("lw $a0 0($sp)\naddi $sp $sp 4\n");

				}
				printf("li $v0 1\nsyscall\n");


			}

			else if (param_type=="float")
			{
				ExpAst * right = parameterExp[i];
				right->generate_code();

				if(right -> astnode_type == "identifier_ref" || right -> astnode_type == "array_ref" || right -> astnode_type == "member" || right -> astnode_type == "arrow" || right -> astnode_type == "de_ref" || right->astnode_type=="funcall")
				{
					printf("l.s $f12 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;} 
				}
				else
				{
					printf("l.s $f12 0($sp)\naddi $sp $sp 4\n");

				}
				printf("li $v0 2\nsyscall\n");


			}

		}

	}
	
	else{

	int retsize;
	string rettype = globalSymbolTable[Funcall :: identifier]->return_type;
	if(rettype=="void")
		retsize=0;
	else if(rettype == "int" || rettype == "float" || rettype.find("*")!=string::npos)			//to check
		retsize = 4;
	else
		retsize= globalSymbolTable[rettype]->sz;

	func_ret_size= retsize;
	

	// space for return value
	printf("addi $sp $sp %d\n", -retsize);
	
	// saving caller saved registers
	printf("addi $sp $sp -28\nsw $s0 24($sp)\nsw $s1 20($sp)\nsw $s2 16($sp)\nsw $s3 12($sp)\nsw $s5 8($sp)\ns.s $f0 4($sp)\ns.s $f1 0($sp)\n");

	int offset=8;
	int param_count = Funcall :: parameterExp.size();

	vector < pair <int, string> > params_of_function;
	string func_to_be_called = Funcall :: identifier;
	map <string, LocalSymbolTableEntry*> :: iterator it = globalSymbolTable[func_to_be_called] -> symtab.begin();
	while(it != globalSymbolTable[func_to_be_called] -> symtab.end()) {
		if(it -> second -> isParameter == 0) {
			it++;
			continue;
		}
		params_of_function.push_back(make_pair(it -> second -> offset, it -> second -> type));
		// cout << "printing func params " << func_to_be_called << "\n";
		// cout << "isparam: " << it -> second -> isParameter << "\n";
		// cout << it -> second -> offset << " " << it -> second -> type << "\n";
		it++;
	}
	sort(params_of_function.begin(), params_of_function.end());
	int tot_param_size=0;
	
	printf("\n");

	for(int i=0; i< params_of_function.size(); i++) {
		printf("# Parameter %d\n", i + 1);
		string param_type= Funcall :: parameterExp[i]->type;
		int param_size;

		if(param_type=="int" || param_type=="float" || param_type.find(",")!=string::npos || param_type.find("*")!=string::npos)
			param_size=4;
		else 
			param_size= globalSymbolTable[param_type]->sz;

		tot_param_size+=param_size;
		parameterExp[i] -> generate_code();
		ExpAst * right = parameterExp[i];

		if(right -> astnode_type == "identifier_ref" || right -> astnode_type == "array_ref" || right -> astnode_type == "member" || right -> astnode_type == "arrow" || right -> astnode_type == "de_ref" || right->astnode_type=="funcall")
			{	
				if(right->type=="float")
					printf("l.s $f1 0($s2)\n");
				else if ((right -> type.find("*")!=string :: npos && right->type.find(",")==string::npos) || right -> type == "int")
					printf("lw $s1 0($s2)\n");
				else if (right ->type.find(",")!=string :: npos)
					printf("addi $s1 $s2 0\n");
				else
					printf("move $s3 $s2\nlw $s1 0($s3)\n");
			}
		else
			{
				if(right ->type == "float")
					printf("l.s $f1 0($sp)\naddi $sp $sp 4\n");
				else if (right -> type.find("*")!=string :: npos || right -> type == "int")
					printf("lw $s1 0($sp)\naddi $sp $sp 4\n"); // assume that values needed for unary,binary operations are store at the top of the stack 
				
				else
					printf("lw $s3 0($sp)\nlw $s1 0($s3)\naddi $sp $sp 4\n");
			}
	
		string exp_type = params_of_function[params_of_function.size() - 1 - i].second;
		// cout << "type = " << exp_type << "\n";

		if(remove_ret)
			{printf("addi $sp $sp %d\n",func_ret_size);remove_ret=false;}

		printf("addi $sp $sp %d\n",-param_size);
		
		if(exp_type != "float" && right -> type != "float") {
			printf("sw $s1 0($sp)\n");
		}
		else if(exp_type != "float" && right -> type == "float") {
			printf("cvt.w.s $f1 $f1\nmfc1 $s1 $f1\n");
			printf("sw $s1 0($sp)\n");
		}
		else if(exp_type == "float" && right -> type != "float") {
			printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("s.s $f1 0($sp)\n");
		}
		else {
			printf("s.s $f1 0($sp)\n");
		}

		if(exp_type.find("*")==string :: npos && exp_type.find(",")==string::npos && exp_type!="int" && exp_type!="float")
			{
				int sz=get_sizestruct(exp_type);
				for(int i=0;i<(sz-4)/4;i++)
					{
						printf("addi $s3 $s3 4\nlw $s1 0($s3)\naddi $sp $sp 4\nsw $s1 0($sp)\n");

					}
				printf("addi $sp $sp %d\n",-(sz-4));

		//adw
		
			}
		printf("\n");
		
	}
printf("jal %s\n\naddi $sp $sp %d\n",Funcall :: identifier.c_str(),tot_param_size);
printf("lw $s0 24($sp)\nlw $s1 20($sp)\nlw $s2 16($sp)\nlw $s3 12($sp)\nlw $s5 8($sp)\nl.s $f0 4($sp)\nl.s $f1 0($sp)\naddi $sp $sp 28\n");
printf("move $s2 $sp\n");
remove_ret = true;

//see how to take ret value

}
}

///////////////////////////////////////////////

UnaryExpAst :: UnaryExpAst(ExpAst * expressionAst, string op) {
	UnaryExpAst :: expressionAst = expressionAst;
	UnaryExpAst :: op = op;
	UnaryExpAst :: astnode_type = "unary_exp_ast";
}

void UnaryExpAst :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	cout << op;
	(UnaryExpAst :: expressionAst) -> print(counter + 1);
	printf(")");
}
void UnaryExpAst :: generate_code(){
	ExpAst * cur = UnaryExpAst :: expressionAst;
	cur -> generate_code();
	string t = cur->type;

	if(cur -> astnode_type == "identifier_ref" || cur -> astnode_type == "array_ref" || cur -> astnode_type == "member" || cur -> astnode_type == "arrow" || cur ->astnode_type == "de_ref" || cur->astnode_type == "funcall")
		{
			if(t == "float")
			{
				printf("l.s $f0 0($s2)\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\ns.s $f0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //we already have retvalue on stack
				else
					printf("addi $sp $sp -4\ns.s $f0 0($sp)\n");
			}
				
			else
			{
				printf("lw $s0 0($s2)\n");
				if(remove_ret)
					{printf("addi $sp $sp %d\nsw $s0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //we already have space on stack for retval
				else
					printf("addi $sp $sp -4\nsw $s0 0($sp)\n");
			}
		}

	if(t == "float")
		printf("l.s $f0 0($sp)\n");
	else
		printf("lw $s0 0($sp)\n");
		

	if(UnaryExpAst :: op == "-") {
		if(t == "float")
			printf("mul.s $f0 $f0 -1\n");
		else
			printf("mul $s0 $s0 -1\n");
	}
	else if(UnaryExpAst :: op == "!") {
		
		if(t == "float") {
			printf("c.eq.s $f0 $0\nbc1f L%d\n", label_counter);
			printf("addi $s0 $0 1\n");
			printf("j L%d\n", label_counter + 1);
			printf("L%d:\n", label_counter);
			label_counter+=2;
			printf("addi $s0 $0 0\n");
			printf("L%d:\n", label_counter-1);	
		}
		else {
			printf("bne $s0 $0 L%d\n", label_counter);
			printf("addi $s0 $0 1\n");
			printf("j L%d\n", label_counter + 1);
			printf("L%d:\n", label_counter);
			label_counter+=2;
			printf("addi $s0 $0 0\n");
			printf("L%d:\n", label_counter-1);
		}
	}
	
	
	else if(UnaryExpAst :: op == "INC_OP") {
		if(t == "float")
			printf("sw $f0 0($sp)\nli.s $f4 $f4 1.0\nadd.s $f0 $f0 $f4\ns.s $f0 0($s2)\n");
		else
			printf("sw $s0 0($sp)\naddi $s0 $s0 1\nsw $s0 0($s2)\n");
			
	}

	if(UnaryExpAst :: op != "INC_OP")
	{
	if(UnaryExpAst :: type == "float")
		printf("s.s $f0 0($sp)\n");
	else
		printf("sw $s0 0($sp)\n");
	}


		
}

///////////////////////////////////////////////

BinaryExpAst :: BinaryExpAst(ExpAst * leftExp, ExpAst * rightExp, string op) {
	BinaryExpAst :: leftExp = leftExp;
	BinaryExpAst :: rightExp = rightExp;
	BinaryExpAst :: op = op;
	BinaryExpAst :: astnode_type = "binary_exp_ast";
}

void BinaryExpAst :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	cout << op;
	if(is_intop)cout<<"_int";
	if(is_floatop)cout<<"_float";
	(BinaryExpAst :: leftExp) -> print(counter + 1);
	(BinaryExpAst :: rightExp) -> print(counter + 1);
	printf(")");
	
}
void BinaryExpAst :: generate_code(){

	string tleft = BinaryExpAst :: leftExp -> type;
	string tright = BinaryExpAst :: rightExp -> type;
	if(BinaryExpAst :: op =="="){
		Ass * tmp = new Ass
		(this);
		tmp->generate_code();
		printf("addi $sp $sp -4\n");
		if(tleft != "float") 
			printf("sw $s5 0($sp)\n");
		else
			printf("s.s $f1 0($sp)\n");

	}

	else{
		BinaryExpAst :: leftExp -> generate_code();
		if(BinaryExpAst :: leftExp -> astnode_type == "identifier_ref" || BinaryExpAst :: leftExp -> astnode_type == "array_ref" || BinaryExpAst :: leftExp -> astnode_type == "member" || BinaryExpAst :: leftExp -> astnode_type == "arrow" || BinaryExpAst :: leftExp -> astnode_type == "de_ref" || BinaryExpAst :: leftExp -> astnode_type == "funcall")
			{
				if(tleft == "float")
				{
					printf("l.s $f0 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\ns.s $f0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //we already have retvalue on stack
					else
						printf("addi $sp $sp -4\ns.s $f0 0($sp)\n");
				}
				
				else
				{
					printf("lw $s0 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\nsw $s0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //collected retval from func return call now remove space for it on stack
					else
						printf("addi $sp $sp -4\nsw $s0 0($sp)\n");
				}
			}		
		BinaryExpAst :: rightExp -> generate_code();
		if(BinaryExpAst :: rightExp -> astnode_type == "identifier_ref" || BinaryExpAst :: rightExp -> astnode_type == "array_ref" || BinaryExpAst :: rightExp -> astnode_type == "member" || BinaryExpAst :: rightExp -> astnode_type == "arrow" || BinaryExpAst :: rightExp -> astnode_type == "de_ref" || BinaryExpAst :: rightExp -> astnode_type == "funcall")
			{
				
				if(tright == "float")
				{
					printf("l.s $f0 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\ns.s $f0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //we already have retvalue on stack
					else
						printf("addi $sp $sp -4\ns.s $f0 0($sp)\n");
				}
				
				else
				{
					printf("lw $s0 0($s2)\n");
					if(remove_ret)
						{printf("addi $sp $sp %d\nsw $s0 0($sp)\n",func_ret_size-4);remove_ret=false;}  //we already have space on stack for retval
					else
						printf("addi $sp $sp -4\nsw $s0 0($sp)\n");
				}
			}

		if(tright == "float")
			printf("l.s $f1 0($sp)\n");
		else
			printf("lw $s1 0($sp)\n");

		if(tleft == "float")
			printf("l.s $f0 4($sp)\n");
		else
			printf("lw $s0 4($sp)\n");

		printf("addi $sp $sp 4\n");

		if(op=="+")
			{	
				if(tleft != "float" && tright != "float")
					printf("add $s0 $s0 $s1\nsw $s0 0($sp)\n");

				else if(tleft != "float" && tright == "float")
						printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\nadd.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else if(tleft == "float" && tright != "float")
						printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\nadd.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else
					printf("add.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

			}
		else if (op=="-")
			{	
				if(tleft != "float" && tright != "float")
					printf("sub $s0 $s0 $s1\nsw $s0 0($sp)\n");

				else if(tleft != "float" && tright == "float")
						printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\nsub.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else if(tleft == "float" && tright != "float")
						printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\nsub.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else
					printf("sub.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

			}
		else if (op=="*")
			{	
				if(tleft != "float" && tright != "float")
					printf("mul $s0 $s0 $s1\nsw $s0 0($sp)\n");

				else if(tleft != "float" && tright == "float")
						printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\nmul.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else if(tleft == "float" && tright != "float")
						printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\nmul.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else
					printf("mul.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

			}
		else if (op=="/")
			{	
				if(tleft != "float" && tright != "float")
					printf("div $s0 $s0 $s1\nsw $s0 0($sp)\n");

				else if(tleft != "float" && tright == "float")
						printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\ndiv.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else if(tleft == "float" && tright != "float")
						printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\ndiv.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

				else
					printf("div.s $f0 $f0 $f1\ns.s $f0 0($sp)\n");

			}

		//boolean expressions
		//AND_OP OR_OP EQ_OP NE_OP GE_OP LE_OP

		else if (op == "AND_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			int t = label_counter;
			label_counter++;
			printf("mtc1 $0 $f2\nc.eq.s $f0 $f2\nbc1t L%d\nc.eq.s $f1 $f2\nbc1t L%d\n", t, t);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\nj L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("sw $0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if (op == "OR_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			int t = label_counter;
			label_counter++;
			printf("mtc1 $0 $f2\nc.eq.s $f0 $f2\nbc1f L%d\nc.eq.s $f1 $f2\nbc1f L%d\n", t, t);
			printf("sw $0 0($sp)\nj L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if (op == "EQ_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.eq.s $f0 $f1\nbc1f L%d\n", label_counter++);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("sw $0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if (op == "NE_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.eq.s $f0 $f1\nbc1f L%d\n", label_counter++);
			printf("sw $0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if (op == "GE_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.lt.s $f0 $f1\nbc1t L%d\n", label_counter++);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("sw $0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if (op == "LE_OP"){
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.le.s $f0 $f1\nbc1f L%d\n", label_counter++);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("sw $0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}	
		else if(op == "<") {
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.lt.s $f0 $f1\nbc1t L%d\n", label_counter++);
			printf("sw $0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else if(op == ">") {
			if(tleft != "float")
				printf("mtc1 $s0 $f0\ncvt.s.w $f0 $f0\n");
			if(tright != "float")
				printf("mtc1 $s1 $f1\ncvt.s.w $f1 $f1\n");
			printf("c.le.s $f0 $f1\nbc1f L%d\n", label_counter++);
			printf("sw $0 0($sp)\n");
			printf("j L%d\n", label_counter++);
			printf("L%d:\n", label_counter - 2);
			printf("addi $s0 $0 1\nsw $s0 0($sp)\n");
			printf("L%d:\n", label_counter - 1);
		}
		else {
			printf("unidentified\n");
		}	
	}
	
}

///////////////////////////////////////////////

IdentifierRef :: IdentifierRef(string identifier) {
	IdentifierRef :: identifier = identifier;
	IdentifierRef :: astnode_type = "identifier_ref";
}

void IdentifierRef :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Id \"");
	cout << IdentifierRef :: identifier;
	printf("\")");
}
void IdentifierRef :: generate_code(){
	int off_var = offset_local(this);
	printf("addi $s2 $fp %d\n", off_var);
	
}

///////////////////////////////////////////////

ArrayRef :: ArrayRef(RefAst * identifier, ExpAst * expressionAst) {
	ArrayRef :: identifier = identifier;
	ArrayRef :: expressionAst = expressionAst;
	ArrayRef :: astnode_type = "array_ref";
}

void ArrayRef :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Array ");
	(ArrayRef :: identifier) -> print(counter + 1);
	(ArrayRef :: expressionAst) -> print(counter + 1);
	printf(")");
}
void ArrayRef :: generate_code(){

	
	
	
	expressionAst ->  generate_code();
	if(expressionAst -> astnode_type == "identifier_ref" || expressionAst -> astnode_type == "array_ref" || expressionAst -> astnode_type == "member" || expressionAst -> astnode_type == "arrow" || expressionAst ->astnode_type == "de_ref" || expressionAst->astnode_type == "funcall")
	{
		printf("lw $s0 0($s2)\n");
		if(remove_ret)
			{printf("addi $sp $sp %d\nsw $s0 0($sp)\n",func_ret_size-4);remove_ret=false;}
		else
			{printf("addi $sp $sp -4\nsw $s0 0($sp)\n");}
	}


	identifier -> generate_code();
	string identifier_type = identifier->type;
	if(identifier->astnode_type=="identifier_ref")
	{
		string identity= ((IdentifierRef*)identifier)->identifier;
		

		if(identifier_type.find(",")!=string::npos)
		{
			if(paramcheck(identity))
				printf("lw $s2 0($s2)\n");
		}
		
	}
	if(identifier_type.find("*")!=string::npos && identifier_type.find(",")==string::npos)
			printf("lw $s2 0($s2)\n");
	
	bool is_pointer = false;
	string type = ArrayRef :: type;

	int j=0,product=1;


	for(;j<type.length()&&type[j]!=',';j++);
	

	string base_type = type.substr(0, j);


	if(type[j-1]=='*')is_pointer=true;

	j++;
	while(j<type.length())					//calculating size of an if variable is array ,eg int a[5] has size 20
					{
						string index="";
						for(; j<type.length()&&type[j]!=',' ; j++)
						{
							index+=type[j];
							
						}
                        int num= stoi(index);
                        product*=num;
						j++;
					}
	if(is_pointer){
					product=4*product;
	}
	else if(base_type.compare("int")==0){
					product=4*product;
	}
	else if(base_type.compare("float")==0){
					product=4*product;
	}

	else{	
        	
			map <string, GlobalSymbolTableEntry *> :: iterator it ;
			it = globalSymbolTable.find(base_type);
			if(it==globalSymbolTable.end())	{cout<<"wtf\n";}
			product = (globalSymbolTable[base_type] -> sz)*product;
		}

	printf("lw $s0 0($sp)\naddi $sp $sp 4\naddi $s4 $0 %d\nmul $s0 $s0 $s4\nadd $s2 $s2 $s0\n",product);
}

///////////////////////////////////////////////

///////////////////////////////////////////////

DeRef :: DeRef(RefAst * identifier) {
	DeRef :: identifier = identifier;
	DeRef :: astnode_type = "de_ref";
}

void DeRef :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Dereference ");
	(DeRef :: identifier) -> print(counter + 1);
	printf(")");
}
void DeRef :: generate_code(){
	identifier -> generate_code();
	if(identifier->astnode_type!="pointer")
		printf("lw $s2 0($s2)\n");
	else 
		printf("addi $sp $sp 4\n");
}

Member :: Member(RefAst * identifier, IdentifierRef * member) {
	Member :: identifier = identifier;
	Member :: member = member;
	Member :: astnode_type = "member";
}

void Member :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Member ");
	(Member :: identifier) -> print(counter + 1);
	(Member :: member) -> print(counter + 1);
	printf(")");
}
void Member :: generate_code(){

	identifier -> generate_code();

	int off = offset_struct(member, Member :: identifier -> type);
	printf("addi $s2 $s2 %d\n",off);
	
}

Arrow :: Arrow(RefAst * identifier, IdentifierRef * member) {
	Arrow :: identifier = identifier;
	Arrow :: member = member;
	Arrow :: astnode_type = "arrow";
}

void Arrow :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Arrow ");
	(Arrow :: identifier) -> print(counter + 1);
	(Arrow :: member) -> print(counter + 1);
	printf(")");
}
void Arrow :: generate_code(){
	identifier -> generate_code();
	printf("lw $s2 0($s2)\n");
	// cout << (((IdentifierRef *)(Arrow :: identifier)) -> identifier) << "\n";
	string structname =  Arrow :: identifier -> type;
	structname =structname.substr(0,structname.length()-1);
	int off = offset_struct(member, structname);
	printf("addi $s2 $s2 %d\n",off);
}




///////////////////////////////////////////////

Pointer :: Pointer(RefAst * identifier) {
	Pointer :: identifier = identifier;
	Pointer :: astnode_type = "pointer";
}

void Pointer :: print(int counter) {
	if(to_float)cout<<"(To_float ";
	else if(to_int)cout<<"(To_int ";
	else printf("(");
	printf("Pointer ");
	(Pointer :: identifier) -> print(counter + 1);
	printf(")");
}
void Pointer :: generate_code(){

	Pointer :: identifier -> generate_code();
	printf("addi $sp $sp -4\nsw $s2 0($sp)\n");
}