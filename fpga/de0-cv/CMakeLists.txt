include(Quartus)

get_property(ALU_SOURCES GLOBAL PROPERTY G_ALU_SOURCES)
get_property(CORE_SOURCES GLOBAL PROPERTY G_CORE_SOURCES)

configure_file(Top.qsf Top.qsf)

set(FPGA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Top.sv
    ${CMAKE_CURRENT_SOURCE_DIR}/SysPLL.v
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/VirtualJTAG.sv
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/JTAGBridge.sv
    ${CMAKE_CURRENT_SOURCE_DIR}/../../rtl/cdc/BitSync.sv
    ${CMAKE_CURRENT_SOURCE_DIR}/../../rtl/cdc/SyncPulse.sv
    ${CMAKE_CURRENT_SOURCE_DIR}/../../rtl/cdc/MCP.sv)

set(ALL_SOURCES ${CORE_SOURCES} ${FPGA_SOURCES})

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/../../rtl/microcode/Microcode.v PROPERTIES
                            COMPILE_FLAGS "-DMICROCODE_ROM_PATH=\\\"${CMAKE_CURRENT_BINARY_DIR}/../../rtl/microcode/\\\"")

add_custom_command(OUTPUT microcode.mif
                   COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/../../rtl/microcode/microcode.mif .
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../../rtl/microcode/microcode.mif generate_microcode)
add_fpga(PROJECT Top
         FAMILY "Cyclone V"
         PART "5CEBA4F23C7"
         SOURCES ${ALL_SOURCES})

add_custom_command(OUTPUT __program_de0_cv__
                   COMMAND ${QUARTUS_PGM_EXECUTABLE} --mode=jtag -o \"P\;Top.sof\"
                   DEPENDS Top.sof)

add_custom_target(de0-cv DEPENDS Top.sta.rpt)
add_custom_target(de0-cv-program DEPENDS __program_de0_cv__)