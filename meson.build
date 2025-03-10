project('wine-nvcuda', ['c'], version: 'v1.0', license: 'mixed')

include_path    = include_directories('./include')

is_windows      = target_machine.system() == 'windows'
arch_dir_prefix = target_machine.cpu_family() == 'x86_64' ? 'x86_64' : 'i386'
os_dir_suffix   = is_windows ? 'windows' : 'unix'
install_dir     = get_option('libdir') / 'wine' / arch_dir_prefix + '-' + os_dir_suffix

cc = meson.get_compiler('c')

if not is_windows
    lib_dl = cc.find_library('dl')
endif

winebuild = find_program('winebuild')
wrc       = find_program('wrc')

subdir('src')
