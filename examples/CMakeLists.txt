
add_executable(tree tree.cc)
target_link_libraries(tree malpunek::libalgo)
target_compile_options(tree PRIVATE -Werror -Wall)
target_compile_features(tree PRIVATE cxx_std_17)

add_executable(set set.cc)
target_link_libraries(set malpunek::libalgo)
target_compile_options(set PRIVATE -Werror -Wall)
target_compile_features(set PRIVATE cxx_std_17)
