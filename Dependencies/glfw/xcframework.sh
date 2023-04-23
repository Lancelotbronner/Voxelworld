cd "${0%/*}"
xcodebuild -create-xcframework \
-library macos-universal/glfw3.a -headers include \
-output glfw3.xcframework
