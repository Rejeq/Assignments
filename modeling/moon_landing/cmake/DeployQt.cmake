if (WIN32)
  if (MOON_QT_BIN_DIR)
    set(WIN_DEPLOY "${MOON_QT_BIN_DIR}/windeployqt.exe")
  else()
    find_program(WIN_DEPLOY "windeployqt")
    if (WIN_DEPLOY-NOTFOUND)
      message(WARNING "Unable to find windeployqt: not in PATH and MOON_QT_BIN_DIR is not provided")
    endif()
  endif()

  add_custom_command(TARGET MoonLanding POST_BUILD
    COMMENT "Deploying Qt libraries using windeployqt for 'MoonLanding' ..."
    COMMAND "${WIN_DEPLOY}"
      \"$<TARGET_FILE:MoonLanding>\"
      $<IF:$<CONFIG:Debug>,--debug,--release>
      $<IF:$<CONFIG:Debug>,--pdb,>
      --verbose 1
      --no-angle
      --no-compiler-runtime
      --no-opengl
      --no-opengl-sw
      --no-system-d3d-compiler
      --no-translations
  )
endif()
