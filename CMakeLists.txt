cmake_minimum_required(VERSION 3.17)
project(geometry)

set(CMAKE_CXX_STANDARD 14)

add_executable(A a.cpp)
add_executable(B b.cpp)
add_executable(C c.cpp)
add_executable(D d.cpp)
add_executable(E e.cpp)
add_executable(F f.cpp)
add_executable(G g.cpp)
add_executable(H h.cpp)
add_executable(I i.cpp)
add_executable(J j.cpp)
add_executable(K k.cpp)
target_link_libraries(A -I/usr/include -L/usr/lib/x86_64-linux-gnu -lgmp)

