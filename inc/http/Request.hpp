#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../utility/colors.hpp"
# include "../utility/utility.hpp"
# include "../http/status.hpp"
# include "../http/headers.hpp"

/**
 * @note Because we have to support multiple method we need enum
 * because they are easier to work with than strings
 */


/**
 * @note Every member attribute of the request class is stored as pair like that [value, flag]
 * (*) Value is the raw parsed http element, (*) Flag is to know if webserv support the request or not.
 * value can be a string or number and flag is a boolean
 */
typedef std::pair < std::string, bool >						str_flag;
typedef std::pair < int, bool >								i_flag;
/**
 * @note headers is a vector of key-value (raw header and directive).
 *  header and directive are a pair of value-flag as well
 */
typedef std::vector < std::pair < str_flag, str_flag > >	headr_dirctiv;



/**
 * @brief Request class who get the request as raw string and
 * store separately every element of the request in order to process it.
 */
class Request
{

	public:

		Request();
		Request( const Request &src );
		Request &operator=( const Request &rhs );
		~Request();

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
		std::string			getMethodasString();
		std::string			getHost();
		std::string			findHeader(std::string key) const;
		str_flag			getBody(void) const;
		//std::string			getStatus(void) const;

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
				void			checkHeader(str_flag &hdr, str_flag &direct);
			void				setBody(std::string &req);

		bool hasNestedRequestPath; // flag if the request path is nested
	private:

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
};

/**
 * @brief overload to print the fully parsed Request
 */
std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif
