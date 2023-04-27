#include "Lexical.h"

Lexical::Lexical(const std::string& filename)
{
	if (!this->checkExtension(filename)) {
		return;
	}
	this->_is = std::ifstream(filename);
	if (!this->_is.is_open()) {
		compiler::getConsoleError(compiler::LEXICAL, "Failed to open file!!");
		return;
	}

	this->_error = false;


	// Initialize attributes
	this->_peek = ' ';
	this->_lineNumber = 1;
	this->_charPosition = 0;
	this->_prevCharPosition = 0;
	this->_currentLine = "";

	this->_bufferSwitch = true;
	this->_bufferCounter1 = 0;
	this->_bufferCounter2 = 0;

	this->_buffer1 = new char[compiler::COMPILER_BUFFER_SIZE];
	this->_buffer2 = new char[compiler::COMPILER_BUFFER_SIZE];

	memset(this->_buffer1, 0, compiler::COMPILER_BUFFER_SIZE);
	memset(this->_buffer2, 0, compiler::COMPILER_BUFFER_SIZE);

}

Lexical::~Lexical()
{
}

void Lexical::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Lexical::linkReservedWords(std::shared_ptr<ReservedWords> table)
{
	this->_reservedWords = table;
}

void Lexical::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
}

void Lexical::run()
{
	auto helper1 = std::thread(compiler::readFileToBuffers,
		std::ref(this->_is), this->_buffer1);
	helper1.join();

	auto helper2 = std::thread(compiler::readFileToBuffers,
		std::ref(this->_is), this->_buffer2);
	helper2.join();


	bool loop = true;
	while (loop) {
		loop = this->getNextToken();
	}

	this->_is.close();
}

bool Lexical::getError()
{
	return this->_error;
}

bool Lexical::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_MAIN_LEN + 1) {
		compiler::getConsoleError(compiler::LEXICAL, "Invalid file string specified: length size");
		return false;
	}

	// check if directory or file

	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_MAIN_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_MAIN) {
		compiler::getConsoleError(compiler::LEXICAL, "Invalid file string specified: file extension");
		return false;
	}

	return true;
}

void Lexical::readFileToBuffer(std::ifstream& is, char* buf)
{
	is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);
	return;
}

void Lexical::reportError(const std::string& errorchar)
{
	this->_logFileBuffer->logLexicalError(this->_lineNumber, this->_prevCharPosition, errorchar, this->_currentLine);
}

void Lexical::appendToTokenList(compiler::TOKEN token, std::string lexeme)
{
	this->_tokenList->append(this->_lineNumber, this->_prevCharPosition, token, lexeme);
}

void Lexical::resetPeek()
{
	this->_peek = ' ';
}

bool Lexical::getNextToken()
{
	// peek into the buffer
	// if whitespace then ignore
	// if newline	 then increase linecount
	// otherwise, move on
	for (;; this->readNextChar()) {

		if (this->_peek == NULL) {
			//std::cout << "END OF FILE" << std::endl;
			return false;
		}

		if (this->_peek == ' ' || this->_peek == '\t')
			continue;
		else if (this->_peek == '\n')
		{
			this->_currentLine.pop_back();
			this->_charPosition = 0;
			this->_lineNumber += 1;
			//std::cout << this->_currentLine << std::endl;
			this->_currentLine = "";
		}
		else
			break;
	}

	// special characters involved, go here
	switch (this->_peek) {
	case ';':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::SEMICOLON, ";");
		this->resetPeek();
		return true;
	case '(':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::LEFT_PAREN, "(");
		this->resetPeek();
		return true;
	case ')':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::RIGHT_PAREN, ")");
		this->resetPeek();
		return true;
	case '=':
		if (readNextChar('=')) {
			this->_prevCharPosition = this->_charPosition;
			appendToTokenList(compiler::COMP_EQUAL, "==");
			this->resetPeek();
			return true;
		}
		else {
			this->_prevCharPosition = this->_charPosition;
			appendToTokenList(compiler::EQUAL, "=");
			this->resetPeek();
			return true;
		}
	case '<':
		if (readNextChar('=')) {
			this->_prevCharPosition = this->_charPosition;
			appendToTokenList(compiler::COMP_LEQUAL, "<=");
			this->resetPeek();
			return true;
		}
		else if (readNextChar('>')) {
			this->_prevCharPosition = this->_charPosition;
			appendToTokenList(compiler::COMP_NOT, "<>");
			this->resetPeek();
			return true;
		}
		else {
			this->_prevCharPosition = this->_charPosition;
			appendToTokenList(compiler::COMP_LTHAN, "<");
			this->resetPeek();
			return true;
		}
	case '>':
		this->_prevCharPosition = this->_charPosition;
		if (readNextChar('=')) {
			appendToTokenList(compiler::COMP_GEQUAL, ">=");
			this->resetPeek();
			return true;
		}
		else {
			appendToTokenList(compiler::COMP_GTHAN, ">");
			this->resetPeek();
			return true;
		}
	case ',':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::COMMA, ",");
		this->resetPeek();
		return true;
	case '+':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::PLUS, "+");
		this->resetPeek();
		return true;
	case '*':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::MULTIPLY, "*");
		this->resetPeek();
		return true;
	case '-':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::MINUS, "-");
		this->resetPeek();
		return true;
	case '%':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::MODULUS, "%");
		this->resetPeek();
		return true;
	case '/':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::DIVIDE, "/");
		this->resetPeek();
		return true;
	case '[':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::LEFT_BRACK, "[");
		this->resetPeek();
		return true;
	case ']':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::RIGHT_BRACK, "]");
		this->resetPeek();
		return true;
	case '.':
		this->_prevCharPosition = this->_charPosition;
		appendToTokenList(compiler::DOT, ".");
		this->resetPeek();
		return true;
	}

	// If Numeric Value
	if (std::isdigit(this->_peek)) {
		this->_prevCharPosition = this->_charPosition;
		std::string v = "";
		do {
			v += this->_peek;
			this->readNextChar();
		} while (std::isdigit(this->_peek));

		if (this->_peek != '.') {
			appendToTokenList(compiler::VALUE_INTEGER, v);
			return true;
		}

		v += ".";
		for (;;) {
			this->readNextChar();
			if (!std::isdigit(this->_peek)) {
				break;
			}
			v += this->_peek;
		}

		if (this->_peek != 'e') {
			// if a letter is placed other than a 'e', means a ID will be position
			return true;
		}

		v += this->_peek;
		this->readNextChar();

		if (this->_peek == '+' || this->_peek == '-') {
			v += this->_peek;
		}

		for (;;) {
			this->readNextChar();
			if (!std::isdigit(this->_peek)) {
				break;
			}
			v += this->_peek;
		}

		appendToTokenList(compiler::VALUE_DOUBLE, v);
		return true;
	}

	std::cout << this->_peek << std::endl;
	// If ID or Keywords
	if (std::isalpha(this->_peek)) {
		this->_prevCharPosition = this->_charPosition;
		std::string b = "";
		do {
			b += this->_peek;
			this->readNextChar();
		} while (std::isalnum(this->_peek));

		if (this->_reservedWords->findReservedWord(b)) {
			appendToTokenList(static_cast<compiler::TOKEN>(this->_reservedWords->findReservedWordIndex(b)), b);
		}
		else {
			appendToTokenList(compiler::ID, b);
		}

		//std::cout << b << std::endl;
		return true;
	}

	std::cout << this->_peek << std::endl;
	this->_prevCharPosition = this->_charPosition;
	std::string s(&this->_peek);
	this->reportError(s);
	this->_error = true;
	this->resetPeek();
	return true;
}

void Lexical::readNextChar()
{
	//std::cout << "::" <<this->_bufferSwitch << "::   Char: " << this->_peek << "    " << std::endl;
	if (this->_bufferSwitch) {
		this->_peek = this->_buffer1[this->_bufferCounter1];
		this->_bufferCounter1 += 1;
		this->_charPosition += 1;
		this->_currentLine += this->_peek;

		if (this->_bufferCounter1 == compiler::COMPILER_BUFFER_SIZE) {
			this->_currentLine.pop_back();
			// Get the next char from buffer 2
			this->_peek = this->_buffer2[this->_bufferCounter2];
			this->_bufferCounter2 += 1;
			this->_charPosition += 1;
			this->_currentLine += this->_peek;

			this->_bufferSwitch = !this->_bufferSwitch;

			memset(this->_buffer1, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(compiler::readFileToBuffers, std::ref(this->_is), std::ref(this->_buffer1));
			helper1.join();

			this->_bufferCounter1 = 0;
		}
	}
	else {
		this->_peek = this->_buffer2[this->_bufferCounter2];
		this->_bufferCounter2 += 1;
		this->_charPosition += 1;
		this->_currentLine += this->_peek;

		if (this->_bufferCounter2 == compiler::COMPILER_BUFFER_SIZE) {
			this->_currentLine.pop_back();
			// Get the next char from buffer 1
			this->_peek = this->_buffer1[this->_bufferCounter1];
			this->_bufferCounter1 += 1;
			this->_charPosition += 1;
			this->_currentLine += this->_peek;

			this->_bufferSwitch = !this->_bufferSwitch;

			memset(this->_buffer2, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(compiler::readFileToBuffers, std::ref(this->_is), std::ref(this->_buffer2));
			helper1.join();

			this->_bufferCounter2 = 0;
		}
	}
}

bool Lexical::readNextChar(char c)
{
	this->readNextChar();

	if (this->_peek != c) {
		if (this->_bufferSwitch) {
			this->_bufferCounter1 -= 1;
			this->_charPosition -= 1;
		}
		else {
			this->_bufferCounter2 -= 1;
			this->_charPosition -= 1;
		}
		return false;
	}

	this->resetPeek();
	return true;
}
