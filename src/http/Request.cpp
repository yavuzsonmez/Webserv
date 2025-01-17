
#include "../../inc/http/Request.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"


/**
 * @brief Default constructor, default values for testing
 */
Request::Request() :
	hasNestedRequestPath(false),
	_method(UNKNOWN, false),
	_protocol("http", true),
	_domain("", true),
	_port(80, true),
	_script("", false),
	_path("/", true),
	_query("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false)
	{}

/**
 * @brief Copy constructor
 */
Request::Request( const Request &src ) :
	hasNestedRequestPath(src.hasNestedRequestPath),
	_method(src._method),
	_protocol(src._protocol),
	_port(src._port),
	_script(src._script),
	_path(src._path),
	_query(src._query),
	_fragment(src._fragment),
	_httpVersion(src._httpVersion),
	_headers(src._headers),
	_body(src._body)  {}

Request & Request::operator = (const Request &rhs)
{
	_method = rhs._method;
	_protocol = rhs._protocol;
	_port = rhs._port;
	_script = rhs._script;
	_path = rhs._path;
	_query = rhs._query;
	_fragment = rhs._fragment;
	_httpVersion = rhs._httpVersion;
	_headers = rhs._headers;
	_body = rhs._body;
	hasNestedRequestPath = rhs.hasNestedRequestPath;
	return *this;
}

Request::~Request() {}

/**
 * @brief Getters functions, read only
 */
i_flag				Request::getMethod() const		{ return _method; }
str_flag			Request::getProtocol() const	{ return _protocol; }
str_flag			Request::getDomain() const		{ return _domain; }
i_flag				Request::getPort() const		{ return _port; }
str_flag			Request::getScript() const		{ return _script; }
str_flag			Request::getPath() const		{ return _path; }

/**
 * @brief Returns the query string of the request
 */
str_flag			Request::getQuery() const { return _query; }

str_flag			Request::getFragment() const	{ return _fragment; }
str_flag			Request::getHttpversion() const	{ return _httpVersion; }
headr_dirctiv		Request::getHeaders() const		{ return _headers; }
str_flag			Request::getBody() const		{ 
	return _body;
}

/**
 * @brief Main parsing function, each subfunction will
 * parse and store a specific element of the request and erase it
 * in the request string.
 */
void Request::parser(std::string &req) {
	USE_DEBUGGER;
	if (req.empty())
		throw (Bad_Request);
	try {
		setMethod(req);
		setUrl(req);
		setHttpversion(req);
		setHeaders(req);
		setBody(req);
	}
	catch (std::string status) {
		debugger.verbose(status);
		throw (status);
	}
}

/**
 * @brief Check the method in the request, and verify if supported
 */
void Request::setMethod(std::string &req) {
	std::string tmp;
	size_t pos = req.find(" ");
	if (pos == std::string::npos)
		throw(Bad_Request);
	tmp = req.substr(0, pos);
	if (!tmp.compare("GET"))			{ _method.first = GET; _method.second = true; }
	else if (!tmp.compare("POST"))		{ _method.first = POST; _method.second = true; }
	else if (!tmp.compare("DELETE"))	{ _method.first = DELETE; _method.second = true; }
	else { _method.first = UNKNOWN; _method.second = false; throw(Method_Not_Allowed); }
	req.erase(0, pos + 1);
}

/**
 * @brief Returns the method as string
 * This is important for headers in the environment.
 */
std::string Request::getMethodasString()
{
	if (_method.first == GET) return "GET";
	else if (_method.first == POST)		return "POST";
	else if (_method.first == DELETE)	return "DELETE";
	else if (_method.first == UNKNOWN)	return"UNKNOWN";
	return ("");
}

// TODO check std::string::npos before substr qnd erase
// TODO domain is case insensitive

/**
 * @brief Separate the different element in the URL of the request
 */
void Request::setUrl(std::string &req) {
	size_t pos = req.find(" ");
	if (pos == std::string::npos)
		throw(Bad_Request);
	std::string url = req.substr(0, pos);
	req.erase(0, pos + 1);
	removeDoubleSlashesInUrl(url);
	// if path does not end with file ending, add a slash if there is not a path already
	if (url.find('.') == std::string::npos && url.find('/') == std::string::npos)
		url += "/";
	setProtocol(url);
	setDomain(url);
	setScript(url);
	setPath(url);
}

/**
 * @brief Check if the protocol is described in the request
 * if yes, check if we support it (only http)
 */
void Request::setProtocol(std::string &url) {

	size_t pos = url.find("://");
	if (pos == std::string::npos)
		return ;
	_protocol.first = url.substr(0, pos);
	if (_protocol.first.compare("http"))
		_protocol.second = false;
	url.erase(0, pos + 3);
}

/**
 * @brief Store the domain name of the request
 */
void Request::setDomain(std::string &url) {
	setPort(url);
	size_t pos = url.find("/");
	if (pos == std::string::npos)
	{
		_domain.first.clear();
		_domain.second = false;
		throw(Bad_Request);
	}
	_domain.first = url.substr(0, pos);
	url.erase(0, pos + 1);
}

/**
 * @brief Check if a port is provided in the URL of the request,
 */
void Request::setPort(std::string &url) {
	USE_DEBUGGER;
	removeDoubleSlashesInUrl(url);
	debugger.debug(url);
	size_t x = url.find(":");
	size_t y = url.find("/");
	if (x == std::string::npos)
		return;
	if (y == std::string::npos)
		y = url.length();
	std::string port = url.substr(x + 1, y - x - 1);
	std::stringstream ss(port);
	ss >> _port.first;
	if (_port.first < 0)
	{
		_port.second = false;
		throw(Bad_Request);
	}
	url.erase(x, y - x );
	debugger.debug(url);
}

// /**
//  * @brief check if client requests a CGI
//  */
// void Request::setScript(std::string &url) {
// 	size_t pos = url.find_last_of("/");
// 	if (pos == std::string::npos)
// 		return ;
// 	_script.first = url.substr(0, pos);
// 	std::cout << "_script: " << _script.first << std::endl;
// 	if (_script.first.substr(0, _script.first.find("/")).compare("cgi")) // check if cgi the client request the CGI in the cgi/ folder
// 		_script.second = false;
// 	else
// 		_script.second = true;
// 	url.erase(0, pos + 1);
// 	// TODO check if URL containes something after script like /cgi/index.php/ or /cgi/index.php
// 	// fix research based on last / for script
// }

/**
 * @brief check if client requests a CGI
 */
void Request::setScript(std::string &url) {
	size_t pos = url.find_last_of(".");
	if (pos == std::string::npos)
		return ;
	pos = url.find_last_of("/");
	if (pos == std::string::npos)
		_script.first = url.substr(0);
	else
		_script.first = url.substr(pos);
	if (_script.first.substr(0, _script.first.find("/")).compare("cgi")) // check if cgi the client request the CGI in the cgi/ folder
		_script.second = false;
	else
		_script.second = true;
	setFragment(url);
	setQuery(url);
	if (pos == std::string::npos)
		url.erase(0);
	else
		url.erase(pos);
	// TODO check if URL containes something after script like /cgi/index.php/ or /cgi/index.php
}


/**
 * @brief sets the path.
 * Also modifies the string heavily. We do this to avoid that a missing slash in the url is interpreted incorrectly.
 * that is why we add a new slash if no slash is available to match the locations in the config file (which will ALWAYS end with a slash)
 * But we do not add a slash if there are query paramters in the url (the question mark) because that we complicate things. Also we do not
 * add a slash if we find a dot. Means, directories with dots may be handled not 100% correctly.
 */
void Request::setPath(std::string &url)
{
	// remove everything after double slashes
	removeDoubleSlashesInUrl(url);

	// Count amount of slashes in the url
	std::string nestedPath =  url + getScript().first;
	size_t pos = nestedPath.find("/");
	size_t count = 0;
	while (pos != std::string::npos)
	{
		count++;
		pos = nestedPath.find("/", pos + 1);
	}
	if (count > 1)
	{
		hasNestedRequestPath = true;
	}

	// if the url does not end with an fileending and the url does not end with a slash, add a slash. Also if it contains a question mark do not modify the string
	if (url.find('.') == std::string::npos && url.find('/') == std::string::npos && url.find('?') == std::string::npos)
		url += "/";
	if (url.length())
		_path.first = url.substr(0, url.length());
	url.erase(0, url.length());
}

// /**
//  * @brief
//  */
// void Request::setPath(std::string &url)
// {
// 	setFragment(url);
// 	setQuery(url);

// 	if (url.length())
// 		_path.first = url.substr(0, url.length());
// 	url.erase(0, url.length());
// }


/**
 * @brief
 */
void Request::setQuery(std::string &url) {
	size_t pos = url.find("?");
	if (pos != std::string::npos)
	{
		_query.first = url.substr(pos + 1, url.length());
		url.erase(pos, url.length());
		_query.second = true;
	}
}

/**
 * @brief
 */
void Request::setFragment(std::string &url) {
	size_t pos = url.find("#");
	if (pos != std::string::npos)
	{
		_fragment.first = url.substr(pos + 1, url.length());
		url.erase(pos, url.length());
		_fragment.second = true;
	}
}

/**
 * @brief Check the HTTP version used in the request. Webserv support only HTTP/1.1
 */
void Request::setHttpversion(std::string &req)
{
	size_t pos = req.find("\n");

	_httpVersion.first = req.substr(0, pos);
	if(!_httpVersion.first.compare("HTTP/1.1"))
		_httpVersion.second = true;
	else
		_httpVersion.second = false;
	req.erase(0, pos + 1);
}

// TODO support presence of ':' inside body
/**
 * @brief Store every single header in a vector of pair
 * vector -> <header: directive>
 * 			header -> <header, flag>
 *			directive -> <directive, flag>
 */
// void Request::setHeaders(std::string &req)
// {
// 	str_flag hdr, direct;
// 	size_t pos = req.find(":");
// 	while (pos != std::string::npos)
// 	{
// 		hdr = std::make_pair(req.substr(0, pos), true);
// 		req.erase(0, pos + 1);
// 		pos = req.find("\n");
// 		direct = std::make_pair(req.substr(0, pos), true);
// 		_headers.push_back(std::make_pair(hdr, direct));
// 		req.erase(0, pos + 1);
// 		pos = req.find(":");
// 	}
// }

/**
 * @brief Get the host value from the headers
 * 
 */
std::string Request::getHost() {
	std::string host = findHeader("Host");
	return host;
}

/*
 * @brief Store every single header in a vector of pair
 * vector -> <header: directive>
 * 			header -> <header, flag>
 *			directive -> <directive, flag>
 */
void Request::setHeaders(std::string &req)
{
	// std::cout << "req_header: " << req << std::endl;
	str_flag hdr, direct;
	size_t end = req.find("\r\n\r\n");
	// std::cout << "end: " << end << std::endl;
	size_t pos = req.find(":");
	while (pos != std::string::npos && pos < end)
	{
		hdr = std::make_pair(req.substr(0, pos), true);
		req.erase(0, pos + 1);
		pos = req.find("\n");
		direct = std::make_pair(req.substr(0, pos), true);
		checkHeader(hdr, direct);
		_headers.push_back(std::make_pair(hdr, direct));
		req.erase(0, pos + 1);
		pos = req.find(":");
		end = req.find("\r\n");
	}
}

/**
 * @brief Check if the header is valid
 * 
 * @param hdr 
 * @param direct 
 */
void Request::checkHeader(str_flag &hdr, str_flag &direct)
{
	if (hdr.first == Host)
	{
		std::string host_header = direct.first;
		removeDoubleSlashesInUrl(host_header);
		setPort(host_header);
	}
	if (hdr.first == Content_Length)
	{
		if (_method.first != DELETE && _method.first != POST)
		{
			_method.second = false;
			return ;
		}
		//check max content length from the parsed config file
	}
	//if (hdr.first == Content_Type)
}

/**
 * @brief Sets the body of the request. Will throw an Forbidden for a GET request when a body is present.
 * @param req 
 */
void Request::setBody(std::string &req)
{
	size_t pos = req.find("\n");
	if (pos != std::string::npos && _method.first == GET) // you cannot provide a body when using GET!
	{
		_method.second = false;
		throw (Forbidden);
	}
	if (pos != std::string::npos)
	{
		_body.first = req.substr(pos + 1, req.length());
		_body.second = true;
	} else 
		_body.second = false;
	//if (req.length())
	//	req.erase(pos);
}

/**
 * @brief returns the value of the in header defined header
 */
std::string	Request::findHeader(std::string key) const
{
	std::string null;
	headr_dirctiv::const_iterator	it;
	for (it = _headers.begin(); it != _headers.end(); it++)
	{
		if ( lower_str_ret((*it).first.first) == lower_str_ret(key) )
			return (*it).second.first;
	}
	return null;
}

/**
 * @brief Overload that print all information of the parsed request
 *	for debugging
 */
std::ostream &			operator<<( std::ostream & o, Request const & i )
{

	i_flag method = i.getMethod();
	std::string method_str;
	if (method.first == GET)
		method_str = "GET";
	else if (method.first == POST)
		method_str = "POST";
	else if (method.first == DELETE)
		method_str = "DELETE";
	else
		method_str = "UKNOWN";
	str_flag		protocol = i.getProtocol();
	str_flag		domain = i.getDomain();
	i_flag			port = i.getPort();
	str_flag		script = i.getScript();
	str_flag		path = i.getPath();
	str_flag		query = i.getQuery();
	str_flag		fragment = i.getFragment();
	str_flag		httpVersion = i.getHttpversion();
	headr_dirctiv	headers = i.getHeaders();
	headr_dirctiv::const_iterator it = headers.begin();
	headr_dirctiv::const_iterator ite = headers.end();
	str_flag	body = i.getBody();

	o << P << "/* ************************************************************************** */" << std::endl << "/* "
	<< Y << "ELEMENT			SUPPORTED? ( 1 yes / 0 no)										"	<< std::endl
	<< P << "/* ************************************************************************** */" << std::endl << "/* "
	<< R << "method:\t" << B << method_str << "\t\t\t\t" << Reset << method.second << std::endl
	<< P << "/* ************************************************************************** */"  << std::endl << "/* "
	<< R << "protocol:\t" << B << protocol.first << "\t\t\t\t" << Reset << protocol.second << std::endl << P << "/* "
	<< R << "domain:\t" << B << domain.first << "\t\t" << Reset << domain.second << std::endl << P << "/* "
	<< R << "port:\t" << B << port.first << "\t\t\t\t" << Reset << port.second << std::endl << P << "/* "
	<< R << "script:\t" << B << script.first << Reset << "\t\t\t\t" << script.second << std::endl << P << "/* "
	<< R << "path:\t" << B << path.first << "\t\t\t\t" << Reset << path.second << std::endl << P << "/* "
	<< R << "query:\t" << B << query.first << Reset << "\t\t\t\t" << query.second << std::endl << P << "/* "
	<< R << "fragment:\t" << B << fragment.first << "\t\t\t\t" << Reset << fragment.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "http version:\t" << B << httpVersion.first << Reset << "\t\t" << httpVersion.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "headers:" << std::endl;
	for (; it != ite; ++it)
	{
		o << B << (*it).first.first << Reset << " " << (*it).first.second << " : "
		<< B << (*it).second.first << Reset << " " << (*it).second.second << std::endl;
	}
	o << P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "body:\t" << B << body.first << Reset << "\t\t\t\t" << body.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl;
	//<< G << i.getStatus() << Reset << std::endl;
	return o;
}