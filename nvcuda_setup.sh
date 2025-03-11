#!/bin/bash

nvcuda_dir="$(dirname "$(readlink -fm "$0")")"
arch='x86_64'
slib="lib/wine"

if [ ! -f "$nvcuda_dir/$slib/$arch-unix/nvcuda.so" ]; then
    echo "Files not found in $nvcuda_dir/$slib/$arch-unix" >&2
    exit 1
fi

if [ -z "$WINE_BIN" ]; then
    echo -ne "WINE_BIN is not set!\n"
    echo -ne "Example: WINE_BIN='/home/user/winebinaries/wine-staging/'\n"
    echo -ne "OBS. Installing to system wine install is not recommended.\n"
    echo -ne "Better use custom wine binary.\n"
    exit 1
fi

if [[ -d "$WINE_BIN/lib/wine/$arch-windows" ]]; then
    lib="lib/wine"
elif [[ -d "$WINE_BIN/lib64/wine/$arch-windows" ]]; then
    lib="lib64/wine"
fi

if [ ! -f "$WINE_BIN/$lib/$arch-windows/dxgi.dll" ]; then
    echo -ne "Wine binaries not found in $WINE_BIN! Wine not installed or wrong path!\n" >&2
    exit 1
fi

function win_install {
    cp -f "$nvcuda_dir/$slib/$arch-windows/$1" "$WINE_BIN/$lib/$arch-windows/"
}

function install {
    cp -f "$nvcuda_dir/$slib/$arch-unix/$1" "$WINE_BIN/$lib/$arch-unix/"
}

end(){
    echo -ne "All done - NVCUDA copied to $WINE_BIN\n"
    echo -ne "You need to run wineboot -u with a wineprefix to use nvcuda!\n"
    exit 0
}

echo -ne "Copying 64-bit files..."
win_install nvcuda.dll
install nvcuda.so
echo -ne " OK\n"

echo -ne "Copying 32-bit files..."
arch='i386'
lib="lib/wine"
if ! [[ -d "$WINE_BIN/$lib/$arch-windows" ]]; then
    echo -ne "Cant find 32-bit libraries! Exiting\n"
    end
fi
win_install nvcuda.dll
if [ -f "$WINE_BIN/$lib/$arch-unix/ntdll.so" ]; then
    install nvcuda.so
else
    echo -ne " Wow64 wine binaries detected!\n"
    echo -ne "Skipping 32-bit unixlib..."
fi
echo -ne " OK\n"
end
