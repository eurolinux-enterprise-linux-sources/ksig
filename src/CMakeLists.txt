project(ksig)
add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)
set(ksig_SRCS 
   siggenerator.cpp 
   siglistview.cpp 
   siglistviewitem.cpp 
   ksig.cpp 
   standardtext.cpp 
   main.cpp )

kde4_add_ui_files(ksig_SRCS standardtextbase.ui )

kde4_add_executable(ksig ${ksig_SRCS})

target_link_libraries(ksig   ${KDE4_KDE3SUPPORT_LIBS})

install(TARGETS ksig DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install( FILES ksig.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES ksigui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ksig )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

find_package(Msgfmt REQUIRED)
find_package(Gettext REQUIRED)
add_subdirectory( po )
add_subdirectory( doc )
