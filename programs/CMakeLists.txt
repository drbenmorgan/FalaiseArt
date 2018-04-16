# Two variants - text and pba, these determine the TBuffer
# format. Would roughly expecttext to be more portable
add_executable(brio_portability_test brio_portability_test.cc)
target_link_libraries(brio_portability_test PRIVATE Bayeux::Bayeux)

add_executable(brio_portability_test_xml brio_portability_test.cc)
target_compile_definitions(brio_portability_test_xml PRIVATE BRIO_TEXT_FORMAT)
target_link_libraries(brio_portability_test_xml PRIVATE Bayeux::Bayeux)


add_executable(brio_dump brio_dump.cc)
target_link_libraries(brio_dump PRIVATE Bayeux::Bayeux)

add_executable(test_xml_reader test_xml_reader.cc)
target_link_libraries(test_xml_reader PRIVATE Bayeux::Bayeux)
