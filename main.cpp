#include "json_builder.h"
#include <iostream>

using namespace std;

int main() {
	try
	{
		json::Print(
			json::Document{
		json::Builder{}.Value("s"s).Value("1"s).Build()
			},
			cout
		);
	}
	catch (const std::logic_error& err) {
		cout << "logic_error - " << err.what() << endl;
	}
	catch (const std::exception& err) {
		cout << "unexpect_error - " << err.what() << endl;

	}
	try
	{
		json::Print(
			json::Document{
		json::Builder{}.Value("s"s).StartDict().Build()
			},
			cout
		);
	}
	catch (const std::logic_error& err) {
		cout << "logic_error - " << err.what() << endl;
	}
	catch (const std::exception& err) {
		cout << "unexpect_error - " << err.what() << endl;

	}
	try
	{
		json::Print(
			json::Document{
				json::Builder{}
				.Value("just a string"s)
				.Build()
			},
			cout
		);
	}
	catch (const std::logic_error& err) {
		cout << "logic_error - " << err.what() << endl;
	}
	catch (const std::exception& err) {
		cout << "unexpect_error - " << err.what() << endl;

	}
	try
	{
		json::Print(
			json::Document{
				// Форматирование не имеет формального значения:
				// это просто цепочка вызовов методов
		json::Builder{}
		.StartDict()
			.Key("key1"s).Value(123)
			.Key("key2"s).Value("value2"s)
			.Key("key3"s).StartArray()
				.Value(456)
				.StartDict().EndDict()
				.StartDict()
					.Key(""s).Value(nullptr)
				.EndDict()
				.Value(""s)
			.EndArray()
		.EndDict()
		.Build()
			},
			cout
		);
	}
	catch (const std::logic_error& err) {
		cout << "logic_error - " << err.what() << endl;
	}
	catch (const std::exception& err) {
		cout << "unexpect_error - " << err.what() << endl;

	}   
}