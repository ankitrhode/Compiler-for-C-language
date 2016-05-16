#include <bits/stdc++.h>
using namespace std;

class LocalSymbolTableEntry {
	public:
		bool isParameter;		// parameter or local variable
		string type;
		int sz;
		int offset;

		LocalSymbolTableEntry(bool isParameter, string type, int sz, int offset);
};



// void addToGlobalSymbolTable(string symbol_name, string return_type);

class abstract_astnode {
	public:
		abstract_astnode();
		virtual void print(int counter) = 0;
		virtual void generate_code() = 0;
		//~ virtual basic_types getType() = 0;
		//~ virtual bool checkTypeofAST() = 0;
	//~ protected:
		//~ virtual void setType(basic_types) = 0;
	//~ private:
		//~ typeExp astnode_type;

		string astnode_type;

	   string type;
	    int vali=-1;		//default values
	   float valf=-1;
};

//////////ExpAst Family////////////

class ExpAst : public abstract_astnode {
	public:
		ExpAst();
		virtual void print(int counter) = 0;
		bool to_int=false;
		bool to_float=false;
		bool is_intop=false;
		bool is_floatop=false;
		bool is_lvalue=false;
		bool is_local_addr=false;
		bool is_local=false;
		bool parameter=false;
		int num_regs=0;
		virtual void generate_code() = 0;

};

//done
class BinaryExpAst : public ExpAst {
	public:
		ExpAst * leftExp, * rightExp;
		string op;
		BinaryExpAst(ExpAst * leftExp, ExpAst * rightExp, string op);
		void print(int counter);
		void generate_code();
};

//done
class UnaryExpAst : public ExpAst {
	public:
		ExpAst * expressionAst;
		string op;
		UnaryExpAst(ExpAst * expressionAst, string op);
		void print(int counter);
		void generate_code();
};

//done
class Funcall : public ExpAst {
	public:
		string identifier;
		vector < ExpAst * > parameterExp;
		Funcall(string identifier, vector < ExpAst * > parameterExp);
		void print(int counter);
		void generate_code();
};

//done
class FloatConst : public ExpAst {
	public:
		float val;
		FloatConst(string s);
		void print(int counter);
		void generate_code();

};

//done
class IntConst : public ExpAst {
	public:
		int val;
		IntConst(string s);
		void print(int counter);
		void generate_code();
};

//done
class StringConst : public ExpAst {
	public:
		string val;
		StringConst(string s);
		void print(int counter);
		void generate_code();
};



////////////////RefAst Family/////////////

class RefAst : public ExpAst {
	public:
		RefAst();
		virtual void print(int counter) = 0;
		virtual void generate_code() = 0 ;
	};

class Pointer : public ExpAst {
	public:
		RefAst * identifier;
		Pointer(RefAst * identifier);
		void print(int counter);
		void generate_code();
};
//done
class IdentifierRef : public RefAst {
	public:
		string identifier;
		IdentifierRef(string identifier);
		void print(int counter);
		void generate_code();
};

//done
class ArrayRef : public RefAst {
	public:
		RefAst * identifier;
		ExpAst * expressionAst;
		ArrayRef(RefAst * identifier, ExpAst * expressionAst);
		void print(int counter);
		void generate_code();
};

//done


//done
class DeRef : public RefAst {
	public:
		RefAst * identifier;
		DeRef(RefAst * identifier);
		void print(int counter);
		void generate_code();
};



class Member : public RefAst {
	public:
		RefAst * identifier;
		IdentifierRef * member;
		Member(RefAst * identifier, IdentifierRef * member);
		void print(int counter);
		void generate_code();

};

class Arrow : public RefAst {
	public:
		RefAst * identifier;
		IdentifierRef * member;
		Arrow(RefAst * identifier, IdentifierRef * member);
		void print(int counter);
		void generate_code();

};

////////////////StmtAst Family/////////////

class StmtAst : public abstract_astnode {
	public:
		StmtAst();
		virtual void print(int counter) = 0;
		virtual void generate_code() = 0;
};

//done
class Empty : public StmtAst {
	public:
		Empty();
		void print(int counter);
		void generate_code();
};

//done
class Seq : public StmtAst {
	public:
		vector < StmtAst* > statementList;
		Seq(vector < StmtAst* > statementList);
		void print(int counter);
		void generate_code();
};

//done
class Ass : public StmtAst {
	public:
		ExpAst * expressionAst;
		Ass(ExpAst * expressionAst);
		void print(int counter);
		void generate_code();
};

//done
class Return : public StmtAst {
	public:
		ExpAst * expressionAst;
		Return(ExpAst * expressionAst);
		void print(int counter);
		void generate_code();
};

//done
class If : public StmtAst {
	public:
		ExpAst * expressionAst;
		StmtAst * thenAst, * elseAst;
		If(ExpAst * expressionAst, StmtAst * thenAst, StmtAst * elseAst);
		void print(int counter);
		void generate_code();
};

//done
class While : public StmtAst {
	public:
		ExpAst * expressionAst;
		StmtAst * statementAst;
		While(ExpAst * expressionAst, StmtAst * statementAst);
		void print(int counter);
		void generate_code();
};

//done
class For : public StmtAst {
	public:
		ExpAst * initialiserExp, * guardExp, * stepExp;
		StmtAst * statementAst;

		For(ExpAst * initialiserExp, ExpAst * guardExp, ExpAst * stepExp, StmtAst * statementAst);
		void print(int counter);
		void generate_code();
};

class GlobalSymbolTableEntry {
	public:
		string identity;
		string return_type;
		int sz;
		map < string, LocalSymbolTableEntry * > symtab;
		Seq * ast;
		int cnt;

		GlobalSymbolTableEntry(string identity, string return_type, int sz);
};