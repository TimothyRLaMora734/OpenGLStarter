#ifndef platform_path_h
#define platform_path_h

#include <aribeiro/common.h>
#include <string>

namespace aRibeiro {

	/// \brief PlatformPath
	///
	/// Common path operations
	///
	/// \author Alessandro Ribeiro
	///
	class PlatformPath {

	public:

	    static std::string SEPARATOR;

		/// \brief getExecutablePath
		///
		/// Compute the executable path from the application first argument.
		///
		/// \author Alessandro Ribeiro
		/// \param argv0 argument 0 from the application main procedure
		/// \return the computed path
		///
		static std::string getExecutablePath(const char* argv0);

		/// \brief getExecutablePath
		///
		/// Compute the executable path from the application first argument.
		///
		/// \author Alessandro Ribeiro
		/// \param arg0 argument 0 from the application main procedure
		/// \return the computed path
		///
		static std::string getExecutablePath(const std::string &arg0);

		/// \brief getWorkingPath
		///
		/// Get the current executable path.
		///
		/// \author Alessandro Ribeiro
		/// \return the current path
		///
		static std::string getWorkingPath();

		/// \brief setWorkingPath
		///
		/// Set the current executable path.
		///
		/// \author Alessandro Ribeiro
		/// \param path the new path to set
		/// \return true if its OK
		///
		static bool setWorkingPath(const std::string &path);

		/// \brief getSaveGamePath
		///
		/// Get and create the savegame path based to a root folder and game name.
		///
		/// In windows, the path is commonly created at:
		/// C:\Users\username\Saved Games\[rootFolder]\[gameName]
		///
		/// In unix, the path is commonly created at:
		/// /home/username/.[rootFolder]/[gameName]
		///
		/// \author Alessandro Ribeiro
		/// \param rootFolder base path used to group several games
		/// \param gameName the game name
		/// \return true if its OK
		///
		static std::string getSaveGamePath(const std::string &rootFolder, const std::string &gameName);

		/// \brief isDirectory
		///
		/// Check if the path is a directory
		///
		/// \author Alessandro Ribeiro
		/// \param path the path to test
		/// \return true if it is a directory
		///
		static bool isDirectory(const std::string &path);

		/// \brief isFile
		///
		/// Check if the path is a file
		///
		/// \author Alessandro Ribeiro
		/// \param path the path to test
		/// \return true if it is a file
		///
		static bool isFile(const std::string &path);
        
        
        /// \brief Split the path+file string into path file extension
        ///
        /// Make easy work with path input from main argument
        ///
        /// \author Alessandro Ribeiro
        /// \param the arg string from main parameter
        /// \param returns folder path
        /// \param returns filename with extension
        /// \param returns filename without extension
        /// \param returns extension from filename
        static void splitPathString(const std::string &input, std::string *outFolder, std::string *outFilename, std::string *outFileWOExt, std::string *outFileExt);


		//static bool createDir(const std::string &path);
		//int readFile(const std::string path, char* buffer, int bufferMaxSize);
		//void writeFile(const std::string path, char* buffer, int bufferSize);

	};

}

#endif
