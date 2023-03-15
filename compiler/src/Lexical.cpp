#include "Lexical.h"


void readFileToBuffers(std::ifstream& is, char* buf) {
	is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);

	return;
}

Lexical::Lexical(const std::string& filename)
{
	if (!this->checkExtension(filename)) {
		return;
	}

	this->is = std::ifstream(filename);
	if (!this->is.is_open()) {
		std::cout << "Failed to open file" << std::endl;
		return;
	}


	this->peek = ' ';
	this->line = 0;
	this->buffer_switch = true;
	this->buffer1_count = 0;
	this->buffer2_count = 0;
	this->buffer1 = new char[compiler::COMPILER_BUFFER_SIZE];
	this->buffer2 = new char[compiler::COMPILER_BUFFER_SIZE];

	this->sTable = SymbolTable();



	memset(this->buffer1, 0, compiler::COMPILER_BUFFER_SIZE);
	memset(this->buffer2, 0, compiler::COMPILER_BUFFER_SIZE);


	auto helper1 = std::thread(readFileToBuffers, std::ref(is), this->buffer1);
	helper1.join();


	auto helper2 = std::thread(readFileToBuffers, std::ref(is), this->buffer2);
	helper2.join();




	bool loop = true;
	while (loop) {
		loop = this->nextToken();
	}
}

bool Lexical::checkExtension(const std::string& filepath)
{

	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_LEN + 1) {
		std::cout << "Invalid file string specified: length size" << std::endl;
		return false;
	}

	// check if directory or file

	if (filepath.substr(filepath.length() - 3) != compiler::COMPILER_FILE_EXTENSION) {
		std::cout << "Invalid file string specified: file extension" << std::endl;
		return false;
	}

	return true;
}

void Lexical::readFileToBuffer(std::ifstream& is, char* buf) {
	is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);

	return;
}

bool Lexical::nextToken()
{
	// peek into the buffer
	// if whitespace then ignore
	// if newline	 then increase linecount
	// otherwise, move on
	for (;; this->readChar()) {

		if (this->peek == NULL) {
			std::cout << "END OF FILE" << std::endl;
			return false;
		}

		if (this->peek == ' ' || this->peek == '\t')
			continue;
		else if (this->peek == '\n')
			this->line += 1;
		else
			break;
	}

	// special characters involved, go here
	switch (this->peek) {
	case ';':
		this->sTable.append("SEMICO", ";", this->line);
		//std::cout << ";" << std::endl;
		this->peek = ' ';
		return true;
	case '(':
		this->sTable.append("LPAREN", "(", this->line);
		//std::cout << "(" << std::endl;
		this->peek = ' ';
		return true;
	case ')':
		this->sTable.append("RPAREN", ")", this->line);
		//std::cout << ")" << std::endl;
		this->peek = ' ';
		return true;
	case '&':
		if (readChar('&')) {
			this->sTable.append("EXPRES", "&&", this->line);
			//std::cout << "&&" << std::endl;
			this->peek = ' ';
			return true;
		}
		else {
			this->sTable.append("OPERAT", "&", this->line);
			//std::cout << "&" << std::endl;
			this->peek = ' ';
			return true;
		}
	case '|':
		if (readChar('|')) {
			this->sTable.append("EXPRES", "||", this->line);
			//std::cout << "||" << std::endl;
			this->peek = ' ';
			return true;
		}
		else {
			this->sTable.append("OPERAT", "|", this->line);
			//std::cout << "|" << std::endl;
			this->peek = ' ';
			return true;
		}
	case '=':
		if (readChar('=')) {
			this->sTable.append("EXPRES", "==", this->line);
			//std::cout << "==" << std::endl;
			this->peek = ' ';
			return true;
		}
		else {
			this->sTable.append("OPERAT", "=", this->line);
			//std::cout << "=" << std::endl;
			this->peek = ' ';
			return true;
		}
	case '<':
		if (readChar('=')) {
			this->sTable.append("LETHAN", "<=", this->line);
			//std::cout << "<=" << std::endl;
			this->peek = ' ';
			return true;
		}
		else if (readChar('>')) {
			this->sTable.append("NEQUAL", "<>", this->line);
			//std::cout << "<>" << std::endl;
			this->peek = ' ';
			return true;
		}
		else {
			this->sTable.append("LLTHAN", "<", this->line);
			//std::cout << "<" << std::endl;
			this->peek = ' ';
			return true;
		}
	case '>':
		if (readChar('=')) {
			this->sTable.append("GETHAN", ">=", this->line);
			//std::cout << ">=" << std::endl;
			this->peek = ' ';
			return true;
		}
		else {
			this->sTable.append("GGTHAN", ">", this->line);
			//std::cout << ">" << std::endl;
			this->peek = ' ';
			return true;
		}
	case ',':
		this->sTable.append("GRAMMR", ",", this->line);
		//std::cout << "," << std::endl;
		this->peek = ' ';
		return true;
	case '+':
		this->sTable.append("ARITHM", "+", this->line);
		//std::cout << "+" << std::endl;
		this->peek = ' ';
		return true;
	case '*':
		this->sTable.append("ARITHM", "*", this->line);
		//std::cout << "*" << std::endl;
		this->peek = ' ';
		return true;
	case '-':
		this->sTable.append("ARITHM", "-", this->line);
		//std::cout << "-" << std::endl;
		this->peek = ' ';
		return true;
	case '%':
		this->sTable.append("ARITHM", "%", this->line);
		//std::cout << "%" << std::endl;
		this->peek = ' ';
		return true;
	case '/':
		this->sTable.append("ARITHM", "/", this->line);
		//std::cout << "/" << std::endl;
		this->peek = ' ';
		return true;
	case '[':
		this->sTable.append("GRAMMR", "[", this->line);
		//std::cout << "[" << std::endl;
		this->peek = ' ';
		return true;
	case ']':
		this->sTable.append("GRAMMR", "]", this->line);
		//std::cout << "]" << std::endl;
		this->peek = ' ';
		return true;
	case '.':
		this->sTable.append("GRAMMR", ".", this->line);
		//std::cout << "." << std::endl;
		this->peek = ' ';
		return true;

	}

	if (std::isdigit(this->peek)) {
		int v = 0;
		//std::cout << "std:::::" << this->peek << " " << static_cast<int>(this->peek) <<std::endl;
		do {
			v = 10 * v + int(this->peek - '0');
			this->readChar();
		} while (std::isdigit(this->peek));

		if (this->peek != '.') {
			this->sTable.append("INT", std::to_string(v), this->line);
			std::cout << v << std::endl;
			return true;
		}

		float x = static_cast<float>(v);
		float d = 10;

		for (;;) {
			this->readChar();
			if (!std::isdigit(this->peek)) {
				break;
			}
			x = x + float(this->peek - '0') / 10;
			d = d * 10;
		}
		
		this->sTable.append("FLOAT", std::to_string(x), this->line);
		std::cout << x << std::endl;
		return true;
	}

	if (std::isalpha(this->peek)) {
		std::string b = "";
		do {
			b += this->peek;
			this->readChar();
		} while (std::isalnum(this->peek));

		this->sTable.append("IDENTI", b, this->line);
		std::cout << b << std::endl;
		return true;
	}

	std::string s(&this->peek);
	this->sTable.append("OTHER", s, this->line);
	std::cout << this->peek << std::endl;
	this->peek = ' ';
	return true;
}

void Lexical::readChar()
{
	//std::cout << "::" <<this->buffer_switch << "::   Char: " << this->peek << "    " << std::endl;
	if (this->buffer_switch) {
		this->peek = this->buffer1[this->buffer1_count];
		this->buffer1_count += 1;
		
		if (this->buffer1_count == compiler::COMPILER_BUFFER_SIZE) {
			// Get the next char from buffer 2
			this->peek = this->buffer2[this->buffer2_count];
			this->buffer2_count += 1;

			this->buffer_switch = !this->buffer_switch;

			memset(buffer1, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(readFileToBuffers, std::ref(is), std::ref(this->buffer1));
			helper1.join();

			this->buffer1_count = 0;
		}
	}
	else {
		this->peek = this->buffer2[this->buffer2_count];
		this->buffer2_count += 1;

		if (this->buffer2_count == compiler::COMPILER_BUFFER_SIZE) {
			// Get the next char from buffer 1
			this->peek = this->buffer1[this->buffer1_count];
			this->buffer1_count += 1;

			this->buffer_switch = !this->buffer_switch;

			memset(buffer2, 0, compiler::COMPILER_BUFFER_SIZE);
			auto helper1 = std::thread(readFileToBuffers, std::ref(is), std::ref(this->buffer2));
			helper1.join();

			this->buffer2_count = 0;
		}
	}
}

bool Lexical::readChar(char c)
{
	this->readChar();
	if (this->buffer_switch) {
		this->buffer1_count -= 1;
	}
	else {
		this->buffer2_count -= 1;
	}

	if (this->peek != c)
		return false;

	this->peek = ' ';
	return true;
}