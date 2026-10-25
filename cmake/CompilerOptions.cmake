function(ck_apply_compiler_options target)
  if (MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  
    if (CK_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
    
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic
      -Wformat=2 -Wconversion -Wsign-conversion
      -Wshadow -Wnull-dereference -Wdouble-promotion
    )
    
    if (CK_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE
        -Werror
        -Wno-error=unused-parameter
        -Wno-error=unused-function
      )
    endif()
    
    if (CK_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_options(${target} PRIVATE
        -fsanitize=address,undefined 
        -fno-omit-frame-pointer
        -fno-sanitize-recover=all
        -g3
      )
      target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
    
  endif()

  if (CK_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_BIN NAMES clang-tidy REQUIRED)
    set_target_properties(${target} PROPERTIES 
      CXX_CLANG_TIDY "${CLANG_TIDY_BIN}"
    )
  endif()
endfunction()
