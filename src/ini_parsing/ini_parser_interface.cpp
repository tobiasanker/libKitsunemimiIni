/**
 *  @file    ini_parser_interface.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <ini_parsing/ini_parser_interface.h>
#include <ini_parser.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

# define YY_DECL \
    Kitsunemimi::Ini::IniParser::symbol_type inilex (Kitsunemimi::Ini::IniParserInterface& driver)
YY_DECL;


namespace Kitsunemimi
{
namespace Ini
{
using Kitsunemimi::splitStringByDelimiter;

bool IniParserInterface::m_outsideComment = true;

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
    m_output = nullptr;

    // run parser-code
    this->scan_begin(inputString);
    Kitsunemimi::Ini::IniParser parser(*this);
    int res = parser.parse();
    this->scan_end();

    if(res != 0) {
        return false;
    }
    return true;
}

/**
 * @brief IniParserInterface::removeQuotes
 * @param input
 * @return
 */
std::string
IniParserInterface::removeQuotes(std::string input)
{
    if(input.length() == 0) {
        return input;
    }

    if(input[0] == '\"' && input[input.length()-1] == '\"')
    {
        std::string result = "";
        for(uint32_t i = 1; i < input.length()-1; i++)
        {
            result += input[i];
        }
        return result;
    }
    return input;
}

/**
 * Is called for the parser after successfully parsing the input-string
 *
 * @param output parser-output as DataArray
 */
void
IniParserInterface::setOutput(DataItem *output)
{
     m_output = output;
}

/**
 * getter for the data-output of the parser
 *
 * @return parser-output as DataArray
 */
DataItem*
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
IniParserInterface::error(const Kitsunemimi::Ini::location& location,
                          const std::string& message)
{
    // get the broken part of the parsed string
    const uint32_t errorStart = location.begin.column;
    const uint32_t errorLength = location.end.column - location.begin.column;
    const uint32_t linenumber = location.begin.line;

    std::vector<std::string> splittedContent;
    splitStringByDelimiter(splittedContent, m_inputString, '\n');

    // -1 because the number starts for user-readability at 1 instead of 0
    const std::string errorStringPart = splittedContent[linenumber - 1].substr(errorStart - 1,
                                                                               errorLength);

    // build error-message
    m_errorMessage =  "ERROR while parsing ini-formated string \n";
    m_errorMessage += "parser-message: " + message + " \n";
    m_errorMessage += "line-number: " + std::to_string(linenumber) + " \n";
    m_errorMessage += "position in line: " + std::to_string(location.begin.column) + " \n";
    m_errorMessage += "broken part in string: \"" + errorStringPart + "\" \n";
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
}  // namespace Kitsunemimi
