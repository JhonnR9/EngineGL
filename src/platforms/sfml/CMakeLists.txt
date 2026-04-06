# all package Network Graphics Window Audio System
find_package(SFML COMPONENTS Window CONFIG REQUIRED)

# All targets SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
target_link_libraries(EngineGL PRIVATE SFML::Window)