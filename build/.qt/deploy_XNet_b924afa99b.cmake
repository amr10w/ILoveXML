include("C:/Users/Amr/Desktop/Study/DSA Project/XML-Social-Network-Analyzer/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/XNet-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/Amr/Desktop/Study/DSA Project/XML-Social-Network-Analyzer/build/XNet.exe"
    GENERATE_QT_CONF
)
