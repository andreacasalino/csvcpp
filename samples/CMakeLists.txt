function(MAKE_SAMPLE NAME SOURCE)

add_executable(${NAME} ${SOURCE})

target_link_libraries(${NAME} PUBLIC csvpp)

target_compile_definitions(${NAME} PUBLIC
    -D ASSETS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# install(TARGETS ${NAME}

endfunction()

MAKE_SAMPLE(Csvpp-ReadMe ReadMe.cpp)

MAKE_SAMPLE(Csvpp-ReadMovies-Sample ReadMovies.cpp)
MAKE_SAMPLE(Csvpp-ReadPlaces-Sample ReadPlaces.cpp)
MAKE_SAMPLE(Csvpp-WriteResults-Sample WriteResults.cpp)
