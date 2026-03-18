target_sources(ck_lib PRIVATE 
    crypto/crypto.cpp
    crypto/random.cpp
    crypto/secure_wipe.cpp
    
    config/path.cpp
    config/print.cpp
    config/serialize.cpp
    config/deserialize.cpp
    config/active.cpp
    config/parse_key.cpp
    config/set.cpp
    config/insert_vault.cpp
    config/default.cpp
    config/save.cpp
    
    input/wisper.cpp
    
    index/find.cpp
    index/theme.cpp
    index/print_tree.cpp
    index/parse_path.cpp
    index/walk_path.cpp
    index/deserialize.cpp
    index/serialize.cpp
    index/get_idx_file.cpp
    index/insert_entry.cpp
    index/write_index.cpp
    index/fingerprints.cpp
    
    mount/mount.cpp
)
