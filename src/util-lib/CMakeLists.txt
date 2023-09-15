add_library(util-lib STATIC
        datetime.cpp
        datetime.hpp
        misc_utils.cpp
        misc_utils.hpp
        timer.cpp
        timer.hpp
        )
target_include_directories(util-lib PUBLIC .)
