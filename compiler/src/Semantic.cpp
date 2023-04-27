#include "Semantic.h"

Semantic::Semantic()
{
	this->_error = false;

	this->_tokenListIndex = 0;
	this->_tokenListLimit = 0;
	this->_scopeStack = std::stack<std::string>();
	this->_categoryStack = std::stack<std::string>();

}

Semantic::~Semantic()
{
}

void Semantic::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Semantic::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
	this->_tokenListLimit = this->_tokenList->getSize();
	this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
}

void Semantic::linkSyntaxTree(std::shared_ptr<SyntaxTree> tree)
{
	this->_syntaxTree = tree;
}

void Semantic::linkSymbolTableManager(std::shared_ptr<SymbolTableManager> table)
{
	this->_symbolTableManager = table;
}

void Semantic::run()
{
	this->start();
}

bool Semantic::getError()
{
	return this->_error;
}

compiler::TOKEN Semantic::getPeek()
{
	return this->_currentToken;
}

void Semantic::getNextToken()
{
	this->_tokenListIndex += 1;

	if (this->_tokenListIndex == this->_tokenListLimit) {
		this->_currentToken = compiler::DOLLAR_SIGN;
	}
	else {
		this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
	}
}

bool Semantic::matchToken(compiler::TOKEN token)
{
	if (getPeek() != token) {
		return false;
	}

	if (this->_myRow.get() != nullptr) {
		if (this->_categoryStack.top() == "function") { // FDEC (function declarations)
			if (getPeek() == compiler::KW_DEF) {
				this->_myRow->category = this->_categoryStack.top();
				this->_myRow->scope = this->_scopeStack.top();
			}
			else if (getPeek() == compiler::KW_INT) {
				this->_myRow->token = compiler::KW_INT;
				this->_myRow->type = "int";
			}
			else if (getPeek() == compiler::KW_DOUBLE) {
				this->_myRow->token = compiler::KW_DOUBLE;
				this->_myRow->type = "double";
			}
			else if (getPeek() == compiler::ID) {
				std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
				if (!this->_symbolTableManager->isIdentifierInCurrentScope(id)) {
					this->_myRow->lexeme = this->_tokenList->getLexeme(this->_tokenListIndex);
					this->_myRow->lineNum = this->_tokenList->getLineNumber(this->_tokenListIndex);
					this->_myRow->charPos = this->_tokenList->getCharPosition(this->_tokenListIndex);

					this->_symbolTableManager->appendRow(this->_myRow);
					pushScopeStack(this->_myRow->lexeme);
				}
				else {
					compiler::printConsoleError(compiler::SEMANTIC, "Function " + id + " was already defined...");
					this->_error = true;
					pushScopeStack(id);
				}
				this->_myRow = nullptr;
			}
		}
		else if (this->_categoryStack.top() == "param") { // PARAMS (parameter declarations)
			if (getPeek() == compiler::KW_INT || getPeek() == compiler::KW_DOUBLE) {
				this->_myRow->category = this->_categoryStack.top();
				this->_myRow->scope = this->_scopeStack.top();
				if (getPeek() == compiler::KW_INT) {
					this->_myRow->token = compiler::KW_INT;
					this->_myRow->type = "int";
				} else if (getPeek() == compiler::KW_DOUBLE) {
					this->_myRow->token = compiler::KW_DOUBLE;
					this->_myRow->type = "double";
				}
			} else if (getPeek() == compiler::ID) {
				std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
				if (!this->_symbolTableManager->isIdentifierInCurrentScope(id)) {
					this->_myRow->lexeme = this->_tokenList->getLexeme(this->_tokenListIndex);
					this->_myRow->lineNum = this->_tokenList->getLineNumber(this->_tokenListIndex);
					this->_myRow->charPos = this->_tokenList->getCharPosition(this->_tokenListIndex);
					this->_symbolTableManager->appendRow(this->_myRow);
				}
				else {
					compiler::printConsoleError(compiler::SEMANTIC, "Variable " + id + " was already defined...");
					this->_error = true;
				}
				this->_myRow = nullptr;
			}
		}
		else if (this->_categoryStack.top() == "decl") { // DECL (declarations)
			if (getPeek() == compiler::KW_INT || getPeek() == compiler::KW_DOUBLE) {
				this->_myRow->category = this->_categoryStack.top();
				this->_myRow->scope = this->_scopeStack.top();
				if (getPeek() == compiler::KW_INT) {
					this->_myRow->token = compiler::KW_INT;
					this->_myRow->type = "int";
				}
				else if (getPeek() == compiler::KW_DOUBLE) {
					this->_myRow->token = compiler::KW_DOUBLE;
					this->_myRow->type = "double";
				}
			}
			else if (getPeek() == compiler::ID) {
				std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
				if (!this->_symbolTableManager->isIdentifierInCurrentScope(id)) {
					auto nn = std::make_shared<SymbolTable::SymbolTableRow>();
					nn->category = this->_myRow->category;
					nn->scope = this->_myRow->scope;
					nn->token = this->_myRow->token;
					nn->type = this->_myRow->type;

					nn->lexeme = this->_tokenList->getLexeme(this->_tokenListIndex);
					nn->lineNum = this->_tokenList->getLineNumber(this->_tokenListIndex);
					nn->charPos = this->_tokenList->getCharPosition(this->_tokenListIndex);
					this->_symbolTableManager->appendRow(nn);
				}
				else {
					compiler::printConsoleError(compiler::SEMANTIC, "Variable " + id + " was already defined...");
					this->_error = true;
				}
			}
		}
	}
	else {
		if (this->_categoryStack.top() == "factor") {
			if (getPeek() == compiler::ID) {
				std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
				if (!this->_symbolTableManager->isIdentifierInCurrentScope(id)) {
					compiler::printConsoleError(compiler::SEMANTIC, "Variable " + id + " not defined...");
					this->_error = true;
					// must report error
					// TODO : Must build semantic error handling here. Must print to log file
				}
			}
		}
	}

	getNextToken();
	return true;
}

void Semantic::pushCategoryStack(std::string category)
{
	this->_categoryStack.push(category);
}

void Semantic::popCategoryStackTop()
{
	this->_categoryStack.pop();
}

void Semantic::pushScopeStack(std::string scope)
{
	this->_scopeStack.push(scope);
	this->_symbolTableManager->enterScope(scope);
}

void Semantic::popScopeStackTop()
{
	this->_scopeStack.pop();
	this->_symbolTableManager->exitScope();
}

void Semantic::start()
{
	pushCategoryStack("global");
	pushScopeStack("global");

	bool hmm = PROGRAM();

	if (!hmm) {
		this->_error = true;
		std::cout << "INCOMPLETE" << std::endl;
		return;
	}

	std::cout << "COMPLETE" << std::endl;
	return;
}

bool Semantic::PROGRAM()
{
	if (getPeek() == compiler::DOLLAR_SIGN || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_DEF || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		return FDECLS() && DECLARATIONS()
			&& STATEMESEQ() && matchToken(compiler::DOT);
	}

	// reportError("PROGRAM");
	return false;
}

bool Semantic::FDECLS()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchToken(compiler::SEMICOLON)
			&& FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DEF
		|| getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_IF
		|| getPeek() == compiler::KW_INT || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return FDECLS_EXT();
	}

	// reportError("FDECLS");
	return false;
}

bool Semantic::FDECLS_EXT()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchToken(compiler::SEMICOLON) && FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	// reportError("FDECLS_EXT");
	return false;
}

bool Semantic::FDEC()
{
	if (getPeek() == compiler::KW_DEF) {
		pushCategoryStack("function");
		this->_myRow = std::make_shared<SymbolTable::SymbolTableRow>();


		bool out = matchToken(compiler::KW_DEF) && TYPE()
			&& FNAME() && matchToken(compiler::LEFT_PAREN)
			&& PARAMS() && matchToken(compiler::RIGHT_PAREN)
			&& DECLARATIONS() && STATEMESEQ()
			&& matchToken(compiler::KW_FED);

		popCategoryStackTop();
		popScopeStackTop();
		return out;
	}

	// reportError("FDEC");
	return false;
}

bool Semantic::PARAMS()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		pushCategoryStack("param");

		this->_myRow = std::make_shared<SymbolTable::SymbolTableRow>();
		bool out = TYPE() && VAR()
			&& PARAMS_EXT();

		popCategoryStackTop();
		return out;
	}

	// reportError("PARAMS");
	return false;
}

bool Semantic::PARAMS_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && PARAMS();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	// reportError("PARAMS_EXT");
	return false;
}

bool Semantic::FNAME()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID);
	}

	// reportError("FNAME");
	return false;
}

bool Semantic::DECLARATIONS()
{ 
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchToken(compiler::SEMICOLON)
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return DECLARATIONS_EXT();
	}

	// reportError("DECLARATIONS");
	return false;
}

bool Semantic::DECLARATIONS_EXT()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchToken(compiler::SEMICOLON)
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	// reportError("DECLARATIONS_EXT");
	return false;
}

bool Semantic::DECL()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		pushCategoryStack("decl");

		this->_myRow = std::make_shared<SymbolTable::SymbolTableRow>();
		bool out = TYPE() && VARLIST();
		this->_myRow = nullptr;

		popCategoryStackTop();
		return out;
	}

	// reportError("DECL");
	return false;
}

bool Semantic::TYPE()
{
	if (getPeek() == compiler::KW_DOUBLE) {
		return matchToken(compiler::KW_DOUBLE);
	}
	else if (getPeek() == compiler::KW_INT) {
		return matchToken(compiler::KW_INT);
	}

	// reportError("TYPE");
	return false;
}

bool Semantic::VARLIST()
{
	if (getPeek() == compiler::ID) {
		return VAR() && VARLIST_EXT();
	}

	// reportError("VARLIST");
	return false;
}

bool Semantic::VARLIST_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && VARLIST();
	}
	else if (getPeek() == compiler::SEMICOLON) {
		// EPSILON
		return true;
	}

	// reportError("VARLIST_EXT");
	return false;
}

bool Semantic::STATEMESEQ()
{
	if (getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		return STATEMENT() && STATEMESEQ_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return STATEMENT() && STATEMESEQ_EXT();
	}

	// reportError("STATEMENTSEQ");
	return false;
}

bool Semantic::STATEMESEQ_EXT()
{
	if (getPeek() == compiler::SEMICOLON) {
		return matchToken(compiler::SEMICOLON) && STATEMESEQ();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return true;
	}

	// reportError("STATEMENTSEQ_EXT");
	return false;
}

bool Semantic::STATEMENT()
{
	if (getPeek() == compiler::ID) {
		return VAR() && matchToken(compiler::EQUAL)
			&& EXPR();
	}
	else if (getPeek() == compiler::KW_IF) {
		pushCategoryStack("if");

		bool out = matchToken(compiler::KW_IF) && BEXPR()
			&& matchToken(compiler::KW_THEN) && STATEMESEQ()
			&& STATEMENT_EXT();

		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::KW_PRINT) {
		pushCategoryStack("print");

		bool out = matchToken(compiler::KW_PRINT) && EXPR();

		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::KW_RETURN) {
		pushCategoryStack("return");

		bool out = matchToken(compiler::KW_RETURN) && EXPR();

		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::KW_WHILE) {
		pushCategoryStack("while");

		bool out = matchToken(compiler::KW_WHILE) && BEXPR()
			&& matchToken(compiler::KW_DO) && STATEMESEQ()
			&& matchToken(compiler::KW_OD);
		
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return true;
	}

	// reportError("STATEMENT");
	return false;
}

bool Semantic::STATEMENT_EXT()
{
	if (getPeek() == compiler::KW_ELSE) {
		pushCategoryStack("else");

		bool out = matchToken(compiler::KW_ELSE) && STATEMESEQ()
			&& matchToken(compiler::KW_FI);

		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::KW_FI) {
		return matchToken(compiler::KW_FI);
	}

	// reportError("STATEMENT_EXT");
	return false;
}

bool Semantic::EXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return TERM() && EXPR_EXT();
	}

	// reportError("EXPR");
	return false;
}

bool Semantic::EXPR_EXT()
{
	if (getPeek() == compiler::PLUS) {
		return matchToken(compiler::PLUS) && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == compiler::MINUS) {
		return matchToken(compiler::MINUS) && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	// reportError("EXPR_EXT");
	return false;
}

bool Semantic::TERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return FACTOR() && TERM_EXT();
	}

	// reportError("TERM");
	return false;
}

bool Semantic::TERM_EXT()
{
	if (getPeek() == compiler::MODULUS) {
		return matchToken(compiler::MODULUS) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::MULTIPLY) {
		return matchToken(compiler::MULTIPLY) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::DIVIDE) {
		return matchToken(compiler::DIVIDE) && FACTOR()
			&& TERM_EXT();
	}
	else if (
		getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::PLUS || getPeek() == compiler::MINUS
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	// reportError("TERM_EXT");
	return false;
}

bool Semantic::FACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && EXPR()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		return NUMBER();
	}
	else if (getPeek() == compiler::ID) {
		pushCategoryStack("factor");

		bool out = matchToken(compiler::ID) && FACTOR_EXT();

		popCategoryStackTop();
		return out;
	}

	// reportError("FACTOR");
	return false;
}

bool Semantic::FACTOR_EXT()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && EXPRSEQ() && matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::MODULUS || getPeek() == compiler::RIGHT_PAREN
		|| getPeek() == compiler::LEFT_BRACK || getPeek() == compiler::MULTIPLY
		|| getPeek() == compiler::DIVIDE || getPeek() == compiler::PLUS
		|| getPeek() == compiler::MINUS || getPeek() == compiler::COMMA
		|| getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return VAR_EXT();
	}

	// reportError("FACTOR_EXT");
	return false;
}

bool Semantic::EXPRSEQ()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return EXPR() && EXPRSEQ_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		return true;
	}

	// reportError("EXPRSEQ");
	return false;
}

bool Semantic::EXPRSEQ_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && EXPRSEQ();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	// reportError("EXPRSEQ_EXT");
	return false;
}

bool Semantic::BEXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BTERM() && BEXPR_EXT();
	}

	// reportError("BEXPR");
	return false;
}

bool Semantic::BEXPR_EXT()
{
	if (getPeek() == compiler::KW_OR) {
		return matchToken(compiler::KW_OR) && BTERM()
			&& BEXPR_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	// reportError("BEXPR_EXT");
	return false;
}

bool Semantic::BTERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BFACTOR() && BTERM_EXT();
	}

	// reportError("BTERM");
	return false;
}

bool Semantic::BTERM_EXT()
{
	if (getPeek() == compiler::KW_AND) {
		return matchToken(compiler::KW_AND) && BFACTOR()
			&& BTERM_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_OR || getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	// reportError("BTERM_EXT");
	return false;
}

bool Semantic::BFACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && BFACTOR_EXT()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::KW_NOT) {
		return matchToken(compiler::KW_NOT) && BFACTOR();
	}

	// reportError("BFACTOR");
	return false;
}

bool Semantic::BFACTOR_EXT()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		if (BEXPR()) {
			return true;
		}

		if (EXPR()) {
			return true;
		}
	}
	else if (getPeek() == compiler::KW_NOT) {
		return BEXPR();
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::ID
		|| getPeek() == compiler::VALUE_INTEGER) {
		return EXPR() && COMP()
			&& EXPR();
	}

	// reportError("BFACTOR_EXT");
	return false;
}

bool Semantic::COMP()
{
	if (getPeek() == compiler::COMP_LTHAN) {
		return matchToken(compiler::COMP_LTHAN);
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		return matchToken(compiler::COMP_LEQUAL);
	}
	else if (getPeek() == compiler::COMP_NOT) {
		return matchToken(compiler::COMP_NOT);
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		return matchToken(compiler::COMP_EQUAL);
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		return matchToken(compiler::COMP_GTHAN);

	}
	else if (getPeek() == compiler::COMP_GEQUAL) {
		return matchToken(compiler::COMP_GEQUAL);
	}

	// reportError("COMP");
	return false;
}

bool Semantic::VAR()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID) && VAR_EXT();
	}

	// reportError("VAR");
	return false;
}

bool Semantic::VAR_EXT()
{
	if (getPeek() == compiler::LEFT_BRACK) {
		return matchToken(compiler::LEFT_BRACK) && EXPR()
			&& matchToken(compiler::LEFT_BRACK);
	}
	else if (getPeek() == compiler::MODULUS || getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::MULTIPLY || getPeek() == compiler::DIVIDE
		|| getPeek() == compiler::PLUS || getPeek() == compiler::MINUS
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::COMP_LTHAN
		|| getPeek() == compiler::COMP_LEQUAL || getPeek() == compiler::COMP_NOT
		|| getPeek() == compiler::EQUAL || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	// reportError("VAR_EXT");
	return false;
}

bool Semantic::NUMBER()
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		return matchToken(compiler::VALUE_DOUBLE);
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		return matchToken(compiler::VALUE_INTEGER);
	}

	// reportError("NUMBER");
	return false;
}