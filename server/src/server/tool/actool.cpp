//------------------------------------------------------------------------------
// 文件: actool.cpp
// 作者: 洪坤安
// 日期: 2017年4月25日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#include "actool.h"
#include "aho_corasick.hpp"
#include <chrono>
#include <iostream>
#include "strtool.h"

namespace actool
{
	namespace ac = aho_corasick;
	using trie = ac::trie;

	using namespace std;

	string gen_str(int len) {
		static const char alphanum[] =
			"0123456789"
			"!@#$%^&*"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		string str;
		for (int i = 0; i < len; ++i) {
			str.append(1, alphanum[rand() % (sizeof(alphanum) - 1)]);
		}
		return string(str);
	}

	size_t bench_naive(vector<string> text_strings, vector<string> patterns) {
		size_t count = 0;
		for (auto& text : text_strings) {
			for (auto& pattern : patterns) {
				size_t pos = text.find(pattern);
				while (pos != text.npos) {
					pos = text.find(pattern, pos);
					count++;
				}
			}
		}
		return count;
	}

	size_t bench_aho_corasick(vector<string> text_strings, trie& t) {
		size_t count = 0;
		for (auto& text : text_strings) {
			auto matches = t.parse_text(text);
			count += matches.size();
		}
		return count;
	}

	// 性能测试
	void test()
	{
		cout << "*** Aho-Corasick Benchmark ***" << endl;

		cout << "Generating input text ...";
		set<string> input_strings;
		while (input_strings.size() < 10) {
			input_strings.insert(strtool::random_str(256));
		}
		vector<string> input_vector(input_strings.begin(), input_strings.end());
		cout << " done" << endl;

		cout << "Generating search patterns ...";
		set<string> patterns;
		while (patterns.size() < 1000000) {
			patterns.insert(strtool::random_str(8));
		}
		vector<string> pattern_vector(patterns.begin(), patterns.end());
		cout << " done" << endl;

		cout << "Generating trie ...";
		trie t;
		for (auto& pattern : patterns) {
			t.insert(pattern);
		}
		t.parse_text("");
		cout << " done" << endl;

		map<size_t, tuple<chrono::high_resolution_clock::duration, chrono::high_resolution_clock::duration>> timings;

		cout << "Running ";
		for (size_t i = 10; i > 0; --i) {
			cout << ".";
			auto start_time = chrono::high_resolution_clock::now();
			size_t count_1 = bench_naive(input_vector, pattern_vector);
			auto end_time = chrono::high_resolution_clock::now();
			auto time_1 = end_time - start_time;

			start_time = chrono::high_resolution_clock::now();
			size_t count_2 = bench_aho_corasick(input_vector, t);
			end_time = chrono::high_resolution_clock::now();
			auto time_2 = end_time - start_time;

			if (count_1 != count_2) {
				cout << "failed" << endl;
			}

			timings[i] = make_tuple(time_1, time_2);
		}
		cout << " done" << endl;

		cout << "Results: " << endl;
		for (auto& i : timings) {
			cout << "  loop #" << i.first;
			cout << ", naive: " << chrono::duration_cast<chrono::milliseconds>(get<0>(i.second)).count();
			cout << "ms, ac: " << chrono::duration_cast<chrono::milliseconds>(get<1>(i.second)).count() << "ms";
			cout << endl;
		}
	}
}