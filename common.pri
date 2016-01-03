# Common functions for all projects
isEmpty(BASEDIR) : error("BASEDIR is not defined")

message("Included common.pri")

unix {
QMAKE_CXXFLAGS += -Wall -Werror -Wl,--verbose
}

# Link against a library within this project
# Usage: LINK_LOCAL(directory, libname)
# directory - path relative to the base directory (e.g. library/foo)
# libname - name of library (e.g. foo NOT libfoo)
defineTest(LINK_LOCAL) {
	_dir = $$1
	_lib = $$2

	# Create the full path
	_localDir = $${_PRO_FILE_PWD_}/$${BASEDIR}/$${_dir}

	message ("Using $${_lib} from $${_localDir}")

	LIBS    += -L$${_localDir} -l$${_lib}
	*g++*: QMAKE_LFLAGS += -Wl,-R$${_localDir}

	export(LIBS)
	export(QMAKE_LFLAGS)
}

INCLUDEPATH += . $${BASEDIR}/include
DEPENDPATH  += . $${BASEDIR}/include

#DESTDIR = $${BASEDIR}/bin
