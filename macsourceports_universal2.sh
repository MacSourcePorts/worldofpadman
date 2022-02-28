# game/app specific values
export APP_VERSION="1.0"
export ICONSDIR="misc"
export ICONSFILENAME="wop"
export PRODUCT_NAME="WorldOfPadman"
export EXECUTABLE_NAME="wopexe"
export PKGINFO="APPLEWOP"
export COPYRIGHT_TEXT="World of Padman Copyright © 2007 Padworld Entertainment. All rights reserved."

#constants
source ../MSPScripts/constants.sh

rm -rf ${BUILT_PRODUCTS_DIR}

#create makefiles with cmake, perform builds with make
rm -rf ${X86_64_BUILD_FOLDER}
mkdir ${X86_64_BUILD_FOLDER}
cd ${X86_64_BUILD_FOLDER}
#/usr/local/bin/cmake -DARCH=x86_64 -DSDL2_DIR=/usr/local/opt/sdl2/lib/cmake/SDL2 -DSDL2_INCLUDE_DIRS=/usr/local/opt/sdl2/include/SDL2 -DSDL2_LIBRARIES=/usr/local/opt/sdl2/lib ..
/usr/local/bin/cmake -DARCH=x86_64 ..
make -j$NCPU
cd ..

rm -rf ${ARM64_BUILD_FOLDER}
mkdir ${ARM64_BUILD_FOLDER}
cd ${ARM64_BUILD_FOLDER}
cmake ..
make -j$NCPU
cd ..

rm -rf ${X86_64_BUILD_FOLDER}/${WRAPPER_NAME}
mv wop.x86_64.app ${X86_64_BUILD_FOLDER}/${WRAPPER_NAME}
mv ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/wop.x86_64  ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}
mv renderer_opengl1_x86_64.dylib  ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
mv renderer_opengl2_x86_64.dylib  ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
mv renderer_vulkan_x86_64.dylib  ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
cp -a libs/libs/macosx/. ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}

rm -rf ${ARM64_BUILD_FOLDER}/${WRAPPER_NAME}
mv wop.arm64.app ${ARM64_BUILD_FOLDER}/${WRAPPER_NAME}
mv ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/wop.arm64  ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}
mv renderer_opengl1_arm64.dylib  ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
mv renderer_opengl2_arm64.dylib  ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
mv renderer_vulkan_arm64.dylib  ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}
cp -a libs/libs/macosx/. ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}

# create the app bundle
# since the one reference in the executable is covered we can skip lipo and dylibbundler in this script
"../MSPScripts/build_app_bundle.sh" "skiplipo"

#lipo the executable and libs
lipo ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME} ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME} -output "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}" -create

cp ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_opengl1_x86_64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}
cp ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_opengl2_x86_64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}
cp ${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_vulkan_x86_64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}
cp ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_opengl1_arm64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}
cp ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_opengl2_arm64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}
cp ${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/renderer_vulkan_arm64.dylib ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}

cp -a libs/libs/macosx/. ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}

export ENTITLEMENTS_FILE="misc/osx/WorldOfPadman.entitlements"

# sign and notarize
"../MSPScripts/sign_and_notarize.sh" "$1" entitlements