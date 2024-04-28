// По своей сути костыль, что бы не модить 'JsonFileLoader'
//  в оригинальной версии в ошибке не выводится файл в котором произошла ошибка,
//  что является довольно критично когда очень много файлов

// Возможно будет развиваться, но пока что не вижу в этом никакого смысла
class NewStoryGroupJsonFileLoader<Class T>
{
	protected static ref JsonSerializer _Serializer;

	static void JsonLoadFile(string filename, out T data)
	{
		if( FileExist( filename ) ) {
			string file_content;
			string line_content;
			string error;
			
			FileHandle handle = OpenFile( filename, FileMode.READ );

			if ( handle == 0 ) {
				return;
			}
			
			while ( FGets( handle,  line_content ) >= 0 ) {
				file_content += line_content;
			}
			
			CloseFile( handle );
			
			if( !_Serializer ) {
				_Serializer = new JsonSerializer();
			}
			
			if( !_Serializer.ReadFromString( data, file_content, error ) ) {
				Error( error + "\n\nFile location:\n" + filename + "\n" );
			}
		}
	}
}
