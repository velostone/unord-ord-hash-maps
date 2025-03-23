#pragma once
#include <utility> // std::pair
#include <vector>
#include <optional> // std::optional
#include <algorithm> // std::lower_bound
#include <cstdint> // uint32_t for hashing
#include <list>

using std::size_t;
using std::uint32_t;

namespace maps
{
	template<class T>
	class TableInterface
	{
	public:
		virtual bool insert(size_t key, const T& data) = 0;
		virtual bool erase(size_t key) = 0;
		virtual std::optional<std::pair<size_t, T>> find(size_t key) const = 0;
		virtual size_t getSize() const noexcept = 0;
		virtual bool isEmpty() const noexcept = 0;
		virtual void clear() = 0;
	};

	template<class T, class CellType = std::pair<size_t, T>>
	class TableByArray : public TableInterface<T>
	{
	protected:
		std::vector<CellType> values_;
		size_t sz_ = 0;
	public:
		size_t getSize() const noexcept override { return sz_; }
		bool isEmpty() const noexcept override { return sz_ == 0; }
		std::pair<size_t, T>& operator[](size_t pos) { return values_.at(pos); }
		void clear() override
		{
			values_.clear();
			sz_ = 0;
		}
	};

	template<class T>
	class UnorderedTable : public TableByArray<T>
	{
		using TableByArray<T>::values_;
		using TableByArray<T>::sz_;
	public:
		using TableByArray<T>::clear;
		using TableByArray<T>::isEmpty;
		using TableByArray<T>::operator[];
		// ctor & dtor
		UnorderedTable() = default;
		~UnorderedTable() { clear(); }
		// methods
		std::optional<std::pair<size_t, T>> find(size_t key) const override // std::optional either contains a value or does not contain a value
		{
			for (size_t i = 0; i < sz_; ++i)
				if (values_[i].first == key) return values_[i];
			return std::nullopt;
		}
		std::optional<size_t> find_idx(size_t key) const
		{
			for (size_t i = 0; i < sz_; ++i)
				if (values_[i].first == key) return i;
			return std::nullopt;
		}
		bool insert(size_t key, const T& data) override
		{
			if (isEmpty() || !find(key).has_value())
			{
				values_.push_back(std::make_pair(key, data));
				++sz_;
				return true;
			}
			else return false;
		}
		bool erase(size_t key) override
		{
			if (isEmpty()) return false;
			std::optional idx = find_idx(key);
			if (idx.has_value())
			{
				if (sz_ != 1) std::swap(values_[idx.value()], values_[sz_ - 1]);
				values_.pop_back();
				--sz_;
				return true;
			}
			else return false;
		}
	};

	template<class T>
	class OrderedTable : public TableByArray<T>
	{
		using TableByArray<T>::values_;
		using TableByArray<T>::sz_;
	public:
		using TableByArray<T>::clear;
		using TableByArray<T>::isEmpty;
		using TableByArray<T>::operator[];
		// ctor & dtor
		OrderedTable() = default;
		~OrderedTable() { clear(); }
		// methods
		std::optional<std::pair<size_t, T>> find(size_t key) const override
		{
			auto idx = std::lower_bound(values_.begin(), values_.end(), std::make_pair(key, T()),
				[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T>& rhs) { return (lhs.first < rhs.first); }); // lambda key comparator 
			if (idx != values_.end() && (*idx).first == key) return *idx;
			else return std::nullopt;
		}
		bool insert(size_t key, const T& data) override
		{
			auto idx = std::lower_bound(values_.begin(), values_.end(), std::make_pair(key, data),
				[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T> rhs) { return (lhs.first < rhs.first); });
			if (idx != values_.end())
			{
				if ((*idx).first == key) return false;
				else
				{
					values_.insert(idx, std::make_pair(key, data));
					++sz_;
					return true;
				}
			}
			else
			{
				values_.push_back(std::make_pair(key, data));
				++sz_;
				return true;
			}
			return false;
		}
		bool erase(size_t key) override
		{
			if (isEmpty()) return false;
			else
			{
				auto idx = std::lower_bound(values_.begin(), values_.end(), std::make_pair(key, T()),
					[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T> rhs) { return (lhs.first < rhs.first); });
				if (idx != values_.end() && (*idx).first == key)
				{
					if (*idx == values_[sz_ - 1])
					{
						values_.pop_back();
						--sz_;
						return true;
					}
					else
					{
						values_.erase(idx);
						--sz_;
						return true;
					}
				}
				else return false;
			}
			return false;
		}
	};

	template<class T>
	class HashTable : public TableByArray<T, std::list<std::pair<size_t, T>>*>
	{
		// separate chaining is used as a collision resolution technique
		using TableByArray<T, std::list<std::pair<size_t, T>>*>::values_;
		using TableByArray<T, std::list<std::pair<size_t, T>>*>::sz_; // number of records
		size_t values_size_ = 0; // number of rows (vector size)
		uint32_t murmurhash3_32(const void* key, size_t length, uint32_t seed = 0) const
		{
			const uint8_t* data = static_cast<const uint8_t*>(key);
			const size_t nblocks = length / 4;
			uint32_t h1 = seed;
			const uint32_t c1 = 0xcc9e2d51;
			const uint32_t c2 = 0x1b873593;
			const uint32_t* blocks = reinterpret_cast<const uint32_t*>(data);
			for (size_t i = 0; i < nblocks; ++i) {
				uint32_t k1 = blocks[i];
				k1 *= c1;
				k1 = (k1 << 15) | (k1 >> 17);
				k1 *= c2;
				h1 ^= k1;
				h1 = (h1 << 13) | (h1 >> 19);
				h1 = h1 * 5 + 0xe6546b64;
			}
			const uint8_t* tail = data + nblocks * 4;
			uint32_t k1 = 0;
			switch (length & 3)
			{
			case 3: k1 ^= tail[2] << 16;
			case 2: k1 ^= tail[1] << 8;
			case 1: k1 ^= tail[0];
				k1 *= c1;
				k1 = (k1 << 15) | (k1 >> 17);
				k1 *= c2;
				h1 ^= k1;
			}
			h1 ^= length;
			h1 ^= h1 >> 16;
			h1 *= 0x85ebca6b;
			h1 ^= h1 >> 13;
			h1 *= 0xc2b2ae35;
			h1 ^= h1 >> 16;
			return h1;
		}
	public:
		using TableByArray<T, std::list<std::pair<size_t, T>>*>::clear;
		using TableByArray<T, std::list<std::pair<size_t, T>>*>::isEmpty;
		// ctor & dtor
		HashTable(size_t sz = 100) : values_size_(sz) 
		{
			values_.resize(values_size_);
		};
		~HashTable() { clear(); }
		// hash function
		uint32_t h(size_t key, uint32_t seed = 0) const // size_t to uint32_t & hashing as 32-bit
		{
			uint32_t lower = static_cast<uint32_t>(key);
			uint32_t upper = static_cast<uint32_t>(key >> 32);
			uint32_t h1 = murmurhash3_32(&lower, sizeof(lower), seed);
			uint32_t h2 = murmurhash3_32(&upper, sizeof(upper), seed);
			return (h1 ^ h2) % values_size_; // compressed hash
		}
		// other methods
		std::optional<std::pair<size_t, T>> find(size_t key) const override
		{
			auto idx = h(key);
			if (values_[idx] != nullptr)
			{
				auto list_ = values_[idx];
				for (auto it = list_->begin(); it != list_->end(); ++it)
					if ((*it).first == key) return *it;
			}
			return std::nullopt;
		}
		bool insert(size_t key, const T& data) override
		{
			auto idx = h(key);
			if (values_[idx] == nullptr)
			{
				auto p = new std::list<std::pair<size_t, T>>;
				p->push_back(std::make_pair(key, data));
				values_[idx] = p;
				++sz_;
				return true;
			}
			else
			{
				if (find(key).has_value()) return false;
				else
				{
					values_[idx]->push_back(std::make_pair(key, data));
					++sz_;
					return true;
				}
			}
			return false;
		}
		bool erase(size_t key) override
		{
			auto idx = h(key);
			if (values_[idx] == nullptr) return false;
			else
			{
				for (auto it = values_[idx]->begin(); it != values_[idx]->end(); ++it)
					if ((*it).first == key)
					{
						values_[idx]->erase(it);
						if (values_[idx]->size() == 0)
						{
							delete values_[idx];
							values_[idx] = nullptr;
							--sz_;
						}
						return true;
					}
			}
			return false;
		}
		std::pair<size_t, T> operator[](size_t key) 
		{ 
			std::optional<std::pair<size_t, T>> p = find(key);
			return p.value_or(std::make_pair(0, -1));
		}
	};
}