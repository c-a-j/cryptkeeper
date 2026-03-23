target_sources(ck_lib PRIVATE 
    crypto/crypto.cpp
    crypto/random.cpp
    crypto/secure_wipe.cpp
    
    config/config.cpp
    
    input/wisper.cpp
    
    index/deserialize.cpp
    index/fingerprints.cpp
    index/index.cpp
    index/insert.cpp
    index/print.cpp
    index/theme.cpp
    index/walk_path.cpp
    index/write.cpp
    
    mount/deserialize.cpp
    mount/mount.cpp
    mount/print.cpp
    mount/resolve.cpp
    mount/theme.cpp
    mount/write.cpp
    
    path/path.cpp
    path/parse_path.cpp
    path/join.cpp
)
