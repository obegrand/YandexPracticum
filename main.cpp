#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
	return path(data, data + sz);
}

// напишите эту функцию
static bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories, int line_num = 0) {
	ifstream in(in_file);
	if (!in)return false;
	ofstream out(out_file, ios::app);

	static regex include_quote_regex(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
	static regex include_brack_regex(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");
	smatch quote;

	string line;
	while (getline(in, line)) {
		if (regex_match(line, quote, include_quote_regex)) {
			++line_num;
			path new_in_file = quote[1].str();
			new_in_file.make_preferred();
			path res_in_file = in_file.parent_path() / new_in_file;
			if (!Preprocess(res_in_file, out_file, include_directories, line_num)) {
				for (auto& dir : include_directories) {
					path res_in_file = dir / new_in_file;
					if (Preprocess(res_in_file, out_file, include_directories, line_num)) { break; }
					else if (include_directories[static_cast<int>(include_directories.size() - 1)] == dir) {
						cout << "unknown include file " << new_in_file.string() << " at file " << in_file.string() << " at line " << line_num << endl;
						return false;
					}
				}
			}
		}
		else if (regex_match(line, quote, include_brack_regex)) {
			++line_num;
			path new_in_file = quote[1].str();
			new_in_file.make_preferred();
			for (auto& dir : include_directories) {
				path res_in_file = dir / new_in_file;
				if (Preprocess(res_in_file, out_file, include_directories, line_num)) { break; }
				else if (include_directories[static_cast<int>(include_directories.size() - 1)] == dir) {
					cout << "unknown include file " << new_in_file.string() << " at file " << in_file.string() << " at line " << line_num << endl;
					return false;
				}
			}
		}
		else {
			out << line << endl;
			++line_num;
		}
	}
	return true;
}

string GetFileContents(string file) {
	ifstream stream(file);

	// конструируем string по двум итераторам
	return { (istreambuf_iterator<char>(stream)), istreambuf_iterator<char>() };
}

void Test() {
	error_code err;
	filesystem::remove_all("sources"_p, err);
	filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
	filesystem::create_directories("sources"_p / "include1"_p, err);
	filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

	{
		ofstream file("sources/a.cpp");
		file << "// this comment before include\n"
			"#include \"dir1/b.h\"\n"
			"// text between b.h and c.h\n"
			"#include \"dir1/d.h\"\n"
			"\n"
			"int SayHello() {\n"
			"    cout << \"hello, world!\" << endl;\n"
			"#   include<dummy.txt>\n"
			"}\n"s;
	}
	{
		ofstream file("sources/dir1/b.h");
		file << "// text from b.h before include\n"
			"#include \"subdir/c.h\"\n"
			"// text from b.h after include"s;
	}
	{
		ofstream file("sources/dir1/subdir/c.h");
		file << "// text from c.h before include\n"
			"#include <std1.h>\n"
			"// text from c.h after include\n"s;
	}
	{
		ofstream file("sources/dir1/d.h");
		file << "// text from d.h before include\n"
			"#include \"lib/std2.h\"\n"
			"// text from d.h after include\n"s;
	}
	{
		ofstream file("sources/include1/std1.h");
		file << "// std1\n"s;
	}
	{
		ofstream file("sources/include2/lib/std2.h");
		file << "// std2\n"s;
	}

	assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p, { "sources"_p / "include1"_p,"sources"_p / "include2"_p })));

	ostringstream test_out;
	test_out << "// this comment before include\n"
		"// text from b.h before include\n"
		"// text from c.h before include\n"
		"// std1\n"
		"// text from c.h after include\n"
		"// text from b.h after include\n"
		"// text between b.h and c.h\n"
		"// text from d.h before include\n"
		"// std2\n"
		"// text from d.h after include\n"
		"\n"
		"int SayHello() {\n"
		"    cout << \"hello, world!\" << endl;\n"s;

	assert(GetFileContents("sources/a.in"s) == test_out.str());
}

int main() {
	Test();
}