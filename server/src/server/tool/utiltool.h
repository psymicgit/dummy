//------------------------------------------------------------------------------
// 文件: utiltool.h
// 作者: 洪坤安
// 日期: 2017年4月27日
// 说明: 通用函数
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

namespace utiltool
{
	// set加上set
	template <typename Container1, typename Container2>
	inline void Add(Container1 &a, const Container2 &b)
	{
		a.insert(b.begin(), b.end());
	}

	// set加上map
	template <typename Key, typename Val>
	inline void Add(std::set<Key> &a, const std::map<Key, Val> &b)
	{
		for (const auto &itr : b)
		{
			a.insert(itr.first);
		}
	}

	template <typename Container, typename Key>
	inline bool Has(Container& container, const Key &key)
	{
		return container.find(key) != container.end();
	}

	// set减去set
	template <typename T>
	inline void Del(std::set<T> &a, const std::set<T> &b)
	{
		for (const T &t : b)
		{
			a.erase(t);
		}
	}

	// set减去map
	template <typename Key, typename Val>
	inline void Del(std::set<Key> &a, const std::map<Key, Val> &b)
	{
		for (const auto &itr : b)
		{
			a.erase(itr.first);
		}
	}

	// map减去set
	template <typename Key, typename Val>
	inline void Del(std::map<Key, Val> &a, const std::set<Key> &b)
	{
		for (const Key &t : b)
		{
			a.erase(t);
		}
	}

	// set加上set中符合条件的成员
	template <typename Key, typename Op>
	inline void AddIf(std::set<Key> &a, const std::set<Key> &b, const Op& op)
	{
		for (const Key &key : b)
		{
			if (op(key))
			{
				a.insert(key);
			}
		}
	}

	// 删除set中符合指定条件的元素
	template <typename Container, typename Value>
	inline void VectorErase(Container& container, const Value& val)
	{
		for (int i = 0, n = (int)container.size(); i < n; ++i)
		{
			if (container[i] == val)
			{
				container.erase(container.begin() + i);
				break;
			}
		}
	}

	// 删除set中符合指定条件的元素
	template <typename Container, typename Op>
	inline void EraseIf(Container& container, const Op& op)
	{
		for (auto it = container.begin(); it != container.end(); )
		{
			if (op(*it)) container.erase(it++);
			else ++it;
		}
	}

	// 删除map中符合指定条件的元素
	template <typename Container, typename Op>
	inline void MapEraseIf(Container& container, const Op& op)
	{
		for (auto it = container.begin(); it != container.end(); )
		{
			if (op(it->first, it->second)) container.erase(it++);
			else ++it;
		}
	}

	// 查询map中a键对应的值是否包含了b
	template <typename Container, typename Key1, typename Key2>
	inline bool HasInMap(const Container &container, const Key1 &a, const Key2 &b)
	{
		auto &itr = container.find(a);
		if (itr == container.end())
		{
			return false;
		}

		return Has(itr->second, b);
	}

	// 获取单个文件依赖的所有其他文件（假设a依赖b1和b2，b1和b2又依赖了b3 ~ b100，则最终a依赖b1 ~ b100）
	template <typename T, typename AddTodoFunc>
	void GetChain(std::set<T> &chain, T top, const AddTodoFunc& expand)
	{
		std::set<T> todo;
		std::set<T> &done = chain;

		todo.insert(top);

		while (!todo.empty())
		{
			const T &cur = *todo.begin();

			if (done.find(cur) == done.end())
			{
				done.insert(cur);

				std::set<T> more;
				expand(done, more, cur);

				todo.erase(todo.begin());
				Add(todo, more);
			}
			else
			{
				todo.erase(todo.begin());
			}
		}
	}

	inline bool Between(float x, float low, float high)
	{
		return low <= x && x <= high;
	}
}