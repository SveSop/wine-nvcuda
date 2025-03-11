nvcuda_unixlib_src = [
  'nvcuda_unixlib.c',
  'nvcuda_unixlib_wow64.c',
]

if is_windows
    nvcuda_res = custom_target('nvcuda.res',
        output  : 'nvcuda.res',
        input   : 'nvcuda.rc',
        command : [
          wrc,
          '-I', meson.project_source_root() + '/include',
          '-o', '@OUTPUT@',
          '@INPUT@',
        ],
        install : false)

    nvcuda_dll = shared_library('nvcuda', ['nvcuda.c', nvcuda_res],
        name_prefix         : '',
        include_directories : include_path,
        link_depends        : ['nvcuda.spec'],
        link_args           : ['-Wl,--wine-builtin', meson.current_source_dir() / 'nvcuda.spec'],
        install             : true,
        install_dir         : install_dir)
else
    nvcuda_so = shared_library('nvcuda', nvcuda_unixlib_src,
        name_prefix         : '',
        include_directories : include_path,
        dependencies        : [lib_dl],
        install             : true,
        install_dir         : install_dir)
endif
