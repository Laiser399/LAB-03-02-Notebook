#pragma once

#ifndef HASHMULTITABLE_H
#define HASHMULTITABLE_H

#include <vector>
#include <string>
#include "TempHashFunction.h"

#define HMT_DEFAULT_MULTI 1777
#define HMT_DEFAULT_SIZE 32
#define HMT_DEFAULT_MAX_DEPTH 5
#define ABS(a) ((a) < 0 ? -(a) : (a))

template<typename key_t, typename value_t>
class HashMultiTable
{
private:
	std::vector<std::vector<std::pair<key_t, std::vector<value_t> > > > table;
	int currDepth, maxDepth, hashMulti;
	bool canRebuild;

	int hashFunc(key_t &key) const;
	void rebuild();
public:
	HashMultiTable();
	HashMultiTable(int size, int maxDepth = HMT_DEFAULT_MAX_DEPTH,
		int hashMulti = HMT_DEFAULT_MULTI);
	HashMultiTable(HashMultiTable<key_t, value_t> &another);
	HashMultiTable<key_t, value_t> & operator =(HashMultiTable<key_t, value_t> &another);

	bool insert(key_t key, value_t &value);
	bool exist(key_t key) const;
	const std::vector<value_t> & find(key_t key) const;
	bool change(key_t key, std::vector<value_t> &new_arr);
	bool change(key_t key, value_t &prev_val, value_t &new_val);
    void clear();

    bool erase(key_t key);
    bool erase(key_t key, value_t value);


    //template<typename key_t, typename value_t>
	friend std::ostream & operator <<(std::ostream &out, HashMultiTable<key_t, const value_t> &ob);

};

//-------------|
//   private   |
//-------------|

template<typename key_t, typename value_t>
int HashMultiTable<key_t, value_t>::hashFunc(key_t &key) const {
	return ABS(tempHashFunc<key_t>(key, hashMulti)) % table.size();
}

template<typename key_t, typename value_t>
void HashMultiTable<key_t, value_t>::rebuild() {
	HashMultiTable<key_t, value_t> newHashTable(this->table.size() * 2, this->maxDepth, this->hashMulti);
	newHashTable.canRebuild = false;

	// it - указ на эл-нт главной таблицы
	for (auto it = table.begin(), it_last = table.end();
		it != it_last; ++it)
	{
		// in_it - указатель на эл-нт массива (ключ - массив значений)
		for (auto in_it = (*it).begin(), in_it_last = (*it).end();
			in_it != in_it_last; ++in_it)
		{
			for (auto it_val = in_it->second.begin(), it_val_last = in_it->second.end();
				it_val != it_val_last; ++it_val)
			{
				newHashTable.insert(in_it->first, *it_val);
			}
			//newHashTable.insert((*in_it).first, (*in_it).second);
		}
	}

	newHashTable.canRebuild = true;
	*this = newHashTable;
}

//------------|
//   public   |
//------------|
template<typename key_t, typename value_t>
HashMultiTable<key_t, value_t>::HashMultiTable() {
	table.resize(HMT_DEFAULT_SIZE);
	currDepth = 0;
	maxDepth = HMT_DEFAULT_MAX_DEPTH;
	hashMulti = HMT_DEFAULT_MULTI;
	canRebuild = true;
}

template<typename key_t, typename value_t>
HashMultiTable<key_t, value_t>::HashMultiTable(int size, int maxDepth, int hashMulti) {
	if (size <= 0)
		throw std::string("HashMultiTable: constructor: received size <= 0!");
	if (maxDepth < 1)
		throw std::string("HashMultiTable: constructor: received maxDepth < 1!");
	if (hashMulti == 0)
		throw std::string("HashMultiTable: constructor: received hashMulti == 1!");

	table.resize(size);
	currDepth = 0;
	this->maxDepth = maxDepth;
	this->hashMulti = hashMulti;
	this->canRebuild = true;
}

template<typename key_t, typename value_t>
HashMultiTable<key_t, value_t>::HashMultiTable(HashMultiTable<key_t, value_t> &another) {
	this->table.resize(another.table.size());
	this->currDepth = another.currDepth;
	this->maxDepth = another.maxDepth;
	this->hashMulti = another.hashMulti;
	this->canRebuild = another.canRebuild;

	// it указ на €чейку хеш таблицы
	for (auto it = table.begin(), an_it = another.table.begin();
		(it != table.end()) && (an_it != another.table.end()); ++it, ++an_it)
	{
		// inside_it указ на пару ключ - массив
		for (auto inside_it = (*an_it).begin(); inside_it != (*an_it).end(); ++inside_it) {
			std::vector<value_t> tm_arr;
			// val_it указ на элемент массива значений дл€ тек ключа
			for (auto val_it = inside_it->second.begin(); val_it != inside_it->second.end(); ++val_it) {
				tm_arr.push_back(*val_it);
			}
			(*it).push_back(std::pair<key_t, std::vector<value_t> >(inside_it->first, tm_arr));
		}
	}
}

template<typename key_t, typename value_t>
HashMultiTable<key_t, value_t> & HashMultiTable<key_t, value_t>::operator =(HashMultiTable<key_t, value_t> &another) {
	this->table.resize(another.table.size());
	this->currDepth = another.currDepth;
	this->maxDepth = another.maxDepth;
	this->hashMulti = another.hashMulti;
	this->canRebuild = another.canRebuild;

	// it указ на €чейку хеш таблицы
	for (auto it = table.begin(), an_it = another.table.begin();
		(it != table.end()) && (an_it != another.table.end()); ++it, ++an_it)
	{
		it->resize(0);
		// inside_it указ на пару ключ - массив
		for (auto inside_it = (*an_it).begin(); inside_it != (*an_it).end(); ++inside_it) {
			std::vector<value_t> tm_arr;
			// val_it указ на элемент массива значений дл€ тек ключа
			for (auto val_it = inside_it->second.begin(); val_it != inside_it->second.end(); ++val_it) {
				tm_arr.push_back(*val_it);
			}
			(*it).push_back(std::pair<key_t, std::vector<value_t> >(inside_it->first, tm_arr));
		}
	}

	return *this;
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::insert(key_t key, value_t &value) {
	int index = hashFunc(key);

	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			// serch on repeat, insert
			// поиск совпадени€ / несовпадени€ в массиве значений дл€ данного ключа
			for (auto it_val = it->second.begin(); it_val != it->second.end(); ++it_val) {
				if (*it_val == value) 
					return false;
			}
			it->second.push_back(value);
			return true;
		}
	}

	// not found -> insert
	std::vector<value_t> tm_arr;
	tm_arr.push_back(value);
	table[index].push_back(std::pair<key_t, std::vector<value_t> >(key, tm_arr));

	if (table[index].size() > currDepth)
		currDepth = table[index].size();

	while (currDepth > maxDepth) {
        rebuild();
    }
	return true;
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::exist(key_t key) const {
	int index = hashFunc(key);

	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			return true;
		}
	}
	return false;
}

template<typename key_t, typename value_t>
const std::vector<value_t> & HashMultiTable<key_t, value_t>::find(key_t key) const {
	int index = hashFunc(key);

	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			return it->second;
		}
	}

	throw std::string("HashMultiTable: find: not found array with this key!");
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::change(key_t key, std::vector<value_t> &new_arr) {
	int index = hashFunc(key);

	// it - указ на пару (ключ - массив значений)
	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if (it->first == key) {
			it->second.resize(0);
			for (auto it_val = new_arr.begin(), it_val_last = new_arr.end(); it_val != it_val_last; ++it_val) {
				it->second.push_back(*it_val);
			}
			return true;
		}
	}
	return false;
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::change(key_t key, value_t &prev_val, value_t &new_val) {
	int index = hashFunc(key);

	// it - указ на пару (ключ - массив значений)
	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if (it->first == key) {
			for (auto it_val = it->second.begin(), it_val_last = it->second.end(); it_val != it_val_last; ++it_val) {
				if (*it_val == prev_val) {
					*it_val = new_val;
				}
			}
			return false;
		}
	}
	return false;
}

template<typename key_t, typename value_t>
void HashMultiTable<key_t, value_t>::clear() {
    for (auto it = table.begin(), it_l = table.end();
         it != it_l; ++it)
    {
        it->resize(0);
    }
    currDepth = 0;
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::erase(key_t key) {
    int index = hashFunc(key);
    // итератор на пару (ключ - массив значений)
    for (auto it_p = table[index].begin(), it_p_l = table[index].end();
         it_p != it_p_l; ++it_p)
    {
        if (it_p->first == key) {
            table[index].erase(it_p);
            return true;
        }
    }
    return false;
}

template<typename key_t, typename value_t>
bool HashMultiTable<key_t, value_t>::erase(key_t key, value_t value) {
    int index = hashFunc(key);
    // итератор на пару (ключ - массив значений)
    for (auto it_p = table[index].begin(), it_p_l = table[index].end();
         it_p != it_p_l; ++it_p)
    {
        if (it_p->first == key) {
            // итератор на значение дл€ данного ключа
            for (auto it_v = it_p->second.begin(), it_v_l = it_p->second.end();
                 it_v != it_v_l; ++it_v)
            {
                if (*it_v == value) {
                    it_p->second.erase(it_v);
                    // если значений дл€ данного ключа не осталось
                    if (it_p->second.size() == 0) {
                        table[index].erase(it_p);
                    }
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

template<typename key_t, typename value_t>
std::ostream & operator <<(std::ostream &out, HashMultiTable<key_t, value_t> &ob) {
	for (auto it = ob.table.begin(), it_last = ob.table.end();
		it != it_last; ++it)
	{
		int index = it - ob.table.begin();
		for (auto in_it = (*it).begin(), in_it_last = (*it).end();
			in_it != in_it_last; ++in_it)
		{
            out << "index: " << index << ", key: " << (*in_it).first << ", value: " << '\n';
			for (auto it_val = in_it->second.begin(); it_val != in_it->second.end(); ++it_val) {
                out << "   " << *it_val << '\n';
			}

		}
	}
    out << "depth: " << ob.currDepth << '\n';
	return out;
}

#endif // HASHMULTITABLE_H
