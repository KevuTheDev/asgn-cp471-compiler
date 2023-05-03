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

bool Syntax::matchToken(compiler::TOKEN token, std::shared_ptr<SyntaxNode> node, std::string scope)
{
	if (getPeek() != token) {
		return false;
	}

	auto newNode = createAndConnectNode(node, scope);
	// change newNode token data value
	newNode->setData(this->_tokenList->getLexeme(this->_tokenListIndex));

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

std::shared_ptr<SyntaxNode> Syntax::createAndConnectNode(std::shared_ptr<SyntaxNode> dst, std::string scope)
{
	std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>(scope);
	dst->append(newNode);
	return newNode;
}



void Syntax::start()
{
	std::shared_ptr<SyntaxNode> newNode = std::make_shared<SyntaxNode>("PROGRAM");
	this->_syntaxTree->connectNodeToHead(newNode);

	bool hmm = PROGRAM(newNode);

	if (!hmm) {
		this->_error = true;
		std::cout << "INCOMPLETE" << std::endl;
		return;
	}

	std::cout << "COMPLETE" << std::endl;
}

bool Syntax::PROGRAM(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::DOLLAR_SIGN || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_DEF || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		return FDECLS(node) && DECLARATIONS(node)
			&& STATEMENTSEQ(node) && matchToken(compiler::DOT, node, "PROGRAM");
	}

	reportError("PROGRAM");
	return false;
}

bool Syntax::FDECLS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DEF) {
		auto newNode = createAndConnectNode(node, "FDECLS");
		return FDEC(newNode) && matchToken(compiler::SEMICOLON, newNode, "FDECLS")
			&& FDECLS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DEF
		|| getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_IF
		|| getPeek() == compiler::KW_INT || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		auto newNode = createAndConnectNode(node, "FDECLS");
		return FDECLS_EXT(newNode);
	}

	reportError("FDECLS");
	return false;
}

bool Syntax::FDECLS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DEF) {
		auto newNode = createAndConnectNode(node, "FDECLS_EXT");
		return FDEC(newNode) && matchToken(compiler::SEMICOLON, newNode, "FDECLS_EXT") && FDECLS_EXT(newNode);
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
		auto newNode = createAndConnectNode(node, "FDEC");
		return matchToken(compiler::KW_DEF, newNode, "FDEC") && TYPE(newNode)
			&& FNAME(newNode) && matchToken(compiler::LEFT_PAREN, newNode, "FDEC")
			&& PARAMS(newNode) && matchToken(compiler::RIGHT_PAREN, newNode, "FDEC")
			&& DECLARATIONS(newNode) && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_FED, newNode, "FDEC");
	}

	reportError("FDEC");
	return false;
}

bool Syntax::PARAMS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		auto newNode = createAndConnectNode(node, "PARAMS");
		return TYPE(newNode) && VAR(newNode)
			&& PARAMS_EXT(newNode);
	}

	reportError("PARAMS");
	return false;
}

bool Syntax::PARAMS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		auto newNode = createAndConnectNode(node, "PARAMS_EXT");
		return matchToken(compiler::COMMA, newNode, "PARAMS_EXT") && PARAMS(newNode);
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
		auto newNode = createAndConnectNode(node, "FNAME");
		return matchToken(compiler::ID, newNode, "FNAME");
	}

	reportError("FNAME");
	return false;
}

bool Syntax::DECLARATIONS(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		auto newNode = createAndConnectNode(node, "DECLARATIONS");
		return DECL(newNode) && matchToken(compiler::SEMICOLON, newNode, "DECLARATIONS")
			&& DECLARATIONS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		auto newNode = createAndConnectNode(node, "DECLARATIONS");
		return DECLARATIONS_EXT(newNode);
	}

	reportError("DECLARATIONS");
	return false;
}

bool Syntax::DECLARATIONS_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		auto newNode = createAndConnectNode(node, "DECLARATIONS_EXT");
		return DECL(newNode) && matchToken(compiler::SEMICOLON, newNode, "DECLARATIONS_EXT")
			&& DECLARATIONS_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	reportError("DECLARATIONS_EXT");
	return false;
}

bool Syntax::DECL(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		auto newNode = createAndConnectNode(node, "DECL");
		return TYPE(newNode) && VARLIST(newNode);
	}

	reportError("DECL");
	return false;
}

bool Syntax::TYPE(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_DOUBLE) {
		auto newNode = createAndConnectNode(node, "TYPE");
		return matchToken(compiler::KW_DOUBLE, newNode, "TYPE");
	}
	else if (getPeek() == compiler::KW_INT) {
		auto newNode = createAndConnectNode(node, "TYPE");
		return matchToken(compiler::KW_INT, newNode, "TYPE");
	}

	reportError("TYPE");
	return false;
}

bool Syntax::VARLIST(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		auto newNode = createAndConnectNode(node, "VARLIST");
		return VAR(newNode) && VARLIST_EXT(newNode);
	}

	reportError("VARLIST");
	return false;
}

bool Syntax::VARLIST_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		auto newNode = createAndConnectNode(node, "VARLIST_EXT");
		return matchToken(compiler::COMMA, newNode, "VARLIST_EXT") && VARLIST(newNode);
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
		auto newNode = createAndConnectNode(node, "STATEMENTSEQ");
		return STATEMENT(newNode) && STATEMENTSEQ_EXT(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		auto newNode = createAndConnectNode(node, "STATEMENTSEQ");
		return STATEMENT(newNode) && STATEMENTSEQ_EXT(newNode);
	}

	reportError("STATEMENTSEQ");
	return false;
}

bool Syntax::STATEMENTSEQ_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::SEMICOLON) {
		auto newNode = createAndConnectNode(node, "STATEMENTSEQ_EXT");
		return matchToken(compiler::SEMICOLON, newNode, "STATEMENTSEQ_EXT") && STATEMENTSEQ(newNode);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return true;
	}

	reportError("STATEMENTSEQ_EXT");
	return false;
}

bool Syntax::STATEMENT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		auto newNode = createAndConnectNode(node, "STATEMENT");
		return VAR(newNode) && matchToken(compiler::EQUAL, node, "STATEMENT")
			&& EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_IF) {
		auto newNode = createAndConnectNode(node, "STATEMENT");
		return matchToken(compiler::KW_IF, newNode, "STATEMENT") && BEXPR(newNode)
			&& matchToken(compiler::KW_THEN, newNode, "STATEMENT") && STATEMENTSEQ(newNode)
			&& STATEMENT_EXT(newNode);
	}
	else if (getPeek() == compiler::KW_PRINT) {
		auto newNode = createAndConnectNode(node, "STATEMENT");
		return matchToken(compiler::KW_PRINT, newNode, "STATEMENT") && EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_RETURN) {
		auto newNode = createAndConnectNode(node, "STATEMENT");
		return matchToken(compiler::KW_RETURN, newNode, "STATEMENT") && EXPR(newNode);
	}
	else if (getPeek() == compiler::KW_WHILE) {
		auto newNode = createAndConnectNode(node, "STATEMENT");
		return matchToken(compiler::KW_WHILE, newNode, "STATEMENT") && BEXPR(newNode)
			&& matchToken(compiler::KW_DO, newNode, "STATEMENT") && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_OD, newNode, "STATEMENT");
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return true;
	}

	reportError("STATEMENT");
	return false;
}

bool Syntax::STATEMENT_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_ELSE) {
		auto newNode = createAndConnectNode(node, "STATEMENT_EXT");
		return matchToken(compiler::KW_ELSE, newNode, "STATEMENT_EXT") && STATEMENTSEQ(newNode)
			&& matchToken(compiler::KW_FI, newNode, "STATEMENT_EXT");
	}
	else if (getPeek() == compiler::KW_FI) {
		auto newNode = createAndConnectNode(node, "STATEMENT_EXT");
		return matchToken(compiler::KW_FI, newNode, "STATEMENT_EXT");
	}

	reportError("STATEMENT_EXT");
	return false;
}

bool Syntax::EXPR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "EXPR");
		return TERM(newNode) && EXPR_EXT(newNode);
	}

	reportError("EXPR");
	return false;
}

bool Syntax::EXPR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::PLUS) {
		auto newNode = createAndConnectNode(node, "EXPR_EXT");
		return matchToken(compiler::PLUS, newNode, "EXPR_EXT") && TERM(newNode)
			&& EXPR_EXT(newNode);
	}
	else if (getPeek() == compiler::MINUS) {
		auto newNode = createAndConnectNode(node, "EXPR_EXT");
		return matchToken(compiler::MINUS, newNode, "EXPR_EXT") && TERM(newNode)
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
		return true;
	}

	reportError("EXPR_EXT");
	return false;
}

bool Syntax::TERM(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "TERM");
		return FACTOR(newNode) && TERM_EXT(newNode);
	}

	reportError("TERM");
	return false;
}

bool Syntax::TERM_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::MODULUS) {
		auto newNode = createAndConnectNode(node, "TERM_EXT");
		return matchToken(compiler::MODULUS, newNode, "TERM_EXT") && FACTOR(newNode)
			&& TERM_EXT(newNode);
	}
	else if (getPeek() == compiler::MULTIPLY) {
		auto newNode = createAndConnectNode(node, "TERM_EXT");
		return matchToken(compiler::MULTIPLY, newNode, "TERM_EXT") && FACTOR(newNode)
			&& TERM_EXT(newNode);
	}
	else if (getPeek() == compiler::DIVIDE) {
		auto newNode = createAndConnectNode(node, "TERM_EXT");
		return matchToken(compiler::DIVIDE, newNode, "TERM_EXT") && FACTOR(newNode)
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
		return true;
	}

	reportError("TERM_EXT");
	return false;
}

bool Syntax::FACTOR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		auto newNode = createAndConnectNode(node, "FACTOR");
		return matchToken(compiler::LEFT_PAREN, newNode, "FACTOR") && EXPR(newNode)
			&& matchToken(compiler::RIGHT_PAREN, newNode, "FACTOR");
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "FACTOR");
		return NUMBER(newNode);
	}
	else if (getPeek() == compiler::ID) {
		auto newNode = createAndConnectNode(node, "FACTOR");
		return matchToken(compiler::ID, newNode, "FACTOR") && FACTOR_EXT(newNode);
	}

	reportError("FACTOR");
	return false;
}

bool Syntax::FACTOR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		auto newNode = createAndConnectNode(node, "FACTOR_EXT");
		return matchToken(compiler::LEFT_PAREN, newNode, "FACTOR_EXT")
			 && EXPRSEQ(newNode) && matchToken(compiler::RIGHT_PAREN, newNode, "FACTOR_EXT");
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
		auto newNode = createAndConnectNode(node, "FACTOR_EXT");
		bool out = VAR_EXT(newNode);
		if (newNode->hasNoChildren()) {
			node->removeLastNode();
		}
		
		return out;
	}

	reportError("FACTOR_EXT");
	return false;
}

bool Syntax::EXPRSEQ(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "EXPRSEQ");
		return EXPR(newNode) && EXPRSEQ_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		return true;
	}

	reportError("EXPRSEQ");
	return false;
}

bool Syntax::EXPRSEQ_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMMA) {
		auto newNode = createAndConnectNode(node, "EXPRSEQ_EXT");
		return matchToken(compiler::COMMA, newNode, "EXPRSEQ_EXT") && EXPRSEQ(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	reportError("EXPRSEQ_EXT");
	return false;
}

bool Syntax::BEXPR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		auto newNode = createAndConnectNode(node, "BEXPR");
		return BTERM(newNode) && BEXPR_EXT(newNode);
	}

	reportError("BEXPR");
	return false;
}

bool Syntax::BEXPR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_OR) {
		auto newNode = createAndConnectNode(node, "BEXPR_EXT");
		return matchToken(compiler::KW_OR, newNode, "BEXPR_EXT") && BTERM(newNode)
			&& BEXPR_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	reportError("BEXPR_EXT");
	return false;
}

bool Syntax::BTERM(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		auto newNode = createAndConnectNode(node, "BTERM");
		return BFACTOR(newNode) && BTERM_EXT(newNode);
	}

	reportError("BTERM");
	return false;
}

bool Syntax::BTERM_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::KW_AND) {
		auto newNode = createAndConnectNode(node, "BTERM_EXT");
		return matchToken(compiler::KW_AND, newNode, "BTERM_EXT") && BFACTOR(newNode)
			&& BTERM_EXT(newNode);
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_OR || getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	reportError("BTERM_EXT");
	return false;
}

bool Syntax::BFACTOR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		auto newNode = createAndConnectNode(node, "BFACTOR");
		return matchToken(compiler::LEFT_PAREN, newNode, "BFACTOR") && BFACTOR_EXT(newNode)
			&& matchToken(compiler::RIGHT_PAREN, newNode, "BFACTOR");
	}
	else if (getPeek() == compiler::KW_NOT) {
		auto newNode = createAndConnectNode(node, "BFACTOR");
		return matchToken(compiler::KW_NOT, newNode, "BFACTOR") && BFACTOR(newNode);
	}

	reportError("BFACTOR");
	return false;
}

bool Syntax::BFACTOR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_PAREN) {
		auto newNode = createAndConnectNode(node, "BFACTOR_EXT");
		if (BEXPR(newNode)) {
			return true;
		}

		if (EXPR(newNode)) {
			return true;
		}
	}
	else if (getPeek() == compiler::KW_NOT) {
		auto newNode = createAndConnectNode(node, "BFACTOR_EXT");
		return BEXPR(newNode);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::ID
		|| getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "BFACTOR_EXT");
		return EXPR(newNode) && COMP(newNode)
			&& EXPR(newNode);
	}

	reportError("BFACTOR_EXT");
	return false;
}

bool Syntax::COMP(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::COMP_LTHAN) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_LTHAN, newNode, "COMP");
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_LEQUAL, newNode, "COMP");
	}
	else if (getPeek() == compiler::COMP_NOT) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_NOT, newNode, "COMP");
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_EQUAL, newNode, "COMP");
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_GTHAN, newNode, "COMP");
	}
	else if (getPeek() == compiler::COMP_GEQUAL) {
		auto newNode = createAndConnectNode(node, "COMP");
		return matchToken(compiler::COMP_GEQUAL, newNode, "COMP");
	}

	reportError("COMP");
	return false;
}

bool Syntax::VAR(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::ID) {
		auto newNode = createAndConnectNode(node, "VAR");
		return matchToken(compiler::ID, newNode, "VAR") && VAR_EXT(newNode);
	}

	reportError("VAR");
	return false;
}

bool Syntax::VAR_EXT(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::LEFT_BRACK) {
		auto newNode = createAndConnectNode(node, "VAR_EXT");
		return matchToken(compiler::LEFT_BRACK, newNode, "VAR_EXT") && EXPR(newNode)
			&& matchToken(compiler::LEFT_BRACK, newNode, "VAR_EXT");
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

	reportError("VAR_EXT");
	return false;
}

bool Syntax::NUMBER(std::shared_ptr<SyntaxNode> node)
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		auto newNode = createAndConnectNode(node, "NUMBER");
		return matchToken(compiler::VALUE_DOUBLE, newNode, "NUMBER");
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		auto newNode = createAndConnectNode(node, "NUMBER");
		return matchToken(compiler::VALUE_INTEGER, newNode, "NUMBER");
	}

	reportError("NUMBER");
	return false;
}