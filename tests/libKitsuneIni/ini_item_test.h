/**
 *  @file    ini_item_test.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef PARSERTEST_H
#define PARSERTEST_H

#include <algorithm>
#include <testing/unit_test.h>

namespace Kitsune
{
namespace Ini
{
class IniItem_Test
        : public Kitsune::Common::UnitTest
{
public:
    IniItem_Test();

private:
    void parse_test();
    void get_test();
    void set_test();
    void removeGroup_test();
    void removeEntry_test();
    void print_test();

    std::string getTestString();
};

}  // namespace Ini
}  // namespace Kitsune

#endif // PARSERTEST_H