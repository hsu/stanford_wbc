### included via subdir() command in top-level CMakeLists.txt

if (HAVE_XMLRPC)
  add_executable (testDirectoryServer testDirectoryServer.cpp TestDirectory.cpp)
  target_link_libraries (testDirectoryServer Stanford_WBC XmlRpc ${MAYBE_GCOV})
else (HAVE_XMLRPC)
  message ("WARNING XmlRpc++ not found, cannot build directory server test")
endif (HAVE_XMLRPC)

add_executable (testProcess testProcess.cpp)
target_link_libraries (testProcess Stanford_WBC ${MAYBE_GCOV})

add_executable (testBehaviorParser testBehaviorParser.cpp)
target_link_libraries (testBehaviorParser Stanford_WBC ${MAYBE_GCOV})

add_executable (testDtorCheck testDtorCheck.cpp)
target_link_libraries (testDtorCheck Stanford_WBC ${MAYBE_GCOV})
