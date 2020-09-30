
add_executable(tree tree.cc)
target_link_libraries(tree malpunek::libalgo)
target_compile_options(tree PRIVATE -Werror -Wall)
target_compile_features(tree PRIVATE cxx_std_17)