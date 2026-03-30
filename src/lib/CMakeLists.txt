target_sources(ck_lib PRIVATE 
    crypto/crypto.cpp
    crypto/random.cpp
    crypto/secure_wipe.cpp
    
    config/config.cpp
    config/deserialize.cpp
    config/fio.cpp
    config/print.cpp
    config/pwgen.cpp
    config/serialize.cpp
    config/set.cpp
    config/validate.cpp
    
    input/wisper.cpp
    
    index/deserialize.cpp
    index/fingerprints.cpp
    index/fio.cpp
    index/index.cpp
    index/insert.cpp
    index/print.cpp
    index/serialize.cpp
    index/theme.cpp
    index/tree.cpp
    
    mount/chroot.cpp
    mount/deserialize.cpp
    mount/fio.cpp
    mount/hash.cpp
    mount/initialized.cpp
    mount/mount.cpp
    mount/mounts.cpp
    mount/print.cpp
    mount/resolve.cpp
    mount/serialize.cpp
    mount/theme.cpp
    mount/umount.cpp
    
    path/existence.cpp
    path/get_gpg_id_file.cpp
    path/join.cpp
    path/path.cpp
    path/parse_path.cpp
    
    fio/msg.cpp
    fio/sync_dir.cpp
    fio/atomic_write.cpp
)

if (WIN32)
  target_sources(ck_lib PRIVATE
    fio/tempfile_win32.cpp
  )
else()
  target_sources(ck_lib PRIVATE
    fio/tempfile_posix.cpp
    fio/write_all.cpp
  )
endif()
