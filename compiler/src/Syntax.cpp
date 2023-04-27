#include "Syntax.h"

Syntax::Syntax()
{
	this->_error = false;

	this->_tokenListIndex = 0;
	this->_tokenListLimit = 0;
}

Syntax::~Syntax()
{
}

void Syntax::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Syntax::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
	this->_tokenListLimit = this->_tokenList->getSize();
	this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
}

void Syntax::linkSyntaxTree(std::shared_ptr<SyntaxTree> tree)
{
	this->_syntaxTree = tree;
}

void Syntax::run()
{
	this->start();
}

bool Syntax::getError()
{
	return false;
}

compiler::TOKEN Syntax::getPeek()
{
	return this->_currentToken;
}

void Syntax::getNextToken()
{
	this->_tokenListIndex += 1;

	if (this->_tokenListIndex == this->_tokenListLimit) {
		this->_currentToken = compiler::DOLLAR_SIGN;
	}
	else {
		this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
	}
}

bool Syntax::matchToken(compiler::TOKEN token, std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() != token) {
		return false;
	}


	std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(true, this->_tokenList->getLexeme(this->_tokenListIndex));
	node->append(newNode);

	getNextToken();
	return true;
}

void Syntax::reportError(const std::string& errorchar)
{
	uint32_t linenum = this->_tokenList->getLineNumber(this->_tokenListIndex);
	uint32_t charpos = this->_tokenList->getCharPosition(this->_tokenListIndex);
	compiler::TOKEN token = this->_tokenList->getToken(this->_tokenListIndex);
	std::string lexeme = this->_tokenList->getLexeme(this->_tokenListIndex);

	this->_logFileBuffer->logSyntaxError(linenum, charpos, lexeme);
}

void Syntax::start()
{
	bool hmm = PROGRAM();

	if (!hmm) {
		this->_error = true;
		std::cout << "INCOMPLETE" << std::endl;
		return;
	}

	std::cout << "COMPLETE" << std::endl;

}

bool Syntax::PROGRAM()
{
	if (getPeek() == compiler::DOLLAR_SIGN || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_DEF || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "PROGRAM");
		this->_syntaxTree->connectNodeToHead(newNode);

		
		return FDECLS(newNode) && DECLARATIONS(newNode)
			&& STATEMENTSEQ(newNode) && matchToken(compiler::DOT, newNode);
	}

	reportError("PROGRAM");
	return false;
}

bool Syntax::FDECLS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DEF) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FDECLS");
		node->append(newNode);
		return FDEC(newNode) && matchToken(compiler::SEMICOLON, newNode)
			&& FDECLS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DEF
		|| getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_IF
		|| getPeek() == compiler::KW_INT || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FDECLS");
		node->append(newNode);
		return FDECLS_EXT(newNode);
	}

	reportError("FDECLS");
	return false;
}

bool Syntax::FDECLS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DEF) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FDECLS_EXT");
		node->append(newNode);
		return FDEC(newNode) && matchToken(compiler::SEMICOLON, newNode) && FDECLS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	reportError("FDECLS_EXT");
	return false;
}

bool Syntax::FDEC(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DEF) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FDEC");
		node->append(newNode);
		return matchToken(compiler::KW_DEF, newNode) && TYPE(newNode)
			&& FNAME(newNode) && matchToken(compiler::LEFT_PAREN, newNode)
			&& PARAMS(newNode) && matchToken(compiler::RIGHT_PAREN, newNode)
			&& DECLARATIONS(newNode) && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_FED, newNode);
	}

	reportError("FDEC");
	return false;
}

bool Syntax::PARAMS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "PARAMS");
		node->append(newNode);
		return TYPE(newNode) && VAR(newNode)
			&& PARAMS_EXT(newNode);
	}

	reportError("PARAMS");
	return false;
}

bool Syntax::PARAMS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "PARAMS_EXT");
		node->append(newNode);
		return matchToken(compiler::COMMA, newNode) && PARAMS(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	reportError("PARAMS_EXT");
	return false;
}

bool Syntax::FNAME(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FNAME");
		node->append(newNode);
		return matchToken(compiler::ID, newNode);
	}

	reportError("FNAME");
	return false;
}

bool Syntax::DECLARATIONS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "DECLARATIONS");
		node->append(newNode);
		return DECL(newNode) && matchToken(compiler::SEMICOLON, newNode)
			&& DECLARATIONS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "DECLARATIONS");
		node->append(newNode);
		return DECLARATIONS_EXT(newNode);
	}

	reportError("DECLARATIONS");
	return false;
}

bool Syntax::DECLARATIONS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "DECLARATIONS_EXT");
		node->append(newNode);
		return DECL(newNode) && matchToken(compiler::SEMICOLON, newNode)
			&& DECLARATIONS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "DECLARATIONS_EXT");
		node->append(newNode);
		return true;
	}

	reportError("DECLARATIONS_EXT");
	return false;
}

bool Syntax::DECL(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "DECL");
		node->append(newNode);
		return TYPE(newNode) && VARLIST(newNode);
	}

	reportError("DECL");
	return false;
}

bool Syntax::TYPE(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TYPE");
		node->append(newNode);
		return matchToken(compiler::KW_DOUBLE, newNode);
	}
	else if (getPeek() == compiler::KW_INT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TYPE");
		node->append(newNode);
		return matchToken(compiler::KW_INT, newNode);
	}

	reportError("TYPE");
	return false;
}

bool Syntax::VARLIST(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "VARLIST");
		node->append(newNode);
		return VAR(newNode) && VARLIST_EXT(newNode);
	}

	reportError("VARLIST");
	return false;
}

bool Syntax::VARLIST_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "VARLIST_EXT");
		node->append(newNode);
		return matchToken(compiler::COMMA, newNode) && VARLIST(newNode);
	}
	else if (getPeek() == compiler::SEMICOLON) {
		// EPSILON
		return true;
	}

	reportError("VARLIST_EXT");
	return false;
}

bool Syntax::STATEMENTSEQ(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENTSEQ");
		node->append(newNode);
		return STATEMENT(newNode) && STATEMENTSEQ_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENTSEQ");
		node->append(newNode);
		return STATEMENT(newNode) && STATEMENTSEQ_EXT(newNode);
	}

	reportError("STATEMENTSEQ");
	return false;
}

bool Syntax::STATEMENTSEQ_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::SEMICOLON) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENTSEQ_EXT");
		node->append(newNode);
		return matchToken(compiler::SEMICOLON, newNode) && STATEMENTSEQ(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENTSEQ_EXT");
		node->append(newNode);
		return true;
	}

	reportError("STATEMENTSEQ_EXT");
	return false;
}

bool Syntax::STATEMENT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return VAR(newNode) && matchToken(compiler::EQUAL, newNode)
			&& EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_IF) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return matchToken(compiler::KW_IF, newNode) && BEXPR(newNode)
			&& matchToken(compiler::KW_THEN, newNode) && STATEMENTSEQ(newNode)
			&& STATEMENT_EXT(newNode);
	}
	else if (getPeek() == compiler::KW_PRINT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return matchToken(compiler::KW_PRINT, newNode) && EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_RETURN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return matchToken(compiler::KW_RETURN, newNode) && EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_WHILE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return matchToken(compiler::KW_WHILE, newNode) && BEXPR(newNode)
			&& matchToken(compiler::KW_DO, newNode) && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_OD, newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT");
		node->append(newNode);
		return true;
	}

	reportError("STATEMENT");
	return false;
}

bool Syntax::STATEMENT_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_ELSE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT_EXT");
		node->append(newNode);
		return matchToken(compiler::KW_ELSE) && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_FI);
	}
	else if (getPeek() == compiler::KW_FI) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "STATEMENT_EXT");
		node->append(newNode);
		return matchToken(compiler::KW_FI);
	}

	reportError("STATEMENT_EXT");
	return false;
}

bool Syntax::EXPR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPR");
		node->append(newNode);
		return TERM(newNode) && EXPR_EXT(newNode);
	}

	reportError("EXPR");
	return false;
}

bool Syntax::EXPR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::PLUS) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPR_EXT");
		node->append(newNode);
		return matchToken(compiler::PLUS) && TERM(newNode)
			&& EXPR_EXT(newNode);
	}
	else if (getPeek() == compiler::MINUS) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPR_EXT");
		node->append(newNode);
		return matchToken(compiler::MINUS) && TERM(newNode)
			&& EXPR_EXT(newNode);
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
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPR_EXT");
		node->append(newNode);
		return true;
	}

	reportError("EXPR_EXT");
	return false;
}

bool Syntax::TERM(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TERM");
		node->append(newNode);
		return FACTOR(newNode) && TERM_EXT(newNode);
	}

	reportError("TERM");
	return false;
}

bool Syntax::TERM_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::MODULUS) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TERM_EXT");
		node->append(newNode);
		return matchToken(compiler::MODULUS) && FACTOR(newNode)
			&& TERM_EXT(newNode);
	}
	else if (getPeek() == compiler::MULTIPLY) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TERM_EXT");
		node->append(newNode);
		return matchToken(compiler::MULTIPLY) && FACTOR(newNode)
			&& TERM_EXT(newNode);
	}
	else if (getPeek() == compiler::DIVIDE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TERM_EXT");
		node->append(newNode);
		return matchToken(compiler::DIVIDE) && FACTOR(newNode)
			&& TERM_EXT(newNode);
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
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "TERM_EXT");
		node->append(newNode);
		return true;
	}

	reportError("TERM_EXT");
	return false;
}

bool Syntax::FACTOR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FACTOR");
		node->append(newNode);
		return matchToken(compiler::LEFT_PAREN) && EXPR(newNode)
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FACTOR");
		node->append(newNode);
		return NUMBER(newNode);
	}
	else if (getPeek() == compiler::ID) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FACTOR");
		node->append(newNode);
		return matchToken(compiler::ID) && FACTOR_EXT(newNode);
	}

	reportError("FACTOR");
	return false;
}

bool Syntax::FACTOR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FACTOR_EXT");
		node->append(newNode);
		return matchToken(compiler::LEFT_PAREN) && EXPRSEQ(newNode) && matchToken(compiler::RIGHT_PAREN);
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
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "FACTOR_EXT");
		node->append(newNode);
		return VAR_EXT(newNode);
	}

	reportError("FACTOR_EXT");
	return false;
}

bool Syntax::EXPRSEQ(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPRSEQ");
		node->append(newNode);
		return EXPR(newNode) && EXPRSEQ_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPRSEQ");
		node->append(newNode);
		return true;
	}

	reportError("EXPRSEQ");
	return false;
}

bool Syntax::EXPRSEQ_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPRSEQ_EXT");
		node->append(newNode);
		return matchToken(compiler::COMMA) && EXPRSEQ(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "EXPRSEQ_EXT");
		node->append(newNode);
		return true;
	}

	reportError("EXPRSEQ_EXT");
	return false;
}

bool Syntax::BEXPR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BEXPR");
		node->append(newNode);
		return BTERM(newNode) && BEXPR_EXT(newNode);
	}

	reportError("BEXPR");
	return false;
}

bool Syntax::BEXPR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_OR) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BEXPR_EXT");
		node->append(newNode);
		return matchToken(compiler::KW_OR) && BTERM(newNode)
			&& BEXPR_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_THEN) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BEXPR_EXT");
		node->append(newNode);
		return true;
	}

	reportError("BEXPR_EXT");
	return false;
}

bool Syntax::BTERM(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BTERM");
		node->append(newNode);
		return BFACTOR(newNode) && BTERM_EXT(newNode);
	}

	reportError("BTERM");
	return false;
}

bool Syntax::BTERM_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_AND) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BTERM_EXT");
		node->append(newNode);
		return matchToken(compiler::KW_AND) && BFACTOR(newNode)
			&& BTERM_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_OR || getPeek() == compiler::KW_THEN) {
		// EPSILON
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BTERM_EXT");
		node->append(newNode);
		return true;
	}

	reportError("BTERM_EXT");
	return false;
}

bool Syntax::BFACTOR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BFACTOR");
		node->append(newNode);
		return matchToken(compiler::LEFT_PAREN) && BFACTOR_EXT(newNode)
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::KW_NOT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BFACTOR");
		node->append(newNode);
		return matchToken(compiler::KW_NOT) && BFACTOR(newNode);
	}

	reportError("BFACTOR");
	return false;
}

bool Syntax::BFACTOR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BFACTOR_EXT");
		node->append(newNode);
		if (BEXPR(newNode)) {
			return true;
		}

		if (EXPR(newNode)) {
			return true;
		}
	}
	else if (getPeek() == compiler::KW_NOT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BFACTOR_EXT");
		node->append(newNode);
		return BEXPR(newNode);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::ID
		|| getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "BFACTOR_EXT");
		node->append(newNode);
		return EXPR(newNode) && COMP(newNode)
			&& EXPR(newNode);
	}

	reportError("BFACTOR_EXT");
	return false;
}

bool Syntax::COMP(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMP_LTHAN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_LTHAN);
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_LEQUAL);
	}
	else if (getPeek() == compiler::COMP_NOT) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_NOT);
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_EQUAL);
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_GTHAN);

	}
	else if (getPeek() == compiler::COMP_GEQUAL) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "COMP");
		node->append(newNode);
		return matchToken(compiler::COMP_GEQUAL);
	}

	reportError("COMP");
	return false;
}

bool Syntax::VAR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "VAR");
		node->append(newNode);
		return matchToken(compiler::ID) && VAR_EXT(newNode);
	}

	reportError("VAR");
	return false;
}

bool Syntax::VAR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_BRACK) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "VAR_EXT");
		node->append(newNode);
		return matchToken(compiler::LEFT_BRACK) && EXPR(newNode)
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
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "VAR_EXT");
		node->append(newNode);
		return true;
	}

	reportError("VAR_EXT");
	return false;
}

bool Syntax::NUMBER(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "NUMBER");
		node->append(newNode);
		return matchToken(compiler::VALUE_DOUBLE);
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(false, "NUMBER");
		node->append(newNode);
		return matchToken(compiler::VALUE_INTEGER);
	}

	reportError("NUMBER");
	return false;
}