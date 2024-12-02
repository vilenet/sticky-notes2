cmake_minimum_required(VERSION 3.5)
project(sticky-notes2)

# Path to FLTK
set(FLTK_INCLUDE_DIR "C:/libs/fltk/include")
set(FLTK_LIBRARY_DIR "C:/libs/fltk/lib")

# Search for FLTK
find_package(FLTK REQUIRED PATHS "C:/libs/fltk" NO_DEFAULT_PATH)

# Including header files
include_directories(${FLTK_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Adding path to SQLite libraries
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Create an executable file
add_executable(sticky-notes2
    src/main.cpp 
    src/app.cpp 
    src/note.cpp 
    src/data.cpp
    src/file.cpp
    src/color.cpp
    src/open_note.cpp 
    src/settings.cpp
    src/database.cpp
)

# Linker flags (remove "-mwindows" to show CMD console)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

# Linking libraries
target_link_libraries(sticky-notes2
    ${FLTK_LIBRARY_DIR}/libfltk.a
    ${FLTK_LIBRARY_DIR}/libfltk_forms.a
    ${FLTK_LIBRARY_DIR}/libfltk_images.a
    ${FLTK_LIBRARY_DIR}/libfltk_jpeg.a
    ${FLTK_LIBRARY_DIR}/libfltk_png.a
    ${FLTK_LIBRARY_DIR}/libfltk_z.a
    sqlite3 # Added: linking with SQLite
    gdi32
    comdlg32
    comctl32
    ws2_32
    ole32
    gdiplus
)