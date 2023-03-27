#include "Lexical.h"

Lexical::Lexical(const std::string& filename) 
{
	if (!this->checkExtension(filename)) {
		return;
	}

	this->_is = std::ifstream(filename);
	if (!this->_is.is_open()) {
		std::cout << "Failed to open file" << std::endl;
		return;
	}
	sanitizeFileName(filename);


	// Initialize attributes
	this->_peek = ' ';
	this->_lineNumber = 1;
	this->_doubleBufferSwitch = true;
	this->_doubleBufferCounter1 = 0;
	this->_doubleBufferCounter2 = 0;
	this->_doubleBuffer1 = new char[compiler::COMPILER_BUFFER_SIZE];
	this->_doubleBuffer2 = new char[compiler::COMPILER_BUFFER_SIZE];


	this->_tokenFileBuffer = TokenFileBuffer(this->_fileName + 
		compiler::COMPILER_FILE_EXTENSION_TOKEN);
	this->_logFileBuffer = LogFileBuffer(this->_fileName + 
		compiler::COMPILER_FILE_EXTENSION_LOG);

	memset(this->_doubleBuffer1, 0, compiler::COMPILER_BUFFER_SIZE);
	memset(this->_doubleBuffer2, 0, compiler::COMPILER_BUFFER_SIZE);
}

void Lexical::linkSymbolTable(const SymbolTable& st)
{
	this->_symbolTable = st;
}

void Lexical::run()
{
	auto helper1 = std::thread(compiler::readFileToBuffers,
		std::ref(this->_is), this->_doubleBuffer1);
	helper1.join();

	auto helper2 = std::thread(compiler::readFileToBuffers,
		std::ref(this->_is), this->_doubleBuffer2);
	helper2.join();


	bool loop = true;
	while (loop) {
		loop = this->getNextToken();
	}

	this->_tokenFileBuffer.finish();
	this->_logFileBuffer.finish();
	this->_is.close();


	this->_symbolTable.printTable();
	this->_symbolTable.printTable(this->_tokenFileBuffer);
}

bool Lexical::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_MAIN_LEN + 1) {
		std::cout << "Invalid file string specified: length size" << std::endl;
		return false;
	}

	// check if directory or file

	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_MAIN_LEN) 
		!= compiler::COMPILER_FILE_EXTENSION_MAIN) {
		std::cout << "Invalid file string specified: file extension" << std::endl;
		return false;
	}

	return true;
}

void Lexical::readFileToBuffer(std::ifstream& is, char* buf)
{
	is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);

	return;
}

void Lexical::sanitizeFileName(const std::string& filename)
{
	// This assumes that the given filename is already valid
	this->_fileName = filename.substr(0, filename.length() - compiler::COMPILER_FILE_EXTENSION_MAIN_LEN);
	size_t char_pos = this->_fileName.find('/');

	
	while (char_pos != std::string::npos) {
		std::cout << this->_fileName << std::endl;
		this->_fileName = this->_fileName.substr(char_pos + 1);
		char_pos = this->_fileName.find('/');
	}
}

void Lexical::appendToTokenFileBuffer(const char* token)
{
	this->_tokenFileBuffer.append(token);
}

void Lexical::appendToTokenFileBuffer(const std::string& token)
{
	this->_tokenFileBuffer.append(token);
}

void Lexical::appendToTokenFileBuffer(int token)
{
	this->_tokenFileBuffer.append(std::to_string(token));
}

void Lexical::appendToTokenFileBuffer(double token)
{
	this->_tokenFileBuffer.append(std::to_string(token));
}

void Lexical::appendToLogFileBuffer(int linenumber, int rownumber, const std::string& errorchar)
{
	this->_logFileBuffer.errorChar(linenumber, rownumber, errorchar);
}

void Lexical::appendToSymbolTable(std::string token, std::string lexeme, int lineNumber)
{
	this->_symbolTable.append(token, lexeme, lineNumber);
}

bool Lexical::getNextToken()
{
	// peek into the buffer
	// if whitespace then ignore
	// if newline	 then increase linecount
	// otherwise, move on
	for (;; this->readNextChar()) {

		if (this->_peek == NULL) {
			std::cout << "END OF FILE" << std::endl;
			return false;
		}

		if (this->_peek == ' ' || this->_peek == '\t')
			continue;
		else if (this->_peek == '\n')
		{
			this->_charNumber = 0;
			this->_lineNumber += 1;
		}
		else
			break;
	}

	// special characters involved, go here
	switch (this->_peek) {
	case ';':
		appendToSymbolTable("SEMICO", ";", this->_lineNumber);
		appendToTokenFileBuffer(";");
		//std::cout << ";" << std::endl;
		this->_peek = ' ';
		return true;
	case '(':
		appendToSymbolTable("LPAREN", "(", this->_lineNumber);
		appendToTokenFileBuffer("(");
		//std::cout << "(" << std::endl;
		this->_peek = ' ';
		return true;
	case ')':
		appendToSymbolTable("RPAREN", ")", this->_lineNumber);
		appendToTokenFileBuffer(")");
		//std::cout << ")" << std::endl;
		this->_peek = ' ';
		return true;
	case '&':
		if (readNextChar('&')) {
			appendToSymbolTable("EXPRES", "&&", this->_lineNumber);
			appendToTokenFileBuffer("&&");
			//std::cout << "&&" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else {
			appendToSymbolTable("OPERAT", "&", this->_lineNumber);
			appendToTokenFileBuffer("&");
			//std::cout << "&" << std::endl;
			this->_peek = ' ';
			return true;
		}
	case '|':
		if (readNextChar('|')) {
			appendToSymbolTable("EXPRES", "||", this->_lineNumber);
			appendToTokenFileBuffer("||");
			//std::cout << "||" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else {
			appendToSymbolTable("OPERAT", "|", this->_lineNumber);
			appendToTokenFileBuffer("|");
			//std::cout << "|" << std::endl;
			this->_peek = ' ';
			return true;
		}
	case '=':
		if (readNextChar('=')) {
			appendToSymbolTable("EXPRES", "==", this->_lineNumber);
			appendToTokenFileBuffer("==");
			//std::cout << "==" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else {
			appendToSymbolTable("OPERAT", "=", this->_lineNumber);
			appendToTokenFileBuffer("=");
			//std::cout << "=" << std::endl;
			this->_peek = ' ';
			return true;
		}
	case '<':
		if (readNextChar('=')) {
			appendToSymbolTable("LETHAN", "<=", this->_lineNumber);
			appendToTokenFileBuffer("<=");
			//std::cout << "<=" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else if (readNextChar('>')) {
			appendToSymbolTable("NEQUAL", "<>", this->_lineNumber);
			appendToTokenFileBuffer("<>");
			//std::cout << "<>" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else {
			appendToSymbolTable("LLTHAN", "<", this->_lineNumber);
			appendToTokenFileBuffer("<");
			//std::cout << "<" << std::endl;
			this->_peek = ' ';
			return true;
		}
	case '>':
		if (readNextChar('=')) {
			appendToSymbolTable("GETHAN", ">=", this->_lineNumber);
			appendToTokenFileBuffer(">=");
			//std::cout << ">=" << std::endl;
			this->_peek = ' ';
			return true;
		}
		else {
			appendToSymbolTable("GGTHAN", ">", this->_lineNumber);
			appendToTokenFileBuffer(">");
			//std::cout << ">" << std::endl;
			this->_peek = ' ';
			return true;
		}
	case ',':
		appendToSymbolTable("GRAMMR", ",", this->_lineNumber);
		appendToTokenFileBuffer(",");
		//std::cout << "," << std::endl;
		this->_peek = ' ';
		return true;
	case '+':
		appendToSymbolTable("ARITHM", "+", this->_lineNumber);
		appendToTokenFileBuffer("+");
		//std::cout << "+" << std::endl;
		this->_peek = ' ';
		return true;
	case '*':
		appendToSymbolTable("ARITHM", "*", this->_lineNumber);
		appendToTokenFileBuffer("*");
		//std::cout << "*" << std::endl;
		this->_peek = ' ';
		return true;
	case '-':
		appendToSymbolTable("ARITHM", "-", this->_lineNumber);
		appendToTokenFileBuffer("-");
		//std::cout << "-" << std::endl;
		this->_peek = ' ';
		return true;
	case '%':
		appendToSymbolTable("ARITHM", "%", this->_lineNumber);
		appendToTokenFileBuffer("%");
		//std::cout << "%" << std::endl;
		this->_peek = ' ';
		return true;
	case '/':
		appendToSymbolTable("ARITHM", "/", this->_lineNumber);
		appendToTokenFileBuffer("/");
		//std::cout << "/" << std::endl;
		this->_peek = ' ';
		return true;
	case '[':
		appendToSymbolTable("GRAMMR", "[", this->_lineNumber);
		appendToTokenFileBuffer("[");
		//std::cout << "[" << std::endl;
		this->_peek = ' ';
		return true;
	case ']':
		appendToSymbolTable("GRAMMR", "]", this->_lineNumber);
		appendToTokenFileBuffer("]");
		//std::cout << "]" << std::endl;
		this->_peek = ' ';
		return true;
	case '.':
		appendToSymbolTable("GRAMMR", ".", this->_lineNumber);
		appendToTokenFileBuffer(".");
		//std::cout << "." << std::endl;
		this->_peek = ' ';
		return true;

	}

	if (std::isdigit(this->_peek)) {
		int v = 0;
		//std::cout << "std:::::" << this->_peek << " " << static_cast<int>(this->_peek) <<std::endl;
		do {
			v = 10 * v + int(this->_peek - '0');
			this->readNextChar();
		} while (std::isdigit(this->_peek));

		if (this->_peek != '.') {
			appendToSymbolTable("INT", std::to_string(v), this->_lineNumber);
			appendToTokenFileBuffer(v);
			//std::cout << v << std::endl;
			return true;
		}

		float x = static_cast<float>(v);
		float d = 10;

		for (;;) {
			this->readNextChar();
			if (!std::isdigit(this->_peek)) {
				break;
			}
			x = x + float(this->_peek - '0') / 10;
			d = d * 10;
		}
		
		appendToSymbolTable("FLOAT", std::to_string(x), this->_lineNumber);
		appendToTokenFileBuffer(x);
		//std::cout << x << std::endl;
		return true;
	}

	if (std::isalpha(this->_peek)) {
		std::string b = "";
		do {
			b += this->_peek;
			this->readNextChar();
		} while (std::isalnum(this->_peek));

		appendToSymbolTable("IDENTI", b, this->_lineNumber);
		appendToTokenFileBuffer(b);
		//std::cout << b << std::endl;
		return true;
	}

	std::string s(&this->_peek);
	this->appendToLogFileBuffer(this->_lineNumber, this->_charNumber, s);
	//appendToSymbolTable("OTHER", s, this->_lineNumber);
	//appendToTokenFileBuffer("ERROR: " + s);
	//std::cout << this->_peek << std::endl;
	this->_peek = ' ';
	return true;
}

void Lexical::readNextChar()
{
	//std::cout << "::" <<this->_doubleBufferSwitch << "::   Char: " << this->_peek << "    " << std::endl;
	if (this->_doubleBufferSwitch) {
		this->_peek = this->_doubleBuffer1[this->_doubleBufferCounter1];
		this->_doubleBufferCounter1 += 1;
		this->_charNumber += 1;
		
		if (this->_doubleBufferCounter1 == compiler::COMPILER_BUFFER_SIZE) {
			// Get the next char from buffer 2
			this->_peek = this->_doubleBuffer2[this->_doubleBufferCounter2];
			this->_doubleBufferCounter2 += 1;
			this->_charNumber += 1;

			this->_doubleBufferSwitch = !this->_doubleBufferSwitch;

			memset(this->_doubleBuffer1, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(compiler::readFileToBuffers, std::ref(this->_is), std::ref(this->_doubleBuffer1));
			helper1.join();

			this->_doubleBufferCounter1 = 0;
		}
	}
	else {
		this->_peek = this->_doubleBuffer2[this->_doubleBufferCounter2];
		this->_doubleBufferCounter2 += 1;
		this->_charNumber += 1;

		if (this->_doubleBufferCounter2 == compiler::COMPILER_BUFFER_SIZE) {
			// Get the next char from buffer 1
			this->_peek = this->_doubleBuffer1[this->_doubleBufferCounter1];
			this->_doubleBufferCounter1 += 1;
			this->_charNumber += 1;

			this->_doubleBufferSwitch = !this->_doubleBufferSwitch;

			memset(this->_doubleBuffer2, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(compiler::readFileToBuffers, std::ref(this->_is), std::ref(this->_doubleBuffer2));
			helper1.join();

			this->_doubleBufferCounter2 = 0;
		}
	}
}

bool Lexical::readNextChar(char c)
{
	this->readNextChar();
	if (this->_doubleBufferSwitch) {
		this->_doubleBufferCounter1 -= 1;
		this->_charNumber -= 1;
	}
	else {
		this->_doubleBufferCounter2 -= 1;
		this->_charNumber -= 1;
	}

	if (this->_peek != c)
		return false;

	this->_peek = ' ';
	return true;
}