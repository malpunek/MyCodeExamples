
add_executable(interval_tree interval_tree.cc)
target_link_libraries(interval_tree malpunek::libalgo)
target_compile_options(interval_tree PRIVATE -Werror -Wall)
target_compile_features(interval_tree PRIVATE cxx_std_17)

add_executable(set set.cc)
target_link_libraries(set malpunek::libalgo)
target_compile_options(set PRIVATE -Werror -Wall)
target_compile_features(set PRIVATE cxx_std_17)

add_executable(suffix_tree suffix_tree.cc)
target_link_libraries(suffix_tree malpunek::libalgo)
target_compile_options(suffix_tree PRIVATE -Werror -Wall)
target_compile_features(suffix_tree PRIVATE cxx_std_17)
