import qbs.base 1.0

Plugin {
    Depends { name: "Lib" }
    Depends { name: "LibJPEG" }
//    condition: qbs.targetOS.contains("linux")
//    cpp.dynamicLibraries: qbs.targetOS.contains("linux") ? [ "jpeg" ] : []
    files: [ "*.cpp", "*.h" ]
}
