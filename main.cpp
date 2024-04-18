#include <iostream>
#include <list>
#include <string>
#include <algorithm>

//#define DEBUG

using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::list<char>& list)
{
	for (auto& i : list) {
		ostr << i;
	}
	return ostr;
}

class Editor {
public:
	string get_ptr() {
		return string(1,*cursor_);
	}
	
	Editor() {
		line_.insert(line_.begin(), '|');
		cursor_ = find(line_.begin(), line_.end(), '|');
	}
	// �������� ������ �����
	void Left() {
		if (cursor_ == line_.begin())return;
		line_.erase(cursor_--);
		line_.insert(cursor_, '|');
		--cursor_;
	}
	// �������� ������ ������ 
	void Right() {
		if (cursor_ == --line_.end())return;
		line_.erase(cursor_++);
		line_.insert(++cursor_, '|');
		--cursor_;
	}
	// �������� ������ token
	void Insert(char token) {
		line_.insert(cursor_, token);
	}
	// �������� �� ����� tokens ��������, ������� � ������� ������� �������
	void Cut(size_t tokens = 1) {
		Copy(tokens);
		if (cursor_ != line_.end()) ++cursor_;
		while (cursor_ != line_.end() && tokens > 0) {
			line_.erase(cursor_++);
			--tokens;
		}
		cursor_ = find(line_.begin(), line_.end(), '|');
	}
	// c���������� �� ����� tokens ��������, ������� � ������� ������� �������
	void Copy(size_t tokens = 1) {
		buffer.clear();
		if (cursor_ != line_.end()) ++cursor_;
		while (cursor_ != line_.end() && tokens > 0) {
			buffer.insert(buffer.end(), *cursor_++);
			--tokens;
		}
		cursor_ = find(line_.begin(), line_.end(), '|');
	}
	// �������� ���������� ������ � ������� ������� �������
	void Paste() {
		for (char letter : buffer) {
			line_.insert(cursor_, letter);
		}
	}
	// �������� ������� ���������� ���������� ���������
	string GetText() const {
		string result;
		for (const auto& ch : line_) {
			if (ch != '|') { // ��������� ������ '|' �� ����������
				result += ch;
			}
		}
		return result;
	}
private:
	list<char> line_;
	list<char> buffer;
	std::list<char>::iterator cursor_;
};

int test() {

	Editor editor;
	const string text = "hello, world"s;
	for (char c : text) {
		editor.Insert(c);
	}
	// ������� ��������� ���������: `hello, world|`
#ifdef DEBUG
	{
		string right = "hello, world|"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	for (size_t i = 0; i < text.size(); ++i) {
		editor.Left();
	}
	// ������� ��������� ���������: `|hello, world`
#ifdef DEBUG
	{
		string right = "|hello, world"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	editor.Cut(7);
	// ������� ��������� ���������: `|world`
	// � ������ ������ ��������� ����� `hello, `
#ifdef DEBUG
	{
		string right = "|world"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	for (size_t i = 0; i < 5; ++i) {
		editor.Right();
		//cout << "\t" << editor.get_ptr() << endl;
	}
	// ������� ��������� ���������: `world|`
#ifdef DEBUG
	{
		string right = "world|"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	editor.Insert(',');
	editor.Insert(' ');
	// ������� ��������� ���������: `world, |`
#ifdef DEBUG
	{
		string right = "world, |"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	editor.Paste();
	// ������� ��������� ���������: `world, hello, |`
	editor.Left();
	editor.Left();
#ifdef DEBUG
	{
		string right = "world, hello|, "s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	//������� ��������� ���������: `world, hello|, `
	editor.Cut(3);  // ����� �������� 2 �������
	// ������� ��������� ���������: `world, hello|`
#ifdef DEBUG
	{
		string right = "world, hello|"s;
		editor.GetText() == right ? cout << "Test ok - [" << right << "]" << endl : cout << "[" << right << "] - [" << editor.GetText() << "]" <<__LINE__<< endl;
	}
#endif DEBUG
	cout << editor.GetText();
	return 0;
}

int main() {
	system("color A");
	test();
}