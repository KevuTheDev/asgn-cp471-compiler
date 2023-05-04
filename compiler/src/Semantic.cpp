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
	if (!node->isTerminalNode()) {
		std::string scope = node->getCategory();
		pushCategoryStack(scope);
		
		if (scope == "FDEC") {
			this->_myRow = std::make_shared<SymbolTable::SymbolTableRow>();
		}
	}
	else {
		if (getTopCategory() == "FDEC") {

		}
	}


	for (auto i : node->getChildren()) {
		start_aux(i);
	}


	if (!node->isTerminalNode()) {
		popCategoryStack();
	}
}

