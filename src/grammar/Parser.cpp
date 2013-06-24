/*----------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-----------------------------------------------------------------------*/


#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::ident(_Formula& f, _FormulaParsingContext& fpc) {
		Expect(_IDENTIFIER);
		_parser2013_pushIdentifier (this, f, fpc, t->val, false, false); 
}

void Parser::number(_Formula& f, _FormulaParsingContext& fpc) {
		Expect(_FLOAT);
		_parser2013_pushNumber (this, f, fpc, t->val); 
}

void Parser::matrix_row(_SimpleList & matrix_entries, _FormulaParsingContext& fpc, unsigned long& column_count, bool& is_const) {
		Expect(_OPEN_BRACE);
		_Formula* f = new _Formula; unsigned long my_column_count = 0; 
		if (_parser2013_isFollowedByAnCommaOrClosingBrace (this)) {
			Expect(_MULTIPLY);
		} else if (StartOf(1)) {
			expression(*f, fpc);
		} else SynErr(37);
		_parser2013_add_matrix_entry (this, matrix_entries, f, fpc, is_const); my_column_count++; 
		while (la->kind == _COMMA) {
			Get();
			f = new _Formula; 
			if (_parser2013_isFollowedByAnCommaOrClosingBrace (this)) {
				Expect(_MULTIPLY);
			} else if (StartOf(1)) {
				expression(*f, fpc);
			} else SynErr(38);
			_parser2013_add_matrix_entry (this, matrix_entries, f, fpc, is_const); my_column_count++; 
		}
		Expect(_CLOSE_BRACE);
		_parser2013_matrix_checkRowLengths (this, fpc, column_count, my_column_count); 
}

void Parser::expression(_Formula& f, _FormulaParsingContext& fpc) {
		while (!(StartOf(2))) {SynErr(39); Get();}
		assignment_op(f, fpc);
}

void Parser::dense_matrix(_Formula& f, _FormulaParsingContext& fpc) {
		Expect(_OPEN_BRACE);
		_SimpleList matrix_entries; unsigned long n_rows = 0; unsigned long n_cols = 0; bool is_const = true; 
		matrix_row(matrix_entries, fpc, n_cols, is_const);
		n_rows ++; 
		while (la->kind == _OPEN_BRACE) {
			matrix_row(matrix_entries, fpc, n_cols, is_const);
			n_rows ++; 
		}
		Expect(_CLOSE_BRACE);
		_parser2013_pushObject (this, f, fpc, _parser2013_createDenseMatrix (this, fpc, &matrix_entries, n_rows, n_cols, is_const));  
}

void Parser::matrix_element(_SimpleList & matrix_definition, _FormulaParsingContext& fpc, bool& is_const) {
		Expect(_OPEN_BRACE);
		_Formula * h = new _Formula, * v = new _Formula, * d = new _Formula; 
		expression(*h, fpc);
		Expect(_COMMA);
		expression(*v, fpc);
		Expect(_COMMA);
		expression(*d, fpc);
		Expect(_CLOSE_BRACE);
		_parser2013_pushSparseElementEntry (this, fpc, matrix_definition, h, v, d, is_const); 
}

void Parser::sparse_matrix(_Formula& f, _FormulaParsingContext& fpc) {
		Expect(_OPEN_BRACE);
		bool is_matrix = false; _SimpleList* matrix_definition = new _SimpleList; 
		_Formula * hd = new _Formula, *vd = new _Formula; bool is_const = true; 
		if (_parser2013_StringAndColon (this)) {
			if (StartOf(1)) {
				_Formula *key = new _Formula, *value = new _Formula; 
				expression(*key, fpc);
				Expect(_COLON);
				expression(*value, fpc);
				_parser2013_addADictionaryElement (this, *matrix_definition, key, value, fpc, is_const); 
			}
			while (la->kind == _COMMA) {
				_Formula *key = new _Formula, *value = new _Formula; 
				Get();
				expression(*key, fpc);
				Expect(_COLON);
				expression(*value, fpc);
				_parser2013_addADictionaryElement (this, *matrix_definition, key, value, fpc, is_const); 
			}
		} else if (StartOf(1)) {
			is_matrix = true; 
			expression(*hd, fpc);
			if (la->kind == _COMMA) {
				Get();
				expression(*vd, fpc);
				while (la->kind == _COMMA) {
					Get();
					matrix_element(*matrix_definition, fpc, is_const);
				}
			}
		} else SynErr(40);
		Expect(_CLOSE_BRACE);
		if (is_matrix) {
		   _parser2013_createSparseMatrix (this, f, fpc, hd, vd, matrix_definition, is_const);}
		else {
		   _parser2013_createDictionary (this, f, fpc, *matrix_definition, is_const);
		   delete (hd); delete (vd);
		}
		
}

void Parser::function_call(_Formula& f, _FormulaParsingContext& fpc) {
		_List argument_names; 
		Expect(_IDENTIFIER);
		_String func_id (t->val); 
		Expect(_OPEN_PARENTHESIS);
		while (StartOf(1)) {
			expression(f, fpc);
			argument_names.AppendNewInstance(new _String);
			while (la->kind == _COMMA) {
				Get();
				expression(f, fpc);
				argument_names.AppendNewInstance(new _String);
			}
		}
		Expect(_CLOSE_PARENTHESIS);
		_parser2013_pushFunctionCall (this, f, fpc, func_id, argument_names); 
}

void Parser::primitive(_Formula& f, _FormulaParsingContext& fpc) {
		if (la->kind == _FLOAT) {
			number(f, fpc);
		} else if (la->kind == _DOUBLE_QUOTE_STRING) {
			Get();
			_parser2013_pushString (this, f, fpc, t->val); 
		} else if (la->kind == _SINGLE_QUOTE_STRING) {
			Get();
			_parser2013_pushString (this, f, fpc, t->val); 
		} else if (la->kind == _OPEN_PARENTHESIS) {
			Get();
			expression(f, fpc);
			Expect(_CLOSE_PARENTHESIS);
		} else if (_parser2013_TwoOpenBraces (this)) {
			dense_matrix(f, fpc);
		} else if (la->kind == _OPEN_BRACE) {
			sparse_matrix(f, fpc);
		} else if (la->kind == _NONE_OBJECT) {
			Get();
			_parser2013_pushNone (this, f, fpc); 
		} else if (_parser2013_IdentFollowedByAnOpenParenthesis(this)) {
			function_call(f, fpc);
		} else if (la->kind == _IDENTIFIER) {
			ident(f, fpc);
		} else SynErr(41);
}

void Parser::indexing_operation(_Formula& f, _FormulaParsingContext& fpc) {
		primitive(f, fpc);
		int bracket_count = 0; 
		if (la->kind == _OPEN_BRACKET) {
			Get();
			expression(f, fpc);
			Expect(_CLOSE_BRACKET);
			bracket_count ++; 
			if (la->kind == _OPEN_BRACKET) {
				Get();
				expression(f, fpc);
				Expect(_CLOSE_BRACKET);
				bracket_count ++; 
			}
		}
		if (bracket_count > 0) { _parser2013_pushOp (this, f, fpc, HY_OP_CODE_MACCESS, bracket_count+1);} 
}

void Parser::reference_like(_Formula& f, _FormulaParsingContext& fpc) {
		long op_code = HY_OP_CODE_NONE; 
		if (la->kind == _MULTIPLY || la->kind == 22 /* "^" */) {
			if (la->kind == _MULTIPLY) {
				Get();
				op_code = HY_OP_CODE_MUL; 
			} else {
				Get();
				op_code = HY_OP_CODE_POWER; 
			}
		}
		indexing_operation(f, fpc);
		if (op_code != HY_OP_CODE_NONE) _parser2013_pushOp (this, f,fpc,op_code,1); 
}

void Parser::power_like(_Formula& f, _FormulaParsingContext& fpc) {
		reference_like(f, fpc);
		long op_code; 
		while (la->kind == 22 /* "^" */) {
			Get();
			op_code = HY_OP_CODE_POWER; 
			reference_like(f, fpc);
			_parser2013_pushOp (this, f, fpc, op_code , 2); 
		}
}

void Parser::multiplication_like(_Formula& f, _FormulaParsingContext& fpc) {
		power_like(f, fpc);
		long op_code; 
		while (StartOf(3)) {
			if (la->kind == _MULTIPLY) {
				Get();
				op_code = HY_OP_CODE_MUL; 
			} else if (la->kind == 23 /* "/" */) {
				Get();
				op_code = HY_OP_CODE_DIV; 
			} else if (la->kind == 24 /* "$" */) {
				Get();
				op_code = HY_OP_CODE_IDIV; 
			} else {
				Get();
				op_code = HY_OP_CODE_MOD; 
			}
			power_like(f, fpc);
			_parser2013_pushOp (this, f, fpc, op_code , 2); 
		}
}

void Parser::addition_like(_Formula& f, _FormulaParsingContext& fpc) {
		long unary_code = HY_OP_CODE_NONE, op_code; 
		if (la->kind == 26 /* "+" */ || la->kind == 27 /* "-" */) {
			if (la->kind == 26 /* "+" */) {
				Get();
				unary_code = HY_OP_CODE_ADD; 
			} else {
				Get();
				unary_code = HY_OP_CODE_SUB; 
			}
		}
		multiplication_like(f, fpc);
		if (unary_code != HY_OP_CODE_NONE) {
		_parser2013_pushOp (this, f, fpc, unary_code , 1); 
		} 
		while (la->kind == 26 /* "+" */ || la->kind == 27 /* "-" */) {
			if (la->kind == 26 /* "+" */) {
				Get();
				op_code = HY_OP_CODE_ADD; 
			} else {
				Get();
				op_code = HY_OP_CODE_SUB; 
			}
			multiplication_like(f, fpc);
			_parser2013_pushOp (this, f, fpc, op_code , 2); 
		}
}

void Parser::logical_comp(_Formula& f, _FormulaParsingContext& fpc) {
		addition_like(f, fpc);
		long op_code; 
		while (StartOf(4)) {
			switch (la->kind) {
			case 28 /* "==" */: {
				Get();
				op_code = HY_OP_CODE_EQ; 
				break;
			}
			case 29 /* "!=" */: {
				Get();
				op_code = HY_OP_CODE_NEQ; 
				break;
			}
			case 30 /* ">" */: {
				Get();
				op_code = HY_OP_CODE_GREATER; 
				break;
			}
			case 31 /* "<" */: {
				Get();
				op_code = HY_OP_CODE_LESS; 
				break;
			}
			case 32 /* ">=" */: {
				Get();
				op_code = HY_OP_CODE_GEQ; 
				break;
			}
			case 33 /* "<=" */: {
				Get();
				op_code = HY_OP_CODE_LEQ; 
				break;
			}
			}
			addition_like(f, fpc);
			_parser2013_pushOp (this, f, fpc, op_code , 2); 
		}
}

void Parser::logical_and(_Formula& f, _FormulaParsingContext& fpc) {
		logical_comp(f, fpc);
		while (la->kind == 34 /* "&&" */) {
			Get();
			logical_comp(f, fpc);
			_parser2013_pushOp (this, f, fpc, HY_OP_CODE_AND , 2); 
		}
}

void Parser::logical_or(_Formula& f, _FormulaParsingContext& fpc) {
		logical_and(f, fpc);
		while (la->kind == 35 /* "||" */) {
			Get();
			logical_and(f, fpc);
			_parser2013_pushOp (this, f, fpc, HY_OP_CODE_OR , 2); 
		}
}

void Parser::assignment_op(_Formula& f, _FormulaParsingContext& fpc) {
		while (!(StartOf(2))) {SynErr(42); Get();}
		logical_or(f, fpc);
		if (StartOf(5)) {
			_Formula rhs; long assignment_type = _HY_OPERATION_ASSIGNMENT_VALUE, op_code = HY_OP_CODE_NONE;
			switch (la->kind) {
			case _EQUAL: {
				Get();
				break;
			}
			case _PLUS_EQUAL: {
				Get();
				op_code = HY_OP_CODE_ADD; 
				break;
			}
			case _MINUS_EQUAL: {
				Get();
				op_code = HY_OP_CODE_SUB; 
				break;
			}
			case _TIMES_EQUAL: {
				Get();
				op_code = HY_OP_CODE_MUL; 
				break;
			}
			case _DIV_EQUAL: {
				Get();
				op_code = HY_OP_CODE_DIV; 
				break;
			}
			case _ASSIGN: {
				Get();
				printf ("\nExpression assignment\n"); 
				break;
			}
			}
			logical_or(rhs, fpc);
			_parser2013_handleAssignment (this, f,  rhs, fpc, assignment_type, op_code, _parser2013_checkLvalue (this, f, fpc));   
			
		}
}

void Parser::statement(_ExecutionList &current_code_stream) {
		_Formula f; _FormulaParsingContext fpc; 
		expression(f, fpc);
		Expect(_SEMICOLON);
		_String s ((_String*)f.Compute()->toStr()); printf ("Formula: %s\n", s.sData); 
}

void Parser::block(_ExecutionList &current_code_stream) {
		Expect(_OPEN_BRACE);
		while (StartOf(1)) {
			if (StartOf(1)) {
				statement(current_code_stream);
			} else {
				block(current_code_stream);
			}
		}
		Expect(_CLOSE_BRACE);
}

void Parser::hyphy_batch_language() {
		if (StartOf(2)) {
			_ExecutionList current_code_stream; 
			while (StartOf(1)) {
				if (StartOf(1)) {
					statement(current_code_stream);
				} else {
					block(current_code_stream);
				}
			}
		} else if (StartOf(1)) {
			_Formula f; _FormulaParsingContext fpc; 
			expression(f, fpc);
			while (!(la->kind == _EOF)) {SynErr(43); Get();}
		} else SynErr(44);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	hyphy_batch_language();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 36;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[6][38] = {
		{T,T,T,T, T,T,T,x, x,x,x,x, T,x,x,T, x,x,x,x, x,x,T,x, x,x,T,T, x,x,x,x, x,x,x,x, x,x},
		{x,T,T,T, T,T,T,x, x,x,x,x, T,x,x,T, x,x,x,x, x,x,T,x, x,x,T,T, x,x,x,x, x,x,x,x, x,x},
		{T,T,T,T, T,T,T,x, x,x,x,x, T,x,x,T, x,x,x,x, x,x,T,x, x,x,T,T, x,x,x,x, x,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, x,x,x,x, x,x,x,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,T,T, T,T,x,x, x,x},
		{x,x,x,x, x,x,x,x, T,T,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"IDENTIFIER expected"); break;
			case 2: s = coco_string_create(L"FLOAT expected"); break;
			case 3: s = coco_string_create(L"SINGLE_QUOTE_STRING expected"); break;
			case 4: s = coco_string_create(L"DOUBLE_QUOTE_STRING expected"); break;
			case 5: s = coco_string_create(L"NONE_OBJECT expected"); break;
			case 6: s = coco_string_create(L"OPEN_PARENTHESIS expected"); break;
			case 7: s = coco_string_create(L"CLOSE_PARENTHESIS expected"); break;
			case 8: s = coco_string_create(L"EQUAL expected"); break;
			case 9: s = coco_string_create(L"ASSIGN expected"); break;
			case 10: s = coco_string_create(L"COMMA expected"); break;
			case 11: s = coco_string_create(L"CLOSE_BRACE expected"); break;
			case 12: s = coco_string_create(L"OPEN_BRACE expected"); break;
			case 13: s = coco_string_create(L"CLOSE_BRACKET expected"); break;
			case 14: s = coco_string_create(L"OPEN_BRACKET expected"); break;
			case 15: s = coco_string_create(L"MULTIPLY expected"); break;
			case 16: s = coco_string_create(L"COLON expected"); break;
			case 17: s = coco_string_create(L"SEMICOLON expected"); break;
			case 18: s = coco_string_create(L"PLUS_EQUAL expected"); break;
			case 19: s = coco_string_create(L"MINUS_EQUAL expected"); break;
			case 20: s = coco_string_create(L"TIMES_EQUAL expected"); break;
			case 21: s = coco_string_create(L"DIV_EQUAL expected"); break;
			case 22: s = coco_string_create(L"\"^\" expected"); break;
			case 23: s = coco_string_create(L"\"/\" expected"); break;
			case 24: s = coco_string_create(L"\"$\" expected"); break;
			case 25: s = coco_string_create(L"\"%\" expected"); break;
			case 26: s = coco_string_create(L"\"+\" expected"); break;
			case 27: s = coco_string_create(L"\"-\" expected"); break;
			case 28: s = coco_string_create(L"\"==\" expected"); break;
			case 29: s = coco_string_create(L"\"!=\" expected"); break;
			case 30: s = coco_string_create(L"\">\" expected"); break;
			case 31: s = coco_string_create(L"\"<\" expected"); break;
			case 32: s = coco_string_create(L"\">=\" expected"); break;
			case 33: s = coco_string_create(L"\"<=\" expected"); break;
			case 34: s = coco_string_create(L"\"&&\" expected"); break;
			case 35: s = coco_string_create(L"\"||\" expected"); break;
			case 36: s = coco_string_create(L"??? expected"); break;
			case 37: s = coco_string_create(L"invalid matrix_row"); break;
			case 38: s = coco_string_create(L"invalid matrix_row"); break;
			case 39: s = coco_string_create(L"this symbol not expected in expression"); break;
			case 40: s = coco_string_create(L"invalid sparse_matrix"); break;
			case 41: s = coco_string_create(L"invalid primitive"); break;
			case 42: s = coco_string_create(L"this symbol not expected in assignment_op"); break;
			case 43: s = coco_string_create(L"this symbol not expected in hyphy_batch_language"); break;
			case 44: s = coco_string_create(L"invalid hyphy_batch_language"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


