class NewStoryGroupSystemFunction
{
	static array<string> FindFilesOnDirectory(string dir)
	{
		string fileName     = "";
		FileAttr fileAttr   = 0;
		bool found          = true;

		FindFileHandle fileHandle = FindFile( dir, fileName, fileAttr, FindFileFlags.ALL );

		if ( fileHandle == 0 ) {
			return null;
		}

		array<string> files = new array<string>();

		while (found) {
			if ( fileName != "" && fileAttr != FileAttr.DIRECTORY ) {
				files.Insert( fileName );
			}

			found = FindNextFile( fileHandle, fileName, fileAttr );
		}

		CloseFindFile( fileHandle );
		
		return files;
	}

	static array<string> FindDirectoriesOnLocation(string dir)
	{
		string folderName   = "";
		FileAttr fileAttr   = 0;
		bool found          = true;

		string folderPath   = dir + "*";
		FindFileHandle fileHandle = FindFile( folderPath, folderName, fileAttr, FindFileFlags.DIRECTORIES );

		if ( fileHandle == 0 ) {
			return null;
		}

		array<string> folders = new array<string>();

		while( found ) {
			if ( folderName != "" && fileAttr == FileAttr.DIRECTORY ) {
				folders.Insert( folderName );
			}

			found = FindNextFile( fileHandle, folderName, fileAttr );
		}

		CloseFindFile( fileHandle );

		return folders;
	}

	// Здесь предполагается что в конце 'dir' уже присутствует '\\'
	// в 'func' передаем колбек функцию в которой есть только 1 входной аргумент (string fileDir, string fileName)
	//  в который бедет передаваться полный путь к файлу
	static void ProcessAllFilesOnDirectory(string dir, ScriptCaller callbackFunc, string fileMask = "")
	{
		string folderName   = "";
		string fileName     = "";
		FileAttr fileAttr   = 0;
		bool found          = true;

		string folderPath = dir + "*";
		FindFileHandle fileHandle = FindFile( folderPath, folderName, fileAttr, FindFileFlags.DIRECTORIES );

		if ( fileHandle != 0 ) {
			while( found ) {
				if ( folderName != "" && fileAttr == FileAttr.DIRECTORY ) {
					ProcessAllFilesOnDirectory( dir + folderName + "\\", callbackFunc, fileMask );
				}

				found = FindNextFile( fileHandle, folderName, fileAttr );
			}

			CloseFindFile( fileHandle );
		}

		string filePath = dir + "*" + fileMask;
		fileHandle = FindFile( filePath, fileName, fileAttr, FindFileFlags.ALL );
		
		if ( fileHandle != 0 ) {
			found = true;

			while( found ) {
				if ( fileName != "" ) {
					callbackFunc.Invoke( dir, fileName );
				}

				found = FindNextFile( fileHandle, fileName, fileAttr );
			}

			CloseFindFile( fileHandle );
		}
	}

	static void CreateFolder(string folder)
	{
		if ( !FileExist(folder) ) {
			MakeDirectory(folder);
		}
	}

	static void DeleteCheckFile(string path)
	{
		if ( FileExist(path) ) {
			DeleteFile(path);
		}
	}
}
