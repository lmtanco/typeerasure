cmake_minimum_required(VERSION 3.5) # setting this is required
project(typeerasure)
add_executable(tyerase src/typeerasure.cpp)
target_compile_features(tyerase PUBLIC cxx_std_23)



