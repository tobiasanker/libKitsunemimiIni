/**
 *  @file    ini_parser_interface.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef INIPARSERINTERFACE_H
#define INIPARSERINTERFACE_H

#include <vector>
#include <string>
#include <string>
#include <map>
#include <libKitsunemimiCommon/common_items/data_items.h>

#include <iostream>

using Kitsunemimi::Common::DataItem;
using std::string;
using std::map;
using std::pair;

namespace Kitsunemimi
{
namespace Ini
{
class location;

class IniParserInterface
{

public:
    IniParserInterface(const bool traceParsing);

    // connection the the scanner and parser
    void scan_begin(const std::string &inputString);
    void scan_end();
    bool parse(const std::string &inputString);
    std::string removeQuotes(std::string input);

    // output-handling
    void setOutput(DataItem* output);
    DataItem* getOutput() const;

    // Error handling.
    void error(const Kitsunemimi::Ini::location &location,
               const std::string& message);
    std::string getErrorMessage() const;

    // static variables, which are used in lexer and parser
    static bool m_outsideComment;

private:
    DataItem* m_output;
    std::string m_errorMessage = "";
    std::string m_inputString = "";

    bool m_traceParsing = false;
};

}  // namespace Ini
}  // namespace Kitsunemimi

#endif // INIPARSERINTERFACE_H
