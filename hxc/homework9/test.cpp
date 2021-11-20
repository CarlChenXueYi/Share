#include <iostream>
#include <tabulate/table.hpp>

using namespace tabulate;

int main()
{
    Table table;

    table.format().corner("♥").font_style({FontStyle::bold}).corner_color(Color::green).border_color(Color::green);

    table.add_row({"新宇爱", "学毅！！"});
    table.add_row({"French", "Je t’aime"});
    table.add_row({"Spanish", "Te amo"});
    table.add_row({"German", "Ich liebe Dich"});
    table.add_row({"Mandarin Chinese", "我爱你"});
    table.add_row({"Japanese", "愛してる"});
    table.add_row({"Korean", "사랑해 (Saranghae)"});
    table.add_row({"Greek", "Σ΄αγαπώ (Se agapo)"});
    table.add_row({"Italian", "Ti amo"});
    table.add_row({"Russian", "Я тебя люблю (Ya tebya liubliu)"});
    table.add_row({"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});

    // Column 1 is using mult-byte characters
    table.column(1).format().multi_byte_characters(true);

    std::cout << table << std::endl;
}