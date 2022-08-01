#include <string>
#include <iostream>
#include "Singleton.hpp"

/**
 * @brief Loglevel of the enums
 * INFO i
 * WARNING W
 */
enum LogLevel { 
	INFO = 'I', 
	WARNING = 'W',
};

/**
 * @brief	Simple debugger printer class.
 * 			Will only print out debug stuff during debug mode.
 * 			SINGLETON CLASS! DO NOT INITIALIZE. Use it like this:
 * 			DebuggerPrinter debugger = debugger.getInstance();
 * 			
 * 
 * @note	LEVEL INFO - info message
 * 			LEVEL WARNING - warning message
 */
class DebuggerPrinter: public Singleton<DebuggerPrinter> {
	public:
		DebuggerPrinter();
		DebuggerPrinter( const DebuggerPrinter &src );
		~DebuggerPrinter();
		DebuggerPrinter & operator = (const DebuggerPrinter &src);
		void info( const std::string &message );
		void warning( const std::string &message );
	private:
		void printString( const std::string &message, LogLevel level );
};