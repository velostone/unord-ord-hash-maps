#pragma once
#include <utility>
#include <vector>
#include <optional>
#include <algorithm>

using std::size_t;

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
		using TableByArray<T>::clear;
	public:
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
		std::optional<size_t> find_idx(size_t key) const // std::optional either contains a value or does not contain a value
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
		std::pair<size_t, T>& operator[](size_t pos) { return values_.at(pos); }
	};

	template<class T>
	class OrderedTable : public TableByArray<T>
	{
		using TableByArray<T>::values_;
		using TableByArray<T>::sz_;
		using TableByArray<T>::clear;
	public:
		// ctor & dtor
		OrderedTable() = default;
		~OrderedTable() { clear(); }
		// methods
		std::optional<std::pair<size_t, T>> find(size_t key) const
		{
			auto idx = std::lower_bound(values_.begin(), values_.end(), std::make_pair(key, T()),
				[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T>& rhs) { return (lhs.first < rhs.first); }); // lambda key comparator 
			if (idx != values_.end() && (*idx).first == key) return *idx;
			else return std::nullopt;
		}
		bool insert(size_t key, const T& data)
		{
			auto idx = std::lower_bound(values_.begin(), values_.end(), std::make_pair(key, data),
				[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T> rhs) { return (lhs.first < rhs.first); }); // lambda key comparator 
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
					[](const std::pair<size_t, T>& lhs, const std::pair<size_t, T> rhs) { return (lhs.first < rhs.first); }); // lambda key comparator 
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
		std::pair<size_t, T>& operator[](size_t pos) { return values_.at(pos); }
	};
}