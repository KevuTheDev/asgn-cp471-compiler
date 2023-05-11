#include "Semantic.h"

Semantic::Semantic()
{
	this->_error = false;

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

void Semantic::pushCategoryStack(std::string category)
{
	this->_categoryStack.push(category);
}

void Semantic::popCategoryStack()
{
	this->_categoryStack.pop();
}

std::string Semantic::getTopCategory()
{
	return this->_categoryStack.top();
}

void Semantic::pushScopeStack(std::string scope)
{
	this->_scopeStack.push(scope);
	this->_symbolTableManager->enterScope(scope);
}

void Semantic::popScopeStack()
{
	this->_scopeStack.pop();
	this->_symbolTableManager->exitScope();
}

std::string Semantic::getTopScope()
{
	return this->_scopeStack.top();
}

void Semantic::start()
{
	pushScopeStack("global");

	auto node = this->_syntaxTree->getHead();
	start_aux(node);
}

void Semantic::start_aux(std::shared_ptr<SyntaxNode> node)
{
	std::string scope = node->getCategory();
	if (!node->isTerminalNode()) {

		if (scope == "FDEC" || scope == "PARAMS" || scope == "DECL" || scope == "STATEMENT"
			|| scope == "STATEMENT_EXT" || scope == "FACTOR") {
			pushCategoryStack(scope);
		}

		
		if (scope == "FDEC") {
			this->_myRow = std::make_shared<SymbolTable::SymbolTableRow>();
		}
	}
	else {
		if (getTopCategory() == "FDEC") {
			if (node->getTokenToken() == compiler::KW_DEF) {
				this->_myRow->category = this->_categoryStack.top();
				this->_myRow->scope = this->_scopeStack.top();
			}
			else if (node->getTokenToken() == compiler::KW_INT) {
				this->_myRow->token = compiler::KW_INT;
				this->_myRow->type = "int";
			}
			else if (node->getTokenToken() == compiler::KW_DOUBLE) {
				this->_myRow->token = compiler::KW_DOUBLE;
				this->_myRow->type = "double";
			}
			else if (node->getTokenToken() == compiler::ID) {
				std::string id = node->getTokenLexeme();
				if (!this->_symbolTableManager->isIdentifierInCurrentScope(id)) {
					this->_myRow->lexeme = node->getTokenLexeme();
					this->_myRow->lineNum = node->getTokenLineNum();
					this->_myRow->charPos = node->getTokenCharPos();

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
	}


	for (auto i : node->getChildren()) {
		start_aux(i);
	}


	if (!node->isTerminalNode()) {
		if (scope == "FDEC" || scope == "PARAMS" || scope == "DECL" || scope == "STATEMENT"
			|| scope == "STATEMENT_EXT" || scope == "FACTOR") {
			popCategoryStack();
		}
	}
}

