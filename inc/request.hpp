#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include <utility>
# include "colors.hpp"
# include "status.hpp"

// The only member attribute which requires a method coz with support multiple method
enum method { GET, POST, DELETE, UNKNOWN };

// Every member attribute of the request class is stored as pair like that [value, flag]
// (*) Value is the raw parsed http element, (*) Flag is to know if webserv support the request or not.
// (true, 1, supported); (false, 0, not supported)
// value can be a string or number and flag is a boolean
typedef std::pair < std::string, bool >						str_flag;
typedef std::pair < int, bool >								i_flag;
// headers is a vector of key-value (raw header and directive).
// header and directive are a pair of value-flag as well
typedef std::vector < std::pair < str_flag, str_flag > >	headr_dirctiv;


class Request
{

	public:

		Request();
		Request( std::string &req ); // prefered constructor where the raw request is passed
		Request( const Request &src );
		~Request();

		//getters
		i_flag				getMethod(void) const;
		str_flag			getProtocol(void) const;
		str_flag			getDomain(void) const;
		i_flag				getPort(void) const;
		str_flag			getScript() const;
		str_flag			getPath(void) const;
		str_flag			getQuery(void) const;
		str_flag			getFragment(void) const;
		str_flag			getHttpversion(void) const;
		headr_dirctiv		getHeaders(void) const;
		str_flag			getBody(void) const;
		std::string			getStatus(void) const;

		// parsing
		void				parser(std::string &req);
			void				setMethod(std::string &req);
			void				setUrl(std::string &req);
				void				setProtocol(std::string &url);
				void				setDomain(std::string &url);
				void				setPort(std::string &url);
				void				setScript(std::string &url);
				void				setPath(std::string &url);
				void				setQuery(std::string &url);
				void				setFragment(std::string &url);
			void				setHttpversion(std::string &req);
			void				setHeaders(std::string &req);
			void				setBody(std::string &req);

	private:

		// <value, flag>
		// value = raw value
		// flag = check if supported - compatible or not 1 yes, 0 no
		i_flag				_method;
		str_flag			_protocol;
		str_flag			_domain;
		i_flag				_port;
		str_flag			_script;
		str_flag			_path;
		str_flag			_query;
		str_flag			_fragment;
		str_flag			_httpVersion;
		headr_dirctiv		_headers;
		str_flag			_body;

		std::string			_status;
};

// overload to print the request
std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */