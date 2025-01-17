#ifndef CONFIGURATIONKEY
# define CONFIGURATIONKEY

#include <string>
#include <vector>
#include "../../inc/config_file/InvalidConfigurationFile.hpp"

enum method {  GET, POST, DELETE, PUT, UNKNOWN };

/**
 * All keys which can be used in the configuration file are defined here.
 * To add a new key, add it to the KEY_DEFINES and to the enum.
 * Add parsing behavior to determineConfigurationKeys in ConfigFileParsing.cpp
 * Then add adding behavior to addConfigurationKeyToLocation
 */

# define	KEY_LOCATION				"location"
# define	KEY_INDEX					"index"
# define	KEY_SERVER_START_SEGMENT	"server"
# define	KEY_ROOT					"root"
# define	KEY_SERVER_NAMES			"server_name"
# define	KEY_LISTEN					"listen"
# define	KEY_INVALID					"INVALID"
# define	KEY_METHODS					"methods"
# define	KEY_EXECUTABLE_PATH			"cgi_path"
# define	KEY_FILEENDING				"cgi_fileending"
# define	KEY_NOT_FOUND_PAGE			"not_found_error_page"
# define	KEY_NOT_AVAILABLE_PAGE		"not_available_page"
# define	KEY_GENERAL_ERROR_PAGE		"general_error_page"
# define	KEY_POST_MAX_SIZE			"post_max_size"
# define	KEY_REDIRECTION				"redirection"
# define	KEY_DIRECTORY_LISTING		"directory_listing"

/**
 * Defines the type of information a configuration key holds.
 * Lists all available configuration types.
 * Can be extended easily.
 * 
 * SERVERSTARTSEGMENT indicates that a new server block starts. This value will not actually be used in ConfigurationKey and
 * is just an indicator for the parser.
 */
enum ConfigurationKeyType {
	INDEX,
	SERVER_NAME,
	LISTEN,
	LOCATION,
	ROOT,
	INVALID,
	SERVERSTARTSEGMENT,
	METHODS,
	CGI_EXECUTABLE_PATH,
	CGI_FILEENDING,
	POST_MAX_SIZE,
	NOT_FOUND_ERROR_PAGE,
	GENERAL_ERROR_PAGE,
	REDIRECTION,
	DIRECTORY_LISTING,
	NOT_AVAILABLE_PAGE
};

/**
 * The internal keypair used within the class to transport key and value. This is being isolated to avoid confusion between attribute key/value and
 * internal handling of key and value
 */
typedef std::pair <std::string, std::string>	internal_keyvalue;

/**
 * Usage: ConfigurationKey(key, value);
 *
 * @note The configuration key represents a single entry (or a body containing values) in the configuration file.
 *       It is to be used in an array, and parses the given string-value automatically
 *       to the correct type in the constructor and sets it own configurationType.
 *       It can be nested, but only certain nestings are valid. (root in location block as example)
 *
 * Currently, it supports following types:
 *  - server_name
 *  - port
 *
 * To use it, split the key/value from the config_file from the first space and pass the first value as key and the rest as value.
 * Then it parses the value and by combining key and value it decides which configurationType it is.
 * You can find the available configuration types in the ConfigurationKey.hpp in the enums.
 *
 * If there is no type which fits the value/key, it throws an exception and the program stops.
 *
 * @note Internal handling in ConfigurationKey class is done with internal_keyvalue Pair typedef for readabiliy.
 *
 * Later the ConfigurationKey will be stored as a ServerBlock, which itself is stored in a vector of server blocks.
 *
 */
class ConfigurationKey {
	public:
		ConfigurationKey();
		ConfigurationKey( const ConfigurationKey &src );
		~ConfigurationKey();
		ConfigurationKey & operator = (const ConfigurationKey &src);
		ConfigurationKey(std::string key, std::string value, bool location_block, int current_line, std::string raw_input);
		
		void setLocationBlockParsing(bool value);

		// General Attributes

		std::string key;
		std::string value;
		std::string raw_input;

		// Configuration type
		ConfigurationKeyType configurationType;

		// Attributes of the configuration types
		std::vector <std::string> indexes; // indexes, if type is INDEX. sorted by relevance and position within the key.
		std::vector <std::string> server_names; // server_names, if type is SERVER_NAMES. sorted by relevance and position within the key.
		std::vector <std::string> methods; // methods, if type is METHODS. sorted by relevance and position within the key.
		std::vector <method> allowedMethods; // allowedMethods, contains all the enums of the allowed methds
		std::string root; // returns the path of the root
		std::string location; // returns the locationpath of the location
		std::string cgi_path; // returns the locationpath of the location
		std::string cgi_fileending; // those file endings should be executed with a cgi
		std::string redirection; // a redirection which is set in the configuration file for a certain location
		std::string not_found_error_page_path; // returns the location of the error path to the error file
		std::string general_error_page_path; // returns the location of the error path to the error file
		std::string not_available_page_path; // returns the location of the error path to the error file
		bool directory_listing; // flag if directory listing is enabled or not
		int post_max_size; // post max size in megabyte
		std::vector <unsigned int> ports; // returns the ports which are being listened to by the listener handler
		std::vector<ConfigurationKeyType> nestedConfigurationKeyTypesinLocationBlock; // describes the properties within the location block
	private:
		// Those are the internal functions which are used to parse the value to the correct type.
		ConfigurationKeyType detectConfigurationType(internal_keyvalue &raw);
		ConfigurationKeyType detectLocationKeyConfiguration(internal_keyvalue &raw);
		bool isServerNameKeyType(internal_keyvalue raw);
		bool isListenKeyType(internal_keyvalue raw);
		bool isServerStartSegment(internal_keyvalue raw);
		bool isIndexKeyType(internal_keyvalue raw);
		bool isRootKeyType(internal_keyvalue raw);
		bool isLocationKeyType(internal_keyvalue &raw);
		bool isMethodsKeyType(internal_keyvalue raw);
		bool isCgiExecutableKeyType(internal_keyvalue raw);
		bool isCgiFileEndingKeyType(internal_keyvalue raw);
		bool isNotFoundErrorPagePathType(internal_keyvalue raw);
		bool isPostMaxSizeType(internal_keyvalue raw);
		bool isGeneralErrorPagePathType(internal_keyvalue raw);
		bool isNotAvailableErrorPagePathType(internal_keyvalue raw);
		bool isRedirectionKeyType(internal_keyvalue raw);
		bool isValidMethod(std::string method);
		bool validatePort(unsigned int port);
		bool is_digits(const std::string &str);
		void throwInvalidConfigurationFileExceptionWithMessage(std::string message);
		bool validateCgiFileEnding(std::string to_validate);
		bool validatePostMaxSize(std::string to_validate);
		bool validateRedirection(std::string value);
		void addMethodToMethodEnum(std::string methodToAdd);
		bool isDirectoryListingConfigurationKeyType(internal_keyvalue raw);
		
		
		/**
		 * Indicates if we are currently parsing keys within a location block or not.
		 */
		bool isCurrentlyParsingLocationBlock;
		int current_line;
};

#endif