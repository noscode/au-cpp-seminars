#pragma once
#include <vector>
#include <functional>

template<class T>
class enumerable;

template<class T>
class enumerator
{
public:
	virtual T get_current() const = 0;
	virtual bool move_next() = 0;
	virtual void reset() = 0;
	virtual ~enumerator() {};

	virtual enumerator<T>* clone() const = 0;
};

template<class T, class FwdIt>
class iterator_enumerator : public enumerator<T>
{
	FwdIt begin_;
	FwdIt end_;
	FwdIt current_;
	bool initialized_;

public:
	iterator_enumerator(FwdIt begin,
		FwdIt end,
		FwdIt current = FwdIt(),
		bool initialized = false)
		: begin_(begin)
		, end_(end)
		, current_(current)
		, initialized_(initialized)
	{
	}

	virtual T get_current() const override
	{
		if (!initialized_)
			throw std::logic_error("Call move_next before current");
		return *current_;
	}

	virtual bool move_next() override
	{
		if (!initialized_)
		{
			current_ = begin_;
			initialized_ = true;
		}
		else
			++current_;
		return current_ != end_;
	}

	virtual void reset() override
	{
		initialized_ = false;
	}

	virtual enumerator<T>* clone() const override 
	{
		return new iterator_enumerator(*this);
	};
};


template<class T, class FwdIt>
iterator_enumerator<T, FwdIt> make_enumerator(FwdIt begin, FwdIt end)
{
	return iterator_enumerator<T, FwdIt>(begin, end);
}

template<class T>
class where_enumerator : public enumerator<T>
{
	enumerator<T>* parent_;
	std::function<bool(T)> predicate_;

public:
	explicit where_enumerator(enumerator<T>& parent, std::function<bool(T)> predicate)
		: parent_(parent.clone())
		, predicate_(predicate)
	{
	}

	virtual T get_current() const override
	{
		return parent_->get_current();
	}

	virtual bool move_next() override
	{
		while (parent_->move_next())
		{
			if (predicate_(parent_->get_current()))
				return true;
		}
		return false;
	}

	virtual void reset() override
	{
		parent_->reset();
	}

	virtual enumerator<T>* clone() const override
	{
		return new where_enumerator(*parent_, predicate_);
	};

	virtual ~where_enumerator()
	{
		delete parent_;
	}
};

template<class V, class T>
class select_enumerator : public enumerator<V>
{
	enumerator<T>* parent_;
	std::function<V(T)> transform_;

public:
	explicit select_enumerator(enumerator<T>& parent, std::function<V(T)> transform)
		: parent_(parent.clone())
		, transform_(transform)
	{
	}

	virtual V get_current() const override
	{
		return transform_(parent_->get_current());
	}

	virtual bool move_next() override
	{
		return parent_->move_next();
	}

	virtual void reset() override
	{
		parent_->reset();
	}

	virtual enumerator<V>* clone() const override
	{
		return new select_enumerator(*parent_, transform_);
	};

	virtual ~select_enumerator()
	{
		delete parent_;
	}
};

template<class T>
class enumerable
{
public:
	explicit enumerable(const enumerator<T>& enumerator)
		: enumerator_(enumerator.clone())
	{
	}

	enumerable<T> where(std::function<bool(T)> predicate)
	{
		return enumerable<T>(where_enumerator<T>(*enumerator_, predicate));
	}

	template<class V>
	enumerable<V> select(std::function<V(T)> transform)
	{
		return enumerable<V>(select_enumerator<T, V>(*enumerator_, transform));
	}

	bool any()
	{
		enumerator_->reset();
		return enumerator_->move_next();
	}

	bool any(std::function<bool(T)> predicate)
	{
		return enumerable<T>(where_enumerator<T>(*enumerator_, predicate)).any();
	}

	size_t count()
	{
		size_t count = 0;
		enumerator_->reset();
		while (enumerator_->move_next())
			count++;
		return count;
	}

	size_t count(std::function<bool(T)> predicate)
	{
		return enumerable<T>(where_enumerator<T>(*enumerator_, predicate)).count();
	}

	std::vector<T> to_vector()
	{
		std::vector<T> result;
		enumerator_->reset();
		while (enumerator_->move_next())
			result.push_back(enumerator_->get_current());
		return result;
	}

	~enumerable()
	{
		delete enumerator_;
	}
private:
	enumerator<T>* enumerator_;
};

template<typename T>
enumerable<T> from(const std::vector<T>& v)
{
	return enumerable<T>(make_enumerator<T>(v.begin(), v.end()));
}
