add_library(internal src/list.c src/table.c include/list.h include/table.h include/errors.h include/transaction.h src/transaction.c src/ioutil.c include/ioutil.h include/lehman-yao.h include/slice.h src/slice.c src/lehman-yao.c)
target_include_directories(internal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_directories(internal PUBLIC /usr/local/lib /Users/anishsinha/Home/lib/primesieve/build)
target_link_libraries(internal PUBLIC primesieve)