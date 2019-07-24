/**
 *  @file    iniParserInterface.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include <ini_parsing/iniParserInterface.h>
#include <iniparser.h>

# define YY_DECL \
    Kitsune::Ini::IniParser::symbol_type inilex (Kitsune::Ini::IniParserInterface& driver)
YY_DECL;

namespace Kitsune
{
namespace Ini
{

/**
 * The class is the interface for the bison-generated parser.
 * It starts the parsing-process and store the returned values.
 *
 * @param traceParsing If set to true, the scanner prints all triggered rules.
 *                     It is only for better debugging.
 */
IniParserInterface::IniParserInterface(const bool traceParsing)
{
    m_traceParsing = traceParsing;
}

/**
 * Start the scanner and parser
 *
 * @param inputFile string which should be parsed
 * @return true, if parsing was successful, else false
 */
bool
IniParserInterface::parse(const std::string &inputString)
{
    // init global values
    m_inputString = inputString;
    m_errorMessage = "";
    m_output = map<string, map<string, JsonItem *>>();

    // run parser-code
    this->scan_begin(inputString);
    Kitsune::Ini::IniParser parser(*this);
    int res = parser.parse();
    this->scan_end();

    if(res != 0) {
        return false;
    }
    return true;
}

/**
 * Is called for the parser after successfully parsing the input-string
 *
 * @param output parser-output as Json::JsonArray
 */
void
IniParserInterface::setOutput(map<string, map<string, JsonItem *>> output)
{
     m_output = output;
}

/**
 * getter for the json-output of the parser
 *
 * @return parser-output as Json::JsonArray
 */
map<std::string, map<std::string, JsonItem *>>
IniParserInterface::getOutput() const
{
    return m_output;
}

/**
 * Is called from the parser in case of an error
 *
 * @param location location-object of the bison-parser,
 *                 which contains the informations of the location
 *                 of the syntax-error in the parsed string
 * @param message error-specific message from the parser
 */
void
IniParserInterface::error(const Kitsune::Ini::location& location,
                             const std::string& message)
{
    // get the broken part of the parsed string
    const uint32_t errorStart = location.begin.column;
    const uint32_t errorLength = location.end.column - location.begin.column;
    const std::string errorStringPart = m_inputString.substr(errorStart, errorLength);

    // build error-message
    m_errorMessage =  "error while parsing ini-template \n";
    m_errorMessage += "parser-message: " + message + " \n";
    m_errorMessage += "line-number: " + std::to_string(location.begin.line) + " \n";
    m_errorMessage += "position in line: " + std::to_string(location.begin.column) + " \n";
    m_errorMessage += "broken part in template: \"" + errorStringPart + "\" \n";
}

/**
 * getter fot the error-message in case of an error while parsing
 *
 * @return error-message
 */
std::string
IniParserInterface::getErrorMessage() const
{
    return m_errorMessage;
}

}  // namespace Ini
}  // namespace Kitsune