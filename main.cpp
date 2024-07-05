#include "json_builder.h"
#include <iostream>

using namespace std;

int main() {
	//json::Builder{}.StartDict().Build();  // правило 3
	//json::Builder{}.StartDict().Key("1"s).Value(1).Value(1);  // правило 2
	//json::Builder{}.StartDict().Key("1"s).Key(""s);  // правило 1
	//json::Builder{}.StartArray().Key("1"s);  // правило 4
	//json::Builder{}.StartArray().EndDict();  // правило 4
	//json::Builder{}.StartArray().Value(1).Value(2).EndDict();  // правило 5



	try {
		json::Print(
			json::Document{
				json::Builder{}
				.Value("just a string"s)
				.Build()
			},
			cout
		);
	}
	catch (const std::logic_error& err) { cout << "1 logic_error - " << err.what() << endl; }
	catch (const std::exception& err) { cout << "2 unexpect_error - " << err.what() << endl; }
	try {
		json::Print(
			json::Document{
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
	catch (const std::logic_error& err) { cout << "2 logic_error - " << err.what() << endl; }
	catch (const std::exception& err) { cout << "2 unexpect_error - " << err.what() << endl; }
}