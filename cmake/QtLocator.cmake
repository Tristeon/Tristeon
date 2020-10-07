# credit: https://stackoverflow.com/a/47723189/6896221
SET(QT_MISSING True)
# msvc only; mingw will need different logic
IF(MSVC)
    # look for user-registry pointing to qtcreator
    GET_FILENAME_COMPONENT(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.cpp\\shell\\Open\\Command] PATH)

    # get root path so we can search for 5.3, 5.4, 5.5, etc
    STRING(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
    LIST(GET QT_BIN 0 QT_BIN)
    FILE(GLOB QT_VERSIONS "${QT_BIN}/5.*")
    LIST(SORT QT_VERSIONS)

    # assume the latest version will be last alphabetically
    LIST(REVERSE QT_VERSIONS)

    LIST(GET QT_VERSIONS 0 QT_VERSION)

    # fix any double slashes which seem to be common
    STRING(REPLACE "//" "/"  QT_VERSION "${QT_VERSION}")
        

    # check for 64-bit os
    # may need to be removed for older compilers as it wasn't always offered
    IF(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
        SET(QT_MSVC_POSTFIX "_64")

    if ((${MSVC_VERSION} GREATER_EQUAL 1920) AND (EXISTS ${QT_VERSION}/msvc2019${QT_MSVC_POSTFIX}/))
        SET(QT_MSVC 2019)
    elseif ((${MSVC_VERSION} GREATER_EQUAL 1910) AND (EXISTS ${QT_VERSION}/msvc2017${QT_MSVC_POSTFIX}/))
        SET(QT_MSVC 2017)
    elseif ((${MSVC_VERSION} GREATER_EQUAL 1900) AND (EXISTS ${QT_VERSION}/msvc2015${QT_MSVC_POSTFIX}/))
        SET(QT_MSVC 2015)
    else ()
        # do some math trickery to guess folder
        # - qt uses (e.g.) "msvc2012"
        # - cmake uses (e.g.) "1800"
        # - see also https://cmake.org/cmake/help/v3.0/variable/MSVC_VERSION.html
        MATH(EXPR QT_MSVC "2000 + (${MSVC_VERSION} - 600) / 100")
    endif ()

    ENDIF()
    SET(QT_PATH "${QT_VERSION}/msvc${QT_MSVC}${QT_MSVC_POSTFIX}")
    SET(QT_MISSING False)

    MESSAGE("-- Qt found: ${QT_PATH}")
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PATH}/lib/cmake/Qt5/")
ELSE()
    SET(QT_PATH usr/lib/qt5/bin)
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PATH}/")
ENDIF()