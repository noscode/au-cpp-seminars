#include <string.h>
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <cstdint>

using namespace std;


namespace serialization
{
	typedef char			byte_t;
	typedef vector<byte_t>	bytes_t;

	typedef uint64_t		size_type;

	struct output_stream
	{
		explicit output_stream(bytes_t&& from = bytes_t())
			: buffer_(move(from))
		{
		}

		void write(const void* data, size_t size)
		{
			const byte_t* p = static_cast<const byte_t*>(data);

			size_t old_size = buffer_.size();
			buffer_.resize(old_size + size);
			memcpy(buffer_.data() + old_size, p, size);
		}

		bytes_t detach()
		{
			return move(buffer_);
		}

		bytes_t const& data() const 
		{
			return buffer_;
		}

	private:
		bytes_t	buffer_;
	};

	struct input_stream
	{
		explicit input_stream(bytes_t const& from)
			: begin_(from.data())
			, end_  (from.data() + from.size())
			, cur_  (from.data())
		{
		}

		void read(void* to, size_t size)
		{
			assert(cur_ + size <= end_);
			memcpy(to, cur_, size);
			cur_ += size;
		}

	private:
		byte_t const* begin_;
		byte_t const* end_;
		byte_t const* cur_;

	};

	
	namespace type_traits
	{
		template <class T>
		struct is_container
		{
			struct yes { char nothing;    };
			struct no  { yes  nothing[2]; };

			template <typename U>
			static auto test(U* u) -> decltype(typename U::iterator(), yes());
			static no   test(...);

			enum { value = (sizeof(yes) == sizeof test((T*)0)) };
			typedef integral_constant<bool, value> type;
		};

		template <class T>
		struct is_container_or_pod
		{
			enum { value = is_container<T>::value || is_pod<T>::value };
			typedef integral_constant<bool, value> type;
		};



	} // type_traits


	// common
	template<class type>
	void write(output_stream& os, type const& obj,
            typename enable_if<!type_traits::is_container_or_pod<type>::value>::type* = 0)
	{
		serialize(os, const_cast<type&>(obj)); // little fuck-up
	}

	template<class type>
	void read(input_stream& is, type& obj,
            typename enable_if<!type_traits::is_container_or_pod<type>::value>::type* = 0)
	{
		serialize(is, obj);
	}
    // for pods and containers
	template<class type>
	void serialize(output_stream& os, type const& obj,
            typename enable_if<type_traits::is_container_or_pod<type>::value>::type* = 0)
	{
		write(os, obj);
	}

	template<class type>
	void serialize(input_stream& is, type& obj,
            typename enable_if<type_traits::is_container_or_pod<type>::value>::type* = 0)
	{
		read(is, obj);
	}



	//////////////////////////////////////////////////////////////////////////
	// readers
	template<class type>
	void read(input_stream& is, type& obj,
            typename enable_if<is_pod<type>::value>::type* = 0)
	{
		is.read(&obj, sizeof(obj));
	}

	template<class type1, class type2>
	void read(input_stream& is, pair<type1, type2>& p)
	{
		// for map & unordered_map
		typedef typename remove_const<type1>::type first_type;
		typedef typename remove_const<type2>::type second_type;

		read(is, const_cast<first_type &>(p.first )); // to solve const map key problem
		read(is, const_cast<second_type&>(p.second)); // just to be simetric 
	}

	template<class type>
	void read(input_stream& is, type& container,
            typename enable_if<type_traits::is_container<type>::value>::type* = 0)
	{
		size_type sz = 0;
		read(is, sz);

		container.clear();
		for (size_t i = 0; i < sz; ++i)
		{
			typename type::value_type obj;
			read(is, obj);
			container.insert(container.end(), move(obj));
		}
	}

	template<class type, class alloc>
	void read(input_stream& is, vector<type, alloc>& vec,
            typename enable_if<is_pod<type>::value>::type* = 0)
	{
		size_type sz = 0;
		read(is, sz);

		vec.resize(typename vector<type, alloc>::size_type(sz));
		is.read(vec.data(), vec.size() * sizeof(type));
	}

	void read(input_stream& is, string& str)
	{
		size_type sz = 0;
		read(is, sz);

		str.resize(string::size_type(sz));
		is.read(&str[0], str.size());
	}

	//////////////////////////////////////////////////////////////////////////
	// writers 
	template<class type>
	void write(output_stream& os, type const& obj,
            typename enable_if<is_pod<type>::value>::type* = 0)
	{
		os.write(&obj, sizeof(obj));
	}

	template<class type1, class type2>
	void write(output_stream& os, pair<type1, type2> const& p)
	{
		write(os, p.first);
		write(os, p.second);
	}

	template<class type>
	void write(output_stream& os, type const& container,
            typename enable_if<type_traits::is_container<type>::value>::type* = 0)
	{
		write(os, size_type(container.size()));

		for (auto it = container.begin(); it != container.end(); ++it)
			write(os, *it);
	}

	template<class type, class alloc>
	void write(output_stream& os, vector<type, alloc>& vec,
            typename enable_if<is_pod<type>::value>::type* = 0)
	{
		write(os, size_type(vec.size()));
		os.write(vec.data(), vec.size() * sizeof(type));
	}

	void write(output_stream& os, string const& str)
	{
		write(os, size_type(str.size()));
		os.write(str.c_str(), str.size());
	}
} // serialization 


//////////////////////////////////////////////////////////////////////////
// tests


template<class type>
bool eq_container(type const& lhs, type const& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	for (auto it = lhs.begin(), jt = rhs.begin(); it != lhs.end(); ++it, ++jt)
		if (!(*it == *jt))
			return false;

	return true;
}


struct custom_record
{
	custom_record()
		: number(0)
	{
	}

	custom_record(string const& t, int n)
		: text  (t)
		, number(n)
		, texts (n, t)
	{
	}
	
	string text;
	int    number;
	vector<string> texts;
};

bool operator==(custom_record const& lhs, custom_record const& rhs)
{
	return 
		lhs.text	== rhs.text			&& 
		lhs.number	== rhs.number		&& 
		eq_container(lhs.texts, rhs.texts);
}

template<class stream>
void serialize(stream& s, custom_record& r)
{
	serialize(s, r.text  );
	serialize(s, r.number);
	serialize(s, r.texts );
}

int main()
{
	// custom struct 
	{
		serialization::output_stream os;

		list<custom_record> v;
		v.push_back(custom_record("1", 1));
		v.push_back(custom_record("2", 2));
		v.push_back(custom_record("3", 3));
		v.push_back(custom_record("4", 4));

		write(os, v); 

		list<custom_record> v2;
		
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}


	// map<string, int>
	{
		serialization::output_stream os;

		std::map<string, int> v;
		v["1"] = 1;
		v["2"] = 2;
		v["3"] = 3;
		v["4"] = 4;

		write(os, v); 

		map<string, int> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	// vector<int>
	{
		serialization::output_stream os;

		std::vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);

		write(os, v); 

		vector<int> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	// vector<sring>
	{
		serialization::output_stream os;

		std::vector<string> v;
		v.push_back("1");
		v.push_back("2");
		v.push_back("3");
		v.push_back("4");

		write(os, v); 

		vector<string> v2;
		serialization::input_stream is (os.data());
		read(is, v2);

		assert(eq_container(v, v2));
	}

	return 0;
}
