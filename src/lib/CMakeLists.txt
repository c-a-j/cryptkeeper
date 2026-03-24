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
    
    mount/chroot.cpp
    mount/deserialize.cpp
    mount/hash.cpp
    mount/initialized.cpp
    mount/mount.cpp
    mount/mounts.cpp
    mount/print.cpp
    mount/resolve.cpp
    mount/theme.cpp
    mount/umount.cpp
    mount/write.cpp
    
    path/path.cpp
    path/parse_path.cpp
    path/join.cpp
    path/existence.cpp
    
    fs/msg.cpp
    fs/write_atomic.cpp
)

if (WIN32)
  target_sources(ck_lib PRIVATE
    fs/tempfile_win32.cpp
  )
else()
  target_sources(ck_lib PRIVATE
    fs/tempfile_posix.cpp
    fs/write_all.cpp
  )
endif()
