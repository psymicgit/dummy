//------------------------------------------------------------------------------
// �ļ�: utiltool.h
// ����: ������
// ����: 2017��4��27��
// ˵��: ͨ�ú���
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

namespace utiltool
{
	// set����set
	template <typename Container1, typename Container2>
	inline void Add(Container1 &a, const Container2 &b)
	{
		a.insert(b.begin(), b.end());
	}

	// set����map
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

	// set��ȥset
	template <typename T>
	inline void Del(std::set<T> &a, const std::set<T> &b)
	{
		for (const T &t : b)
		{
			a.erase(t);
		}
	}

	// set��ȥmap
	template <typename Key, typename Val>
	inline void Del(std::set<Key> &a, const std::map<Key, Val> &b)
	{
		for (const auto &itr : b)
		{
			a.erase(itr.first);
		}
	}

	// map��ȥset
	template <typename Key, typename Val>
	inline void Del(std::map<Key, Val> &a, const std::set<Key> &b)
	{
		for (const Key &t : b)
		{
			a.erase(t);
		}
	}

	// set����set�з��������ĳ�Ա
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

	// ɾ��set�з���ָ��������Ԫ��
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

	// ɾ��set�з���ָ��������Ԫ��
	template <typename Container, typename Op>
	inline void EraseIf(Container& container, const Op& op)
	{
		for (auto it = container.begin(); it != container.end(); )
		{
			if (op(*it)) container.erase(it++);
			else ++it;
		}
	}

	// ɾ��map�з���ָ��������Ԫ��
	template <typename Container, typename Op>
	inline void MapEraseIf(Container& container, const Op& op)
	{
		for (auto it = container.begin(); it != container.end(); )
		{
			if (op(it->first, it->second)) container.erase(it++);
			else ++it;
		}
	}

	// ��ѯmap��a����Ӧ��ֵ�Ƿ������b
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

	// ��ȡ�����ļ����������������ļ�������a����b1��b2��b1��b2��������b3 ~ b100��������a����b1 ~ b100��
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