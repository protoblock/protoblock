#set the boost directory path here
# 1- download the binaries from
#  http://garr.dl.sourceforge.net/project/boost/boost-binaries/1.55.0/boost_1_55_0-msvc-12.0-64.exe
# 2- Enter the destination directory in the installer wizard step. That directory should be the
#    BOOST_DIR path.
BOOST_DIR = C:/Users/moez/Documents/GitHub/paperfootball/boost
#BOOST_DIR = C:/Users/User/Documents/work/paperfootball/boost
LIBS += -L$${BOOST_DIR}/lib64-msvc-12.0
LIBS += -lboost_log-vc120-1_55
INCLUDEPATH += $${BOOST_DIR}
