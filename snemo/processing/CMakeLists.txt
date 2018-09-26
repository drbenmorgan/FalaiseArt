# Processing i.e Producer modules
include_directories(${PROJECT_SOURCE_DIR})
simple_plugin(MockCalorimeterCalibrator "module" Bayeux::Bayeux CLHEP::CLHEP SOURCE MockCalorimeterCalibrator_module.cc MockCalorimeterModel.cc)
simple_plugin(MockGeigerCalibrator "module")


