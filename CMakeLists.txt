cmake_minimum_required(VERSION 3.22.1)

project(MinProxy)

# aux_source_directory(src SRC)

# 服务端
set(SSRC 
    src/main_s.c
)
add_executable(server ${SSRC})

# 客户端
set(CSRC 
    src/main_c.c
)
add_executable(client ${CSRC})
