add_library(lock_table src/list.c src/table.c include/list.h include/table.h include/errors.h include/transaction.h src/transaction.c src/ioutil.c include/ioutil.h)
target_include_directories(lock_table PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_directories(lock_table PUBLIC /usr/local/lib /Users/anishsinha/Home/lib/primesieve/build)
target_link_libraries(lock_table PUBLIC primesieve)