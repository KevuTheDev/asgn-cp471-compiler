
#pragma warning(disable : 4996)
#include <chrono>
#include <iostream>
#include <string>

int main()
{

    for (auto www = 0; www < 1000000; www++) {

    namespace S = std;
    using namespace S::chrono;
    auto tp = zoned_time{ current_zone(), system_clock::now() }.get_local_time();
    auto dp = floor<days>(tp);
    year_month_day ymd{ dp };
    hh_mm_ss time{ floor<milliseconds>(tp - dp) };
    auto y = ymd.year();
    auto m = ymd.month();
    auto d = ymd.day();
    auto h = time.hours();
    auto M = time.minutes();
    auto s = time.seconds();
    auto ms = time.subseconds();


    //S::cout << "year        = " << ymd.year() << '\n';
    //S::cout << "month       = " << ymd.month() << '\n';
    //S::cout << "day         = " << ymd.day() << '\n';
    //S::cout << "hour        = " << time.hours().count() << "h\n";
    //S::cout << "minute      = " << time.minutes().count() << "min\n";
    //S::cout << "second      = " << time.seconds().count() << "s\n";
    //S::cout << "millisecond = " << time.subseconds().count() << "ms\n";

    std::string hh = S::to_string(h.count());
    hh = (hh.length() != 2) ? "0" + hh : hh;

    std::string MM = S::to_string(M.count());
    MM = (MM.length() != 2) ? "0" + MM : MM;

    std::string ss = S::to_string(s.count());
    ss = (ss.length() != 2) ? "0" + ss : ss;


    S::cout << "[" + hh + ":" \
        + MM + ":" + ss + "]" << S::endl;
    }
}