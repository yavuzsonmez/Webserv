#include "inc/config_file/ConfigFileParsing.hpp"
#include "inc/configuration_key/ConfigurationKey.hpp"
#include "inc/configuration_key/ServerBlock.hpp"
#include "inc/debugger/DebuggerPrinter.hpp"
#include "inc/http/Response.hpp"
#include "inc/network/ServerSocket.hpp"
#include "inc/utility/utility.hpp"


/**
 * @brief Checks the argument count of the program and the filename of the configuration
 * Returns true on success.
 */
bool check_arguments_and_filename(int argc, char**argv)
{
	if (argc != 2 || !check_config_file(argv[1]))
	{
		std::cout << "usage: ./webserv [path/webserv.conf]" << std::endl;
		return (false);
	}
	return (true);
}

/**
 * @brief Entrypoint
 * Validates input and then starts the configuration file parsing.
 */
int main(int argc, char **argv)
{
	USE_DEBUGGER;
	if (!check_arguments_and_filename(argc, argv)) return (1);

	ConfigFileParsing *configurationFileParsing = new ConfigFileParsing();
	std::string file_content;
	if (argc == 1)
		file_content = get_file_content("./conf/webserv.conf");
	else
		file_content = get_file_content(argv[1]);

	try {
		configurationFileParsing->parseConfigFile(file_content);
		debugger.info("CONFIG FILE OK");
	} catch (const std::exception& e) {
		std::cout << R << "Something went wrong: " << Reset << e.what() << std::endl;
		return (1);
	}

	try {
		ServerSocket server(configurationFileParsing->serverBlocks[0], *configurationFileParsing, INADDR_ANY);
	} catch (int e) {
		// print exception information
		std::cout << "Something went wrong with error code " << e << std::endl;
	}

	delete configurationFileParsing;


	std::cout << "Program exited cleanly." << std::endl;

	return (0);
}