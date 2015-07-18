import qbs.base 1.0

Project {
    Lib {
        name: "LibJPEG"
        Group {
            name: "files"
            files: [
                "jaricom.c",
                "jcapimin.c",
                "jcapistd.c",
                "jcarith.c",
                "jccoefct.c",
                "jccolor.c",
                "jcdctmgr.c",
                "jchuff.c",
                "jcinit.c",
                "jcmainct.c",
                "jcmarker.c",
                "jcmaster.c",
                "jcomapi.c",
                "jcparam.c",
                "jcprepct.c",
                "jcsample.c",
                "jctrans.c",
                "jdapimin.c",
                "jdapistd.c",
                "jdarith.c",
                "jdatadst.c",
                "jdatasrc.c",
                "jdcoefct.c",
                "jdcolor.c",
                "jddctmgr.c",
                "jdhuff.c",
                "jdinput.c",
                "jdmainct.c",
                "jdmarker.c",
                "jdmaster.c",
                "jdmerge.c",
                "jdpostct.c",
                "jdsample.c",
                "jdtrans.c",
                "jerror.c",
                "jfdctflt.c",
                "jfdctfst.c",
                "jfdctint.c",
                "jidctflt.c",
                "jidctfst.c",
                "jidctint.c",
                "jquant1.c",
                "jquant2.c",
                "jutils.c",
                "jmemmgr.c",
                "jmemnobs.c"
            ]
            prefix: "libjpeg/"
        }
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: [ "libjpeg" ]
        }
    }
}
