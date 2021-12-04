#include <map>
#include <string>
#include <tabulate/table.hpp>
#include <vector>
using namespace tabulate;

void tableInit();
void tableFormat();
void runTimeOutPut(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj);
void finOutPut(std::vector<std::vector<std::string>> modObj,
               std::vector<std::vector<int>>&        commCostMatrix,
               std::map<std::string, int>&           tasksToIndex);
struct outPutFormat
{
    std::string thold;
    std::string moduleNum;
    std::string everyModule;
};

Table table;

outPutFormat toOutPutFormat(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj)
{
    outPutFormat newFormat   = outPutFormat{std::to_string(thold), std::to_string(moduleNum)};
    std::string  everyModule = "{";
    for (auto it = modObj.begin(); it != modObj.end(); it++)
    {
        everyModule += '{';
        for (auto bit = 0; bit < it->size() - 1; bit++)
        {
            everyModule += (*it)[bit];
            everyModule += ',';
        }
        everyModule += (*it)[it->size() - 1];
        if (*it != modObj.back())
            everyModule.append("}  ");
        else
            everyModule.append("}");
    }
    everyModule += '}';
    newFormat.everyModule = everyModule;
    return newFormat;
}
void tableInit()
{
    table.add_row({"threshold", "moduleNum", "everyModule"});
}
void tableFormat()
{
    // Set width of cells in each column
    table.column(0).format().width(15);
    table.column(1).format().width(15);
    table.column(2).format().width(100);

    // Iterate over cells in the first row
    for (auto& cell : table[0])
    {
        cell.format().font_align(FontAlign::center).font_color(Color::blue);
    }

    // Iterator over cells in the first column
    for (auto& cell : table.column(0))
    {
        if (cell.get_text() != "Company")
        {
            cell.format().font_align(FontAlign::center);
        }
    }
    for (auto& cell : table.column(1))
    {
        if (cell.get_text() != "Company")
        {
            cell.format().font_align(FontAlign::center);
        }
    }
    for (auto& cell : table.column(2))
    {
        if (cell.get_text() != "Company")
        {
            cell.format().font_align(FontAlign::center);
        }
    }
    // Iterate over rows in the table
    size_t index = 0;
    for (auto& row : table)
    {
        row.format().font_style({FontStyle::bold});

        // Set blue background color for alternate rows
        if (index > 0)
        {
            for (auto& cell : row)
            {
                cell.format().font_color(Color::grey);
            }
        }
        index += 1;
    }

    std::cout << table << std::endl;
}

void runTimeOutPut(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj)
{
    outPutFormat outPutForm = toOutPutFormat(thold, moduleNum, modObj);
    table.add_row({outPutForm.thold, outPutForm.moduleNum, outPutForm.everyModule});
}

void finOutPut(std::vector<std::vector<std::string>> modObj,
               std::vector<std::vector<int>>&        commCostMatrix,
               std::map<std::string, int>&           tasksToIndex)
{
    int totalCost = 0;
    for (auto ait = modObj.begin(); ait != modObj.end(); ait++)
    {
        for (auto bit = ait + 1; bit != modObj.end(); bit++)
        {
            for (auto mit = ait->begin(); mit != ait->end(); mit++)
            {
                for (auto nit = bit->begin(); nit != bit->end(); nit++)
                {
                    // std::cout << *mit << " " << *nit << std::endl;
                    // std::cout << "cur cost is " << commCostMatrix[tasksToIndex[*mit]][tasksToIndex[*nit]]
                    //           << "and total is " << totalCost << std::endl;
                    totalCost += commCostMatrix[tasksToIndex[*mit]][tasksToIndex[*nit]];
                }
            }
        }
    }
    std::cout << "TotalCost is " << totalCost/10 << std::endl;
}