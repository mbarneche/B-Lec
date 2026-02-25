/**
 * @file world_serializer.h
 * @brief World serialization for saving and loading
 * 
 * Handles saving the world state to disk and loading it back.
 */

#pragma once

#include <glm/glm.hpp>
#include <string>

class World;

/**
 * @class WorldSerializer
 * @brief Serializes and deserializes world data
 */
class WorldSerializer {
public:
    /**
     * @brief Save the world to a file
     * @param world Pointer to the world to save
     * @param filepath Path to save the world to
     * @return true if save was successful
     */
    static bool SaveWorld(World* world, const std::string& filepath);

    /**
     * @brief Load a world from a file
     * @param world Pointer to the world to load into
     * @param filepath Path to load the world from
     * @return true if load was successful
     */
    static bool LoadWorld(World* world, const std::string& filepath);

    /**
     * @brief Check if a world file exists
     * @param filepath Path to check
     * @return true if the file exists
     */
    static bool WorldExists(const std::string& filepath);

    /**
     * @brief Get the default world save directory
     * @return Path to the saves directory
     */
    static std::string GetSaveDirectory();

    /**
     * @brief Get the full path for a world save
     * @param world_name Name of the world
     * @return Full path to the world save file
     */
    static std::string GetWorldPath(const std::string& world_name);

    /**
     * @brief Delete a saved world
     * @param world_name Name of the world to delete
     * @return true if deletion was successful
     */
    static bool DeleteWorld(const std::string& world_name);

private:
    static constexpr const char* SAVE_DIRECTORY = "saves";
    static constexpr const char* FILE_EXTENSION = ".blec";
};
