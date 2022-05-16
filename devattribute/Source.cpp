#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <codecvt>
#include <string>

class UnicodeString {

private:
	wchar_t* wstr = nullptr;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	int size = 0;
public:
	
	friend std::ostream& operator << (std::ostream& out, UnicodeString& _str);
	friend std::wostream& operator << (std::wostream& wout, UnicodeString& _str);

	UnicodeString(const wchar_t* _str = L"")
	{
		size = wcslen(_str) + 1;
		wstr = new wchar_t[size];
		for (size_t i = 0; i < size; i++)
			wstr[i] = _str[i];

	}

	bool operator > (UnicodeString& ustr) {
		return length() > ustr.length();
	}

	int length() {
		return wcslen(wstr);
	}

	operator std::string()
	{
		return converter.to_bytes(wstr);
	}

	~UnicodeString() {
		delete[] wstr;
	}
};

std::ostream& operator << (std::ostream& out, UnicodeString& ustr) {
	out << (std::string)ustr;
	return out;
}

std::wostream& operator << (std::wostream& wout, UnicodeString& ustr) {
	wout << ustr.wstr;
	return wout;
}

int UnicodeString::_compare(const String& rhs) const {

	if (_str_len < rhs._str_len)
		return 1;
	else if (_str_len > rhs._str_len)
		return -1;

	return strcmp(_str, rhs._str);
}

int main() {
	UnicodeString ustr = L"dick";
	UnicodeString penis = L"sucks";
	std::wstring ass = L"ASAS";
	if (penis > ustr) {
		std::cout << "bigger";
	}

	_getch();
}